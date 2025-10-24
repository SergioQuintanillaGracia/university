/*********************************************************************
 *
 * Departamento de Inform·tica de Sistema y Computadores (DISCA)
 * Universidad PolitÈcnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *	   Pedro López (plopez@disca.upv.es)
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

#ifndef main_multicicloH
#define main_multicicloH


/***** Includes ***********************************************/
/*** Includes ********************************************************/

#include <stdio.h>
#include "tipos.h"
#include "estado.h"
#include "enlazador.h"


/*** Inclusion condicional *************************************************/

/*** Definiciones y macros */

#define MAX_FP_LAT   10
#define REG_FPSR   TAM_REGISTROS
#define NUM_REG_VISIBLES 16

#define _BRANCHES \
    case RV32I_BEQ: \
    case RV32I_BNE: \
    case RV32I_BLT: \
    case RV32I_BLTU: \
    case RV32I_BGE: \
    case RV32I_BGEU:

#define _LOADS_INT \
    case RV64I_LD: \
    case RV32I_LW: \
    case RV32I_LH: \
    case RV32I_LB: \
    case RV64I_LWU: \
    case RV32I_LHU: \
    case RV32I_LBU:

#define _STORES_INT \
    case RV64I_SD: \
    case RV32I_SW: \
    case RV32I_SH: \
    case RV32I_SB: \


#define _LOADS_FP \
    case RV32F_FLW: \
    case RV32D_FLD: \
    case OP_FP_L_PS:

#define _STORES_FP \
    case RV32F_FSW: \
    case RV32D_FSD: \
    case OP_FP_S_PS:

#define _MULS_INT \
    case RV32M_MUL: \
    case RV32M_MULH: \
    case RV32M_MULHSU: \
    case RV32M_MULHU: \
    case RV32M_DIV: \
    case RV32M_DIVU: \
    case RV32M_REM: \
    case RV32M_REMU:

#define _MULS_INTW \
    case RV64M_MULW: \
    case RV64M_DIVW: \
    case RV64M_DIVUW: \
    case RV64M_REMW: \
    case RV64M_REMUW: \

#define _ADDS_FP_S \
    case RV32F_FADD_S: \
    case RV32F_FSUB_S: \
    case RV32F_FSGNJ_S: \
    case RV32F_FSGNJN_S: \
    case RV32F_FSGNJX_S: \
    case RV32F_FMIN_S: \
    case RV32F_FMAX_S: \


#define _ADDS_FP_D \
    case RV32D_FADD_D: \
    case RV32D_FSUB_D: \
    case RV32D_FSGNJ_D: \
    case RV32D_FSGNJN_D: \
    case RV32D_FSGNJX_D: \
    case RV32D_FMIN_D: \
    case RV32D_FMAX_D:

#define _MULS_FP_S \
    case RV32F_FMADD_S: \
    case RV32F_FMSUB_S: \
    case RV32F_FNMSUB_S: \
    case RV32F_FNMADD_S: \
    case RV32F_FMUL_S: \
    case RV32F_FDIV_S: \
    case RV32F_FSQRT_S:

#define _MULS_FP_D \
    case RV32D_FMADD_D: \
    case RV32D_FMSUB_D: \
    case RV32D_FNMSUB_D: \
    case RV32D_FNMADD_D: \
    case RV32D_FMUL_D: \
    case RV32D_FDIV_D: \
    case RV32D_FSQRT_D:

#define _CMP_FP_D \
    case RV32D_FEQ_D: \
    case RV32D_FLT_D: \
    case RV32D_FLE_D: \

#define _CMP_FP_S \
    case RV32F_FEQ_S: \
    case RV32F_FLT_S: \
    case RV32F_FLE_S: \

#define _CVT_FP_FP \
    case RV32D_FCVT_D_S: \
    case RV32D_FCVT_S_D: \

#define _CVT_INT_TO_FP_D \
    case RV32D_FCVT_D_W: \
    case RV32D_FCVT_D_WU: \
    case RV64D_FMV_D_X: \
    case RV64D_FCVT_D_L: \
    case RV64D_FCVT_D_LU:

#define _CVT_INT_TO_FP_S \
    case RV32F_FCVT_S_W: \
    case RV32F_FCVT_S_WU: \
    case RV32F_FMV_W_X: \
    case RV64F_FCVT_S_L: \
    case RV64F_FCVT_S_LU:

#define _CVT_FP_TO_INT \
    case RV32D_FCVT_W_D: \
    case RV32D_FCVT_WU_D: \
    case RV32F_FCVT_W_S: \
    case RV32F_FCVT_WU_S: \
    case RV32F_FMV_X_W: \
    case RV64F_FCVT_L_S: \
    case RV64F_FCVT_LU_S: \
    case RV64D_FCVT_L_D: \
    case RV64D_FCVT_LU_D:\
    case RV64D_FMV_X_D:

#define _CLASS \
    case RV32F_FCLASS_S: \
    case RV32D_FCLASS_D:

#define MAX_BUFFER_PREDIC       16
#define _TAM_BUFFER_PREDIC      2
#define _TAM_TABLA_BHSR1         2
#define _TAM_TABLA_BHSR2         2
#define MAX_BITS_PRED_LOCAL 8
#define MAX_TABLA_BHSR 16

#ifdef main_C
#else
#endif

/***************************************************************************/
/***************************************************************************/

#ifdef main_C   /* Privado para el fichero main.c */


/* Registros */
reg_fp_t Rfp[_TAM_REGISTROS];
//tipo_t Rfp_tipo[_TAM_REGISTROS];
//boolean Rfp_dirty[_TAM_REGISTROS];
reg_int_t Rint[_TAM_REGISTROS];
//tipo_t Rint_tipo[_TAM_REGISTROS ];
//boolean Rint_dirty[_TAM_REGISTROS ];


/* Opciones de visualizacion */
int SHOW_DIT = SI;
int SHOW_DATAPATH = SI;
int SHOW_REG = SI;
int SHOW_MEM = SI;
int SHOW_LOG = SI;
int SHOW_CONSOLE = SI;


/* Opciones de la línea de órdenes */
u_instruccion_t tipo_procesador;
riesgos_d_t solucion_riesgos_datos;
riesgos_c_t solucion_riesgos_control;
sal_t salida;
boolean html_merge;
int AJUSTAR_INT = NO;
boolean f_etiquetas;
int small_screen_flag;

/* Otras opciones */
boolean home_link_flag = 0;
char HOME_LINK[128];


/* Latencia de los operadores */
int FP_LS_LAT = 2;
int FP_ADD_LAT = 4;
int FP_MUL_LAT = 7;
int FP_CMP_LAT = 4;
int FP_CVT_LAT = 4;
int FP_MISC_LAT = 4;
int INT_MUL_LAT = 7;

/*** Registros inter-etapa para la segmentacion */
dword PCn, PC;

/* MIPS 5 etapas */
IF_ID_t IF_IDn, IF_ID;
ID_EX_t ID_EXn, ID_EX;
EX_MEM_t EX_MEMn, EX_MEM;
MEM_WB_t MEM_WBn, MEM_WB;
MEM_WB_t iWB;

/* ID a cada operador */
ID_FP_t ID_FP_LS, ID_FP_LSn;
ID_FP_t ID_FP_ADDn, ID_FP_ADD;
ID_FP_t ID_FP_MULn, ID_FP_MUL;
ID_FP_t ID_FP_CMPn, ID_FP_CMP;
ID_FP_t ID_FP_MISCn, ID_FP_MISC;
ID_FP_t ID_FP_CVTn, ID_FP_CVT;
ID_FP_t ID_INT_MULn, ID_INT_MUL;

/* Señales internas */
ID_t ID;
EX_t EX;
MEM_t MEM;
WB_t WB;

/* Operadores FP */
OPER_t FP_LS[MAX_FP_LAT], FP_LSn[MAX_FP_LAT],
    FP_ADD[MAX_FP_LAT], FP_ADDn[MAX_FP_LAT],
    FP_MUL[MAX_FP_LAT], FP_MULn[MAX_FP_LAT],
    FP_CMP[MAX_FP_LAT], FP_CMPn[MAX_FP_LAT],
    FP_MISC[MAX_FP_LAT], FP_MISCn[MAX_FP_LAT],
    FP_CVT[MAX_FP_LAT], FP_CVTn[MAX_FP_LAT];
OPER_t INT_MUL[MAX_FP_LAT], INT_MULn[MAX_FP_LAT];

/* Operadores a WB FP */
MULTI_WB_t FP_LS_WBn, FP_LS_WB,
    FP_ADD_WBn, FP_ADD_WB,
    FP_MUL_WBn, FP_MUL_WB,
    FP_CMP_WBn, FP_CMP_WB,
    FP_MISC_WBn, FP_MISC_WB,
    FP_CVT_WBn, FP_CVT_WB;
MULTI_WB_t INT_MUL_WBn, INT_MUL_WB;
MULTI_WB_t FP_WB;

/* Señales de control */
boolean MEMaEXalu_s;
boolean MEMaEXalu_i;
boolean MEMaEXcomp_s;
boolean MEMaEXcomp_i;
boolean WBaEXalu_s;
boolean WBaEXalu_i;
boolean WBaEXcomp_s;
boolean WBaEXcomp_i;
boolean WBaEXmem;
boolean WBaMEM;
boolean MEMaIDcomp_s;
boolean MEMaIDcomp_i;

boolean MEMaFP_LS1_s;
boolean WBaFP_LS1_s;
boolean MEMaiM1_i;
boolean WBaiM1_i;
boolean MEMaiM1_s;
boolean WBaiM1_s;
boolean MEMaFP_CVT1_s;
boolean WBaFP_CVT1_s;

boolean FP_WBaADD1_i;
boolean FP_WBaADD1_s;
boolean FP_WBaMUL1_i;
boolean FP_WBaMUL1_s;
boolean FP_WBaMUL1_3;
boolean FP_WBaFP_LS1_i;
boolean FP_WBaFP_LS2_i;
boolean FP_WBaCMP1_i;
boolean FP_WBaCMP1_s;
boolean FP_WBaMISC1_i;
boolean FP_WBaMISC1_s;


//FP_WBaIDcomp;

boolean RAW,
WAW,
estructural;

boolean IFstall = NO;
boolean IDstall = NO;
boolean EXstall = NO;
boolean IFnop = NO;
boolean IDnop = NO;
boolean EXnop = NO;
boolean MEMnop = NO;

uword Rint_writings = 0;
uword Rfp_writings = 0;


#endif

/***************************************************************************/
/***************************************************************************/

#ifndef main_C  /* Publico (excepto main.c) */


extern reg_int_t Rint[_TAM_REGISTROS];
extern reg_fp_t Rfp[_TAM_REGISTROS];

// Para el control del tipo de dato escrito
//extern tipo_t Rfp_tipo[_TAM_REGISTROS];
//extern tipo_t Rint_tipo[_TAM_REGISTROS];
//extern boolean Rfp_dirty[_TAM_REGISTROS];
//extern boolean Rint_dirty[_TAM_REGISTROS];


extern char CONSOLA[MAX_TEXTO_CONSOLA];
extern char LOG[MAX_TEXTO_LOG];

/* Opciones de visualizacion */
extern int SHOW_DIT;
extern int SHOW_DATAPATH;
extern int SHOW_REG;
extern int SHOW_MEM;
extern int SHOW_CONSOLE;
extern int SHOW_LOG;


/*** Registros inter-etapa para la segmentación */

extern dword PCn, PC;

extern IF_ID_t IF_IDn, IF_ID;
extern ID_EX_t ID_EXn, ID_EX;
extern EX_MEM_t EX_MEMn, EX_MEM;
extern MEM_WB_t MEM_WBn, MEM_WB;
extern MEM_WB_t iWB;

extern ID_FP_t ID_FP_LS, ID_FP_LSn,
        ID_FP_ADDn, ID_FP_ADD,
        ID_FP_MULn, ID_FP_MUL,
        ID_FP_CMPn, ID_FP_CMP,
        ID_FP_CVTn, ID_FP_CVT,
        ID_FP_MISCn, ID_FP_MISC,
        ID_INT_MULn, ID_INT_MUL;

/* Operadores FP */
extern OPER_t FP_LS[MAX_FP_LAT], FP_LSn[MAX_FP_LAT],
        FP_ADD[MAX_FP_LAT], FP_ADDn[MAX_FP_LAT],
        FP_MUL[MAX_FP_LAT], FP_MULn[MAX_FP_LAT],
        FP_CMP[MAX_FP_LAT], FP_CMPn[MAX_FP_LAT],
        FP_MISC[MAX_FP_LAT], FP_MISCn[MAX_FP_LAT],
        FP_CVT[MAX_FP_LAT], FP_CVTn[MAX_FP_LAT],
        INT_MUL[MAX_FP_LAT], INT_MULn[MAX_FP_LAT];


extern MULTI_WB_t FP_LS_WBn, FP_LS_WB,
        FP_ADD_WBn, FP_ADD_WB,
        FP_MUL_WBn, FP_MUL_WB,
        FP_CMP_WBn, FP_CMP_WB,
        FP_MISC_WBn, FP_MISC_WB,
        FP_CVT_WBn, FP_CVT_WB;
extern MULTI_WB_t INT_MUL_WBn, INT_MUL_WB;

// TODO Eliminar?
extern MULTI_WB_t FP_WB;

/* Señales internas */
extern ID_t ID;
extern EX_t EX;
extern MEM_t MEM;
extern WB_t WB;

/* Opciones de la línea de órdenes */
extern u_instruccion_t tipo_procesador;
extern riesgos_d_t solucion_riesgos_datos;
extern riesgos_c_t solucion_riesgos_control;
extern sal_t salida;
extern boolean html_merge;
extern int AJUSTAR_INT;
extern boolean f_etiquetas;
extern int small_screen_flag;

/* Otras opciones */
extern boolean home_link_flag;
extern char HOME_LINK[128];

/* Latencia de los operadores */
extern int FP_LS_LAT;
extern int FP_ADD_LAT;
extern int FP_MUL_LAT;
extern int FP_CMP_LAT;
extern int FP_MISC_LAT;
extern int FP_CVT_LAT;
extern int INT_MUL_LAT;


/* Señales de control */
extern boolean
MEMaEXalu_s,
        MEMaEXalu_i,
        MEMaEXcomp_s,
        MEMaEXcomp_i,
        WBaEXalu_s,
        WBaEXalu_i,
        WBaEXcomp_s,
        WBaEXcomp_i,
        WBaEXmem,
        WBaMEM,
        MEMaIDcomp_s,
        MEMaIDcomp_i,
        MEMaFP_LS1_s,
        WBaFP_LS1_s,
        MEMaiM1_i,
        WBaiM1_i,
        MEMaiM1_s,
        WBaiM1_s,
        MEMaFP_CVT1_s,
        WBaFP_CVT1_s;

extern boolean FP_WBaADD1_i;
extern boolean FP_WBaADD1_s;
extern boolean FP_WBaMUL1_i;
extern boolean FP_WBaMUL1_s;
extern boolean FP_WBaMUL1_3;
extern boolean FP_WBaFP_LS1_i;
extern boolean FP_WBaFP_LS2_i;
extern boolean FP_WBaCMP1_i;
extern boolean FP_WBaCMP1_s;
extern boolean FP_WBaMISC1_i;
extern boolean FP_WBaMISC1_s;
//FP_WBaIDcomp;


extern boolean RAW,
        WAW,
        estructural;

extern boolean IFstall,
        IDstall,
        EXstall,
        IFnop,
        IDnop,
        EXnop,
        MEMnop;

extern uword Rint_writings;
extern uword Rfp_writings;


/* Variables internas */


extern boolean SaltoEfectivo;

extern boolean final;

extern dword PC_inicial;


#endif

#endif