/*********************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politecnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *	   Pedro López (plopez@gap.upv.es)
 *
 * File: tipos_ruta.h
 *
 * Description:
 *    Contiene las definiciones de tipos comunes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *********************************************************************/

#ifndef tipos_ruta_H
#define tipos_ruta_H


/***** Includes ***********************************************/

#include <sys/types.h>
#include <stdint.h>
#include <inttypes.h>

/***** Constants **********************************************/

/*** Opciones de ejecucion ***/

typedef enum {
    no_segm_monociclo,
    no_segm_multiciclo,
    segm
} u_instruccion_t; /* Configuracion procesador */

typedef enum {
    parada,
    cortocircuito,
    ninguno
} riesgos_d_t; /* Solución riesgos de datos */

typedef enum {
    stall3,
    stall2,
    stall1,
    pnt3,
    pnt2,
    pnt1,
    ds3,
    ds2,
    ds1
} riesgos_c_t; /* Solución riesgos de control */

typedef enum {
    html,
    html_final,
    // res_ciclo, // No funciona en esta versión
    res_tiempo,
    res_final
} sal_t; /* Opciones para la salida de resultados */

/*** Banco de registros ********/

typedef struct {
    valor_t valor; /* Valor del registro */
    ciclo_t busy; /*!< Ciclo en el que está listo para cortocircuito */
    boolean dirty;
    tipo_t tipo;
} reg_int_t;

typedef struct {
    valor_t valor; /* Valor del registro */
    ciclo_t busy; /*!< Ciclo en el que está listo para cortocircuito */
    boolean dirty;
    tipo_t tipo;
} reg_fp_t;

/*** Registros inter-etapas ************************************************/

typedef struct {
    instruccion_t IR; /* IR */
    dword NPC; /* PC+4 */

    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} IF_ID_t;

typedef struct {
    instruccion_t IR; /* IR */
    dword NPC; /* PC+4 */
    dword Ra; /* Valores de los registros*/
    dword Rb;
    dword Imm; /* Inmediato con signo extendido */

    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} ID_EX_t;

typedef struct {
    instruccion_t IR; /* IR */
    dword NPC; /* PC+4 */

    dword ALUout; /* Resultado */
    dword data; /* Dato a escribir */
    boolean cond; /* Resultado condicion de salto */

    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} EX_MEM_t;

typedef struct {
    instruccion_t IR; /* IR */
    dword NPC; /* PC+4 */

    dword ALUout; /* Resultado */
    dword MEMout; /* Resultado */
    dword WBdata;

    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} MEM_WB_t;

typedef struct {
    instruccion_t IR; /* IR */
    dword NPC; /* PC+4 */
    valor_t Fa; /* Valores de los registros*/
    valor_t Fb;
    valor_t Fc;
    dword Ra; /* Valores de los registros*/
    dword Rb;
    dword Imm; /* Inmediato con signo extendido */
    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} ID_FP_t;

typedef struct {
    instruccion_t IR; /* IR */
    valor_t ALUout; /* Resultado */
    valor_t Ra; /* Valores de los registros*/
    valor_t Rb;
    valor_t Fa; /* Valores de los registros*/
    valor_t Fb;
    valor_t Fc;
    dword address; /* LOAD y STORE */
    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} OPER_t;

typedef struct {
    instruccion_t IR; /* IR */
    valor_t ALUout; /* Resultado */

    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} MULTI_WB_t;

/*** Señales intermedias *******************/

typedef struct {
    dword compin1, compin2;
    dword cond_out;
    dword sumin1;
    dword dir;
} ID_t;

typedef struct {
    dword aluin1, aluin2, aluout;
    dword compin1, compin2;
    dword cond_out;
    dword data;
    word muxALU_sup, muxALU_inf;
} EX_t;

typedef struct {
    dword dir;
    dword data;
} MEM_t;

typedef struct {
    dword data;
} WB_t;

/*** Estadisticas ***/



/***** Macros *************************************************/

#define latencia_salto1(i) ((i) == ds1 || (i) == pnt1 || (i) == stall1)
#define latencia_salto2(i) ((i) == ds2 || (i) == pnt2 || (i) == stall2)
#define latencia_salto3(i) ((i) == ds3 || (i) == pnt3 || (i) == stall3)

#endif
