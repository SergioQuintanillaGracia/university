/**
 * @file jerarquia.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Definiciones y tipos para la jerarquía de memoria
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef jerarquia_H
#define jerarquia_H

/* -- Includes -- */

#include <stdio.h>

#include "tipos.h"
#include "memoria.h"
#include "g_string.h"
#include "g_html.h"

/* -- Macros -- */

// #define M_DEBUG(...) fprintf(stderr, __VA_ARGS__)
#define M_DEBUG(...) do { } while(0)

#define PTR_OPER(OPER) ((operacion_nivel_t *)(OPER))
// Para las variables globales
#define PTR_NIVEL(NIVEL) ((nivel_jerarquia_memoria_t *)&(NIVEL))

#define limpia_estado_operacion(OPER) do { \
    PTR_OPER(OPER)->texto[0] = '\0';       \
} while(0)

#define estado_operacion(OPER, ...) do { \
    if (FLAG(show_ops)) {                \
        sprintf(PTR_OPER(OPER)->texto, __VA_ARGS__); \
    }                                    \
} while (0)

/* -- Type related macros -- */

// nivel_jerarquia_memoria_t tiene que estar en el campo "mem" de cada nivel especializado
#define INICIA_NIVEL_MEMORIA(NOMBRE, TIPO) \
{                                          \
.mem = {                                   \
    .activa = NO,                          \
    .nombre = (NOMBRE),                    \
    .tipo = (TIPO)                         \
   }                                       \
}

#define INVOCA_INTERFAZ_MEMORIA(NIVEL, OPERACION, ...) (interfaces_jerarquia[(NIVEL)->tipo].OPERACION((NIVEL) __VA_OPT__(,) __VA_ARGS__))

// Macros de acceso a la interfaz
#define INICIA_OPERACION_MEMORIA(nivel, tipo, src, dir, tam, orden, callback, oper_data) \
    INVOCA_INTERFAZ_MEMORIA(nivel, inicia_operacion, tipo, src, dir, tam, orden, callback, oper_data)

/* -- Types -- */

/**
 * Tipo de operación de memoria: Lectura, escritura, pre-búsqueda, ...
 */
typedef enum {
    OP_NULA,  // No hay una operación en curso
    OP_LECT,  // La operación en curso es una lectura
    OP_ESCR
} tipo_operacion_memoria_t;

/**
 * Estado de una operación de acceso a memoria
 */
typedef enum {
    EST_OP_PENDIENTE,        /* Pendiente de comenzar. Sólo MP */
    EST_OP_ACCEDIENDO,       /* Accediendo a este nivel */
    EST_OP_BLOQUEADA,        /* Bloqueada a la espera de que le acepte la petición el nivel inferior */
    EST_OP_ESPERANDO,        /* Esperando los datos del nivel inferior */
    EST_OP_TERMINADA         /* Operación terminada */
} estado_operacion_memoria_t;

// Declaración anticipada para su uso en las operaciones
struct nivel_jerarquia_memoria_struct;
#define OPER(operacion) (* (operacion))
#define NIVEL_MEM_PTR struct nivel_jerarquia_memoria_struct *

/* Declaración anticipada del tipo operación */
struct operacion_nivel_struct;

typedef void OPER(operacion_finalizada_fn)(struct operacion_nivel_struct *oper,
                                           void *datos, tipo_t *tipo_datos, boolean *dirty_datos);

/**
 * Contiene la información sobre una operación de acceso a memoria
 */
typedef struct operacion_nivel_struct {
    NIVEL_MEM_PTR parent;               /**< Nivel que sirve está operación */
    NIVEL_MEM_PTR src;                  /**< Nivel origen de la operación. IF, LB, SB usarán NULL */
    estado_operacion_memoria_t estado;  /**< Estado de la operación */
    unsigned int indice;                /**< Índice de la operación */

    // Información sobre la petición
    tipo_operacion_memoria_t tipo;      /**< Tipo de operación */
    dword direccion;                    /**< Dirección de la operación */
    size_t tam;                         /**< Tamaño en bytes de los datos solicitados */
    ciclo_t inicio;                     /**< Ciclo en que se ha emitido la operación */
    uint16_t ciclos;                    /**< Ciclos transcurridos desde que ha comenzado a atenderse
                                         * en este nivel (estado ACCEDIENDO) */

    ciclo_t orden;                      /**< Orden de la instrucción que ha encargado la operación */

    // Información sobre el resultado
    boolean acierto;                    /**< ¿El dato estaba en el bloque / buffer de fila? */

    // Estado para la visualización
    char texto[SHORT_STR];              /**< Estado de la operación para sus visualización */

    // Interfaz para notificar al nivel que encarga la operación
    operacion_finalizada_fn finaliza_operacion;
    void *oper_data;                    /**< Datos de la operación en el nivel origen */

    // Siguiente operación
    struct operacion_nivel_struct * next;
} operacion_nivel_t;

/**
 * Tipo de memoria en la jerarquía de memoria
 */
typedef enum {
    nivel_tipo_cache,
    nivel_tipo_memoria_principal,
    num_tipo_niveles_memoria
} tipo_nivel_jerarquia_t;

/**
 * Interfaz común para todos los niveles de la jerarquía de memoria.
 */
typedef struct interfaz_nivel_struct {
    /* Devuelve la configuración en texto plano concatenada a la cadena info */
    void OPER(obtiene_config)(NIVEL_MEM_PTR nivel, formato_presentacion_t formato, g_string_t *info);

    /* Solicita una operación a este nivel de la jerarquía */
    boolean OPER(inicia_operacion)(NIVEL_MEM_PTR nivel, tipo_operacion_memoria_t tipo, NIVEL_MEM_PTR src,
                                   dword dir, size_t tam, ciclo_t orden, operacion_finalizada_fn callback,
                                   void *oper_data);

    /* Actualiza el estado de las operaciones de este nivel */
    void OPER(actualiza_estado)(NIVEL_MEM_PTR nivel);

    /* Vuelca los bloque sucios a memoria principal */
    void OPER(vuelca_datos)(NIVEL_MEM_PTR nivel);

    /* Muestra el estado de este nivel de la jerarquía en el ciclo actual */
    void OPER(muestra_estado)(NIVEL_MEM_PTR nivel, formato_presentacion_t formato, g_string_t *str);
} interfaz_nivel_t;

/**
 * Estadísticas de acceso
 */
typedef struct estadisticas_accesos_struct {
    uint32_t num_accesos;
    uint32_t num_aciertos;
    ciclo_t num_ciclos;
} estadisticas_accesos_t;

/**
 * Estadísticas de un nivel de la jerarquía de memoria
 */
typedef struct estadisticas_memoria_struct {
    estadisticas_accesos_t totales;
    estadisticas_accesos_t lectura;
    estadisticas_accesos_t escritura;
} estadisticas_memoria_t;

typedef struct {
    uint8_t max_oper;                /**< Número máximo de operaciones en este nivel */
    uint8_t max_oper_ciclo;          /**< Número máximo de operaciones que aceptarse cada ciclo */
    uint8_t max_oper_acceso;         /**< Número máximo de operaciones accediendo a este nivel */
} config_operaciones_t;

/**
 * Datos comunes paro todos lo niveles de la jerarquía de memoria
 */
typedef struct nivel_jerarquia_memoria_struct {
    /** Esto parámetros se inicializan al declarar la variable de correspondiente que representa el nivel */
    boolean activa;                   /**< Indica si este nivel está activo */
    const char *nombre;               /**< Nombre del nivel */
    tipo_nivel_jerarquia_t tipo;      /**< Tipo de nivel de memoria en la jerarquía */

    /** Operaciones sobre este nivel */
    operacion_nivel_t *oper_pendientes;    /**< Operaciones en curso en este nivel de la jerarquía. */
    operacion_nivel_t *ultima_pendiente;
    operacion_nivel_t *oper_libres;        /**< Operaciones disponibles en este nivel */
    operacion_nivel_t *ultima_libre;

    config_operaciones_t oper_config; /**< Configuración del buffer de operaciones */

    uint8_t oper_ciclo;               /**< Número actual de operaciones lanzadas en este ciclo */

    estadisticas_memoria_t estad;     /**< Estadísticas en este nivel de memoria */

    /** Puntero al siguiente nivel de la jerarquía */
    struct nivel_jerarquia_memoria_struct *siguiente;
} nivel_jerarquia_memoria_t;


#define SIGUIENTE_NIVEL_MEMORIA(nivel_1, nivel_2) \
    do {                                          \
       (nivel_1)->siguiente = (nivel_jerarquia_memoria_struct *)(nivel_2); \
    } while(0)

/* Estadísticas */

#define CONTABILIZA_ESTAD(contador, cond) \
    do {                                  \
        (contador).num_accesos++;         \
        if (cond)                         \
            (contador).num_aciertos++;    \
    } while(0)

#ifndef jerarquia_C
extern char fase_instruc_jerarquia[TAG_STR];
extern interfaz_nivel_t interfaces_jerarquia[num_tipo_niveles_memoria];
#endif

/* -- Prototypes -- */

#ifndef CPROTO

#include "jerarquia_api.h"

#endif

#endif /* end jerarquia.h */
