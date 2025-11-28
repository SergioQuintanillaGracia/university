/**
 * @file memoria.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las estructuras para el acceso a la memoria física
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef memoria_H
#define memoria_H

/***** Includes ***********************************************/

#include "tipos.h"

/***** Definiciones *******************************************/

#define KB   (1024)

#define TAM_MEMO_INSTRUC        (4*KB)  /* 1K instrucciones = 4KB */
#define TAM_MEMO_DATOS          (16*KB) /* MAX: 4KB para poder utilizar las direcciones en los desplazamientos */
#define TAM_MEMO_PILA           (8)     /* Tamaño de la pila en bytes */

#define TAM_PAGINA (4*KB)

#define TAM_MEM_RESERVADA 0x1000
#define DIR_FINAL_PILA   0x10000

// TODO: El código y los datos se deberían alinear con el tamaño de bloque de LLC y rellenar con 0 o NOP.
#define ALIGN_CODE 4
#define ALIGN_DATA 8

#define ALIGN_REGION TAM_PAGINA /* Las regiones se alinean al tamaño de página */

/***** Macros *************************************************/

#define inicio_region(mm, r) ((mm)->regiones[r].inicio)
#define final_region(mm, r) ((mm)->regiones[r].final)

#define tam_region(mm, r) (final_region(mm, r) - inicio_region(mm, r))

#define en_region(mm, dir, region) ((dir) >= inicio_region(mm, region) && (dir) < final_region(mm, region))

#define dir_a_offset(mm, dir, region) ((dir) - (mm)->regiones[(region)].inicio)
#define offset_a_dir(mm, offset, region) ((offset) + (mm)->regiones[(region)].inicio)

#define es_region_codigo(r) ((r) == r_text)
#define es_region_datos(r) ((r) == r_data || (r) == r_stack)

#define indice_dir(d, tam) (dword)((d) / (tam))

#define indice_byte(d)     indice_dir(d, sizeof(byte))
#define indice_half(d)     indice_dir(d, sizeof(half))
#define indice_word(d)     indice_dir(d, sizeof(word))
#define indice_dword(d)    indice_dir(d, sizeof(dword))
#define indice_float(d)    indice_dir(d, sizeof(float))
#define indice_float_ps(d) indice_dir(d, 2*sizeof(float))
#define indice_double(d)   indice_dir(d, sizeof(double))

#define indice_code(d)     indice_word(d)

#define esta_alineada_dir(d, s) (((d) % (s)) == 0)

#define esta_alineada_dir_byte(d) esta_alineada_dir(d, sizeof(byte))
#define esta_alineada_dir_half(d) esta_alineada_dir(d, sizeof(half))
#define esta_alineada_dir_word(d) esta_alineada_dir(d, sizeof(word))
#define esta_alineada_dir_dword(d) esta_alineada_dir(d, sizeof(dword))
#define esta_alineada_dir_float(d) esta_alineada_dir(d, sizeof(float))
#define esta_alineada_dir_double(d) esta_alineada_dir(d, sizeof(double))
#define esta_alineada_dir_float_ps(d) esta_alineada_dir(d, 2*sizeof(float))

#define esta_alineada_dir_code(d) esta_alineada_dir_word(d)


/***** Tipos **************************************************/

typedef enum {
    INITIAL_MEM,
    OLD_MEM,
    NEW_MEM
} mem_status_t;

/*** Memoria de datos ***********/

typedef union {
    byte *m_byte;
    half *m_half;
    word *m_word;
    dword *m_dword;
    float *m_float;
    double *m_double;
} memoria_datos_t;

/*** Memoria de instrucciones ***/

typedef instruccion_t *memoria_intrucciones_t;

/*** Tipos de segmentos *********/

// El orden de las constantes en este enumerado sigue el orden en que aparecerán en el mapa de memoria
typedef enum {
    r_reserved,
    r_text,
    r_data,
    r_stack,
    num_regiones, // la última constante indica el número de constantes que hay.
    r_invalid     // Indica que la dirección no corresponde a una region válida.
} region_t;

typedef enum {
    tr_code,
    tr_data,
    tr_other
} tipo_region_t;

/*** Zona de memoria **************/

typedef struct {
    memoria_datos_t mem;
    tipo_t * tipos;
    boolean * dirty;
} zona_datos_t;

/*** Segmento de memoria ********/

typedef struct {
    dword inicio;
    dword final;
    int align;
    tipo_region_t tipo;
    char * nombre;
} segmento_t;

// Tamaño de las regiones hasta 'num_regiones'
#define TAM_REGIONES TAM_MEM_RESERVADA, TAM_MEMO_INSTRUC, TAM_MEMO_DATOS, TAM_MEMO_PILA

/*** Mapa de memoria ***/

typedef struct {
  char texto[SHORT_STR];
} texto_instruccion_t;

typedef struct {
    segmento_t regiones[num_regiones];
    dword align[num_regiones];

    zona_datos_t datos[num_regiones];

    memoria_intrucciones_t instruc;
    texto_instruccion_t * ensamblador;
} mapa_memoria_t;

/***** Prototypes *********************************************/

#define lee_mem_datos(mm, dir, tipo, dirty) _lee_mem_datos(mm, dir, tipo, dirty, __FILE__, __LINE__)
#define lee_mem_instruc(mm, dir) _lee_mem_instruc(mm, dir, __FILE__, __LINE__)

#ifndef CPROTO

#include "memoria_api.h"

#endif

/***** Variables exportadas ***********************************/

#ifdef memoria_C   /* Privado (excepto memoria.c) */

mapa_memoria_t * mmem;

size_t tam_regiones[num_regiones] = {
        TAM_REGIONES
};

#else   /* Público (excepto memoria.c) */

extern mapa_memoria_t * mmem;

extern size_t tam_regiones[num_regiones];

#endif


#endif /* end memoria.h*/
