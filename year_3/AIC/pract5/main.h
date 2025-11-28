/**
 * @file main.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las definiciones de constantes y de variables
 *    globales al simulador completo
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef main_H
#define main_H

/*** Includes ********************************************************/

#include <stdio.h>
#include "tipos.h"
#include "estado.h"
#include "enlazador.h"
#include "presentacion.h"

#include "memoria-cache.h"
#include "memoria-principal.h"

/***** Macros *************************************************/

#ifdef MIN
#undef MIN
#endif
#define MIN(x, y) (((x)<(y)) ? (x) : (y))

#ifdef MAX
#undef MAX
#endif
#define MAX(x, y) (((x)>(y)) ? (x) : (y))

// Configuration macros

#define OPTION(VAR) (options.VAR)
#define PTR_OPTION(VAR) (&(options.VAR))
#define SET_OPTION(VAR, VALUE) do { options.VAR = (VALUE); } while (0)

#define FLAG(VAR) (options.f_##VAR)
#define PTR_FLAG(VAR) (&(options.f_##VAR))
#define SET_FLAG(VAR, VALUE) do { options.f_##VAR = (VALUE); } while (0)

// Memoria
#define JERARQUIA_ACTIVA() (L1D.mem.activa || L1I.mem.activa)

/*** Defines *********************************************************/

#define MAX_FICHEROS 5

#define TAM_ESTADO 128 // Tamaño de la tabla hash para mantener el estado previo
#define NUM_STYLES 16

/*** Tamaño de las estructuras de datos */

#define TAM_REGISTROS               32

#define MAX_BUFFER_PREDIC           16
#define INI_tam_buffer_predic       2
#define INI_tam_tabla_BHSR1         2
#define INI_tam_tabla_BHSR2         2

#define MAX_BITS_PRED_LOCAL 8

// Número de elementos en la jerarquía de memoria
#define NUM_ELEM_MEMORIA 4

#if PRAC == 0
#define MAX_CICLOS_HTML     1000
#define MAX_CICLOS_TOTAL    100000000
#else
#define MAX_CICLOS_HTML  100
#define MAX_CICLOS_TOTAL    100000
#endif

/* Nombre de ficheros de consola y log */
#define console_name "_consola.txt"
#define log_name "_log.txt"

/* Nombre del fichero de traza para el predictor perfecto */
#define trace_filename "_branch_trace.txt"

// Lo siguiente es necesario para que los ficheros .o que no se compilan
// vean los cambios planificados en la práctica y para la modificación línea de ordenes

/*** Contantes */

#define MAX_ORDEN MAX_CICLOS_TOTAL
#define MAX_TEXTO_CONSOLA 512
#define MAX_TEXTO_LOG 512

/*** Types ***********************************************************/

typedef table_state_t * simul_snapshot_t;

typedef struct {
    boolean f_etiquetas;
    boolean f_macros;
    boolean f_dump;
    boolean f_firmas;
    boolean f_depuracion;
    boolean f_show_ops;

    formato_presentacion_t formato_presentacion;

    modo_reg_t show_abi_reg;
    modo_formato_t show_format;
} simul_options_t;

typedef enum {
    TRAZA, FINAL
} fase_ejecucion_pred_t;


/*** Inclusion condicional *************************************************/

/***************************************************************************/
/***************************************************************************/

#ifdef main_C   /* Privado para el fichero main.c */

/* Instrucción vacía */
instruccion_t inst_vacia;

/* Indica que en el ensamblador ya se han inicializado los registros o el btb,
 * ya que sólo se deben inicializar en un fichero */

boolean init_ireg = NO,
        init_fpreg = NO,
        init_btb = NO;

/* Indica que en el ensamblador se utilizan los tipos de datos correspondientes */
boolean hay_double = NO,
        hay_float = NO,
        hay_dword = NO,
        hay_word = NO,
        hay_half = NO,
        hay_byte = NO,
        hay_char = NO;

boolean hay_imul = NO;

/*** Opciones de Ejecución */

simul_options_t options;

/*** Estado del simulador */

/*** Variables internas  */

long orden;
ciclo_t Ciclo;
boolean terminando;
boolean SIG_SYSCALL;
boolean SIG_TRAP0;

char nombre_fich[32 * MAX_FICHEROS];
boolean final;
boolean SaltoEfectivo;

estadisticas_t estat;

/*** Estructuras para el predictor */

fase_ejecucion_pred_t fase_ejecucion_pred = FINAL;
entrada_btb_t BTB[MAX_BUFFER_PREDIC];

/* 2 niveles */
word BHSR1[MAX_TABLA_BHSR];
word BHSR2[MAX_TABLA_BHSR];

/* Predictor perfecto */
FILE *f_trace = NULL;

/* Ultima traza leida del fichero */
dword PC1 = -1;
ciclo_t orden1;
ciclo_t ciclo1 = 0;
dword dest1;
boolean cond1;

/* Seguir leyendo del fichero */
boolean leer_sig_traza = SI;

/* Predictor clarividente: El salto no está en la tabla y salta */
boolean parar_clarividente = NO;
ciclo_t orden_clarividente;

/*** Estructuras visibles al programador */

CSR_t CSR;

tipo_t Rfp_tipo[TAM_REGISTROS];
boolean Rfp_dirty[TAM_REGISTROS];
tipo_t Rint_tipo[TAM_REGISTROS];
boolean Rint_dirty[TAM_REGISTROS];

dword PC_inicial = 0;

/* Consola generada en cada ciclo */
char CONSOLA[MAX_TEXTO_CONSOLA];

/* Log generado en cada ciclo */
char LOG[MAX_TEXTO_LOG];

/* Estado del simulador */
simul_snapshot_t estado_previo;

/* Estructura del programa completo */
programa_t programa;

#endif

/***************************************************************************/
/***************************************************************************/

#ifndef main_C  /* Publico (excepto main.c) */

/* Instruccion NOP */
extern instruccion_t inst_vacia;

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

/*** Opciones de Ejecución */

extern simul_options_t options;

/*** Variables internas  */

extern long orden;
extern ciclo_t Ciclo;
extern boolean terminando;
extern boolean SIG_SYSCALL;
extern boolean SIG_TRAP0;

extern char nombre_fich[32 * MAX_FICHEROS];
extern boolean final;

extern boolean SaltoEfectivo;

extern estadisticas_t estat;

/*** Estructuras para el predictor */

extern fase_ejecucion_pred_t fase_ejecucion_pred;
extern entrada_btb_t BTB[MAX_BUFFER_PREDIC];

/* 2 niveles */
extern word BHSR1[MAX_TABLA_BHSR];
extern word BHSR2[MAX_TABLA_BHSR];

/* Predictor perfecto */
extern FILE *f_trace;

extern dword PC1;
extern ciclo_t orden1, ciclo1;
extern dword dest1;
extern boolean cond1;

//extern ciclo_t orden2;
//extern dword dest2;
//extern boolean pred2;

extern boolean leer_sig_traza;

extern boolean parar_clarividente;
extern ciclo_t orden_clarividente;

/*** Estructuras visibles al programador */

extern CSR_t CSR;

extern tipo_t Rfp_tipo[TAM_REGISTROS];
extern boolean Rfp_dirty[TAM_REGISTROS];
extern tipo_t Rint_tipo[TAM_REGISTROS];
extern boolean Rint_dirty[TAM_REGISTROS];

/* PC inicial */
extern dword PC_inicial;

/* Consola generada en cada ciclo */
extern char CONSOLA[MAX_TEXTO_CONSOLA];

/* Log generado en cada ciclo */
extern char LOG[MAX_TEXTO_LOG];

/* Estado del simulador */
extern simul_snapshot_t estado_previo;

/* Estructura del programa completo */
extern programa_t programa;

/* Jerarquía de memoria */

extern memoria_cache_t L1I;
extern memoria_cache_t L1D;
extern memoria_cache_t L2;
extern memoria_principal_t MP;

extern nivel_jerarquia_memoria_t *elementos_memoria[NUM_ELEM_MEMORIA];

#endif

/*** Estructuras específicas ****/

// Ver archivo main_ruta.h
#include "main_ruta.h"

#endif /* Include main.h */


