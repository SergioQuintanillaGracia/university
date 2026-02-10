/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *        Sergio Sáez (ssaez@disca.upv.es)
 *
 *
 * Fichero: riscV_fp.c
 *
 * Descripción:
 *   Contiene las etapas de la extensión multiciclo del RISCV
 *
 *
 ****************************************************************************/

#define riscv_fp_C

/*** Includes **************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <fenv.h>


/*** Definiciones Externas *************************************************/

#include "main.h"
#include "instrucciones.h"
#include "depurar.h"
#include "control.h"
#include "riscv_fp.h"
#include "log.h"
#include "memoria.h"

// Solo se usa para el multiplicador
// Cada operador tiene sus particularidades
#define OP_MULTICICLO3(tipo, operador, fase)  \
    /*************************************/ \
    /*  Variables locales                */ \
    /*************************************/ \
    valor_t aluin1, aluin2, aluin3; \
    /*************************************/ \
    /*  Cuerpo función                   */ \
    /*************************************/ \
    tipo##_WBa##operador##1_s = NO; \
    tipo##_WBa##operador##1_i = NO; \
    tipo##_WBa##operador##1_3 = NO; \
/* propagar datos */ \
    if (fase == 1) { \
        if (ID_##tipo##_##operador.iPC == -1) { \
            tipo##_##operador##n[2].iPC = ID_##tipo##_##operador.iPC; \
            tipo##_##operador##n[2].orden = ID_##tipo##_##operador.orden; \
            tipo##_##operador##n[2].IR = inst_vacia; \
            return; \
        } \
        /* multiplexores */ \
        aluin1 = mux_##tipo##_##operador##_sup(ID_##tipo##_##operador.Fa, FP_WB.ALUout); \
        aluin2 = mux_##tipo##_##operador##_inf(ID_##tipo##_##operador.Fb, FP_WB.ALUout); \
        aluin3 = mux_##tipo##_##operador##_3(ID_##tipo##_##operador.Fc, FP_WB.ALUout); \
        /* hacemos la operacion */ \
        tipo##_##operador##n[2].ALUout = operacion_ALU_##tipo##_##operador(ID_##tipo##_##operador.IR.codop, aluin1, aluin2, aluin3, ID_##tipo##_##operador.IR.rm); \
        /* propagar datos */ \
        tipo##_##operador##n[2].IR = ID_##tipo##_##operador.IR; \
        tipo##_##operador##n[2].iPC = ID_##tipo##_##operador.iPC; \
        tipo##_##operador##n[2].orden = ID_##tipo##_##operador.orden; \
    } else if (fase == tipo##_##operador##_LAT) { \
        if (tipo##_##operador[fase].iPC == -1) { \
            tipo##_##operador##_WBn.iPC = tipo##_##operador[fase].iPC; \
            tipo##_##operador##_WBn.orden = tipo##_##operador[fase].orden; \
            return; \
        } \
        tipo##_##operador##_WBn.ALUout = tipo##_##operador[fase].ALUout; \
        tipo##_##operador##_WBn.IR = tipo##_##operador[fase].IR; \
        tipo##_##operador##_WBn.iPC = tipo##_##operador[fase].iPC; \
        tipo##_##operador##_WBn.orden = tipo##_##operador[fase].orden; \
    } else { \
        if (tipo##_##operador[fase].iPC == -1) { \
            tipo##_##operador##n[fase + 1].iPC = tipo##_##operador[fase].iPC; \
            tipo##_##operador##n[fase + 1].orden = tipo##_##operador[fase].orden; \
            return; \
        } \
        tipo##_##operador##n[fase + 1].ALUout = tipo##_##operador[fase].ALUout; \
        tipo##_##operador##n[fase + 1].IR = tipo##_##operador[fase].IR; \
        tipo##_##operador##n[fase + 1].iPC = tipo##_##operador[fase].iPC; \
        tipo##_##operador##n[fase + 1].orden = tipo##_##operador[fase].orden; \
    } \

#define OP_MULTICICLO2(tipo, operador, fase)  \
    /*************************************/ \
    /*  Variables locales                */ \
    /*************************************/ \
    valor_t aluin1, aluin2; \
    /*************************************/ \
    /*  Cuerpo función                   */ \
    /*************************************/ \
    tipo##_WBa##operador##1_s = NO; \
    tipo##_WBa##operador##1_i = NO; \
            /* propagar datos */ \
    if (fase == 1) { \
        if (ID_##tipo##_##operador.iPC == -1) { \
            tipo##_##operador##n[2].iPC = ID_##tipo##_##operador.iPC; \
            tipo##_##operador##n[2].orden = ID_##tipo##_##operador.orden; \
            tipo##_##operador##n[2].IR = inst_vacia; \
            return; \
        } \
        /* multiplexores */ \
        aluin1 = mux_##tipo##_##operador##_sup(ID_##tipo##_##operador.Fa, FP_WB.ALUout); \
        aluin2 = mux_##tipo##_##operador##_inf(ID_##tipo##_##operador.Fb, FP_WB.ALUout); \
        /* hacemos la operacion */ \
        tipo##_##operador##n[2].ALUout = operacion_ALU_##tipo##_##operador(ID_##tipo##_##operador.IR.codop, aluin1, aluin2, ID_##tipo##_##operador.IR.rm); \
        /* propagar datos */ \
        tipo##_##operador##n[2].IR = ID_##tipo##_##operador.IR; \
        tipo##_##operador##n[2].iPC = ID_##tipo##_##operador.iPC; \
        tipo##_##operador##n[2].orden = ID_##tipo##_##operador.orden; \
    } else if (fase == tipo##_##operador##_LAT) { \
        if (tipo##_##operador[fase].iPC == -1) { \
            tipo##_##operador##_WBn.iPC = tipo##_##operador[fase].iPC; \
            tipo##_##operador##_WBn.orden = tipo##_##operador[fase].orden; \
            return; \
        } \
        tipo##_##operador##_WBn.ALUout = tipo##_##operador[fase].ALUout; \
        tipo##_##operador##_WBn.IR = tipo##_##operador[fase].IR; \
        tipo##_##operador##_WBn.iPC = tipo##_##operador[fase].iPC; \
        tipo##_##operador##_WBn.orden = tipo##_##operador[fase].orden; \
    } else { \
        if (tipo##_##operador[fase].iPC == -1) { \
            tipo##_##operador##n[fase + 1].iPC = tipo##_##operador[fase].iPC; \
            tipo##_##operador##n[fase + 1].orden = tipo##_##operador[fase].orden; \
            return; \
        } \
        tipo##_##operador##n[fase + 1].ALUout = tipo##_##operador[fase].ALUout; \
        tipo##_##operador##n[fase + 1].IR = tipo##_##operador[fase].IR; \
        tipo##_##operador##n[fase + 1].iPC = tipo##_##operador[fase].iPC; \
        tipo##_##operador##n[fase + 1].orden = tipo##_##operador[fase].orden; \
    } \


#define MUX_FP_SUP(operador) \
    valor_t result; \
    FP##_WBa##operador##1_s = NO; \
    result = reg_val; \
    if (hay_destino_FP_WB() && hay_fuente1_FP_##operador##1() && (ID_FP_##operador.IR.rs1 == FP_WB.IR.rd)) { \
        result = wb; \
        FP##_WBa##operador##1_s = SI; \
        write_log_2("corto FP_WBa" #operador "1_s", ID_FP_##operador.iPC, FP_WB.iPC, "FP_#operador", "FP WB"); \
    } \
    return (result); \

#define MUX_FP_INF(operador) \
    valor_t result; \
    FP##_WBa##operador##1_i = NO; \
    result = reg_val;        \
    if (solucion_riesgos_datos != cortocircuito) \
        return result; \
    if (hay_destino_FP_WB() && hay_fuente2_FP_##operador##1() && (ID_FP_##operador.IR.rs2 == FP_WB.IR.rd)) { \
        result = wb; \
        FP##_WBa##operador##1_i = SI; \
        write_log_2("corto FP_WBa" #operador "1_i", ID_FP_##operador.iPC, FP_WB.iPC, "FP_" #operador, "FP WB"); \
    } \
    return (result); \

#define MUX_FP_3(operador) \
    valor_t result; \
    result = reg_val;      \
    if (solucion_riesgos_datos != cortocircuito) \
        return result; \
    FP##_WBa##operador##1_3 = NO; \
    if (hay_destino_FP_WB() && hay_fuente3_FP_##operador##1() && (ID_FP_##operador.IR.rs3 == FP_WB.IR.rd)) { \
        result = wb; \
        FP##_WBa##operador##1_3 = SI; \
        write_log_2("corto FP_WBa" #operador "1_3", ID_FP_##operador.iPC, FP_WB.iPC, "FP_" #operador, "FP WB"); \
    } \
    return (result); \
                           \
/*
#define SELECCIONA_OPERADOR(operador) \
   ID_Op = &ID_##operador; \
   Op = operador; \
   Op_n = operador##n; \
   Op_WB = &operador##_WB; \
   Op_WBn = &operador##_WBn; \
   OP_LAT = operador##_LAT;
*/

#define SELECCIONA_OPERADOR(operador) \
do {                                  \
    ID_Op = &ID_##operador;           \
    Op = operador;                    \
    Op_n = operador##n;               \
    Op_WBn = &operador##_WBn;         \
    OP_LAT = operador##_LAT;          \
} while(0)

#define BORRA_SENYALES_CORTOS(tipo, operador) \
    do {                                      \
    tipo##_WBa##operador##1_s = NO;           \
    tipo##_WBa##operador##1_i = NO;           \
} while (0)



/*** Variables Globales ****************************************************/

/*** Variables Locales *****************************************************/

/*** Funciones Locales *****************************************************/

/**
 *  Indica el tipo de numero en coma flotante
 *
 * @param num Numero a analizar
 */
int fp_classify(float num) {
    int clase = fpclassify(num);
    // Negativo
    if (signbit(num) != 0) {
        if (clase == FP_INFINITE)
            return 0x1 << 0;
        else if (clase == FP_NORMAL)
            return 0x1 << 1;
        else if (clase == FP_SUBNORMAL)
            return 0x1 << 2;
        else if (clase == FP_ZERO)
            return 0x1 << 3;
        else if (clase == FP_NAN)
            return 0x1 << 9;
    } else {
        if (clase == FP_INFINITE)
            return 0x1 << 7;
        else if (clase == FP_NORMAL)
            return 0x1 << 6;
        else if (clase == FP_SUBNORMAL)
            return 0x1 << 5;
        else if (clase == FP_ZERO)
            return 0x1 << 4;
        else if (clase == FP_NAN)
            return 0x1 << 9;
    }

    // Para evitar warnings
    return 0;
}

/**
 *  Configura el redondeo
 *
 * @param RM Tipo de redondeo
 */
void config_Redondeo(int RM) {
    switch (RM) {
        case RNE:
            fesetround(FE_TONEAREST);
            break;
        case RTZ:
            fesetround(FE_TOWARDZERO);
            break;
        case RDN:
            fesetround(FE_DOWNWARD);
            break;
        case RUP:
            fesetround(FE_UPWARD);
            break;
        case RMM:
            /* Not implemented */
            fesetround(FE_TONEAREST);

            break;
        default:
            break;
    }
}

/*** Funciones Exportables *************************************************/

/**
 * Realiza la operacion en la ALU INT
 *
 * @param codop Indice de la operacion
 * @param in1 Operando1
 * @param in2 Operando2
 *
 */
dword operacion_ALU_INT_MUL(int codop, dword in1, dword in2) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    xword result = 0;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    switch (codop) {
        case RV32M_MUL:
#if (XLEN == 32)
            result = ((xword) in1 * (xword) in2) & UINT32_MAX;
#else
            result = ((xword) in1 * (xword) in2) & UINT64_MAX;
#endif
            break;
        case RV32M_MULH:
#if (XLEN == 32)
            result = ((((dword) in1 * (dword) in2)) >> XLEN) & UINT32_MAX;
#else
            result = ((((qword) in1 * (qword) in2)) >> XLEN) & UINT64_MAX;
#endif
            break;
        case RV32M_MULHU:
#if (XLEN == 32)
            result = (((udword) ((udword) in1 * (udword) in2)) >> XLEN) & UINT32_MAX;
#else
            result = ((((uqword) (uqword) in1 * (uqword) in2)) >> XLEN) & UINT64_MAX;
#endif
            break;
        case RV32M_MULHSU:
#if (XLEN == 32)
            result = ((((dword) in1 * (udword) in2)) >> XLEN) & UINT32_MAX;
#else
            result = ((((qword) in1 * (uqword) in2)) >> XLEN) & UINT64_MAX;
#endif
            break;
        case RV32M_DIV:
            result = in1 / in2;
            break;
        case RV32M_DIVU:
            result = ((udword) in1 / (udword) in2);
            break;
        case RV32M_REM:
            result = in1 % in2;
            break;
        case RV32M_REMU:
            result = ((udword) in1 % (udword) in2);
            break;
        case RV64M_MULW:
            result = ((dword) in1 * (dword) in2) & UINT32_MAX;
            break;
        case RV64M_DIVW:
            result = (dword) in1 / in2;
            break;
        case RV64M_DIVUW:
            result = (dword) ((udword) in1 / (udword) in2);
            break;
        case RV64M_REMW:
            result = (dword) in1 % in2;
            break;
        case RV64M_REMUW:
            result = (dword) ((udword) in1 % (udword) in2);
            break;
    }
    fnDebug(NO, "operacion MUL INT codop %d in1 %ld in2 %ld result %ld\n", codop, in1, in2, result);

    return result;
}

/**
 * Realiza la operacion en la ALU FP
 *
 * @param codop Indice de la operacion
 * @param in1 Operando1
 * @param in2 Operando2
 * @param in3 No utilizado
 *
 * @param redondeo Metodo de redondeo
 */
valor_t operacion_ALU_FP_ADD(int codop, valor_t in1, valor_t in2, int redondeo) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;
    int s1, s2;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    switch (codop) {
        case RV32F_FADD_S:
            config_Redondeo(redondeo);
            result.fp_s = in1.fp_s + in2.fp_s;
            break;
        case RV32F_FSUB_S:
            config_Redondeo(redondeo);
            result.fp_s = in1.fp_s - in2.fp_s;
            break;
        case RV32F_FSGNJ_S:
            if (in2.fp_s >= 0)
                result.fp_s = fabs(in1.fp_s);
            else
                result.fp_s = -fabs(in1.fp_s);
            break;
        case RV32F_FSGNJN_S:
            if (in2.fp_s >= 0)
                result.fp_s = -fabs(in1.fp_s);
            else
                result.fp_s = fabs(in1.fp_s);
            break;
        case RV32F_FSGNJX_S:
            s1 = (in1.fp_s >= 0.0);
            s2 = (in2.fp_s >= 0.0);
            if (s1 ^ s2)
                result.fp_s = -fabs(in1.fp_s);
            else
                result.fp_s = fabs(in1.fp_s);
            break;
        case RV32F_FMIN_S:
            if (in1.fp_s < in2.fp_s)
                result.fp_s = in1.fp_s;
            else
                result.fp_s = in2.fp_s;
            break;
        case RV32F_FMAX_S:
            if (in1.fp_s > in2.fp_s)
                result.fp_s = in1.fp_s;
            else
                result.fp_s = in2.fp_s;
            break;

            /* Esto ya no se hace en este operador     
                    case RV32F_FMV_W_X:
                    //result.fp_s = in1.int_d;
                    result.fp_s = in1.fp_s;
                    break;

            case RV32F_FCVT_S_W:
                config_Redondeo(redondeo);
                result.fp_s = (float) in1.int_d;
                break;
            case RV32F_FCVT_S_WU:
                config_Redondeo(redondeo);
                result.fp_s = (float) (uword) in1.int_d;
                break;
             */
        case RV32D_FADD_D:
            config_Redondeo(redondeo);
            result.fp_d = in1.fp_d + in2.fp_d;
            break;
        case RV32D_FSUB_D:
            config_Redondeo(redondeo);
            result.fp_d = in1.fp_d - in2.fp_d;
            break;
        case RV32D_FSGNJ_D:
            if (in2.fp_d >= 0)
                result.fp_d = fabs(in1.fp_d);
            else
                result.fp_d = -fabs(in1.fp_d);
            break;
        case RV32D_FSGNJN_D:
            if (in2.fp_d >= 0)
                result.fp_d = -fabs(in1.fp_d);
            else
                result.fp_d = fabs(in1.fp_d);
            break;
        case RV32D_FSGNJX_D:
            s1 = (in1.fp_d >= 0.0);
            s2 = (in2.fp_d >= 0.0);
            if (s1 ^ s2)
                result.fp_d = -fabs(in1.fp_d);
            else
                result.fp_d = fabs(in1.fp_d);
            break;
        case RV32D_FMIN_D:
            if (in1.fp_d < in2.fp_d)
                result.fp_d = in1.fp_d;
            else
                result.fp_d = in2.fp_d;
            break;
        case RV32D_FMAX_D:
            if (in1.fp_d > in2.fp_d)
                result.fp_d = in1.fp_d;
            else
                result.fp_d = in2.fp_d;
            break;
        case RV32D_FCVT_S_D:
                config_Redondeo(redondeo);
                result.fp_s = (float) in1.fp_d;
                break;
            case RV32D_FCVT_D_S:
                config_Redondeo(redondeo);
                result.fp_d = (double) in1.fp_s;
                break;
            /* Esto ya no se hace en este operador   
            case RV32D_FCVT_D_W:
                config_Redondeo(redondeo);
                result.fp_d = (double) in1.int_d;
                break;
            case RV32D_FCVT_D_WU:
                config_Redondeo(redondeo);
                result.fp_d = (double) ((uword) in1.int_d);
                break;
            case RV64F_FCVT_S_L:
                config_Redondeo(redondeo);
                result.fp_s = (float) in1.int_d;
                break;
            case RV64F_FCVT_S_LU:
                config_Redondeo(redondeo);
                result.fp_s = (float) ((udword) in1.int_d);
                break;
            case RV64D_FCVT_D_L:
                config_Redondeo(redondeo);
                result.fp_d = (double) in1.int_d;
                break;
            case RV64D_FCVT_D_LU:
                config_Redondeo(redondeo);
                result.fp_d = (double) ((udword) in1.int_d);
                break;
            case RV64D_FMV_D_X:
                // result.fp_d = in1.int_d;
                result.fp_d = in1.fp_d;
                break;
             */
        case OP_FP_ADD_PS:
            result.fp_ps[Lo] = in1.fp_ps[Lo] + in2.fp_ps[Lo];
            result.fp_ps[Hi] = in1.fp_ps[Hi] + in2.fp_ps[Hi];
            break;
        case OP_FP_SUB_PS:
            result.fp_ps[Lo] = in1.fp_ps[Lo] - in2.fp_ps[Lo];
            result.fp_ps[Hi] = in1.fp_ps[Hi] - in2.fp_ps[Hi];

            break;

        default:
        fnError("Operacion FP %d no implementada\n\n", codop)
            //fprintf(stderr,
            //        "ERROR (%d %d): Operacion FP no implementada\n",
            //        __LINE__, codop);
            //exit(1);
    } /* endswitch */

    return (result);
}

/**
 * Realiza la operacion en la ALU FP
 *
 * @param codop Indice de la operacion
 * @param in1 Operando1
 * @param in2 Operando2
 * @param in3 Operando3
 * @param redondeo Metodo de redondeo
 */
valor_t operacion_ALU_FP_MUL(int codop, valor_t in1, valor_t in2, valor_t in3, int redondeo) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    switch (codop) {
        case RV32F_FMUL_S:
            config_Redondeo(redondeo);
            result.fp_s = (in1.fp_s * in2.fp_s);
            /* Falta los rounding modes */
            break;
        case RV32F_FDIV_S:
            config_Redondeo(redondeo);
            result.fp_s = (in1.fp_s / in2.fp_s);
            break;
        case RV32F_FSQRT_S:
            config_Redondeo(redondeo);
            result.fp_s = sqrt(in1.fp_s);
            break;
        case RV32F_FMADD_S:
            config_Redondeo(redondeo);
            result.fp_s = (in1.fp_s * in2.fp_s) + in3.fp_s;
            break;
        case RV32F_FMSUB_S:
            config_Redondeo(redondeo);
            result.fp_s = (in1.fp_s * in2.fp_s) - in3.fp_s;
            break;
        case RV32F_FNMADD_S:
            config_Redondeo(redondeo);
            result.fp_s = -(in1.fp_s * in2.fp_s) - in3.fp_s;
            break;
        case RV32F_FNMSUB_S:
            config_Redondeo(redondeo);
            result.fp_s = -(in1.fp_s * in2.fp_s) + in3.fp_s;
            break;
        case RV32D_FMUL_D:
            config_Redondeo(redondeo);
            result.fp_d = (in1.fp_d * in2.fp_d);
            break;
        case RV32D_FDIV_D:
            config_Redondeo(redondeo);
            result.fp_d = (in1.fp_d / in2.fp_d);
            break;
        case RV32D_FSQRT_D:
            config_Redondeo(redondeo);
            result.fp_d = sqrt(in1.fp_d);
            break;
        case RV32D_FMADD_D:
            config_Redondeo(redondeo);
            result.fp_d = (in1.fp_d * in2.fp_d) + in3.fp_d;
            break;
        case RV32D_FMSUB_D:
            config_Redondeo(redondeo);
            result.fp_d = (in1.fp_d * in2.fp_d) - in3.fp_d;
            break;
        case RV32D_FNMADD_D:
            config_Redondeo(redondeo);
            result.fp_d = -(in1.fp_d * in2.fp_d) - in3.fp_d;
            break;
        case RV32D_FNMSUB_D:
            config_Redondeo(redondeo);
            result.fp_d = -(in1.fp_d * in2.fp_d) + in3.fp_d;
            break;

        case OP_FP_MUL_PS:
            result.fp_ps[Lo] = in1.fp_ps[Lo] * in2.fp_ps[Lo];
            result.fp_ps[Hi] = in1.fp_ps[Hi] * in2.fp_ps[Hi];
            break;
        case OP_FP_DIV_PS:
            result.fp_ps[Lo] = in1.fp_ps[Lo] / in2.fp_ps[Lo];
            result.fp_ps[Hi] = in1.fp_ps[Hi] / in2.fp_ps[Hi];

            break;
        default:
        fnError("Operacion FP %d no implementada\n\n", codop)
            //fprintf(stderr,
            //        "ERROR (%d %d): Operacion FP no implementada\n",
            //        __LINE__, codop);
            //exit(1);
    } /* endswitch */

    return (result);
} /* end operacion_ALU */

/**
 * Realiza la comparacion en la ALU FP
 *
 * @param codop Indice de la operacion
 * @param in1 Operando1
 * @param in2 Operando2
 * @param in2 Operando2
 * @param redondeo No utilizado
 */
valor_t operacion_ALU_FP_CMP(int codop, valor_t in1, valor_t in2, int redondeo) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/


    switch (codop) {

        case RV32D_FEQ_D:
            result.int_d = (in1.fp_d == in2.fp_d);
            break;
        case RV32D_FLT_D:
            result.int_d = (in1.fp_d < in2.fp_d);
            break;
        case RV32D_FLE_D:
            result.int_d = (in1.fp_d <= in2.fp_d);
            break;
        case RV32F_FEQ_S:
            result.int_d = (in1.fp_s == in2.fp_s);
            break;
        case RV32F_FLT_S:
            result.int_d = (in1.fp_s < in2.fp_s);
            break;
        case RV32F_FLE_S:
            result.int_d = (in1.fp_s <= in2.fp_s);

            break;
        default:
        fnError("Comparación FP %d no implementada\n", codop)
            //fprintf(stderr,
            //        "ERROR (%d %d): Comparacion FP no implementada\n",
            //        __LINE__, codop);
            //exit(1);
    } /* endswitch */
    //	fprintf(stdout,"DEBUG comparacion_ALU_FP (codop=%d) %f %f result=%d \n",codop,in1,in2,result);

    return (result);
}

/**
 * Realiza la operacion en la ALU MISC
 *
 * @param codop Indice de la operacion
 * @param in1 Operando1
 * @param in2 Operando2
 * @param redondeo Metodo de redondeo
 *
 */
valor_t operacion_ALU_FP_MISC(int codop, valor_t in1, valor_t in2, int redondeo) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    switch (codop) {
        case RV32F_FCVT_W_S:
            config_Redondeo(redondeo);
            result.int_d = lrintf(in1.fp_s);
            break;
        case RV32F_FCVT_WU_S:
            config_Redondeo(redondeo);
            result.int_d = (uword) lrintf(in1.fp_s);
            break;
        case RV32D_FCVT_W_D:
            config_Redondeo(redondeo);
            result.int_d = lrintf(in1.fp_d);
            break;
        case RV32D_FCVT_WU_D:
            config_Redondeo(redondeo);
            result.int_d = (uword) lrintf(in1.fp_d);
            break;
        case RV32F_FMV_X_W:
            result.int_d = (uword) in1.int_d;
            break;
        case RV64F_FCVT_L_S:
            config_Redondeo(redondeo);
            result.int_d = lrintf(in1.fp_s);
            break;
        case RV64F_FCVT_LU_S:
            config_Redondeo(redondeo);
            result.int_d = (udword) lrintf(in1.fp_s);
            break;
        case RV64D_FCVT_L_D:
            config_Redondeo(redondeo);
            result.int_d = lrintf(in1.fp_d);
            break;
        case RV64D_FCVT_LU_D:
            config_Redondeo(redondeo);
            result.int_d = (udword) lrintf(in1.fp_d);
            break;
        case RV64D_FMV_X_D:
            //result.int_d = in1.fp_d;
            result.int_d = in1.int_d;
            break;
        case RV32F_FCLASS_S:
            result.int_d = fp_classify(in1.fp_s);
            break;
        case RV32D_FCLASS_D:
            result.int_d = fp_classify(in1.fp_d);

            break;


        default:
        fnError("Operacion FP %d no implementada\n\n", codop)
            //fprintf(stderr,
            //        "ERROR (%d %d): Operacion FP no implementada\n",
            //        __LINE__, codop);
            //exit(1);
    } /* endswitch */

    return (result);
}

/**
 * Realiza la operacion en la ALU FP
 *
 * @param codop Indice de la operacion
 * @param in1 Operando1
 * @param in2 Operando2
 * @param in3 No utilizado
 *
 * @param redondeo Metodo de redondeo
 */
valor_t operacion_ALU_FP_CVT(int codop, valor_t in1, int redondeo) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    switch (codop) {
        case RV32F_FMV_W_X:
            //result.fp_s = in1.int_d;
            result.fp_s = in1.fp_s;
            break;
        case RV32F_FCVT_S_W:
            config_Redondeo(redondeo);
            result.fp_s = (float) in1.int_d;
            break;
        case RV32F_FCVT_S_WU:
            config_Redondeo(redondeo);
            result.fp_s = (float) (uword) in1.int_d;
            break;
        case RV32D_FCVT_D_W:
            config_Redondeo(redondeo);
            result.fp_d = (double) in1.int_d;
            break;
        case RV32D_FCVT_D_WU:
            config_Redondeo(redondeo);
            result.fp_d = (double) ((uword) in1.int_d);
            break;
        case RV64F_FCVT_S_L:
            config_Redondeo(redondeo);
            result.fp_s = (float) in1.int_d;
            break;
        case RV64F_FCVT_S_LU:
            config_Redondeo(redondeo);
            result.fp_s = (float) ((udword) in1.int_d);
            break;
        case RV64D_FCVT_D_L:
            config_Redondeo(redondeo);
            result.fp_d = (double) in1.int_d;
            break;
        case RV64D_FCVT_D_LU:
            config_Redondeo(redondeo);
            result.fp_d = (double) ((udword) in1.int_d);
            break;
        case RV64D_FMV_D_X:
            // result.fp_d = in1.int_d;
            result.fp_d = in1.fp_d;
            break;

        default:
        fnError("Operacion FP %d no implementada\n\n", codop)
            //fprintf(stderr,
            //        "ERROR (%d %d): Operacion FP no implementada\n",
            //        __LINE__, codop);
            //exit(1);
    } /* endswitch */

    return (result);
}

/**
 * Implementa el multiplexor superior LS
 *
 * @param ra Valor leido del RF
 * @param mm Valor que procede de la etapa MEM
 * @param wb Valor que procede de la etapa WB
 */
dword mux_FP_LS_sup(dword ra, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaFP_LS1_s = NO;
    MEMaFP_LS1_s = NO;

    result = ra;
    if (solucion_riesgos_datos != cortocircuito)
        return result;

    if (hay_destino_MEM() && hay_fuente1_LS1() && (ID_FP_LS.IR.rs1 == EX_MEM.IR.rd)) {
        MEMaFP_LS1_s = SI;
        result = mem;
        write_log_2("corto MEMaFP_LS1_s", ID_FP_LS.iPC, EX_MEM.iPC, "LS", "MEM");
        goto final;
    } /* endif */


    if (hay_destino_WB() && hay_fuente1_LS1() && (ID_FP_LS.IR.rs1 == iWB.IR.rd)) {
        WBaFP_LS1_s = SI;
        result = wb;
        write_log_2("corto WBaFP_LS1_s", ID_FP_LS.iPC, iWB.iPC, "LS", "WB");

    } /* endif */

    final:
    return (result);
} /* end mux_FP_LS_sup */

/**
 * Implementa el multiplexor inferior LS
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_LS_inf(valor_t reg_val, valor_t wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/


    FP_WBaFP_LS1_i = NO;

    result = reg_val;
    if (solucion_riesgos_datos != cortocircuito)
        return result;
    // hay_fuente2_LS1() es true solo en S.D
    // aun así, es redundante comprobarlo, ya que L.D no va a utilizar el valor devuelto

    if (hay_destino_FP_WB() && hay_fuente2_LS1() && (ID_FP_LS.IR.rs2 == FP_WB.IR.rd)) {
        FP_WBaFP_LS1_i = SI;
        result = wb;
        write_log_2("corto FP_WBaFP_LS1_i", ID_FP_LS.iPC, FP_WB.iPC, "FP LS", "FP WB");
    } /* endif */

    return (result);
} /* end mux_FP_LS_inf */

/**
 * Implementa el multiplexor inferior LS, etapa L2
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_LS2_inf(valor_t reg_val, valor_t wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    FP_WBaFP_LS2_i = NO;

    result = reg_val;
    if (solucion_riesgos_datos != cortocircuito)
        return result;
    if (hay_destino_FP_WB() && hay_fuente2_LS2() && (FP_LS[2].IR.rs2 == FP_WB.IR.rd)) {
        FP_WBaFP_LS2_i = SI;
        result = wb;
        write_log_2("corto FP_WBaFP_LS2_i", FP_LS[2].iPC, FP_WB.iPC, "FP LS", "FP WB");
    } /* endif */


    return (result);
} /* end mux_FP_LS2inf */




/**
 * Implementa el multiplexor superior FP ADD
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_ADD_sup(valor_t reg_val, valor_t wb) {

    MUX_FP_SUP(ADD);
}

/**
 * Implementa el multiplexor inferior FP ADD
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_ADD_inf(valor_t reg_val, valor_t wb) {

    MUX_FP_INF(ADD);
}

/**
 * Implementa el multiplexor superior FP CMP
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_CMP_sup(valor_t reg_val, valor_t wb) {

    MUX_FP_SUP(CMP);
}

/**
 * Implementa el multiplexor inferior FP CMP
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_CMP_inf(valor_t reg_val, valor_t wb) {

    MUX_FP_INF(CMP);
}

/**
 * Implementa el multiplexor superior FP MUL
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_MUL_sup(valor_t reg_val, valor_t wb) {

    MUX_FP_SUP(MUL);
}

/**
 * Implementa el multiplexor inferior FP MUL
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_MUL_inf(valor_t reg_val, valor_t wb) {

    MUX_FP_INF(MUL);
}

/**
 * Implementa el multiplexor inferior FP MUL
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_MUL_3(valor_t reg_val, valor_t wb) {

    MUX_FP_3(MUL);
}

/**
 * Implementa el multiplexor superior FP MISC
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_MISC_sup(valor_t reg_val, valor_t wb) {

    MUX_FP_SUP(MISC);
}
// TBD. Los operandos son FP y el resultado es INT. La macro NO SIRVE

/**
 * Implementa el multiplexor inferior FP MISC
 *
 * @param reg_val Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 */
valor_t mux_FP_MISC_inf(valor_t reg_val, valor_t wb) {

    MUX_FP_INF(MISC);
}


/**
 * Implementa el multiplexor superior INT MUL
 *
 * @param reg_val Valor leido del RF
 * @param mem Valor que procede de la etapa MEM
 * @param wb Valor que procede de la etapa WB
 */
dword mux_INT_MUL_sup(dword reg_val, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    MEMaiM1_s = NO;
    WBaiM1_s = NO;

    result = reg_val;
    if (solucion_riesgos_datos != cortocircuito)
        return result;

    /* MEMtoEX */
    if (hay_destino_MEM() && hay_fuente1_FP_MUL1() &&
        (ID_INT_MUL.IR.rs1 == EX_MEM.IR.rd)) {
        result = mem;

        MEMaiM1_s = SI;
        write_log_2("corto MEMaiM1_s", ID_INT_MUL.iPC, EX_MEM.iPC, "INT MUL", "MEM");
        goto final;
    } /* endif */

    /* WBtoEX */
    if (hay_destino_WB() && hay_fuente1_iM1() &&
            (ID_INT_MUL.IR.rs1 == iWB.IR.rd)) {
        result = wb;

        WBaiM1_s = SI;
        write_log_2("corto WBaiM1_s", ID_INT_MUL.iPC, iWB.iPC, "INT_MUL", "WB");
    } /* endif */

    final:

    return (result);
} /* end mux_FP_MULsup */

/**
 * Implementa el multiplexor inferior INT MUL
 *
 * @param reg_val Valor leido del RF
 * @param mem Valor que procede de la etapa MEM
 * @param wb Valor que procede de la etapa WB
 */
dword mux_INT_MUL_inf(dword reg_val, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/
    MEMaiM1_i = NO;
    WBaiM1_i = NO;

    result = reg_val; /* por defecto */
    if (solucion_riesgos_datos != cortocircuito)
        return result;

    /* MEMtoEX */
    if (hay_destino_MEM() && hay_fuente2_iM1() &&
        (ID_INT_MUL.IR.rs2 == EX_MEM.IR.rd)) {
        result = mem;

        MEMaiM1_i = SI;
        write_log_2("corto MEMaiM1_s", ID_INT_MUL.iPC, EX_MEM.iPC, "INT MUL", "MEM");
        goto final;
    } /* endif */

    /* WBtoEX */
    if (hay_destino_WB() && hay_fuente2_iM1() &&
            (ID_INT_MUL.IR.rs2 == iWB.IR.rd)) {
        result = wb;

        WBaiM1_i = SI;
        write_log_2("corto WBaiM1_s", ID_INT_MUL.iPC, iWB.iPC, "INT_MUL", "WB");
    } /* endif */

    final:

    return result;

} /* end mux_FP_MULinf */

/**
 * Implementa el multiplexor superior FP CVT INT->FP
 *
 * @param reg_val Valor leido del RF
 * @param mem Valor que procede de la etapa MEM
 * @param wb Valor que procede de la etapa WB
 */
dword mux_FP_CVT_sup(dword reg_val, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    MEMaFP_CVT1_s = NO;
    WBaFP_CVT1_s = NO;

    result = reg_val;
    if (solucion_riesgos_datos != cortocircuito)
        return result;

    /* MEMtoEX */
    if (hay_destino_MEM() && rs1_valido(ID_FP_CVT.IR) &&
        (ID_FP_CVT.IR.rs1 == EX_MEM.IR.rd)) {
        result = mem;

        MEMaFP_CVT1_s = SI;
        write_log_2("corto MEMaCVT_s", ID_FP_CVT.iPC, EX_MEM.iPC, "FP_CVT", "MEM");
        goto final;
    } /* endif */

    /* WBtoEX */
    if (hay_destino_WB() && rs1_valido(ID_FP_CVT.IR) &&
            (ID_FP_CVT.IR.rs1 == iWB.IR.rd)) {
        result = wb;

        WBaFP_CVT1_s = SI;
        write_log_2("corto WBaCVT_s", ID_FP_CVT.iPC, iWB.iPC, "FP_CVT", "WB");
    } /* endif */

    final:

    return (result);
} /* end mux_FP_MULsup */


/**
 * Implementa el multiplexor de banco de registros  FP
 *
 * @param fp_ls Valor procedende de FP LS
 * @param fp_add Valor procedende de FP ADD
 * @param fp_mul Valor procedende de FP MUL
 *
 */
MULTI_WB_t mux_FP_WB(MULTI_WB_t fp_ls, MULTI_WB_t fp_add, MULTI_WB_t fp_mul, MULTI_WB_t fp_cvt) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    MULTI_WB_t result;
    word count;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    count = 0;

    result.iPC = -1;
    result.orden = -1;
    result.IR = inst_vacia;
    result.ALUout.fp_d = 0.0;

    fnDebug(NO, "muxFPWB (%ld %ld %ld)\n", fp_ls.iPC, fp_add.iPC, fp_mul.iPC);

    //    if ((fp_ls.iPC != -1)&&(inst_load_fp(fp_ls.IR.codop))) {
    if (fp_ls.iPC != -1) {
        result.iPC = fp_ls.iPC;
        result.orden = fp_ls.orden;
        result.IR = fp_ls.IR;
        result.ALUout = fp_ls.ALUout;
        count++;
    };

    if (fp_add.iPC != -1) {
        result.iPC = fp_add.iPC;
        result.orden = fp_add.orden;
        result.IR = fp_add.IR;
        result.ALUout = fp_add.ALUout;
        count++;
    };

    if (fp_mul.iPC != -1) {
        result.iPC = fp_mul.iPC;
        result.orden = fp_mul.orden;
        result.IR = fp_mul.IR;
        result.ALUout = fp_mul.ALUout;
        count++;
    };

    if (fp_cvt.iPC != -1) {
        result.iPC = fp_cvt.iPC;
        result.orden = fp_cvt.orden;
        result.IR = fp_cvt.IR;
        result.ALUout = fp_cvt.ALUout;
        count++;
    };

    if (count > 1) {

        fnError("Dos o más instrucciones FP en WB\n")
        //fprintf(stderr, "ERROR (%d): Dos o mas instrucciones fp en WB\n", __LINE__);
        //exit(1);
    };


    return (result);
}

/**
 * Implementa el multiplexor de banco de registros  entero
 *
 * @param wb Valor procedende MEM/WB
 * @param fp_cmp Valor procedende de FP CMP
 * @param i_mul Valor procedende de INT MUL
 * @param misc Valor procedende de MISC
 */
MEM_WB_t mux_INT_WB(MEM_WB_t wb, MULTI_WB_t fp_cmp, MULTI_WB_t int_mul, MULTI_WB_t to_int) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    MEM_WB_t result;
    word count;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    count = 0;

    result.iPC = -1;
    result.orden = -1;
    result.IR = inst_vacia;
    result.ALUout = 0;

    fnDebug(NO, "muxiWB (%ld %ld %ld %ld)\n", wb.iPC, fp_cmp.iPC, int_mul.iPC, to_int.iPC);

    //    if ((fp_ls.iPC != -1)&&(inst_load_fp(fp_ls.IR.codop))) {
    if (wb.iPC != -1) {
        result = wb;
        count++;
    };

    if (fp_cmp.iPC != -1) {
        result.iPC = fp_cmp.iPC;
        result.orden = fp_cmp.orden;
        result.IR = fp_cmp.IR;
        result.ALUout = fp_cmp.ALUout.int_d;
        count++;
    };

    if (int_mul.iPC != -1) {
        result.iPC = int_mul.iPC;
        result.orden = int_mul.orden;
        result.IR = int_mul.IR;
        result.ALUout = int_mul.ALUout.int_d;
        count++;
    };

    if (to_int.iPC != -1) {
        result.iPC = to_int.iPC;
        result.orden = to_int.orden;
        result.IR = to_int.IR;
        result.ALUout = to_int.ALUout.int_d;
        count++;
    };

    if (count > 1) {

        fnError("Dos o más instrucciones INT en WB\n")
        //fprintf(stderr, "ERROR (%d): Dos o mas instrucciones fp en WB\n", __LINE__);
        //exit(1);
    };

    return result;
}

/**
 * Implementa FP LS
 *
 * @param i Fase
 */
void fase_FP_LS(short i) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t datos_mem, datos_mem_store;
    boolean dirty;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaFP_LS1_s = NO;
    MEMaFP_LS1_s = NO;

    FP_WBaFP_LS1_i = NO;
    if (i==2)
        FP_WBaFP_LS2_i = NO;

    /* propagar datos */

    if (i == 1) {
        if (ID_FP_LS.iPC == -1) {
            FP_LSn[2].iPC = ID_FP_LS.iPC;
            FP_LSn[2].orden = ID_FP_LS.orden;
            FP_LSn[2].IR = inst_vacia;           
            return;
        }

        /* copiar operandos */
        FP_LSn[2].Fa = ID_FP_LS.Fa;
        FP_LSn[2].Fb = mux_FP_LS_inf(ID_FP_LS.Fb, FP_WB.ALUout);
        //LSn[2].address = mux_FP_LSsup(ID_LS.Ra, EX_MEM.ALUout, MEM_WB.MEMout) + ID_LS.Imm;
        FP_LSn[2].address = mux_FP_LS_sup(ID_FP_LS.Ra, EX_MEM.ALUout, WB.data) + ID_FP_LS.Imm;
        FP_LSn[2].IR = ID_FP_LS.IR;
        FP_LSn[2].iPC = ID_FP_LS.iPC;
        FP_LSn[2].orden = ID_FP_LS.orden;
        //		fprintf (stdout, "DEBUG LS(%i) %d %d %f\n",i,Ciclo,LS[i+1].address,LS[i+1].Fb);
        return;
    }

    /* mux datos store en LS2 */
    datos_mem_store = i == 2 ? mux_FP_LS2_inf(FP_LS[i].Fb, FP_WB.ALUout) : FP_LS[i].Fb;

    if (i == FP_LS_LAT) {
        if (FP_LS[i].iPC == -1) {
            FP_LS_WBn.iPC = FP_LS[i].iPC;
            FP_LS_WBn.orden = FP_LS[i].orden;
            return;
        }


        FP_LS_WBn.IR = FP_LS[i].IR;
        FP_LS_WBn.iPC = FP_LS[i].iPC;
        FP_LS_WBn.orden = FP_LS[i].orden;
        switch (FP_LS[i].IR.codop) {
            case RV32D_FLD:
                datos_mem = lee_mem_datos(mmem, FP_LS[i].address, t_double, &dirty, FP_LS[i].iPC);
                FP_LS_WBn.ALUout.fp_d = datos_mem.fp_d;
                break;
            case RV32F_FLW:
                datos_mem = lee_mem_datos(mmem, FP_LS[i].address, t_float, &dirty, FP_LS[i].iPC);
                FP_LS_WBn.ALUout.fp_ps[Lo] = 0.0;
                FP_LS_WBn.ALUout.fp_ps[Hi] = 0.0;
                FP_LS_WBn.ALUout.fp_s = datos_mem.fp_s;
                break;
            case OP_FP_L_PS:
                datos_mem = lee_mem_datos(mmem, FP_LS[i].address, t_float, &dirty, FP_LS[i].iPC);
                FP_LS_WBn.ALUout.fp_ps[Lo] = datos_mem.fp_s;
                datos_mem = lee_mem_datos(mmem, FP_LS[i].address + 4, t_float, &dirty, FP_LS[i].iPC);
                FP_LS_WBn.ALUout.fp_ps[Hi] = datos_mem.fp_s;
                break;
            case RV32D_FSD:
                //datos_mem.fp_d = LS[i].Fb.fp_d;
                datos_mem.fp_d = datos_mem_store.fp_d;
                escribe_mem_datos(mmem, FP_LS[i].address, datos_mem, t_double, SI, FP_LS[i].iPC);
                // Stores no pasan por WB
                pasar_a_FP_LS_WB_NOP();
                estat.instrucciones++;
                estat.multiciclo++;

                break;
            case RV32F_FSW:
                //datos_mem.fp_s = LS[i].Fb.fp_s;
                datos_mem.fp_s = datos_mem_store.fp_s;
                escribe_mem_datos(mmem, FP_LS[i].address, datos_mem, t_float, SI, FP_LS[i].iPC);
                // Stores no pasan por WB
                pasar_a_FP_LS_WB_NOP();
                estat.instrucciones++;
                estat.multiciclo++;

                break;
            case OP_FP_S_PS:
                //datos_mem.fp_s = LS[i].Fb.fp_s;
                datos_mem.fp_s = datos_mem_store.fp_s;
                escribe_mem_datos(mmem, FP_LS[i].address, datos_mem, t_float, SI, FP_LS[i].iPC);
                //datos_mem.fp_s = LS[i].Fb.fp_ps[Hi];
                datos_mem.fp_s = datos_mem_store.fp_ps[Hi];
                escribe_mem_datos(mmem, FP_LS[i].address + 4, datos_mem, t_float, SI, FP_LS[i].iPC);
                // Stores no pasan por WB
                pasar_a_FP_LS_WB_NOP();
                estat.instrucciones++;
                estat.multiciclo++;

                break;
            default:
                break;
        } /* endswitch */
        //				LS_WBn.fpALUout = 0.0;
        //				fprintf (stdout, "DEBUG LS(%i) %d %d %f\n",i,Ciclo,LS[i].address,LS[i].Fb);

    } else {
        if (FP_LS[i].iPC == -1) {
            FP_LSn[i + 1].iPC = FP_LS[i].iPC;
            FP_LSn[i + 1].orden = FP_LS[i].orden;

            return;
        }


        FP_LSn[i + 1].Fa = FP_LS[i].Fa;
        //LSn[i + 1].Fb = LS[i].Fb;
        FP_LSn[i + 1].Fb = datos_mem_store;
        FP_LSn[i + 1].address = FP_LS[i].address;

        FP_LSn[i + 1].IR = FP_LS[i].IR;
        FP_LSn[i + 1].iPC = FP_LS[i].iPC;
        FP_LSn[i + 1].orden = FP_LS[i].orden;
    }
    //       fprintf (stdout, "A(%i) %f\n",i,A[i].ALUout);
} /* end fase_LS */


/**
 * Implementa FP ADD
 *
 * @param i Fase
 */
void fase_FP_ADD(short i) {

    OP_MULTICICLO2(FP, ADD, i);
}


/**
 * Implementa FP MUL
 *
 * @param i Fase
 */
void fase_FP_MUL(short i) {
    OP_MULTICICLO3(FP, MUL, i);
}

/**
     * Implementa INT MUL
     *
     * @param i Fase
     */
void fase_INT_MUL(short i) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword aluin1, aluin2;
    // dword aluin3;
    // int redondeo = 0;
    ID_FP_t *ID_Op;
    OPER_t *Op;
    OPER_t *Op_n;
    // MULTI_WB_t *Op_WB;
    MULTI_WB_t *Op_WBn;
    int OP_LAT;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/
    // Asigna las variables al operador correspondiente
    SELECCIONA_OPERADOR(INT_MUL);
    MEMaiM1_s = NO;
    WBaiM1_s = NO;
    MEMaiM1_i = NO;
    WBaiM1_i = NO;

    /* Ademas hay que:
     *  utilizar los multiplexores asociados al operador
     *  utilizar la funcion asociada al operador
     */

    /* fase 1 de la operacion */
    if (i == 1) {
        if (ID_Op->iPC == -1) {
            Op_n[2].iPC = ID_Op->iPC;
            Op_n[2].orden = ID_Op->orden;
            Op_n[2].IR = inst_vacia;
            return;
        }
        /* multiplexores */
        aluin1 = mux_INT_MUL_sup(ID_Op->Ra, EX_MEM.ALUout, WB.data);
        aluin2 = mux_INT_MUL_inf(ID_Op->Rb, EX_MEM.ALUout, WB.data);

        /* hacemos la operacion */
        // redondeo = ID_Op->IR.rm;
        Op_n[2].ALUout.int_d = operacion_ALU_INT_MUL(ID_Op->IR.codop, aluin1, aluin2);

        /* propagar datos */

        Op_n[2].IR = ID_Op->IR;
        Op_n[2].iPC = ID_Op->iPC;
        Op_n[2].orden = ID_Op->orden;

        /* ultima fase */
    } else if (i == OP_LAT) {
        if (Op[i].iPC == -1) {
            Op_WBn->iPC = Op[i].iPC;
            Op_WBn->orden = Op[i].orden;

            return;
        }

        Op_WBn->ALUout = Op[i].ALUout;
        Op_WBn->IR = Op[i].IR;
        Op_WBn->iPC = Op[i].iPC;
        Op_WBn->orden = Op[i].orden;

        /* fase intermedia */
    } else {
        if (Op[i].iPC == -1) {
            Op_n[i + 1].iPC = Op[i].iPC;
            Op_n[i + 1].orden = Op[i].orden;

            return;
        }

        Op_n[i + 1].ALUout = Op[i].ALUout;

        Op_n[i + 1].IR = Op[i].IR;
        Op_n[i + 1].iPC = Op[i].iPC;
        Op_n[i + 1].orden = Op[i].orden;
    }
}

/**
 * Implementa FP CMP
 *
 * @param i Fase
 */
void fase_FP_CMP(short i) {
    OP_MULTICICLO2(FP, CMP, i);
}

/**
     * Implementa FP CVT int --> FP
     *
     * @param i Fase
     */
void fase_FP_CVT(short i) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t aluin1;
    // dword aluin3;
    ID_FP_t *ID_Op;
    OPER_t *Op;
    OPER_t *Op_n;
    // MULTI_WB_t *Op_WB;
    MULTI_WB_t *Op_WBn;
    int OP_LAT;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/
    // Asigna las variables al operador correspondiente
    SELECCIONA_OPERADOR(FP_CVT);
    MEMaFP_CVT1_s = NO;
    WBaFP_CVT1_s = NO;

    /* Ademas hay que:
     *  utilizar los multiplexores asociados al operador
     *  utilizar la funcion asociada al operador
     */

    /* fase 1 de la operacion */
    if (i == 1) {
        if (ID_Op->iPC == -1) {
            Op_n[2].iPC = ID_Op->iPC;
            Op_n[2].orden = ID_Op->orden;
            Op_n[2].IR = inst_vacia;
            return;
        }
        /* multiplexores */
        aluin1.int_d = mux_FP_CVT_sup(ID_Op->Ra, EX_MEM.ALUout, WB.data);

        /* hacemos la operacion */
        // redondeo = ID_Op->IR.rm;
        Op_n[2].ALUout = operacion_ALU_FP_CVT(ID_Op->IR.codop, aluin1,  ID_Op->IR.rm);

        /* propagar datos */

        Op_n[2].IR = ID_Op->IR;
        Op_n[2].iPC = ID_Op->iPC;
        Op_n[2].orden = ID_Op->orden;

        /* ultima fase */
    } else if (i == OP_LAT) {
        if (Op[i].iPC == -1) {
            Op_WBn->iPC = Op[i].iPC;
            Op_WBn->orden = Op[i].orden;

            return;
        }

        Op_WBn->ALUout = Op[i].ALUout;
        Op_WBn->IR = Op[i].IR;
        Op_WBn->iPC = Op[i].iPC;
        Op_WBn->orden = Op[i].orden;

        /* fase intermedia */
    } else {
        if (Op[i].iPC == -1) {
            Op_n[i + 1].iPC = Op[i].iPC;
            Op_n[i + 1].orden = Op[i].orden;

            return;
        }

        Op_n[i + 1].ALUout = Op[i].ALUout;

        Op_n[i + 1].IR = Op[i].IR;
        Op_n[i + 1].iPC = Op[i].iPC;
        Op_n[i + 1].orden = Op[i].orden;
    }
}


/* Implementa MISC
 *
 * @param i Fase
 */
void fase_FP_MISC(short i) {
    OP_MULTICICLO2(FP, MISC, i);
}


/**
 *  Detecta los riesgos RAW FP, activando las señales IDstall, IFstall en su caso
 *
 */
void detectar_riesgos_RAW_FP(void) {
    switch (solucion_riesgos_datos) {
        case parada:
            /* Riesgos RAW con la dirección */
            switch (IF_ID.IR.codop) {
                _LOADS_FP
                _STORES_FP
                    /* Riesgo entre EX e ID */
                    if (hay_destino_EX() && (IF_ID.IR.rs1 == ID_EX.IR.rd)) {
                        IFstall = SI;
                        IDstall = SI;
                        write_log_2("IDstall, IFstall", IF_ID.iPC, ID_EX.iPC, "ID", "EX");
                        break;
                    } /* endif */

                    /* Riesgo entre MEM e ID */
                    if (hay_destino_MEM() && (IF_ID.IR.rs1 == EX_MEM.IR.rd)) {
                        IFstall = SI;
                        IDstall = SI;
                        write_log_2("IDstall, IFstall", IF_ID.iPC, EX_MEM.iPC, "ID", "MEM");
                    } /* endif */
                    break;
                default:
                    break;
            };

            /* Riesgos RAW  */
            switch (IF_ID.IR.codop) {
                _LOADS_FP

                    break;
                _STORES_FP

                    if (Rfp[IF_ID.IR.rs2].busy != 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1f("RAWi2, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rs2);
                    };

                    break;
                default: // Este código es genérico y debería cubrir también el caso previo
                    if (inst_fp_Rfte1(IF_ID.IR.codop) && Rfp[IF_ID.IR.rs1].busy != 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1f("RAWi1, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rs1);
                    };

                    if (inst_fp_Rfte2(IF_ID.IR.codop) && Rfp[IF_ID.IR.rs2].busy != 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1f("RAWi2, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rs2);
                    };

                    if (inst_fp_Rfte3(IF_ID.IR.codop) && Rfp[IF_ID.IR.rs3].busy != 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1f("RAWi3, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rs3);
                    };

                    break;
            };
            break;

        case cortocircuito:
            /* Riesgos RAW */
            //sprintf(_msg, "RAW Ciclo=%d fte1=%d Busy=%d\n", Ciclo, IF_ID.IR.rs1, Rfp[IF_ID.IR.rs1].busy);
            //DEBUG(__func__);

            //                fprintf(stdout, "DEBUG
            /* Riesgos RAW con la dirección */
            switch (IF_ID.IR.codop) {
                _LOADS_FP
                _STORES_FP
                    /* Riesgo entre EX e ID */
                    if (inst_int_load(ID_EX.IR.codop) && (IF_ID.IR.rs1 == ID_EX.IR.rd)) {
                        IFstall = SI;
                        IDstall = SI;
                        write_log_2("IDstall, IFstall", IF_ID.iPC, ID_EX.iPC, "ID", "EX");
                    } /* endif */
                    /* Riesgo entre MEM e ID */
                    if (inst_int_load(EX_MEM.IR.codop) && (IF_ID.IR.rs1 == EX_MEM.IR.rd)) {
                        IFstall = SI;
                        IDstall = SI;
                        write_log_2("IDstall, IFstall", IF_ID.iPC, EX_MEM.iPC, "ID", "MEM");
                    } /* endif */
                    break;
                default:
                    break;
            };

            /* Riesgos RAW  */
            switch (IF_ID.IR.codop) {
                _LOADS_FP
                    break;
                _STORES_FP
                            // Rfp[] contiene el ciclo en que hara WB
                    if ((int) (Rfp[IF_ID.IR.rs2].busy - 1 - Ciclo) > 1) { // mux datos store en LS2
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1f("RAWL, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rs2);
                        //fprintf(stdout, "DEBUG RAW fte2 Ciclo=%d fte2=%d Busy=%d\n",Ciclo,IF_ID.IR.rs2,Rfp[IF_ID.IR.rs2].busy);
                    };
                    break;

                default: // Este código es genérico y debería cubrir también el caso previo
                    if (inst_fp_Rfte1(IF_ID.IR.codop) && (int) (Rfp[IF_ID.IR.rs1].busy - 1 - Ciclo) > 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1f("RAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rs1);
                        //fprintf(stdout, "DEBUG RAW Ciclo=%d fte1=%d Busy=%d\n",Ciclo,IF_ID.IR.rs1,Rfp[IF_ID.IR.rs1].busy);
                    };
                    if (inst_fp_Rfte2(IF_ID.IR.codop) && (int) (Rfp[IF_ID.IR.rs2].busy - 1 - Ciclo) > 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1f("RAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rs2);
                        //fprintf(stdout, "DEBUG RAW fte2 Ciclo=%d fte2=%d Busy=%d\n",Ciclo,IF_ID.IR.rs2,Rfp[IF_ID.IR.rs2].busy);
                    };
                    if (inst_fp_Rfte3(IF_ID.IR.codop) && (int) (Rfp[IF_ID.IR.rs3].busy - 1 - Ciclo) > 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1f("RAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rs3);
                        //fprintf(stdout, "DEBUG RAW fte2 Ciclo=%d fte2=%d Busy=%d\n",Ciclo,IF_ID.IR.rs2,Rfp[IF_ID.IR.rs2].busy);
                    };
                    break;
            };
        default:;
    } /* end switch */
}

/**
 *  Detecta los riesgos WAW FP, activando las señales IDstall, IFstall en su caso
 *
 */
void detectar_riesgos_WAW_FP(void) {

    switch (IF_ID.IR.codop) {

        _LOADS_FP
            /* Riesgos WAW */
            if (Rfp[IF_ID.IR.rd].busy >= Ciclo + FP_LS_LAT + 1) {
                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1f("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rd);
            };
            break;

        _ADDS_FP_D
        _ADDS_FP_S
        _CVT_FP_FP
        _CVT_INT_TO_FP_S
        _CVT_INT_TO_FP_D
            /* Riesgos WAW */
            if (Rfp[IF_ID.IR.rd].busy >= Ciclo + FP_ADD_LAT + 1) {
                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1f("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rd);
            };
            break;

        _MULS_FP_S
        _MULS_FP_D
            /* Riesgos WAW */
            if (Rfp[IF_ID.IR.rd].busy >= Ciclo + FP_MUL_LAT + 1) {

                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1f("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rd);
            };
            break;

        default:;
    };
}

/**
 *  Detecta los riesgos estructurales en WB, activando las señales IDstall, IFstall en su caso
 *
 */
void detectar_riesgos_WB_FP(void) {
    int i;

    /* Riesgos estructurales WB, con cualquier registro  */
    switch (IF_ID.IR.codop) {

        _LOADS_FP
            i = 1 << (FP_LS_LAT + 1);
            if ((Rfp_writings & i) == i) {
                IDstall = SI;
                IFstall = SI;
                estructural = SI;
                write_log("Estructural_WBfp IDstall, IFstall", IF_ID.iPC);
                fnDebug(NO, " WBconflict codop %d i=%d; Rfp_writings=%x\n", IF_ID.IR.codop, i, Rfp_writings);

            }
            break;

        _ADDS_FP_S
        _ADDS_FP_D
        _CVT_FP_FP
        _CVT_INT_TO_FP_S
        _CVT_INT_TO_FP_D
            //				fprintf (stdout, "DEBUG ID FP_ADD %d \n",Ciclo);
            i = 1 << (FP_ADD_LAT + 1);
            if ((Rfp_writings & i) == i) {
                IDstall = SI;
                IFstall = SI;
                estructural = SI;
                write_log("Estructural_WBfp IDstall, IFstall", IF_ID.iPC);
                fnDebug(NO, " WBconflict codop %d i=%d; Rfp_writings=%x\n", IF_ID.IR.codop, i, Rfp_writings);
            }
            break;

        _MULS_FP_S
        _MULS_FP_D
            i = 1 << (FP_MUL_LAT + 1);
            if ((Rfp_writings & i) == i) {
                IDstall = SI;
                IFstall = SI;
                estructural = SI;
                write_log("Estructural_WBfp IDstall, IFstall", IF_ID.iPC);
                fnDebug(NO, " WBconflict codop %d i=%d; Rfp_writings=%x\n", IF_ID.IR.codop, i, Rfp_writings);
            }
            break;

        default:;

    };
}
