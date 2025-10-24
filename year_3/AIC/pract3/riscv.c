/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *        Sergio Sáez (ssaez@disca.upv.es)
 *
 * Fichero: riscV.c
 *
 * Descripción:
 *   Contiene la descripcion de las fases del procesador RISCV de 5 etapas
 *
 ****************************************************************************/


#define riscv_C



/*** Includes **************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


/*** Definiciones Externas *************************************************/

#include "main.h"
#include "instrucciones.h"
#include "codop.h"
#include "control.h"
#include "presentacion-html.h"
#include "riscv_syscall.h"
#include "depurar.h"
#include "log.h"
#include "riscv_int.h"
#include "riscv_fp.h"
#include "memoria.h"
#include "registros.h"



/*** Variables Globales ****************************************************/

/*** Variables Locales *****************************************************/

/*** Funciones Locales *****************************************************/

/*** Funciones Exportables *************************************************/

/***************************************************************
 *
 * Func: fase_busqueda
 *
 * Desc: Implementa la etapa IF
 *
 **************************************************************/
void fase_busqueda(
        ) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    init_instruc(PC, orden); /*** VISUALIZACIÓN ***/

    /* Busca la siguiente instrucción */

    if (en_region(mmem, PC, r_text)) {
        if (esta_alineada_dir_instruc(PC)) {
            IF_IDn.IR = lee_mem_instruc(mmem, PC); /* Busca la siguiente instrucción */
        } else {
            /* Dirección de acceso a memoria de instrucciones desalineada */
            fprintf(stderr, "\nAtención (IF): acceso desalineado al segmento de instrucciones Ciclo=%u PC=%"PRId64"\n",
                    Ciclo + 1, PC);
        }
    } else {
        /*** Excepción en el acceso a la memoria de instrucciones */
        //fprintf(stderr, "\nAtención (IF): acceso fuera del segmento de instrucciones Ciclo=%u PC=%"PRId64"\n", Ciclo + 1, PC);

    } /* endif */

    IF_IDn.NPC = PC + 4;

    IF_IDn.iPC = PC;
    IF_IDn.orden = orden;

    SaltoEfectivo = NO;

    fnDebug(NO, "IF. Ciclo %u PC=%ld Instruccion %d\n", Ciclo, PC, IF_IDn.IR.codop);
    //printf("Fase busqueda %d %d\n", IF_IDn.IR.codop, EX_MEM.ALUout);


    /* riesgos de control */
    switch (solucion_riesgos_control) {
        case ds3:
            if (EX_MEM.cond) {
                SaltoEfectivo = SI;
                PCn = EX_MEM.ALUout;
                            } else {
                PCn = PC + 4;
            }
            break;

        case ds2:
                        break;

        case ds1:
                        break;

        case pnt3:
            if (EX_MEM.cond) {
                SaltoEfectivo = SI;
                PCn = EX_MEM.ALUout;
                IFnop = SI;
                IDnop = SI;
                EXnop = SI;
            } else {
                PCn = PC + 4;
            }

            break;

        case pnt2:
                        break;

        case pnt1:
                        break;

        case stall3:
            if (EX_MEM.cond) {
                SaltoEfectivo = SI;
                PCn = EX_MEM.ALUout;
                IFnop = SI;
                write_log("IFnop", EX_MEM.iPC);
                write_log("Salto Efectivo", EX_MEM.iPC);
            } else {
                PCn = PC + 4;
            }
            break;

        case stall2:
                        break;

        case stall1:
                        break;

        default:;
    } /* end switch */

} /* end fase_busqueda */

/***************************************************************
 *
 * Func: fase_decodificacion
 *
 * Desc: Implementa la etapa ID
 *
 **************************************************************/
void fase_decodificacion(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    word i;

    int codop;
    int opcode, rs1, rs2, rs3, rd, rm, imm;
    formato_t fmt;
    char nemotecnico[LONG_STR];
    char texto[LONG_STR];

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /*
    if (IF_ID.iPC == -1) {
        ID_EXn.iPC = IF_ID.iPC;
        ID_EXn.orden = IF_ID.orden;
        ID.cond_out = NO;
        return;
    }
     */

    codop = fn_riscv_decode(IF_ID.iPC, IF_ID.IR.valor, &fmt, &opcode, &rs1, &rs2, &rs3, &rd, &rm, &imm,
            nemotecnico, texto);

    IF_ID.IR.codop = codop;
    IF_ID.IR.rs1 = rs1;
    IF_ID.IR.rs2 = rs2;
    IF_ID.IR.rs3 = rs3;
    IF_ID.IR.rd = rd;
    IF_ID.IR.imm = imm;


    fnDebug(NO, "ID. Ciclo %u PC=%ld Instruccion %d\n", Ciclo, IF_ID.iPC, IF_ID.IR.codop);
    fnDebug(NO, "ID. Ciclo %u PC=%ld Instruccion %d rs1 %d rs2 %d rd %d imm %ld %s \n", Ciclo, IF_ID.iPC,
            IF_ID.IR.codop,
            IF_ID.IR.rs1, IF_ID.IR.rs2, IF_ID.IR.rd, IF_ID.IR.imm, texto);


    WAW = NO;
    RAW = NO;
    estructural = NO;

    MEMaIDcomp_i = NO;
    MEMaIDcomp_s = NO;

    if (inst_ecall(IF_ID.IR.codop)) {
        IFstall = SI;
        write_log("IFstall", IF_ID.iPC);
    }

    if inst_int_simple(IF_ID.IR.codop) {
        /* Instrucción entera */

        // Si la instrucción es un salto
        detectar_riesgos_control();
        
        detectar_riesgos_datos();
        detectar_riesgos_RAW_INT();
        detectar_riesgos_WAW_INT();
        detectar_riesgos_WB_INT();


        if latencia_salto1(solucion_riesgos_control) {
            if (IDstall == NO) {
                /* Condicion y direccion de salto */
                ID.compin1 = mux_COMPidsup(Rint[IF_ID.IR.rs1].valor.int_d, EX_MEM.ALUout);
                ID.compin2 = mux_COMPidinf(Rint[IF_ID.IR.rs2].valor.int_d, EX_MEM.ALUout);
                ID.cond_out = operacion_COMP(IF_ID.IR.codop, ID.compin1, ID.compin2);

                if inst_jalr(IF_ID.IR.codop) {
                    ID.sumin1 = ID.compin1; // Del mux, por si tiene dependencia
                    ID.dir = ID.sumin1 + (word) IF_ID.IR.imm;
                } else {
                    ID.sumin1 = IF_ID.iPC;
                    ID.dir = IF_ID.iPC + (word) IF_ID.IR.imm; // 2x desplazamiento
                }
                fnDebug(NO, "ID. Ciclo %u PC=%ld ID.compin1 %ld ID.compin2 %ld ID.dir %ld\n", Ciclo, IF_ID.iPC,
                        ID.compin1, ID.compin2, ID.dir);
            }
        }

        /* alimentar el resto de operadores */
        pasar_a_FP_LS1_NOP();
        pasar_a_FP_A1_NOP();
        pasar_a_FP_M1_NOP();
        pasar_a_FP_C1_NOP();
        pasar_a_iM1_NOP();
        pasar_a_FP_MISC1_NOP();
        pasar_a_FP_CVT1_NOP();

        /* leer operandos */
        ID_EXn.Ra = Rint[IF_ID.IR.rs1].valor.int_d;
        ID_EXn.Rb = Rint[IF_ID.IR.rs2].valor.int_d;
        ID_EXn.Imm = (dword) IF_ID.IR.imm;

        if (IDstall == NO)
            if inst_int_to_Rint(IF_ID.IR.codop) {
                // Las instrucciones de 5 etapas no lo necesitan
                //Rint[IF_ID.IR.rd].busy = Ciclo + 3;
                //Rint_writings = Rint_writings | (1 << (3));
            }
        /*
                    if inst_int_to_Rint(IF_ID.IR.codop) {
                        // LOAD: +2 ciclos desde ID
                        if inst_int_load(IF_ID.IR.codop) {
                            Rint[IF_ID.IR.rd].busy = Ciclo + 2;
                            Rint_writings = Rint_writings | (1 << 2);
                        // resto: +2 ciclos desde ID
                        } else {
                            Rint[IF_ID.IR.rd].busy = Ciclo + 1;
                            Rint_writings = Rint_writings | (1 << 1);
                        }
                    }
         */
        fnDebug(NO, "ID Ciclo %u rd=%u %d \n", Ciclo, IF_ID.IR.rd, Rint[IF_ID.IR.rd].busy);

        /* propagar datos */
        ID_EXn.NPC = IF_ID.NPC;
        ID_EXn.IR = IF_ID.IR;

        ID_EXn.iPC = IF_ID.iPC;
        ID_EXn.orden = IF_ID.orden;

        /* si ajustar duracion y salto con latencia 1, pasar nop a EX */
        //if (AJUSTAR_INT && inst_salto(IF_ID.IR.codop) && latencia_salto1(solucion_riesgos_control)) {
        //    pasar_a_EX_NOP();
        //}
        /* solo aplica a la unidad entera en modo multiciclo */
        //if (AJUSTAR_INT && (hay_float || hay_double) && inst_salto(IF_ID.IR.codop) && latencia_salto1(solucion_riesgos_control)) {
        //    pasar_a_EX_NOP();
        //}

    } else {
        /* Instrucción multiciclo */

        detectar_riesgos_datos();
        detectar_riesgos_RAW_INT();
        detectar_riesgos_WAW_INT();

        detectar_riesgos_RAW_FP();
        detectar_riesgos_WAW_FP();
        detectar_riesgos_WB_FP();
        detectar_riesgos_WB_INT();


        for (i = 0; i < TAM_REGISTROS; i++) {
            // fprintf (stdout, "DEBUG ID Busy  i=%d busy=%d \n",i,Rfp[i].busy);
        }

        /* Si hay algun problema, se habra activado IDstall */
        /* A diferencia de la unidad de 5 etapas, no podemos dejar que la instrucción
         * contnúe, ya que contaminaría el campo busy del Rfp */
        if (IDstall == NO) {

            switch (IF_ID.IR.codop) {
                    _LOADS_FP

                    ID_FP_LSn.Ra = Rint[IF_ID.IR.rs1].valor.int_d;
                    ID_FP_LSn.Rb = Rint[IF_ID.IR.rs2].valor.int_d;
                    ID_FP_LSn.Imm = (dword) IF_ID.IR.imm;
                    //ID_LSn.Fa = Rfp[IF_ID.IR.rs1].valor;
                    //ID_LSn.Fb = Rfp[IF_ID.IR.rs2].valor;

                    Rfp[IF_ID.IR.rd].busy = Ciclo + FP_LS_LAT + 1;
                    Rfp_writings = Rfp_writings | (1 << (FP_LS_LAT + 1));

                    ID_FP_LSn.NPC = IF_ID.NPC;
                    ID_FP_LSn.IR = IF_ID.IR;

                    ID_FP_LSn.iPC = IF_ID.iPC;
                    ID_FP_LSn.orden = IF_ID.orden;

                    pasar_a_EX_NOP();
                    //pasar_a_FP_LS1_NOP();
                    pasar_a_FP_A1_NOP();
                    pasar_a_FP_M1_NOP();
                    pasar_a_FP_C1_NOP();
                    pasar_a_iM1_NOP();
                    pasar_a_FP_MISC1_NOP();
                    pasar_a_FP_CVT1_NOP();
                    break;

                    _STORES_FP

                    ID_FP_LSn.Ra = Rint[IF_ID.IR.rs1].valor.int_d;
                    ID_FP_LSn.Rb = Rint[IF_ID.IR.rs2].valor.int_d;
                    ID_FP_LSn.Imm = (dword) IF_ID.IR.imm;
                    ID_FP_LSn.Fa = Rfp[IF_ID.IR.rs1].valor;
                    ID_FP_LSn.Fb = Rfp[IF_ID.IR.rs2].valor;

                    ID_FP_LSn.NPC = IF_ID.NPC;
                    ID_FP_LSn.IR = IF_ID.IR;

                    ID_FP_LSn.iPC = IF_ID.iPC;
                    ID_FP_LSn.orden = IF_ID.orden;
                    ID_FP_LSn.orden = IF_ID.orden;

                    pasar_a_EX_NOP();
                    //pasar_a_FP_LS1_NOP();
                    pasar_a_FP_A1_NOP();
                    pasar_a_FP_M1_NOP();
                    pasar_a_FP_C1_NOP();
                    pasar_a_iM1_NOP();
                    pasar_a_FP_MISC1_NOP();
                    pasar_a_FP_CVT1_NOP();
                    break;

                    _ADDS_FP_S
                    _ADDS_FP_D
                    _CVT_FP_FP

                    ID_FP_ADDn.Fa = Rfp[IF_ID.IR.rs1].valor;
                    ID_FP_ADDn.Fb = Rfp[IF_ID.IR.rs2].valor;

                    Rfp[IF_ID.IR.rd].busy = Ciclo + FP_ADD_LAT + 1;
                    Rfp_writings = Rfp_writings | (1 << (FP_ADD_LAT + 1));

                    ID_FP_ADDn.NPC = IF_ID.NPC;
                    ID_FP_ADDn.IR = IF_ID.IR;

                    ID_FP_ADDn.iPC = IF_ID.iPC;
                    ID_FP_ADDn.orden = IF_ID.orden;

                    pasar_a_EX_NOP();
                    pasar_a_FP_LS1_NOP();
                    // pasar_a_FP_A1_NOP();
                    pasar_a_FP_M1_NOP();
                    pasar_a_FP_C1_NOP();
                    pasar_a_FP_MISC1_NOP();
                    pasar_a_FP_CVT1_NOP();
                    break;

                    _CVT_INT_TO_FP_S
                    _CVT_INT_TO_FP_D

                    ID_FP_CVTn.Ra = Rint[IF_ID.IR.rs1].valor.int_d;
                    ID_FP_CVTn.Rb = Rint[IF_ID.IR.rs2].valor.int_d;

                    Rfp[IF_ID.IR.rd].busy = Ciclo + FP_CVT_LAT + 1;
                    Rfp_writings = Rfp_writings | (1 << (FP_CVT_LAT + 1));

                    ID_FP_CVTn.NPC = IF_ID.NPC;
                    ID_FP_CVTn.IR = IF_ID.IR;

                    ID_FP_CVTn.iPC = IF_ID.iPC;
                    ID_FP_CVTn.orden = IF_ID.orden;

                    pasar_a_EX_NOP();
                    pasar_a_FP_LS1_NOP();
                    pasar_a_FP_A1_NOP();
                    pasar_a_FP_M1_NOP();
                    pasar_a_FP_C1_NOP();
                    pasar_a_iM1_NOP();
                    pasar_a_FP_MISC1_NOP();
                    //pasar_a_FP_CVT1_NOP();
                    break;

                    _MULS_FP_S
                    _MULS_FP_D

                    ID_FP_MULn.Fa = Rfp[IF_ID.IR.rs1].valor;
                    ID_FP_MULn.Fb = Rfp[IF_ID.IR.rs2].valor;
                    ID_FP_MULn.Fc = Rfp[IF_ID.IR.rs3].valor;

                    Rfp[IF_ID.IR.rd].busy = Ciclo + FP_MUL_LAT + 1;
                    Rfp_writings = Rfp_writings | (1 << (FP_MUL_LAT + 1));

                    fnDebug(NO, "FP_MUL escribira en reg %d en el ciclo %u writings=%x\n", IF_ID.IR.rd,
                            Rfp[IF_ID.IR.rd].busy, Rfp_writings);

                    ID_FP_MULn.NPC = IF_ID.NPC;
                    ID_FP_MULn.IR = IF_ID.IR;

                    ID_FP_MULn.iPC = IF_ID.iPC;
                    ID_FP_MULn.orden = IF_ID.orden;

                    pasar_a_EX_NOP();
                    pasar_a_FP_LS1_NOP();
                    pasar_a_FP_A1_NOP();
                    //pasar_a_FP_M1_NOP();
                    pasar_a_FP_C1_NOP();
                    pasar_a_iM1_NOP();
                    pasar_a_FP_MISC1_NOP();
                    pasar_a_FP_CVT1_NOP();
                    break;

                    // Comparaciones
                    _CMP_FP_S
                    _CMP_FP_D

                    ID_FP_CMPn.Fa = Rfp[IF_ID.IR.rs1].valor;
                    ID_FP_CMPn.Fb = Rfp[IF_ID.IR.rs2].valor;

                    Rint[IF_ID.IR.rd].busy = Ciclo + FP_CMP_LAT + 1;
                    Rint_writings = Rint_writings | (1 << (FP_CMP_LAT + 1));
                    fnDebug(NO, "FP_CMP escribira en reg %d en el ciclo %u \n", IF_ID.IR.rd, Rint[IF_ID.IR.rd].busy);

                    ID_FP_CMPn.NPC = IF_ID.NPC;
                    ID_FP_CMPn.IR = IF_ID.IR;

                    ID_FP_CMPn.iPC = IF_ID.iPC;
                    ID_FP_CMPn.orden = IF_ID.orden;


                    pasar_a_EX_NOP();
                    pasar_a_FP_LS1_NOP();
                    pasar_a_FP_A1_NOP();
                    pasar_a_FP_M1_NOP();
                    // pasar_a_FP_C1_NOP();
                    pasar_a_iM1_NOP();
                    pasar_a_FP_MISC1_NOP();
                    pasar_a_FP_CVT1_NOP();
                    break;

                    _MULS_INT
                    _MULS_INTW

                    ID_INT_MULn.Ra = Rint[IF_ID.IR.rs1].valor.int_d;
                    ID_INT_MULn.Rb = Rint[IF_ID.IR.rs2].valor.int_d;

                    Rint[IF_ID.IR.rd].busy = Ciclo + INT_MUL_LAT + 1;
                    Rint_writings = Rint_writings | (1 << (INT_MUL_LAT + 1));
                    //					fprintf (stdout, "DEBUG ID FP_MUL escribira en reg %d en el ciclo %u \n",IF_ID.IR.rd,Rfp[IF_ID.IR.rd].busy);

                    ID_INT_MULn.NPC = IF_ID.NPC;
                    ID_INT_MULn.IR = IF_ID.IR;

                    ID_INT_MULn.iPC = IF_ID.iPC;
                    ID_INT_MULn.orden = IF_ID.orden;

                    //pasar_a_iM1_NOP();
                    pasar_a_FP_LS1_NOP();
                    pasar_a_FP_A1_NOP();
                    pasar_a_FP_M1_NOP();
                    pasar_a_FP_C1_NOP();
                    pasar_a_EX_NOP();
                    pasar_a_FP_MISC1_NOP();
                    pasar_a_FP_CVT1_NOP();
                    break;

                    _CVT_FP_TO_INT
                    _CLASS

                    ID_FP_MISCn.Fa = Rfp[IF_ID.IR.rs1].valor;
                    ID_FP_MISCn.Fb = Rfp[IF_ID.IR.rs2].valor;

                    ID_FP_MISCn.NPC = IF_ID.NPC;
                    ID_FP_MISCn.IR = IF_ID.IR;

                    ID_FP_MISCn.iPC = IF_ID.iPC;
                    ID_FP_MISCn.orden = IF_ID.orden;

                    Rint[IF_ID.IR.rd].busy = Ciclo + FP_MISC_LAT + 1;
                    Rint_writings = Rint_writings | (1 << (FP_MISC_LAT + 1));

                    pasar_a_EX_NOP();
                    pasar_a_FP_LS1_NOP();
                    pasar_a_FP_A1_NOP();
                    pasar_a_FP_M1_NOP();
                    pasar_a_FP_C1_NOP();
                    pasar_a_iM1_NOP();
                    //pasar_a_FP_MISC1_NOP();
                    pasar_a_FP_CVT1_NOP();
                    break;

                default:
                    fnError("Instrucción %d no implementada\n", IF_ID.IR.codop)
                            // fprintf(stderr,
                            //         "ERROR (%d %d): Instrucción no implementada\n",
                            //         __LINE__, IF_ID.IR.codop);
                            // exit(1);

            };
            fnDebug(NO, "ID Ciclo %u rd=%u %d \n", Ciclo, IF_ID.IR.rd, Rfp[IF_ID.IR.rd].busy);


        }
    };

    /*
    fprintf (stdout, "DEBUG ID: busy");
    for (i=0; i<TAM_REGISTROS; i++){
            fprintf (stdout, "%3d ",Rfp[i].busy);
    }
    fprintf (stdout, "\n");
     */


} /* end fase_decodificacion */

/***************************************************************
 *
 * Func: fase_ejecucion
 *
 * Desc: Implementa la etapa EX
 *
 **************************************************************/
void fase_ejecucion(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /*
    if (ID_EX.iPC == -1) {
        EX_MEMn.iPC = ID_EX.iPC;
        EX_MEMn.orden = ID_EX.orden;
        EX_MEMn.cond = NO;
        EX.cond_out = NO;
        return;
    }
     */

    WBaEXalu_s = NO;
    WBaEXalu_i = NO;
    MEMaEXalu_s = NO;
    MEMaEXalu_i = NO;
    //printf("EX Ciclo %d F0 busy %d\n",Ciclo,Rfp[0].busy);
    fnDebug(NO, "EX. Ciclo %u PC=%ld Instruccion %d\n", Ciclo, ID_EX.iPC, ID_EX.IR.codop);

    if (inst_ecall(ID_EX.IR.codop)) {
        IFstall = SI;
        write_log("IFstall", ID_EX.iPC);
    }

    /* riesgos de control */

    switch (solucion_riesgos_control) {
        case stall3:
        case stall2:
            if (inst_salto(ID_EX.IR.codop)) {

                IFstall = SI;
                write_log("IFstall", ID_EX.iPC);
            }
            break;

        default:;
    } /* end switch */


    /* multiplexores */

    EX.aluin1 = mux_ALUsup(ID_EX.iPC, ID_EX.Ra, EX_MEM.ALUout, WB.data);
    EX.aluin2 = mux_ALUinf(ID_EX.Rb, ID_EX.Imm, EX_MEM.ALUout, WB.data);

    /* hacemos la operacion */

    EX.aluout = operacion_ALU(ID_EX.IR.codop, EX.aluin1, EX.aluin2);
    EX_MEMn.ALUout = EX.aluout;

    /* saltos */

    EX.compin1 = mux_COMPsup(ID_EX.Ra, EX_MEM.ALUout, WB.data);
    EX.compin2 = mux_COMPinf(ID_EX.Rb, EX_MEM.ALUout, WB.data);
    EX.cond_out = operacion_COMP(ID_EX.IR.codop, EX.compin1, EX.compin2);
    EX_MEMn.cond = EX.cond_out;

    /* propagar datos */
    EX.data = mux_EXmem(ID_EX.Rb, WB.data); /* Para SW */
    EX_MEMn.data = EX.data;

    EX_MEMn.NPC = ID_EX.NPC;
    EX_MEMn.IR = ID_EX.IR;

    EX_MEMn.iPC = ID_EX.iPC;
    EX_MEMn.orden = ID_EX.orden;

    /* si ajustar duracion y salto con latencia 2, pasar nop a MEM */
    //if (AJUSTAR_INT && inst_salto(ID_EX.IR.codop) && latencia_salto2(solucion_riesgos_control)) {
    //    pasar_a_MEM_NOP();
    //}
    /* solo aplica a la unidad entera en modo multicilo */
    //if (AJUSTAR_INT && (hay_float || hay_double) && inst_salto(ID_EX.IR.codop) && latencia_salto2(solucion_riesgos_control)) {
    //    pasar_a_MEM_NOP();
    //}
    //printf("EX Ciclo %d F0 busy %d\n",Ciclo,Rfp[0].busy);

} /* end fase_ejecucion */

/***************************************************************
 *
 * Func: fase_memoria
 *
 * Desc: Implementa la etapa MEM
 *
 **************************************************************/
void fase_memoria(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t datos_mem;
    boolean dirty;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /*
    if (EX_MEM.iPC == -1) {
        MEM_WBn.iPC = EX_MEM.iPC;
        MEM_WBn.orden = EX_MEM.orden;
        return;
    }
     */

    fnDebug(NO, "MEM. Ciclo %u PC=%ld Instruccion %d\n", Ciclo, EX_MEM.iPC, EX_MEM.IR.codop);
    //printf("MEM Ciclo %d F0 busy %d\n",Ciclo,Rfp[0].busy);

    WBaMEM = NO;

    if (inst_ecall(EX_MEM.IR.codop)) {
        IFstall = SI;
        write_log("IFstall", EX_MEM.iPC);
    }

    /* riesgos de control */

    switch (solucion_riesgos_control) {
        case stall3:
            if (inst_salto(EX_MEM.IR.codop)) {
                IFstall = SI;
                write_log("IFstall", EX_MEM.iPC);
            }
            break;

        default:;
    } /* end switch */

    /* multiplexores */
    MEM.data = mux_MEMmem(EX_MEM.data, WB.data); /* Para SW */


    /* acceso a memoria */

    switch (EX_MEM.IR.codop) {
        case RV64I_LD:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_dword, &dirty, EX_MEM.iPC);
            MEM_WBn.MEMout = datos_mem.int_d;
            break;
        case RV64I_SD:
            MEM.dir = EX_MEM.ALUout;
            datos_mem.int_d = MEM.data;
            escribe_mem_datos(mmem, MEM.dir, datos_mem, t_dword, SI, EX_MEM.iPC);
            break;
        case RV32I_LW:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_word, &dirty, EX_MEM.iPC);
            MEM_WBn.MEMout = datos_mem.int_d;
            break;
        case RV64I_LWU:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_word, &dirty, EX_MEM.iPC);
            MEM_WBn.MEMout = 0xFFFFFFFF & datos_mem.int_d;
            break;
        case RV32I_SW:
            MEM.dir = EX_MEM.ALUout;
            datos_mem.int_d = MEM.data;
            escribe_mem_datos(mmem, MEM.dir, datos_mem, t_word, SI, EX_MEM.iPC);
            break;
        case RV32I_LH:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_half, &dirty, EX_MEM.iPC);
            MEM_WBn.MEMout = datos_mem.int_d;
            break;
        case RV32I_LHU:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_half, &dirty, EX_MEM.iPC);
            MEM_WBn.MEMout = 0xFFFF & datos_mem.int_d;
            break;
        case RV32I_SH:
            MEM.dir = EX_MEM.ALUout;
            datos_mem.int_d = MEM.data;
            escribe_mem_datos(mmem, MEM.dir, datos_mem, t_half, SI, EX_MEM.iPC);
            break;
        case RV32I_LB:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_byte, &dirty, EX_MEM.iPC);
            MEM_WBn.MEMout = datos_mem.int_d;
            break;
        case RV32I_LBU:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_byte, &dirty, EX_MEM.iPC);
            MEM_WBn.MEMout = 0xFF & datos_mem.int_d;
            break;
        case RV32I_SB:
            MEM.dir = EX_MEM.ALUout;
            datos_mem.int_d = MEM.data;
            escribe_mem_datos(mmem, MEM.dir, datos_mem, t_byte, SI, EX_MEM.iPC);
            break;
        default:
            MEM.dir = 0;
            MEM_WBn.MEMout = 0;
            ;
    } /* endswitch */


    /* propagar datos */

    MEM_WBn.ALUout = EX_MEM.ALUout;

    MEM_WBn.NPC = EX_MEM.NPC;
    MEM_WBn.IR = EX_MEM.IR;

    MEM_WBn.iPC = EX_MEM.iPC;
    MEM_WBn.orden = EX_MEM.orden;

    /* si ajustar duracion y store o salto, pasar nop a WB */
    //if (AJUSTAR_INT && (inst_int_store(EX_MEM.IR.codop) || inst_salto(EX_MEM.IR.codop))) {
    /* solo aplica a la unidad entera en modo multicilo */
    //if (AJUSTAR_INT && (hay_float || hay_double) && (inst_store_int(EX_MEM.IR.codop) || inst_salto(EX_MEM.IR.codop))) {
    if (AJUSTAR_INT && (hay_float || hay_double) && inst_int_store(EX_MEM.IR.codop)) {
        pasar_a_WB_NOP();
    }
    //printf("MEM Ciclo %d F0 busy %d\n",Ciclo,Rfp[0].busy);
} /* end fase_memoria */

/***************************************************************
 *
 * Func: fase_escritura
 *
 * Desc: Implementa la etapa 'WB'
 *
 **************************************************************/
int fase_escritura(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    boolean final,
            libre;
    word i;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /* Selecciona quién pasa a FP WB */
    iWB = mux_INT_WB(MEM_WB, FP_CMP_WB, INT_MUL_WB, FP_MISC_WB);

    fnDebug(NO, "WB.  PC=%ld Instruccion %d\n", iWB.iPC, iWB.IR.codop);

    sprintf(CONSOLA, "%s", "");
    sprintf(LOG, "%s", "");

    /*
    if (iWB.iPC == -1) {
        return 0;
    }
     */

    if (inst_ecall(iWB.IR.codop)) {
        IFstall = SI;
        write_log("IFstall", iWB.iPC);
    }

    if (iWB.iPC != -1) {
        //Instrucciones++;
        estat.instrucciones++;
        estat.enteras++;
    }
    fnDebug(NO, "Estats. Ciclo %u PC %ld Instrucciones %ld \n", Ciclo, iWB.iPC, estat.instrucciones);


    switch (iWB.IR.codop) {
        case RV64I_LD:
            WB.data = iWB.MEMout;
            if (iWB.IR.rd != 0) {
                escribe_int_reg(iWB.IR.rd, iWB.MEMout, t_dword, SI);
                if (Ciclo == Rint[iWB.IR.rd].busy) { // Si el registro es nuestro
                    printf("ERROR. Esto no debe ejecutarse\n");
                    printf("PC=%ld Ciclo=%u\n", iWB.iPC, Ciclo);
                    exit(1);
                    Rint[iWB.IR.rd].busy = 0;
                } else {
                    //		fprintf (stdout, "DEBUG iWB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
                    fnDebug(NO, "iWB Ciclo %u %d \n", Ciclo, Rint[iWB.IR.rd].busy);
                }
            }
            break;
        case RV32I_LW:
        case RV64I_LWU:
            WB.data = iWB.MEMout;
            if (iWB.IR.rd != 0) {
                escribe_int_reg(iWB.IR.rd, iWB.MEMout, t_word, SI);
                if (Ciclo == Rint[iWB.IR.rd].busy) { // Si el registro es nuestro
                    printf("ERROR. Esto no debe ejecutarse\n");
                    printf("PC=%ld Ciclo=%u\n", iWB.iPC, Ciclo);
                    exit(1);
                    Rint[iWB.IR.rd].busy = 0;
                } else {
                    //		fprintf (stdout, "DEBUG iWB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
                    fnDebug(NO, "iWB Ciclo %u %d \n", Ciclo, Rint[iWB.IR.rd].busy);
                }
            }
            break;
        case RV32I_LH:
        case RV32I_LHU:
            WB.data = iWB.MEMout;
            if (iWB.IR.rd != 0) {
                escribe_int_reg(iWB.IR.rd, iWB.MEMout, t_half, SI);
                if (Ciclo == Rint[iWB.IR.rd].busy) { // Si el registro es nuestro
                    printf("ERROR. Esto no debe ejecutarse\n");
                    printf("PC=%ld Ciclo=%u\n", iWB.iPC, Ciclo);
                    exit(1);
                    Rint[iWB.IR.rd].busy = 0;
                } else {
                    //		fprintf (stdout, "DEBUG iWB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
                    fnDebug(NO, "iWB Ciclo %u %d \n", Ciclo, Rint[iWB.IR.rd].busy);
                }
            }
            break;
        case RV32I_LB:
        case RV32I_LBU:
            WB.data = iWB.MEMout;
            if (iWB.IR.rd != 0) {
                escribe_int_reg(iWB.IR.rd, iWB.MEMout, t_byte, SI);
                if (Ciclo == Rint[iWB.IR.rd].busy) { // Si el registro es nuestro
                    printf("ERROR. Esto no debe ejecutarse\n");
                    printf("PC=%ld Ciclo=%u\n", iWB.iPC, Ciclo);
                    exit(1);
                    Rint[iWB.IR.rd].busy = 0;
                } else {
                    //		fprintf (stdout, "DEBUG iWB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
                    fnDebug(NO, "iWB Ciclo %u %d \n", Ciclo, Rint[iWB.IR.rd].busy);
                }
            }
            break;
        case RV64I_SD:
        case RV32I_SW:
        case RV32I_SH:
        case RV32I_SB:
        case RV32I_BEQ:
        case RV32I_BNE:
        case RV32I_BLT:
        case RV32I_BLTU:
        case RV32I_BGE:
        case RV32I_BGEU:
            break;
        case RV32I_JAL:
        case RV32I_JALR:
            WB.data = iWB.NPC;
            if (iWB.IR.rd != 0) {
                escribe_int_reg(iWB.IR.rd, WB.data, t_dword, SI);
                if (Ciclo == Rint[iWB.IR.rd].busy) { // Si el registro es nuestro
                    printf("ERROR. Esto no debe ejecutarse\n");
                    printf("PC=%ld Ciclo=%u\n", iWB.iPC, Ciclo);
                    exit(1);
                    Rint[iWB.IR.rd].busy = 0;
                } else {
                    //		fprintf (stdout, "DEBUG iWB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
                    fnDebug(NO, "iWB Ciclo %u %d \n", Ciclo, Rint[iWB.IR.rd].busy);
                }
            }
            break;
        case OP_TRAP:
            SIG_TRAP0 = SI;
            break;
        case RV32I_ECALL:
            SIG_SYSCALL = SI;
            break;

        default:
            WB.data = iWB.ALUout;
            if (iWB.IR.rd != 0) {
                fnDebug(NO, "Write Rint %d  PC=%ld Instruccion %d\n", iWB.IR.rd, iWB.iPC, iWB.IR.codop);
                escribe_int_reg(iWB.IR.rd, iWB.ALUout, t_dword, SI);
                if (Ciclo == Rint[iWB.IR.rd].busy) { // Si el registro es nuestro
                    Rint[iWB.IR.rd].busy = 0;
                }
            }
            break;


    } /* endswitch */


    /*
          fprintf(stdout, "DEBUG INT WB: busy. Ciclo=%d. ", Ciclo);
             for (i = 0; i < TAM_REGISTROS; i++) {
                 fprintf(stdout, "%d ", Rint[i].busy);
             }
             fprintf(stdout, "\n");
     */

    final = NO;

    if (SIG_TRAP0) {
        final = SI;
        for (i = 0; i < TAM_REGISTROS; i++) {
            final = final && (Rfp[i].busy == 0);
        };
        for (i = 0; i < TAM_REGISTROS; i++) {
            final = final && (Rint[i].busy == 0);
        };
    };

    if (SIG_SYSCALL) {
        libre = SI;
        for (i = 0; i < TAM_REGISTROS; i++) {
            libre = libre && (Rfp[i].busy == 0);
        }
        for (i = 0; i < TAM_REGISTROS; i++) {
            libre = libre && (Rint[i].busy == 0);
        };
        if (libre) {
            SIG_SYSCALL = NO;
            final = process_syscall(iWB.iPC);
        }
    }

    if (final) {
        fprintf(stderr, "\nCiclo %u: El programa está terminando normalmente\n\n", Ciclo + 1);
    }
    return final;

} /* end fase_escritura */

/***************************************************************
 *
 * Func: fase_escritura_FP
 *
 * Desc: Implementa la etapa 'WB'
 *
 **************************************************************/
void fase_escritura_FP(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/


    /* Selecciona quién pasa a FP WB */
    FP_WB = mux_FP_WB(FP_LS_WB, FP_ADD_WB, FP_MUL_WB, FP_CVT_WB);

    fnDebug(NO, "WB. Ciclo %u PC=%ld Instruccion %d\n", Ciclo, FP_WB.iPC, FP_WB.IR.codop);


    if (FP_WB.iPC == -1)
        return;


    if (FP_WB.iPC != -1) {
        //Instrucciones++;
        estat.instrucciones++;
        estat.multiciclo++;
    }


    switch (FP_WB.IR.codop) {
        case RV32D_FLD:
            escribe_fp_reg(FP_WB.IR.rd, FP_WB.ALUout, t_double, SI);
            if (Ciclo == Rfp[FP_WB.IR.rd].busy) { // Si el registro es nuestro
                Rfp[FP_WB.IR.rd].busy = 0;
            } else {
                //		fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
            }
            break;
        case RV32F_FLW:
            escribe_fp_reg(FP_WB.IR.rd, FP_WB.ALUout, t_float, SI);
            if (Ciclo == Rfp[FP_WB.IR.rd].busy) { // Si el registro es nuestro
                Rfp[FP_WB.IR.rd].busy = 0;
            } else {
                //		fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
            }
            break;
        case OP_FP_L_PS:
            escribe_fp_reg(FP_WB.IR.rd, FP_WB.ALUout, t_float_ps, SI);
            if (Ciclo == Rfp[FP_WB.IR.rd].busy) { // Si el registro es nuestro
                Rfp[FP_WB.IR.rd].busy = 0;
            } else {
                //		fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
            }
            break;

            _CVT_INT_TO_FP_D
        case RV32D_FCVT_D_S:
            //estat.flops++;
            escribe_fp_reg(FP_WB.IR.rd, FP_WB.ALUout, t_double, SI);
            if (Ciclo == Rfp[FP_WB.IR.rd].busy) { // Si el registro es nuestro
                Rfp[FP_WB.IR.rd].busy = 0;
            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
            }
            break;

            _ADDS_FP_D
            _MULS_FP_D
            estat.flops++;
            escribe_fp_reg(FP_WB.IR.rd, FP_WB.ALUout, t_double, SI);
            if (Ciclo == Rfp[FP_WB.IR.rd].busy) { // Si el registro es nuestro
                Rfp[FP_WB.IR.rd].busy = 0;
            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
            }
            break;

            _CVT_INT_TO_FP_S
        case RV32D_FCVT_S_D:
            //estat.flops++;
            escribe_fp_reg(FP_WB.IR.rd, FP_WB.ALUout, t_float, SI);
            if (Ciclo == Rfp[FP_WB.IR.rd].busy) { // Si el registro es nuestro
                Rfp[FP_WB.IR.rd].busy = 0;
            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
            }
            break;

            _ADDS_FP_S
            _MULS_FP_S
            estat.flops++;
            escribe_fp_reg(FP_WB.IR.rd, FP_WB.ALUout, t_float, SI);
            if (Ciclo == Rfp[FP_WB.IR.rd].busy) { // Si el registro es nuestro
                Rfp[FP_WB.IR.rd].busy = 0;
            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
            }
            break;

        case OP_FP_ADD_PS:
        case OP_FP_SUB_PS:
        case OP_FP_MUL_PS:
        case OP_FP_DIV_PS:
            estat.flops++;
            escribe_fp_reg(FP_WB.IR.rd, FP_WB.ALUout, t_float_ps, SI);
            if (Ciclo == Rfp[FP_WB.IR.rd].busy) { // Si el registro es nuestro
                Rfp[FP_WB.IR.rd].busy = 0;
            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.rd].busy);
            }
            break;
        case RV64I_SD:
        case RV32I_SW:
        case RV32I_SH:
        case RV32I_SB:
            break;
        default:
            fnError("PC=%ld. La instrucción %d no debería estar en FP WB\n", FP_WB.iPC, FP_WB.IR.codop)
                    //fprintf(stderr,
                    //        "ERROR (%d %d): Esta instrucción no deberia estar en FP WB\n",
                    //        __LINE__, FP_WB.IR.codop);
                    //exit(1);

    } /* endswitch */


    /*    
            fprintf(stdout, "DEBUG WB: busy");
            for (int i = 0; i < TAM_REGISTROS; i++) {
                fprintf(stdout, "%3d ", Rfp[i].busy);
            }
            fprintf(stdout, "\n");
     */    


} /* end fase_escritura */

/***************************************************************
 *
 * Func: impulso_reloj
 *
 * Desc: Actualiza los registros inter-etapa
 *
 **************************************************************/
void impulso_reloj() {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int i;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /* Si se ha recibido la TRAP #0 no buscamos nada mas */
    /*
    if (SIG_TRAP0) {
        IFstall = SI;
        //write_log("IFstall", MEM_WB.iPC);
        //IFnop = SI;
    };
     */


    /* IFnop se activa para abortar una instruccion. Si esta activado tb IFstall, la ignoramos */
    if (IFnop) {
        pasar_a_ID_NOP();
        IFnop = NO;
        IFstall = NO; // Por si estaba activada
    }

    if (!IFstall) {
        orden = orden + 1; /* VISUALIZACIÓN */
        PC = PCn;
    } else {
        if (SaltoEfectivo) {
            /* está activado IDstall por culpa de IDstall y hay un salto en este ciclo */
            /* No podemos perder el PC */
            orden = orden + 1;
            PC = PCn;
        };
        pasar_a_ID_NOP();
        IFstall = NO;
    }

    if (IDnop) {
        cancelar_en_ID();
        /*
                pasar_a_FP_LS1_NOP();
                pasar_a_FP_A1_NOP();
                pasar_a_FP_C1_NOP();
                pasar_a_FP_M1_NOP();
                pasar_a_iM1_NOP();
                pasar_a_FP_MISC1_NOP();
                pasar_a_FP_CVT1_NOP();
         */
        IDnop = NO;
        IDstall = NO; // Por si estaba activada
    }

    if (!IDstall) {
        IF_ID = IF_IDn;
    } else {
        pasar_a_EX_NOP();

        pasar_a_FP_LS1_NOP();
        pasar_a_FP_A1_NOP();
        pasar_a_FP_C1_NOP();
        pasar_a_FP_M1_NOP();
        pasar_a_iM1_NOP();
        pasar_a_FP_MISC1_NOP();
        pasar_a_FP_CVT1_NOP();

        IDstall = NO;
    }


    if (EXnop) {
        cancelar_en_EX();
        /*
        pasar_a_FP_LS2_NOP();
        pasar_a_FP_A2_NOP();
        pasar_a_FP_C2_NOP();
        pasar_a_FP_M2_NOP();
        pasar_a_iM2_NOP();
        pasar_a_FP_MISC2_NOP();
        pasar_a_FP_CVT2_NOP();
	*/
        EXnop = NO;
        EXstall = NO; // Por si estaba activada
    }

    if (!EXstall) {
        ID_EX = ID_EXn;
    } else {
        pasar_a_MEM_NOP();
        EXstall = NO;
    }


    EX_MEM = EX_MEMn;

    MEM_WB = MEM_WBn;

    ID_FP_LS = ID_FP_LSn;
    for (i = 2; i <= FP_LS_LAT; i++) {
        FP_LS[i] = FP_LSn[i];
    }

    ID_FP_ADD = ID_FP_ADDn;
    for (i = 2; i <= FP_ADD_LAT; i++) {
        FP_ADD[i] = FP_ADDn[i];
    }

    ID_FP_MUL = ID_FP_MULn;
    for (i = 2; i <= FP_MUL_LAT; i++) {
        FP_MUL[i] = FP_MULn[i];
    }

    ID_FP_CMP = ID_FP_CMPn;
    for (i = 2; i <= FP_CMP_LAT; i++) {
        FP_CMP[i] = FP_CMPn[i];
    }

    ID_INT_MUL = ID_INT_MULn;
    for (i = 2; i <= FP_MUL_LAT; i++) {
        INT_MUL[i] = INT_MULn[i];
    }

    ID_FP_MISC = ID_FP_MISCn;
    for (i = 2; i <= FP_MISC_LAT; i++) {
        FP_MISC[i] = FP_MISCn[i];
    }

    ID_FP_CVT = ID_FP_CVTn;
    for (i = 2; i <= FP_CVT_LAT; i++) {
        FP_CVT[i] = FP_CVTn[i];
    }

    FP_LS_WB = FP_LS_WBn;
    FP_ADD_WB = FP_ADD_WBn;
    FP_MUL_WB = FP_MUL_WBn;
    FP_CMP_WB = FP_CMP_WBn;
    FP_MISC_WB = FP_MISC_WBn;
    FP_CVT_WB = FP_CVT_WBn;

    INT_MUL_WB = INT_MUL_WBn;



    fnDebug(NO, "Rfp_writings %x --> %x", Rfp_writings, Rfp_writings >> 1);
    fnDebug(NO, "Rint_writings %x --> %x", Rint_writings, Rint_writings >> 1);

    //printf("DEBUG Rint Busy: ");
    //for (i = 0; i < TAM_REGISTROS; i++) {
    //    printf("i=%d %d ", i, Rint[i].busy);
    //}
    //printf("\n");
    //printf("Ciclo %d F0 busy %d\n",Ciclo,Rfp[0].busy);


    Rfp_writings = Rfp_writings >> 1;
    Rint_writings = Rint_writings >> 1;


} /* end impulso_reloj */
