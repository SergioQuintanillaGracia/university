/*********************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *	        Pedro López (plopez@disca.upv.es)
 *
 * File: main.h
 *
 * Description: Contiene las definiciones de constantes y de variables
 *    globales a todo el simulador
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *********************************************************************/

#ifndef main_H
#define main_H

/*** Includes ********************************************************/

#include <stdio.h>
#include "tipos.h"
#include "estado.h"
#include "enlazador.h"

/***** Macros *************************************************/

#ifdef MIN
#undef MIN
#endif
#define MIN(x, y) (((x)<(y)) ? (x) : (y))

#ifdef MAX
#undef MAX
#endif
#define MAX(x, y) (((x)>(y)) ? (x) : (y))

/*** Defines *********************************************************/

#define COLOR_RESET  "\033[0m"
#define BOLD         "\033[1m"
#define BLACK_TEXT   "\033[30;1m"
#define RED_TEXT     "\033[31;1m"
#define GREEN_TEXT   "\033[32;1m"
#define YELLOW_TEXT  "\033[33;1m"
#define BLUE_TEXT    "\033[34;1m"
#define MAGENTA_TEXT "\033[35;1m"
#define CYAN_TEXT    "\033[36;1m"
#define WHITE_TEXT   "\033[37;1m"

#define MAX_FICHEROS 5

#define TAM_ESTADO 128 // Tamaño de la tabla hash para mantener el estado previo
#define NUM_STYLES 16

/*** Tamaño de las estructuras de datos */

#define _TAM_REGISTROS  32

#define MAX_BUFFER_PREDIC       16
#define _TAM_BUFFER_PREDIC      2
#define _TAM_TABLA_BHSR1         2
#define _TAM_TABLA_BHSR2         2

#define MAX_BITS_PRED_LOCAL 8


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
// vean los cambios planificados en la practica y para la modificación línea de ordenes


#ifdef main_C

const int TAM_REGISTROS = _TAM_REGISTROS;
modo_reg_t SHOW_ABI_REG = reg_auto;
modo_formato_t show_format = format_full;

int TAM_BUFFER_PREDIC = _TAM_BUFFER_PREDIC;
int TAM_TABLA_BHSR1 = _TAM_TABLA_BHSR1;
int TAM_TABLA_BHSR2 = _TAM_TABLA_BHSR2;

#else

extern int TAM_REGISTROS;
extern modo_reg_t SHOW_ABI_REG;
extern modo_formato_t show_format;

extern int TAM_BUFFER_PREDIC;
extern int TAM_TABLA_BHSR1;
extern int TAM_TABLA_BHSR2;

#endif

/*** Contantes */

#define MAX_ORDEN MAX_CICLOS_TOTAL
#define MAX_TEXTO_CONSOLA 512
#define MAX_TEXTO_LOG 512



/*** Types ***********************************************************/

typedef table_state_t * estado_simulador_t;

/*** Inclusion condicional *************************************************/

/***************************************************************************/
/***************************************************************************/

#ifdef main_C   /* Privado para el fichero main.c */

/* Instruccion vacia */
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

boolean f_etiquetas = NO;
boolean f_macros = NO;

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

tipo_predictor_t tipo_predictor = BTB_1_BIT;

enum {
    TRAZA, FINAL
} fase_ejecucion_pred = FINAL;

/* BTB */
entrada_btb_t BTB[MAX_BUFFER_PREDIC];
word n_bits_pred_local = 1;
char subtipo_pred_local = 's';

/* Hibrido */
word n_bits_selector = 1;
char subtipo_pred_hybrid = 's';

/* 2 niveles */
word BHSR1[MAX_TABLA_BHSR];
word n_bits_bhsr1 = 0;

word BHSR2[MAX_TABLA_BHSR];
word n_bits_bhsr2 = 0;

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
//reg_t Rfp[_TAM_REGISTROS];
tipo_t Rfp_tipo[_TAM_REGISTROS];
boolean Rfp_dirty[_TAM_REGISTROS];
//reg_t Rint[_TAM_REGISTROS];
tipo_t Rint_tipo[_TAM_REGISTROS];
boolean Rint_dirty[_TAM_REGISTROS];


dword PC_inicial = 0;

/* Consola generada en cada ciclo */
char CONSOLA[MAX_TEXTO_CONSOLA];

/* Log generado en cada ciclo */
char LOG[MAX_TEXTO_LOG];

/* Estado del simulador */
/* Estado del simulador */
estado_simulador_t estado_previo;

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

extern boolean f_etiquetas;
extern boolean f_macros;

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

extern tipo_predictor_t tipo_predictor;

extern enum {
    TRAZA, FINAL
} fase_ejecucion_pred;


/* BTB */
extern entrada_btb_t BTB[MAX_BUFFER_PREDIC];
extern word n_bits_pred_local;
extern char subtipo_pred_local;

/* Hibrido */
extern word n_bits_selector;
extern char subtipo_pred_hybrid;

/* 2 niveles */
/* Predictor 1 */
extern word BHSR1[MAX_TABLA_BHSR];
extern word n_bits_bhsr1;

/* Predictor 2 */
extern word BHSR2[MAX_TABLA_BHSR];
extern word n_bits_bhsr2;

extern FILE* f_trace;

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

//extern reg_t Rfp[_TAM_REGISTROS];
extern tipo_t Rfp_tipo[_TAM_REGISTROS];
extern boolean Rfp_dirty[_TAM_REGISTROS];
//extern reg_t Rint[_TAM_REGISTROS];
extern tipo_t Rint_tipo[_TAM_REGISTROS];
extern boolean Rint_dirty[_TAM_REGISTROS];



/* PC inicial */
extern dword PC_inicial;


/* Consola generada en cada ciclo */
extern char CONSOLA[MAX_TEXTO_CONSOLA];

/* Log generado en cada ciclo */
extern char LOG[MAX_TEXTO_LOG];

/* Estado del simulador */
extern estado_simulador_t estado_previo;

/* Estructura del programa completo */
extern programa_t programa;



#endif

/*** Estructuras espécificas ****/

// Ver archivo main_ruta.h
#include "main_ruta.h"

#endif /* Include main.h */


