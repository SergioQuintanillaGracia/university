/**
 * @file main_ruta.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las definiciones de constantes y de variables
 *    globales al simulador completo.
 *    En este caso las que dependen de la ruta de datos
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef main_ruta_H
#define main_ruta_H

/*** Includes ********************************************************/

#include <stdio.h>

#include "tipos.h"
#include "estado.h"
#include "enlazador.h"

/*** Macros **********************************************************/

#define CONFIG(VAR) (config.VAR)
#define PTR_CONFIG(VAR) (&(config.VAR))
#define SET_CONFIG(VAR, VALUE) do { config.VAR = (VALUE); } while (0)
#define INI_CONFIG(VAR) do { config.VAR = INI_##VAR; } while (0)

// Si una variable de configuración 'foo' se establece a 0 indica que mantiene
//  la configuración inicial 'INI_foo'
#define CHECK_DEFAULT_CONFIG(VAR) \
do { \
   if ((config.VAR) == 0) { \
      (config.VAR) = INI_##VAR; \
   } \
} while (0)

#define CHECK_INI_CONFIG(VAR, VALUE) \
do { \
   if ((config.VAR) == 0) { \
      (config.VAR) = VALUE; \
   } \
} while (0)


/*** Defines *********************************************************/


/*** Tamaño de las estructuras de datos */

#define MAX_VIAS_SUPERESCALAR   16

#if PRAC == 0
#define MAX_CICLOS_HTML         1000
#define MAX_CICLOS_TOTAL        100000000
#else
#define MAX_CICLOS_HTML         100
#define MAX_CICLOS_TOTAL        100000
#endif

#define INI_tam_reorder         20

#define INI_num_vias_issue      1
#define INI_num_vias_bus        1
#define INI_num_vias_commit     1

#define INI_tam_rs_enteros      5
#define INI_tam_rs_sumrest      3
#define INI_tam_rs_multdiv      2
#define INI_tam_rs_carga        3 /* 1 */
#define INI_tam_rs_almacen      3 /* 1 */

#define INI_tam_estaciones (INI_tam_rs.enteros + \
                        INI_tam_rs.sumrest + \
                        INI_tam_rs.multdiv + \
                        INI_tam_rs.carga + \
                        INI_tam_rs.almacen)

#define MAX_ESTACIONES  4096
#define MAX_REORDER     4096
#define MAX_OPERADORES  100 /* Número de operadores * latencia máxima */

#define INI_num_oper_enteros        1
#define INI_num_oper_sumrest        1
#define INI_num_oper_multdiv        1
#define INI_num_oper_direcciones    1
#define INI_num_oper_memdatos       1

#define INI_num_inicio_enteros      INI_num_oper_enteros
#define INI_num_inicio_sumrest      INI_num_oper_sumrest
#define INI_num_inicio_multdiv      INI_num_oper_multdiv
#define INI_num_inicio_direcciones  INI_num_oper_direcciones
#define INI_num_inicio_memdatos     INI_num_oper_memdatos

/*** Tiempos de evaluacion */

#define INI_teval_enteros       1
#define INI_teval_memdatos      2 /* + 1 ciclo del cálculo de la dirección efectiva */
#define INI_teval_sumrest       4
#define INI_teval_multdiv       7
#define INI_teval_direcciones   1

#define INI_l1i_latencia        1
#define INI_l1i_num_bloques     4
#define INI_l1i_tam_bloque      32
#define INI_l1i_grado_assoc     1

#define INI_l1d_latencia        2
#define INI_l1d_num_bloques     4
#define INI_l1d_tam_bloque      32
#define INI_l1d_grado_assoc     2

#define INI_l2_latencia         5
#define INI_l2_num_bloques      16
#define INI_l2_tam_bloque       64
#define INI_l2_grado_assoc      4

#define INI_mp_tam_memoria      4096
#define INI_mp_num_bancos       0
#define INI_mp_tam_banco        32
#define INI_mp_CL               4
#define INI_mp_tRCD             4
#define INI_mp_tRP              4
#define INI_mp_BW               8

// TODO ???
// Lo siguiente es necesario para que los ficheros .o que no se compilan
// vean los cambios planificados en la practica y para la modificación línea de ordenes

/*** Tipos ***********************************************************/

typedef struct {
    dword PC[MAX_VIAS_SUPERESCALAR];
    estado_operacion_memoria_t estado;
    zona_datos_t buffer;
} operacion_busqueda_t;

/*** RISCV configuration */

typedef struct {
    int num_oper;
    int num_inicio;
    int teval;
} oper_info_t;

typedef struct {
    int issue;
    int bus;
    int commit;
} num_vias_info_t;

typedef struct {
    int enteros;
    int sumrest;
    int multdiv;
    int carga;
    int almacen;
} tam_rs_info_t;

typedef struct {
    int tam_buffer_predic;

    tipo_predictor_t tipo_predictor;

    /* BTB */
    word n_bits_pred_local;
    char subtipo_pred_local;

    /* Hibrido */
    word n_bits_selector;
    char subtipo_pred_hybrid;

    /* 2 niveles */
    int tam_tabla_BHSR1;
    word n_bits_BHSR1;

    int tam_tabla_BHSR2;
    word n_bits_BHSR2;
} predic_config_t;

typedef struct {
    dep_mem_t dep_mem;

    num_vias_info_t num_vias;
    tam_rs_info_t tam_rs;

    int tam_reorder;
    int tam_estaciones;

    oper_info_t enteros;
    oper_info_t sumrest;
    oper_info_t multdiv;
    oper_info_t direcciones;
    oper_info_t memdatos;

    /* Predictores */
    predic_config_t predic;

    /* Jerarquía de memoria */
    config_cache_t l1i_config;
    config_cache_t l1d_config;
    config_cache_t l2_config;
    config_memoria_principal_t mp_config;

} riscv_config_t;


#ifdef main_C

riscv_config_t config;

int NUM_OPERADORES = 0;

int INICIO_OP_ENTEROS;
int FIN_OP_ENTEROS;
int INICIO_OP_SUMREST;
int FIN_OP_SUMREST;
int INICIO_OP_MULTDIV;
int FIN_OP_MULTDIV;
int INICIO_OP_DIRECCIONES;
int FIN_OP_DIRECCIONES;
int INICIO_OP_MEMDATOS;
int FIN_OP_MEMDATOS;

int INICIO_REORDER;
int FIN_REORDER;


#else

extern riscv_config_t config;

extern int NUM_OPERADORES;

extern int INICIO_OP_ENTEROS;
extern int FIN_OP_ENTEROS;
extern int INICIO_OP_SUMREST;
extern int FIN_OP_SUMREST;
extern int INICIO_OP_MULTDIV;
extern int FIN_OP_MULTDIV;
extern int INICIO_OP_DIRECCIONES;
extern int FIN_OP_DIRECCIONES;
extern int INICIO_OP_MEMDATOS;
extern int FIN_OP_MEMDATOS;

extern int INICIO_REORDER;
extern int FIN_REORDER;

#endif

/*** Constantes */

#define MARCA_NULA    (MAX_REORDER)

/*** Inicios y finales de las estaciones de reserva */

#define INICIO_ESTACION_RESERVA INICIO_RS_ENTEROS

#define INICIO_RS_ENTEROS (0)
#define FIN_RS_ENTEROS  (INICIO_RS_ENTEROS + CONFIG(tam_rs.enteros) - 1)

#define INICIO_RS_SUMREST (FIN_RS_ENTEROS + 1)
#define FIN_RS_SUMREST (INICIO_RS_SUMREST + CONFIG(tam_rs.sumrest) - 1)

#define INICIO_RS_MULTDIV (FIN_RS_SUMREST + 1)
#define FIN_RS_MULTDIV  (INICIO_RS_MULTDIV + CONFIG(tam_rs.multdiv) - 1)

#define FIN_ESTACION_RESERVA FIN_RS_MULTDIV

#define INICIO_BUFFER_CARGA (FIN_RS_MULTDIV + 1)
#define FIN_BUFFER_CARGA  (INICIO_BUFFER_CARGA + CONFIG(tam_rs.carga) - 1)

#define INICIO_BUFFER_ALMACEN (FIN_BUFFER_CARGA + 1)
#define FIN_BUFFER_ALMACEN  (INICIO_BUFFER_ALMACEN + CONFIG(tam_rs.almacen) - 1)

#define INICIO_BUFFERS         INICIO_BUFFER_CARGA
#define FIN_BUFFERS            FIN_BUFFER_ALMACEN

#define PRIMERA_ESTACION_RESERVA (INICIO_RS_ENTEROS)
#define ULTIMA_ESTACION_RESERVA  (FIN_BUFFER_ALMACEN)

#define ES_ESTACION(s, tipo)  ((s) >= INICIO_##tipo && (s) <= FIN_##tipo)


/*** Inclusion condicional *************************************************/

/***************************************************************************/
/***************************************************************************/

#ifdef main_C   /* Privado para el fichero main.c */

dword PC_ISS = -1;
int RB_cab = -1;
utilizacion_t max_util;
utilizacion_t aver_util;

boolean IFstall;
boolean IFhide;
boolean ISSstall;
boolean Excepcion_Activa;    /* Hay una excepción en la unidad segmentada. Dejar de buscar */
boolean Cancelar_Activa;  /* La fase commit ha activado la señal de cancelar todas las operaciones en curso */

/*** Estructuras visibles al programador */

reg_t Rfp[TAM_REGISTROS];
reg_t Rint[TAM_REGISTROS];

/*** Registros inter-etapa para la segmentación */

/* Representan a los registros inter-etapa que contienen los valores
   para el siguiente ciclo, los valores futuros */
/* DEBEN ESTAR SIEMPRE A LA IZQUIERDA DE UNA ASIGNACION */

reg_if_iss_t M_IF_ISS_1[MAX_VIAS_SUPERESCALAR];

reg_if_iss_t IF_ISS_1;
reg_control_t Control_1;

/* Representan a los registros inter-etapa que contienen los valores
   del presente ciclo. Serán estables hasta que se vuelvan escribir al
   final del ciclo con los nuevos valores */
/* DEBEN ESTAR SIEMPRE A LA DERECHA DE UNA ASIGNACION O EN LAS
   COMPARACIONES */

reg_if_iss_t M_IF_ISS_2[MAX_VIAS_SUPERESCALAR];

reg_if_iss_t IF_ISS_2;
reg_control_t Control_2;

/*** Estructuras internas de la ruta de datos de coma flotante */

/* Todas las estaciones están en el mismo array, pero se puede usar
   RS, LB o SB */
estacion_t RS[MAX_ESTACIONES];
#define LB RS
#define SB RS

int RB_inicio, RB_fin, RB_long;
reorder_t RB[MAX_REORDER];

bus_comun_t BUS;

operador_t Op[MAX_OPERADORES];

#endif

/***************************************************************************/
/***************************************************************************/

#ifndef main_C  /* Publico (excepto main.c) */

extern boolean init_ireg,
        init_fpreg,
        init_btb;

extern boolean hay_double,
        hay_float,
        hay_dword,
        hay_word,
        hay_half,
        hay_byte,
        hay_char;

extern boolean hay_imul;

/*** Variables internas  */

extern char nombre_fich[32 * MAX_FICHEROS];
extern dword PC_ISS;
extern int RB_cab;

extern utilizacion_t max_util;
extern utilizacion_t aver_util;

extern boolean IFstall;
extern boolean IFhide;
extern boolean ISSstall;
extern boolean Excepcion_Activa;  /* Hay una excepción en la unidad segmentada. Dejar de buscar */
extern boolean Cancelar_Activa;  /* La fase commit ha activado la señal de cancelar todas las operaciones en curso */

/*** Estructuras visibles al programador */

extern reg_t Rfp[TAM_REGISTROS];
extern reg_t Rint[TAM_REGISTROS];

/*** Registros inter-etapa para la segmentación */

/* Representan a los registros inter-etapa que contienen los valores
   para el siguiente ciclo, los valores futuros */
/* DEBEN ESTAR SIEMPRE A LA IZQUIERDA DE UNA ASIGNACION */

extern reg_if_iss_t IF_ISS_1;
extern reg_control_t Control_1;

/* Representan a los registros inter-etapa que contienen los valores
   del presente ciclo. Serán estables hasta que se vuelvan escribir al
   final del ciclo con los nuevos valores */
/* DEBEN ESTAR SIEMPRE A LA DERECHA DE UNA ASIGNACION O EN LAS
   COMPARACIONES */
extern reg_if_iss_t IF_ISS_2;
extern reg_control_t Control_2;

/*** Estructuras internas de la ruta de datos de coma flotante */

/* Todas las estaciones están en el mismo array, pero se puede usar
   RS, LB o SB */
extern estacion_t RS[MAX_ESTACIONES];
#define LB RS
#define SB RS

extern int RB_inicio, RB_fin, RB_long;
extern reorder_t RB[MAX_REORDER];

extern bus_comun_t BUS;

extern operador_t Op[MAX_OPERADORES];

#endif

#endif /* Include main_ruta.h */


