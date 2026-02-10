/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *        Sergio Sáez (ssaez@disca.upv.es)
 *
 * Fichero: control.c
 *
 * Descripción:
 *   Contiene las funciones XXnop y las comprobaciones de campos válidos en el IR
 *
 ****************************************************************************/


/*** Includes **************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


/*** Definiciones Externas *************************************************/

#include "depurar.h"
#include "tipos.h"
#include "main.h"
#include "instrucciones.h"
#include "presentacion-html.h"

#define pasar_NOP_fase1(operador, latencia ,tipo) \
    /* printf("pasar_NOP rd=%d %s\n", ID_##operador##n.IR.rd, __FUNCTION__); */  \
    ID_##operador##n.Fa.fp_d = 0; \
    ID_##operador##n.Fb.fp_d = 0; \
    ID_##operador##n.Ra = 0; \
    ID_##operador##n.Rb = 0; \
    ID_##operador##n.IR = inst_vacia; \
    ID_##operador##n.iPC = -1; \
    ID_##operador##n.orden = -1; \

#define pasar_NOP_fase2(operador, latencia, tipo) \
    /* printf("pasar_NOP2 rd=%d %s\n", operador##n[2].IR.rd, __FUNCTION__); */ \
    operador##n[2].ALUout.fp_d = 0; \
    operador##n[2].Fa.fp_d = 0; \
    operador##n[2].Fb.fp_d = 0; \
    operador##n[2].Fc.fp_d = 0; \
    operador##n[2].ALUout.int_d = 0; \
    operador##n[2].Ra.int_d = 0; \
    operador##n[2].Rb.int_d = 0; \
    operador##n[2].address = 0; \
    operador##n[2].IR = inst_vacia; \
    operador##n[2].iPC = -1; \
    operador##n[2].orden = -1; \
    
#define cancelar_ID_multi(operador, latencia, tipo) \
    /* printf("pasar_NOP2 rd=%d %s\n", operador##n[2].IR.rd, __FUNCTION__); */ \
    /* Si hay una operación en curso */ \
    if (ID##_##operador##n.iPC != -1) { \
        if (R##tipo[ID##_##operador##n.IR.rd].busy == Ciclo + latencia +1) { \
        /* Liberar registro, si es nuestro */ \
        R##tipo[ID##_##operador##n.IR.rd].busy = 0; \
             } \
             /* Anular la escritura en x ciclos: */ \
             /* x = x & mask = ~(~x | ~mask) */ \
             nmask = 1 << (latencia + 1); \
             R##tipo##_writings = ~(~R##tipo##_writings | nmask); \
    } \
    
#define cancelar_EX_multi(operador, latencia, tipo) \
    /* printf("pasar_NOP2 rd=%d %s\n", operador##n[2].IR.rd, __FUNCTION__); */ \
    /* Si hay una operación en curso */ \
    if (operador##n[2].iPC != -1) { \
        if (R##tipo[operador##n[2].IR.rd].busy == Ciclo + latencia) { \
        /* Liberar registro, si es nuestro */ \
        R##tipo[operador##n[2].IR.rd].busy = 0; \
             } \
             /* Anular la escritura en x ciclos: */ \
             /* x = x & mask = ~(~x | ~mask) */ \
             nmask = 1 << (latencia); \
             R##tipo##_writings = ~(~R##tipo##_writings | nmask); \
    } \

/*** Variables Globales ****************************************************/

/*** Variables Locales *****************************************************/

/*** Funciones Exportables *************************************************/

/**
 * Envia un -nop- a la etapa ID
 *
 *
 */
void pasar_a_ID_NOP(
        ) {
    IF_IDn.IR = inst_vacia;
    IF_IDn.iPC = -1;
    IF_IDn.orden = -1;
}

/**
 * Envia un -nop- a la etapa EX
 *
 *
 */
void pasar_a_EX_NOP(
        ) {
    ID_EXn.IR = inst_vacia;
    ID_EXn.iPC = -1;
    ID_EXn.orden = -1;
}


/**
 * Envia un -nop- a la etapa MEM
 *
 *
 */
void pasar_a_MEM_NOP(
        ) {
    EX_MEMn.IR = inst_vacia;
    EX_MEMn.iPC = -1;
    EX_MEMn.orden = -1;
    EX_MEMn.cond = 0;
}



/**
 * Envia un -nop- a la etapa WB
 *
 *
 */
void pasar_a_WB_NOP(
        ) {
    MEM_WBn.IR = inst_vacia;
    MEM_WBn.iPC = -1;
    MEM_WBn.orden = -1;
}




/**
 * Envia un -nop- al operador LS, fase1
 *
 *
 */
void pasar_a_FP_LS1_NOP(
        ) {
    pasar_NOP_fase1(FP_LS, FP_LS_LAT, fp);
}

/**
 * Envia un -nop- al operador FP ADD, fase1
 *
 *
 */
void pasar_a_FP_A1_NOP(
        ) {
    pasar_NOP_fase1(FP_ADD, FP_ADD_LAT, fp);
}

/**
 * Envia un -nop- al operador FP MUL, fase1
 *
 *
 *
 *
 *
 */
void pasar_a_FP_M1_NOP(
        ) {
    pasar_NOP_fase1(FP_MUL, FP_MUL_LAT, fp);
}

/**
 * Envia un -nop- al operador iM, etapa 1
 *
 *
 */
void pasar_a_iM1_NOP(
        ) {
    pasar_NOP_fase1(INT_MUL, INT_MUL_LAT, int);
}

/**
 * Envia un -nop- al operador MISC, etapa 1
 *
 *
 */
void pasar_a_FP_MISC1_NOP(
        ) {
    pasar_NOP_fase1(FP_MISC, FP_MISC_LAT, int);
}

/**
 * Envia un -nop- al operador FP CMP, etapa 1
 *
 *
 */
void pasar_a_FP_C1_NOP(
        ) {
    pasar_NOP_fase1(FP_CMP, FP_CMP_LAT, fp);
}

/**
 * Envia un -nop- al operador FP CVT, etapa 1
 *
 *
 */
void pasar_a_FP_CVT1_NOP(
) {
    pasar_NOP_fase1(FP_CVT, FP_CVT_LAT, fp);
}

/**
 * Envia un -nop- al operador LS, etapa 2
 *
 *
 */
void pasar_a_FP_LS2_NOP(
        ) {
    pasar_NOP_fase2(FP_LS, FP_LS_LAT, fp);
}

/**
 * Envia un -nop- al operador FP ADD, etapa 2
 *
 *
 */
void pasar_a_FP_A2_NOP(
        ) {
    pasar_NOP_fase2(FP_ADD, FP_ADD_LAT, fp);
}

/**
 * Envia un -nop- al operador FP MUL, etapa 2
 *
 *
 */
void pasar_a_FP_M2_NOP(
        ) {
    pasar_NOP_fase2(FP_MUL, FP_MUL_LAT, fp);
}

/**
 * Envia un -nop- al operador FP CMP, etapa 2
 *
 *
 */
void pasar_a_FP_C2_NOP(
        ) {
    pasar_NOP_fase2(FP_CMP, FP_CMP_LAT, fp);
}

/**
 * Envia un -nop- al operador FP iMUL, etapa 2
 *
 *
 */
void pasar_a_iM2_NOP(
        ) {
    pasar_NOP_fase2(INT_MUL, INT_MUL_LAT, int);
}

/**
 * Envia un - nop - al operador FP MISC, etapa 2
 *
 *
 */
void pasar_a_FP_MISC2_NOP(
        ) {
    pasar_NOP_fase2(FP_MISC, FP_MISC_LAT, int);
}

/**
 * Envia un - nop - al operador FP MISC, etapa 2
 *
 *
 */
void pasar_a_FP_CVT2_NOP(
) {
    pasar_NOP_fase2(FP_CVT, FP_CVT_LAT, int);
}

/***************************************************************
 *
 * Func: pasar_a_ID_WB_NOP
 *
 * Desc: Se entrega un NOP a la etapa ID_WB al proximo ciclo
 *
 **************************************************************/
/*
 * void pasar_a_ID_WB_NOP(
        ) {
    ID_WBn.fpALUout.fp_d = 0.0;

    ID_WBn.IR = inst_vacia;
    ID_WBn.iPC = -1;
    ID_WBn.orden = -1;
}
 */

/**
 * Envia un -nop- al operador FP LS, etapa WB. Para los stores
 *
 *
 */
void pasar_a_FP_LS_WB_NOP(
        ) {

    FP_LS_WBn.ALUout.fp_d = 0.0;
    FP_LS_WBn.IR = inst_vacia;
    FP_LS_WBn.iPC = -1;
    FP_LS_WBn.orden = -1;
}

/**
 * Cancelar la instruccion que hay en ID
 *
 *
 */
void cancelar_en_ID(
        ) {
    uword nmask;
    fnDebug(NO, "ID rd=%u %d \n", ID_EXn.IR.rd, Rint[ID_EXn.IR.rd].busy);
    if (ID_EXn.iPC != -1) { \
        if (Rint[ID_EXn.IR.rd].busy == Ciclo + 3) {
            /* Con instrucciones de 5 etapas no usamos Rint[].busy */
            printf("ERROR. Esto no debe ejecutarse\n");
            printf("PC=%ld Ciclo=%u\n", PC, Ciclo);
            exit(1);
            /* Liberar registro, si es nuestro */
            Rint[ID_EXn.IR.rd].busy = 0;
        }
        /* Anular la escritura en x ciclos: */
        /* x = x & mask = ~(~x | ~mask) */
        nmask = 1 << (3);
        Rint_writings = ~(~Rint_writings | nmask);
    }

    cancelar_ID_multi(FP_LS, FP_LS_LAT, fp);
    cancelar_ID_multi(FP_ADD, FP_ADD_LAT, fp);
    cancelar_ID_multi(FP_CMP, FP_CMP_LAT, fp);
    cancelar_ID_multi(FP_MUL, FP_MUL_LAT, fp);
    cancelar_ID_multi(INT_MUL, INT_MUL_LAT, int);
    cancelar_ID_multi(FP_MISC, FP_MISC_LAT, int);
    cancelar_ID_multi(FP_CVT, FP_CVT_LAT, int);

    pasar_a_EX_NOP();
    pasar_a_FP_LS1_NOP();
    pasar_a_FP_A1_NOP();
    pasar_a_FP_C1_NOP();
    pasar_a_FP_M1_NOP();
    pasar_a_iM1_NOP();
    pasar_a_FP_MISC1_NOP();
    pasar_a_FP_CVT1_NOP();

}

/**
 * Cancelar la instruccion que hay en EX
 *
 *
 */
void cancelar_en_EX(
        ) {
    uword nmask;

    // Anular la escritura dentro de dos ciclos:
    // x = x & mask = ~(~x | ~masc) = ~(~x | nmask)
    if (EX_MEMn.iPC != -1) { \
        if (Rint[EX_MEMn.IR.rd].busy == Ciclo + 2) {
            printf("ERROR. Esto no debe ejecutarse\n");
            printf("PC=%ld Ciclo=%u\n", PC, Ciclo);
            exit(1);
            /* Liberar registro, si es nuestro */
            Rint[EX_MEMn.IR.rd].busy = 0;
        }
        /* Anular la escritura en (latencia-2) ciclos: */
        /* x = x & mask = ~(~x | ~mask) */
        nmask = 1 << (2);
        Rint_writings = ~(~Rint_writings | nmask);
    }

    cancelar_EX_multi(FP_LS, FP_LS_LAT, fp);
    cancelar_EX_multi(FP_ADD, FP_ADD_LAT, fp);
    cancelar_EX_multi(FP_CMP, FP_CMP_LAT, fp);
    cancelar_EX_multi(FP_MUL, FP_MUL_LAT, fp);
    cancelar_EX_multi(INT_MUL, INT_MUL_LAT, int);
    cancelar_EX_multi(FP_MISC, FP_MISC_LAT, int);
    cancelar_EX_multi(FP_CVT, FP_CVT_LAT, int);

    pasar_a_MEM_NOP();
    pasar_a_FP_LS2_NOP();
    pasar_a_FP_A2_NOP();
    pasar_a_FP_C2_NOP();
    pasar_a_FP_M2_NOP();
    pasar_a_iM2_NOP();
    pasar_a_FP_MISC2_NOP();
    pasar_a_FP_CVT2_NOP();
}


/**
 * Devuelve true si la instruccion tiene un Rfte1 valido
 *
 * @param inst Instruccion
 *
 */
static boolean hay_fuente1(instruccion_t inst) {
    return inst_int_Rfte1(inst.codop);
}

/**
 * Devuelve true si la instruccion tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
static boolean hay_fuente2(instruccion_t inst) {
    return inst_int_Rfte2(inst.codop);
}

/**
 * Devuelve true si la instruccion tiene un Rdst valido
 *
 * @param inst Instruccion
 *
 */
static boolean hay_destino(instruccion_t inst) {
    return inst_to_Rint(inst.codop);
}

/**
 * Devuelve true si la instruccion tiene un Rfte1 valido
 *
 * @param inst Instruccion
 *
 */
boolean rs1_valido(instruccion_t inst
) {
    return inst_int_Rfte1(inst.codop) && (inst.rs1 != 0);
}

/**
 * Devuelve true si la instruccion tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
boolean rs2_valido(instruccion_t inst
) {
    return inst_int_Rfte2(inst.codop) && (inst.rs2 != 0);
}

/**
 * Devuelve true si la instruccion tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
boolean rd_valido(instruccion_t inst
        ) {
    
    return inst_to_Rint(inst.codop)  && (inst.rd != 0);
}

/**
 * Devuelve true si la instruccion de la etapa ID tiene un Rfte1 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente1_ID(
        ) {
    return rs1_valido(IF_ID.IR);
}

/**
 * Devuelve true si la instruccion de la etapa ID tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente2_ID(
        ) {
    return rs2_valido(IF_ID.IR);
}

/**
 * Devuelve true si la instruccion de la etapa EX tiene un Rfte1 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente1_EX(
        ) {
    return rs1_valido(ID_EX.IR);
}

/**
 * Devuelve true si la instruccion de la etapa EX tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente2_EX(
        ) {
    return rs2_valido(ID_EX.IR);
}

/**
 * Devuelve true si la instruccion de la etapa EX tiene un Rfdst valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_destino_EX(
        ) {
    return rd_valido(ID_EX.IR);
}

/**
 * Devuelve true si la instruccion de la etapa MEM tiene un Rdst valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_destino_MEM(
        ) {
    return rd_valido(EX_MEM.IR);
}

/**
 * Devuelve true si la instruccion de la etapa WB tiene un Rdst valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_destino_WB(
        ) {
    return rd_valido(iWB.IR);
}

/**
 * Devuelve true si la instruccion tiene un Rfte1 valido
 *
 * @param inst Instruccion
 *
 */
boolean rfp1_valido(instruccion_t inst
) {
    return inst_fp_Rfte1(inst.codop);
}

/**
 * Devuelve true si la instruccion tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
boolean rfp2_valido(instruccion_t inst
        ) {
    return inst_fp_Rfte2(inst.codop);
}

/**
 * Devuelve true si la instruccion tiene un Rfte3 valido
 *
 * @param inst Instruccion
 *
 */
boolean rfp3_valido(instruccion_t inst
) {
    return inst_fp_Rfte3(inst.codop);
}

/**
 * Devuelve true si la instruccion tiene un Rfte1 valido
 *
 * @param inst Instruccion
 *
 */
boolean rfpd_valido(instruccion_t inst
) {
    return inst_to_Rfp(inst.codop);
}


/***************************************************************
 *
 * Func: hay_fuente1_FP_ID
 *
 * Desc: Indica si en la etapa ID hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente1_FP_ID(
        ) {
    return rfp1_valido(IF_ID.IR);
    //return inst_fp_Rfte1(IF_ID.IR.codop) && (IF_ID.iPC != -1);
} /* hay_fuente1_ID */

/***************************************************************
 *
 * Func: hay_fuente2_ID
 *
 * Desc: Indica si en la etapa ID hay una instrucción que
 *       tiene Rfte2
 *
 **************************************************************/
boolean hay_fuente2_FP_ID(
        ) {
    return rfp2_valido(IF_ID.IR);
    //return inst_fp_Rfte2(IF_ID.IR.codop) && (IF_ID.iPC != -1);
}

/***************************************************************
 *
 * Func: hay_fuente3_ID
 *
 * Desc: Indica si en la etapa ID hay una instrucción que
 *       tiene Rfte3
 *
 **************************************************************/
boolean hay_fuente3_FP_ID(
        ) {
    return rfp3_valido(IF_ID.IR);
    //return inst_fp_Rfte3(IF_ID.IR.codop) && (IF_ID.iPC != -1);
}

/***************************************************************
 *
 * Func: hay_destino_FP_WB
 *
 * Desc: Indica si en la etapa WB hay una instrucción que
 *       tiene Rdst
 *
 **************************************************************/
boolean hay_destino_FP_WB(
        ) {
    return rfpd_valido(FP_WB.IR);
    //return inst_to_Rfp(FP_WB.IR.codop) && (FP_WB.iPC != -1);
}

/**
 * Devuelve true si la instruccion de la etapa FP ADD1 tiene un Rfte1 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente1_FP_ADD1(
        ) {
    return rfp1_valido(ID_FP_ADD.IR);
    //return inst_fp_Rfte1(ID_FP_ADD.IR.codop) && (ID_FP_ADD.iPC != -1);
}

/**
 * Devuelve true si la instruccion de la etapa FP ADD1 tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente2_FP_ADD1(
        ) {
    return rfp2_valido(ID_FP_ADD.IR);
    //return inst_fp_Rfte2(ID_FP_ADD.IR.codop) && (ID_FP_ADD.iPC != -1);
}

/**
 * Devuelve true si la instruccion de la etapa FP MUL1 tiene un Rfte1 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente1_FP_MUL1(
        ) {
    return rfp1_valido(ID_FP_MUL.IR);
    //return inst_fp_Rfte1(ID_FP_MUL.IR.codop) && (ID_FP_MUL.iPC != -1);
}

/**
 * Devuelve true si la instruccion de la etapa FP MUL1 tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente2_FP_MUL1(
        ) {
    return rfp2_valido(ID_FP_MUL.IR);
    //return inst_fp_Rfte2(ID_FP_MUL.IR.codop) && (ID_FP_MUL.iPC != -1);
}

/**
 * Devuelve true si la instruccion de la etapa FP MUL1 tiene un Rfte3 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente3_FP_MUL1(
        ) {
    return rfp3_valido(ID_FP_MUL.IR);
    //return inst_fp_Rfte3(ID_FP_MUL.IR.codop) && (ID_FP_MUL.iPC != -1);
}

/**
 * Devuelve true si la instruccion de la etapa FP CMP1 tiene un Rfte1 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente1_FP_CMP1(
        ) {
    return rfp1_valido(ID_FP_CMP.IR);
    //return inst_fp_Rfte1(ID_FP_CMP.IR.codop) && (ID_FP_CMP.iPC != -1);
}

/**
 * Devuelve true si la instruccion de la etapa FP CMP1 tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente2_FP_CMP1(
        ) {
    return rfp2_valido(ID_FP_CMP.IR);
    //return inst_fp_Rfte2(ID_FP_CMP.IR.codop) && (ID_FP_CMP.iPC != -1);
}

/***************************************************************
 *
 * Func: hay_fuente1_M1
 *
 * Desc: Indica si en la etapa EX hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente1_iM1(
        ) {
    return rs1_valido(ID_INT_MUL.IR);
    //return inst_int_Rfte1(ID_INT_MUL.IR.codop) && (ID_INT_MUL.iPC != -1);
}

/***************************************************************
 *
 * Func: hay_fuente2_M1
 *
 * Desc: Indica si en la etapa EX hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente2_iM1(
        ) {
    return rs2_valido(ID_INT_MUL.IR);
    return inst_int_Rfte2(ID_INT_MUL.IR.codop) && (ID_INT_MUL.iPC != -1);
}

/**
 * Devuelve true si la instruccion de la etapa FP ADD1 tiene un Rfte1 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente1_FP_MISC1(
        ) {
    return rfp1_valido(ID_FP_MISC.IR);
    //return inst_fp_Rfte1(ID_FP_MISC.IR.codop) && (ID_FP_MISC.iPC != -1);
}

/**
 * Devuelve true si la instruccion de la etapa FP ADD1 tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente2_FP_MISC1(
        ) {
    return rfp2_valido(ID_FP_MISC.IR);
    //return inst_fp_Rfte2(ID_FP_MISC.IR.codop) && (ID_FP_MISC.iPC != -1);
}

/***************************************************************
 *
 * Func: hay_fuente1_LS
 *
 * Desc: Indica si en la etapa EX hay una instrucción que
 *       tiene Rfte2
 *
 **************************************************************/
boolean hay_fuente1_LS1(
        ) {
    return rs1_valido(ID_FP_LS.IR);
    //return inst_int_Rfte1(ID_FP_LS.IR.codop)&&(ID_FP_LS.iPC != -1);
}

/***************************************************************
 *
 * Func: hay_fuente2_LS
 *
 * Desc: Indica si en la etapa EX hay una instrucción que
 *       tiene Rfte2
 *
 **************************************************************/
boolean hay_fuente2_LS1(
        ) {
    return rfp2_valido(ID_FP_LS.IR);
    //return inst_fp_Rfte2(ID_FP_LS.IR.codop)&& (ID_FP_LS.iPC != -1);
} /* hay_fuente2_EX */

/**
 * Devuelve true si la instruccion de la etapa LS2 tiene un Rfte2 valido
 *
 * @param inst Instruccion
 *
 */
boolean hay_fuente2_LS2(
        ) {
    return rfp2_valido(FP_LS[2].IR);
    //return inst_fp_Rfte2(FP_LS[2].IR.codop)&& (FP_LS[2].iPC != -1);
} /* hay_fuente2_LS2 */


