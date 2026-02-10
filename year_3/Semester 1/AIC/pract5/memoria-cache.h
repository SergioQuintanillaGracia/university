/**
 * @file memoria-cache.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Definiciones y tipos para los niveles de tipo cache
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef jerarquia_cache_H
#define jerarquia_cache_H

/* -- Includes -- */

#include <stdio.h>

#include "tipos.h"
#include "jerarquia.h"

/* -- Defines -- */

#define SIN_ETIQUETA (-1)

/* -- Types -- */

/**
 * Estado de un bloque de cache
 */
typedef enum {
    BLOQ_INVALIDO,
    BLOQ_VALIDO,
    BLOQ_SUCIO
} estado_bloque_cache_t;

/**
 * Política ante escrituras
 */
typedef enum {
     WRITE_BACK_WRITE_ALLOCATE,
     WRITE_TROUGH_NO_WRITE_ALLOCATE
} politica_escrituras_t;

/**
 * Contiene la información de una línea de caché
 */
typedef struct bloque_cache_struct {
    estado_bloque_cache_t estado;  /**< Estado del bloque: inválido, válido, sucio ... */
    tipo_operacion_memoria_t oper; /**< Operación que se está realizando sobre este bloque.
                                    * Permite reservar bloques que están en uso */

    dword direccion;               /**< Dirección base del bloque por sencillez */
    dword etiqueta;                /**< Etiqueta del bloque */
    ciclo_t ciclo;                 /**< Último ciclo en el que se ha accedido */
    zona_datos_t datos;            /**< Datos contenidos en el bloque de caché */
} bloque_cache_t;

typedef struct operacion_mem_cache_struct {
    operacion_nivel_t info;        /**< Información común sobre una operación */

    dword etiqueta;                /**< Etiqueta del bloque buscado */

    boolean cancelada;             /**< La operación se ha cancelado.
                                    *    No se deben transferir los datos hacía el nivel superior. */

    bloque_cache_t   *bloque;      /**< Bloque de este nivel implicado en la operación */
} operacion_mem_cache_t;

typedef struct {
    dword etiqueta;                 /**< Etiqueta del bloque buscado */

    operacion_mem_cache_t * oper;   /**< Operación candidata */
} operacion_bloque_t;

typedef struct {
    uint8_t latencia;               /**< Ciclos en gestionar un acceso con éxito. Incluye la transferencia */
    // Geometría
    uint32_t num_bloques;           /**< Número de bloques */
    uint16_t tam_bloque;            /**< Tamaño de un bloque en este nivel en bytes */
    uint32_t grado_asoc;            /**< Grado de asociatividad. Puede ser completamente asociativa (num_bloques) */
} config_cache_t;

typedef struct memoria_cache_struct {
    nivel_jerarquia_memoria_t mem;         /**< Datos y operaciones de este nivel de memoria */

    politica_escrituras_t politica;        /**< Política ante escrituras */

    config_cache_t config;          /**< Configuración de la cache */

    bloque_cache_t *bloques;        /**< Bloques de cache */
} memoria_cache_t;

/* -- Type related macros -- */
#define DIREC_BLOQUE_CACHE(cache, dir)    ((dir) / (cache)->config.tam_bloque)
#define DESPLAZ_BLOQUE_CACHE(cache, dir)  ((dir) % (cache)->config.tam_bloque)
#define DIREC_BASE_BLOQUE_CACHE(cache, dir)  ((dir) - DESPLAZ_BLOQUE_CACHE(cache,dir))

#define NUM_CONJUNTOS(cache)      ((cache)->config.num_bloques / (cache)->config.grado_asoc)

#define INDICE_CONJUNTO(cache, dir) (DIREC_BLOQUE_CACHE(cache, dir) % NUM_CONJUNTOS(cache))
#define ETIQUETA_BLOQUE(cache, dir) (DIREC_BLOQUE_CACHE(cache, dir) / NUM_CONJUNTOS(cache))

/* Primer bloque del conjunto con índice 'indice' */
#define PRIMER_BLOQUE_CONJUNTO(cache, indice) ((indice) * (cache)->grado_asoc)

/* -- Prototypes -- */

#ifndef CPROTO

#include "memoria_cache_api.h"

#endif

#endif /* end memoria-cache.h */
