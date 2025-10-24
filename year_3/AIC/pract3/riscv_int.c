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
 * Fichero: riscV_int.c
 *
 * Descripción:
 *   Contiene la microarquitectura del RISCV 5 etapas+multiciclo
 *
 ****************************************************************************/


#define riscv_int_C

/*** Includes **************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


/*** Definiciones Externas *************************************************/
// En *este archivo* (y solo en este), toda referencia a MEM_WB se entiende a iWB
#define MEM_WB iWB
#define es_load(i) inst_load(i.codop)

#include "main.h"
#include "instrucciones.h"
#include "depurar.h"
#include "control.h"
#include "riscv_int.h"
#include "log.h"


/*** Variables Globales ****************************************************/

/*** Variables Locales *****************************************************/

/*** Funciones Locales *****************************************************/

/*** Funciones Exportables *************************************************/

/**
 * Realiza la operacion en la ALU INT
 *
 * @param codop Indice de la operacion
 * @param in1 Operando1
 * @param in2 Operando2
 *
 */
dword operacion_ALU(int codop, dword in1, dword in2) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result = 0;
    uint64_t aux1, aux2;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    switch (codop) {
        case RV32I_ECALL:
            break;
            _BRANCHES
        case RV32I_JAL: // yacc devuelve 2 veces el desplazamiento 
            result = in1  + in2;
            break;
            _LOADS_INT
            _STORES_INT
        case RV32I_JALR:
        case RV32I_ADD:
        case RV32I_ADDI:
            result = in1 + in2;

            break;
        case RV32I_SUB:
            result = in1 - in2;

            break;
        case RV32I_AND:
        case RV32I_ANDI:
            result = in1 & in2;

            break;
        case RV32I_OR:
        case RV32I_ORI:
            result = in1 | in2;

            break;
        case RV32I_XOR:
        case RV32I_XORI:
            result = in1 ^ in2;

            break;
        case RV32I_SRA:
        case RV64I_SRAI:
            result = in1 >> (in2 & 0x3f);

            break;
        case RV32I_SLL:
        case RV64I_SLLI:
            result = in1 << (in2 & 0x3f);
            break;
        case RV32I_SRL:
        case RV64I_SRLI:
            aux1 = (uxword) in1;
            aux2 = (uxword) in2 & 0x3f;
            result = aux1 >> aux2;

            break;
        case RV32I_SLT:
        case RV32I_SLTI:
            result = (in1 < in2);

            break;
        case RV32I_SLTU:
        case RV32I_SLTIU:
            result = ((uxword) in1 < in2);

            break;
        case RV32I_LUI:
            result = in2 << 12;

            break;
        case RV32I_AUIPC:
            result = (in1 + (in2 << 12));
            break;


        case RV64I_ADDIW:
        case RV64I_ADDW:
            result = (word) in1 + in2;

            break;
        case RV64I_SUBW:
            result = (word) in1 - in2;

            break;
        case RV64I_SLLIW:
        case RV64I_SLLW:
            result = (word) in1 << (in2 & 0x1f);

            break;
        case RV64I_SRLIW:
        case RV64I_SRLW:
            aux1 = (uword) in1;
            aux2 = (uword) in2 & 0x1f;
            result = (word) aux1 >> aux2;

            break;
        case RV64I_SRAIW:
        case RV64I_SRAW:
            result = (word) in1 >> (in2 & 0x1f);

            break;
        case PSEUDO_NOP: // Es una nop generada internamente
            break;

        default:
            // fnError("Operación %d no implementada\n", codop)
            fnWarning("Operación %d no implementada\n", codop)
                    //fprintf(stderr,
                    //        "ERROR (%d %d): Operación no implementada\n",
                    //        __LINE__, codop);
                    //exit(1);
    } /* endswitch */
    fnDebug(NO, "operacion ALU codop %d in1 %ld in2 %ld result %ld\n", codop, in1, in2, result);
    return result;
} /* end operacion_ALU */

/**
 * Realiza la operacion en el COMPARADOR de la etapa EX
 *
 * @param codop Indice de la operacion
 * @param in1 Operando1
 * @param in2 Operando2
 *
 */
dword operacion_COMP(ubyte codop, dword in1, dword in2) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    result = 0; /* no salta */

    /*** Operacion en el comparador */
    switch (codop) {
        case RV32I_BEQ:       
            result = (in1 == in2);

            break;
        case RV32I_BNE:
        	result = (in1 != in2);

            break;
        case RV32I_BLT:     
            result = (in1 < in2);

            break;
        case RV32I_BLTU:           
            result = ((uxword) in1 < (uxword) in2);

            break;
        case RV32I_BGE:           
            result = (in1 >= in2);

            break;
        case RV32I_BGEU:
            result = ((uxword) in1 >= (uxword) in2);

            break;
        case RV32I_JAL:
        case RV32I_JALR:
            result = 1;
            break;
        default:
            result = 0; /* No salta */
    } /* endswitch */

 	fnDebug(NO, "operacion COMP codop %d in1 %ld in2 %ld result %ld\n", codop, in1, in2, result);

    return (result);
} /* end operacion_COMP */

/**
 * Implementa el multiplexor superior ALU int
 *
 * @param ncp PC incrementado
 * @param ra Valor leido del RF
 * @param mem Valor que procede de la etapa MEM
 * @param wb Valor que procede de la etapa WB
 *
 * @return Salida del multiplexor
 */
dword mux_ALUsup(dword pc, dword ra, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result = 0;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaEXalu_s = NO;
    MEMaEXalu_s = NO;

    switch (ID_EX.IR.codop) {
            _BRANCHES
        case RV32I_JAL:
            result = pc;
            EX.muxALU_sup = 0;
            break;
        case RV32I_AUIPC:
            result = pc ;
            EX.muxALU_sup = 0;
            break;
        default: /* el resto de instrucciones */
            result = ra; /* por defecto */
            EX.muxALU_sup = 3;
            switch (solucion_riesgos_datos) {
                case ninguno:
                case parada:
                    break;
                case cortocircuito:
                    /* Los cortocircuitos desde MEM tienen prioridad sobre WB, ya
                       que la instruccion que hay en MEM es mas moderna */

                    
                    /* MEMtoEX */

                    if (rd_valido(EX_MEM.IR) && rs1_valido(ID_EX.IR) && (EX_MEM.IR.rd == ID_EX.IR.rs1)) {
                        result = mem;
                        MEMaEXalu_s = SI;
                        EX.muxALU_sup = 1;
                        write_log_2("corto MEMaEXalu_s", ID_EX.iPC, EX_MEM.iPC, "EX", "MEM");
                        break;
                    }

                    /* WBtoEX */
                    
              /* INSERTAR CÓDIGO */

                    if (rd_valido(MEM_WB.IR) && rs1_valido(ID_EX.IR) && (MEM_WB.IR.rd == ID_EX.IR.rs1)) {
                        result = wb;
                        WBaEXalu_s = SI;
                        EX.muxALU_sup = 2;
                        write_log_2("corto WBaEXalu_s", ID_EX.iPC, MEM_WB.iPC, "EX", "WB");
                    }

                    break;
            }
    }

    fnDebug(NO, "muxALUsup pc %ld ra %ld mem %ld wb %ld result %ld\n", pc, ra, mem, wb, result);
    return (result);
} /* end mux_ALUsup */

/**
 * Implementa el multiplexor inferior ALU int
 *
 * @param rb Valor leido del RF
 * @param imm Campo inmediato
 * @param mem Valor que procede de la etapa MEM
 * @param wb Valor que procede de la etapa WB
 *
 * @return Salida del multiplexor
 */
dword mux_ALUinf(dword rb, dword imm, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result = 0;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaEXalu_i = NO;
    MEMaEXalu_i = NO;

    switch (ID_EX.IR.tipo) {
        case FormatoR:
            result = rb; /* por defecto */
            EX.muxALU_inf = 0;
            if (solucion_riesgos_datos == cortocircuito) {
                /* Los cortocircuitos desde MEM tienen prioridad sobre WB, ya
                que la instruccion que hay en MEM es mas moderna */

                
                /* MEMtoEX */
                
              /* INSERTAR CÓDIGO */

                if (rd_valido(EX_MEM.IR) && rs2_valido(ID_EX.IR) && (EX_MEM.IR.rd == ID_EX.IR.rs2)) {
                    result = mem;
                    MEMaEXalu_i = SI;
                    EX.muxALU_inf = 1;
                    write_log_2("corto MEMaEXalu_i", ID_EX.iPC, EX_MEM.iPC, "EX", "MEM");
                    break;
                }

                /* WBtoEX */
                
              /* INSERTAR CÓDIGO */

                if (rd_valido(MEM_WB.IR) && rs2_valido(ID_EX.IR) && (MEM_WB.IR.rd == ID_EX.IR.rs2)) {
                    result = wb;

                    WBaEXalu_i = SI;
                    EX.muxALU_inf = 2;
                    write_log_2("corto WBaEXalu_i", ID_EX.iPC, MEM_WB.iPC, "EX", "WB");
                }

                break;
            }
            break;
        case FormatoI:
        case FormatoIshift:
        case FormatoS:
        case FormatoB:
        case FormatoU:
        case FormatoJ:
            result = imm;
            EX.muxALU_inf = 3;
            break;
        default:
            result = rb;
            EX.muxALU_inf = 0;
    } /* endswitch */

    fnDebug(NO, "muxALUinf rb %ld imm %ld mem %ld wb %ld result %ld\n", rb, imm, mem, wb, result);

    return (result);
} /* end mux_ALUinf */

/**
 * Implementa el multiplexor superior COMP de la etapa EX
 *
 * @param ra Valor leido del RF
 * @param mem Valor que procede de la etapa MEM
 * @param wb Valor que procede de la etapa WB
 *
 * @return Salida del multiplexor
 */
dword mux_COMPsup(dword ra, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    MEMaEXcomp_s = NO;
    WBaEXcomp_s = NO;

    result = ra; /* por defecto */

    if ((latencia_salto3(solucion_riesgos_control) || latencia_salto2(solucion_riesgos_control)) && (solucion_riesgos_datos == cortocircuito)) {
        //if (latencia_salto3(solucion_riesgos_control) && (solucion_riesgos_datos == cortocircuito)) {
        switch (ID_EX.IR.codop) {
                _BRANCHES
                        /* MEMtoEX */
                        
                /* WBtoEX */
                                break;
            default:
                break;
        };
    };
    fnDebug(NO, "muxCOMPsup op=%d ra=%ld mem=%ld wb=%ld result=%ld\n", ID_EX.IR.codop, ra, mem, wb, result);

    return (result);
} /* end mux_COMPsup */

/**
 * Implementa el multiplexor inferior COMP de la etapa EX
 *
 * @param rb Valor leido del RF
 * @param mem Valor que procede de la etapa MEM
 * @param wb Valor que procede de la etapa WB
 *
 * @return Salida del multiplexor
 */
dword mux_COMPinf(dword rb, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    MEMaEXcomp_i = NO;
    WBaEXcomp_i = NO;

    result = rb; /* por defecto */

    if (solucion_riesgos_datos == cortocircuito) {
        switch (ID_EX.IR.codop) {
                _BRANCHES
                        /* MEMtoEX */
                        
                /* WBtoEX */
                                break;
            default:
                break;
        };
    };

    //	fprintf(stdout,"DEBUG muxCOMP op=%d ra=%d mem=%d wb=%d fpsr_wb=%d result=%d\n",ID_EX.IR.codop,ra,mem,wb,fpsr_wb,result);

    return (result);
} /* end mux_COMPinf */

/**
 * Implementa el multiplexor Stores MEM de la etapa EX
 *
 * @param rb Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 *
 * @return Salida del multiplexor
 */
dword mux_EXmem(dword rb, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaEXmem = NO;


    result = rb; /* por defecto */

    switch (solucion_riesgos_datos) {
        case cortocircuito:
            /* WBtoEX */
                        /* MEMtoEX */
            /* No hace falta. WBtoMEM lo resuelve */
            break;
        default:
            break;
    }
    return (result);
} /* end mux_EXmem */

/**
 * Implementa el multiplexor Stores MEM de la etapa MEM
 *
 * @param rb Valor leido del RF
 * @param wb Valor que procede de la etapa WB
 *
 * @return Salida del multiplexor
 */
dword mux_MEMmem(dword rb, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaMEM = NO;

    result = rb; /* por defecto */

    switch (solucion_riesgos_datos) {
        case cortocircuito:
            /* WBtoMEM */
                        break;
        default:
            break;
    }
    return (result);
} /* end mux_MEMmem */

/**
 * Implementa el multiplexor superior COMP de la etapa ID
 *
 * @param ra Valor leido del RF
 * @param mem Valor que procede de la etapa MEM
 *
 * @return Salida del multiplexor
 */
dword mux_COMPidsup(dword ra, dword mem) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    MEMaIDcomp_s = NO;

    //FP_WBaIDcomp = NO;

    result = ra; /* por defecto */

    switch (solucion_riesgos_datos) {
        case cortocircuito:
            switch (IF_ID.IR.codop) {
                    _BRANCHES
                    case RV32I_JALR:
                            /* MEMtoID */
                                                /* WBtoIDn no hay */

                    break;

                default:
                    break;
            };
        default:
            break;
    };

    return (result);
} /* end mux_COMPidsup */

/**
 * Implementa el multiplexor inferior COMP de la etapa ID
 *
 * @param rb Valor leido del RF
 * @param mem Valor que procede de la etapa MEM
 *
 * @return Salida del multiplexor
 */
dword mux_COMPidinf(dword rb, dword mem) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    MEMaIDcomp_i = NO;

    result = rb; /* por defecto */

    switch (solucion_riesgos_datos) {
        case cortocircuito:
            switch (IF_ID.IR.codop) {
                    _BRANCHES
                            /* MEMtoID */
                                                /* WBtoID, no hay */

                    break;
                default:
                    break;
            };
        default:
            break;
    };
    return (result);
} /* end mux_COMPidinf */

/**
 *  Detecta los riesgos de control, activando las señales IDstall, IFstall en su caso
 *
 */
void detectar_riesgos_control(void) {

    /* riesgos de control */

    switch (solucion_riesgos_control) {
        case stall3:
        case stall2:
        case stall1:
            if (inst_salto(IF_ID.IR.codop)) {
                IFstall = SI;
                write_log("IFstall", IF_ID.iPC);
            }
            break;

        default:
            ;
    } /* end switch */
}

/**
 *  Detecta los riesgos de datos, activando las señales IDstall, IFstall en su caso
 *
 */
void detectar_riesgos_datos(void) {

    /* riesgos de datos: insertar ciclos de parada? */

    switch (solucion_riesgos_datos) {
        case parada:
            /* Riesgo entre EX e ID */
            /* if (...) {
                    Señal=SI
                    ...
               }
             */
            
            if (rd_valido(ID_EX.IR) && ((rs1_valido(IF_ID.IR) && (IF_ID.IR.rs1 == ID_EX.IR.rd) || (rs2_valido(IF_ID.IR) && (IF_ID.IR.rs2 == ID_EX.IR.rd))))) {
                IDstall = SI;
                IFstall = SI;
            }


            /* Riesgo entre MEM e ID */
            /* if (...) {
                    Señal=SI
                    ...
               }
             */
            
            if (rd_valido(EX_MEM.IR) && ((rs1_valido(IF_ID.IR) && (IF_ID.IR.rs1 == EX_MEM.IR.rd) || (rs2_valido(IF_ID.IR) && (IF_ID.IR.rs2 == EX_MEM.IR.rd))))) {
                IDstall = SI;
                IFstall = SI;
            }

            break;

        case cortocircuito:
            /* Riesgo entre LOAD en EX e ID */
            if (rd_valido(ID_EX.IR) && rs1_valido(IF_ID.IR) && es_load(ID_EX.IR) && (ID_EX.IR.rd == IF_ID.IR.rs1 || ID_EX.IR.rd == IF_ID.IR.rs2)) {
                IDstall = SI;
                IFstall = SI;
            }
            
              /* INSERTAR CÓDIGO */


                        break;
        default:
            break;

    } /* endswitch */
}
/**
 *  Detecta los riesgos RAW INT, activando las señales IDstall, IFstall en su caso
 *
 */
void detectar_riesgos_RAW_INT(void) {
    }

/**
 *  Detecta los riesgos WAW INT, activando las señales IDstall, IFstall en su caso
 *
 */
void detectar_riesgos_WAW_INT(void) {
    switch (IF_ID.IR.codop) {
            _MULS_INT
                    /* Riesgos WAW */
            if (Rint[IF_ID.IR.rd].busy >= Ciclo + FP_MUL_LAT + 1) {
                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1x("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rd);
            };
            break;
            _CVT_FP_TO_INT
            _CLASS
            if (Rint[IF_ID.IR.rd].busy >= Ciclo + FP_MISC_LAT + 1) {
                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1x("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rd);
                fnDebug(NO, " WAW codop %d Rint[%d].busy=%d\n", IF_ID.IR.codop, IF_ID.IR.rd, Rint[IF_ID.IR.rd].busy);
            };
            break;
            _CMP_FP_S
            _CMP_FP_D
            if (Rint[IF_ID.IR.rd].busy >= Ciclo + FP_CMP_LAT + 1) {
                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1x("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rd);
                fnDebug(SI, " WAW codop %d Rint[%d].busy=%d\n", IF_ID.IR.codop, IF_ID.IR.rd, Rint[IF_ID.IR.rd].busy);
            };
            break;
        default:
            if (Rint[IF_ID.IR.rd].busy >= Ciclo + 2 + 1) {
                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1x("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.rd);
            };
            break;

    };
}

/**
 *  Detecta los riesgos estructurales en WB, activando las señales IDstall, IFstall en su caso
 *
 */
void detectar_riesgos_WB_INT(void) {
    int i;

    /* Riesgos estructurales WB, con cualquier registro  */
    switch (IF_ID.IR.codop) {

            _MULS_INT
            _MULS_INTW
            i = 1 << (INT_MUL_LAT + 1);
            if ((Rint_writings & i) == i) {
                IDstall = SI;
                IFstall = SI;
                estructural = SI;
                write_log("Estructural_WBint IDstall, IFstall", IF_ID.iPC);
                fnDebug(NO, " WBconflict codop %d i=%d; Rint_writings=%x\n", IF_ID.IR.codop, i, Rint_writings);
            }
            break;

            _CMP_FP_S
            _CMP_FP_D
            i = 1 << (FP_CMP_LAT + 1);
            if ((Rint_writings & i) == i) {
                IDstall = SI;
                IFstall = SI;
                estructural = SI;
                write_log("Estructural_WBint IDstall, IFstall", IF_ID.iPC);
                fnDebug(NO, " WBconflict codop %d i=%d; Rint_writings=%x\n", IF_ID.IR.codop, i, Rint_writings);
            }
            break;

            _CVT_FP_TO_INT
            _CLASS
            i = 1 << (FP_MISC_LAT + 1);
            if ((Rint_writings & i) == i) {
                IDstall = SI;
                IFstall = SI;
                estructural = SI;
                write_log("Estructural_WBint IDstall, IFstall", IF_ID.iPC);
                fnDebug(NO, " WBconflict codop %d i=%d; Rint_writings=%x\n", IF_ID.IR.codop, i, Rint_writings);
            }
            break;

        default:
            if inst_int_simple(IF_ID.IR.codop) {
                i = 1 << 3; // Escribe dentro de 3 ciclos
                if ((Rint_writings & i) == i) {
                    IDstall = SI;
                    IFstall = SI;
                    estructural = SI;
                    write_log("Estructural_WBint IDstall, IFstall", IF_ID.iPC);
                    fnDebug(NO, " WBconflict codop %d i=%d; Rint_writings=%x\n", IF_ID.IR.codop, i, Rint_writings);
                }
            }
            break;
            ;

    };
}



