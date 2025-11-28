/**
 * @file f_ejecucion.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene la fase de ejecución de los operadores de la ruta de datos
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define f_ejecucion_C

/***** Includes ***********************************************/

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <math.h>
#include <fenv.h>

#include "main.h"
#include "instrucciones.h"
#include "presentacion.h"
#include "memoria.h"
#include "memoria-cache.h"

#include "tipos.h"
#include "codop.h"


#ifndef CPROTO

#include "f_ejecucion_api.h"

#endif

/**
 * Implementa la fase de ejecución de los operadores de la ruta de datos.
 * 
 * Comienza la ejecución de las operaciones que tienen todos los operandos
 * y el operador libre.
 */
void fase_EX_1() {

    unsigned int oper;
    unsigned int oper_iniciadas[NUM_TIPOS_OPERADORES];

    boolean iniciada;

    for (oper = 0; oper < NUM_TIPOS_OPERADORES; oper++) {
        oper_iniciadas[oper] = 0;
    }

    for (oper = 0; oper < NUM_OPERADORES; oper++) {
        iniciada = NO;
        if (!Op[oper].ocupado) { /* Si el operador está libre */
            switch (Op[oper].tipo) {
                case OPER_ENTEROS:
                    if (oper_iniciadas[OPER_ENTEROS] >= CONFIG(enteros.num_inicio))
                        break;
                    iniciada = fase_EX_1_enteros(oper);
                    break;
                case OPER_SUMREST:
                    if (oper_iniciadas[OPER_SUMREST] >= CONFIG(sumrest.num_inicio))
                        break;
                    iniciada = fase_EX_1_sumrest(oper);
                    break;
                case OPER_MULTDIV:
                    if (oper_iniciadas[OPER_MULTDIV] >= CONFIG(multdiv.num_inicio))
                        break;
                    iniciada = fase_EX_1_multdiv(oper);
                    break;
                case OPER_DIRECCIONES:
                    if (oper_iniciadas[OPER_DIRECCIONES] >= CONFIG(direcciones.num_inicio))
                        break;
                    iniciada = fase_EX_1_direcciones(oper);
                    break;
                case OPER_MEMDATOS:
                    if (oper_iniciadas[OPER_MEMDATOS] >= CONFIG(memdatos.num_inicio))
                        break;
                    iniciada = fase_EX_1_memdatos(oper);
                    break;
                default:
                    fprintf(stderr, "ERROR EX (%s:%d): Operador no implementado\n",
                            __FILE__, __LINE__);
                    //exit(1);
                    break;
            }

            if (iniciada)
                oper_iniciadas[Op[oper].tipo]++;
        }
    }
}

boolean fase_EX_1_enteros(unsigned int oper) {

    marca_t s, r;
    ciclo_t siguiente;

    s = CONFIG(tam_estaciones);
    siguiente = MAX_ORDEN;

    /*** Busca la instrucción más antigua que este preparada */

    for (r = INICIO_RS_ENTEROS;
         r <= FIN_RS_ENTEROS; r++) {
        if (!RS[r].ocupado || RS[r].estado != PENDIENTE)
            continue;

        if (RS[r].Q1 == MARCA_NULA &&
            RS[r].Q2 == MARCA_NULA &&
            /* Tiene todos los operandos disponibles */
            RS[r].orden < siguiente) {
            siguiente = RS[r].orden;
            s = r;
        } /* endif */
    } /* endfor */

    /*** Entra en ejecución la operación más antigua, si existe */

    if (s < CONFIG(tam_estaciones)) {
        Op[oper].ocupado = SI;
        Op[oper].ciclo = 0;
        Op[oper].codigo = RS[s].rob;
        Op[oper].estacion = s;
        RS[s].estado = EN_CURSO;
        RS[s].operador = oper;

        return SI;
    } /* endif */

    return NO;

} /* end fase_EX_1_enteros) */

boolean fase_EX_1_sumrest(
        unsigned int oper
) {

    marca_t s, r;
    ciclo_t siguiente;

    s = CONFIG(tam_estaciones);
    siguiente = MAX_ORDEN;

    /*** Busca la instrucción más antigua que este preparada */

    for (r = INICIO_RS_SUMREST;
         r <= FIN_RS_SUMREST; r++) {
        if (!RS[r].ocupado || RS[r].estado != PENDIENTE)
            continue;

        if (RS[r].Q1 == MARCA_NULA &&
            RS[r].Q2 == MARCA_NULA &&
            /* Tiene todos los operandos disponibles */
            RS[r].orden < siguiente) {
            siguiente = RS[r].orden;
            s = r;
        } /* endif */
    } /* endfor */

    /*** Entra en ejecución la operación más antigua, si existe */

    if (s < CONFIG(tam_estaciones)) {
        Op[oper].ocupado = SI;
        Op[oper].ciclo = 0;
        Op[oper].codigo = RS[s].rob;
        Op[oper].estacion = s;
        RS[s].estado = EN_CURSO;
        RS[s].operador = oper;

        return SI;
    } /* endif */

    return NO;

} /* end fase_EX_1_sumrest) */

boolean fase_EX_1_multdiv(
        unsigned int oper
) {

    marca_t s, r;

    ciclo_t siguiente;

    s = CONFIG(tam_estaciones);
    siguiente = MAX_ORDEN;

    /*** Busca la instrucción más antigua que este preparada */

    for (r = INICIO_RS_MULTDIV;
         r <= FIN_RS_MULTDIV; r++) {
        if (!RS[r].ocupado || RS[r].estado != PENDIENTE)
            continue;

        if (RS[r].Q1 == MARCA_NULA &&
            RS[r].Q2 == MARCA_NULA &&
            /* Tiene todos los operandos disponibles */
            RS[r].orden < siguiente) {

            siguiente = RS[r].orden;
            s = r;
        } /* endif */
    } /* endfor */

    /*** Entra en ejecución la operación más antigua, si existe */

    if (s < CONFIG(tam_estaciones)) {
        Op[oper].ocupado = SI;
        Op[oper].ciclo = 0;
        Op[oper].codigo = RS[s].rob;
        Op[oper].estacion = s;
        RS[s].estado = EN_CURSO;
        RS[s].operador = oper;

        return SI;
    } /* endif */

    return NO;

} /* end fase_EX_1_multdiv) */

boolean fase_EX_1_direcciones(
        unsigned int oper
) {

    marca_t s, r;

    ciclo_t siguiente;

    s = CONFIG(tam_estaciones);
    siguiente = MAX_ORDEN;

    /*** Busca la instrucción más antigua que este preparada */

    for (r = INICIO_BUFFERS;
         r <= FIN_BUFFERS; r++) {
        if (!RS[r].ocupado || RS[r].estado != PENDIENTE)
            continue;

        if (RS[r].Q1 == MARCA_NULA &&
            /* Tiene todos los operandos disponibles */
            RS[r].orden < siguiente) {
            siguiente = RS[r].orden;
            s = r;
        } /* endif */
    } /* endfor */

    /*** Entra en ejecución la operación más antigua, si existe */

    if (s < CONFIG(tam_estaciones)) {
        Op[oper].ocupado = SI;
        Op[oper].ciclo = 0;
        Op[oper].codigo = RS[s].rob; // Se utiliza para las excepciones
        Op[oper].estacion = s;
        RS[s].estado = EN_CURSO;
        RS[s].operador = oper;
        memset(&RS[s].resultado, 0, sizeof(valor_t));

        return SI;
    } /* endif */

    return NO;

} /* end fase_EX_1_direcciones) */

boolean fase_EX_1_memdatos(
        unsigned int oper
) {
    boolean RC = NO;

    marca_t s, r, t;
    ciclo_t siguiente;
    tipo_operacion_memoria_t tipo = OP_NULA;

    s = CONFIG(tam_estaciones);
    siguiente = MAX_ORDEN;

    /*** Busca la instrucción más antigua que este preparada */

    for (r = INICIO_BUFFER_CARGA; r <= FIN_BUFFER_CARGA; r++) {
        if (!LB[r].ocupado || LB[r].estado != PREPARADA)
            continue;

        /* Comprueba que no haya ninguna escritura pendiente en la misma
         * dirección */
        /* No comprueba si las escrituras sin la dirección
           calculada porque como las direcciones se calculan en
           orden esa escritura será posterior a la lectura. REVISAR
         */
        for (t = INICIO_BUFFER_ALMACEN; t <= FIN_BUFFER_ALMACEN; t++) {
            if (SB[t].ocupado && SB[t].estado != PENDIENTE &&
                SB[t].direccion == LB[r].direccion &&
                SB[t].orden < LB[r].orden) {
                if (CONFIG(dep_mem) == l_forwarding &&
                    SB[t].Q2 == MARCA_NULA) {
                    /* Load forwarding */
                    LB[r].estado = FINALIZADA;
                    LB[r].resultado = SB[t].V2;
                } /* endif */
                break;
            }
        }

        if (t > FIN_BUFFER_ALMACEN) { /* No hay conflictos con ninguna escritura */
            if (LB[r].orden < siguiente) {
                siguiente = LB[r].orden;
                s = r;
                tipo = OP_LECT;
            }
        }
    }

    for (r = INICIO_BUFFER_ALMACEN; r <= FIN_BUFFER_ALMACEN; r++) {
        if (!SB[r].ocupado || !SB[r].confirm || SB[r].estado == EN_CURSO)
            continue;

        if (SB[r].Q1 == MARCA_NULA &&
            SB[r].orden < siguiente) {
            siguiente = SB[r].orden;
            s = r;
            tipo = OP_ESCR;
        }
    }

    /*** Entra en ejecución la operación más antigua, si existe */

    if (tipo != OP_NULA) {
        if (!L1D.mem.activa) {
            // L1D no está activa. Se asume una cache perfecta
            Op[oper].ocupado = SI;
            Op[oper].ciclo = 0;
            Op[oper].codigo = RS[s].rob; /* RS = TL = TE */
            Op[oper].estacion = s;
            RS[s].estado = EN_CURSO;
            RS[s].operador = oper;

            RC = SI;
        } else {
            RC = INICIA_OPERACION_MEMORIA(PTR_NIVEL(L1D), tipo, NULL, RS[s].direccion, sizeof(valor_t), RS[s].orden,
                                          operacion_finalizada_L1D, &RS[s]);
            if (RC == SI) {
                RS[s].estado = EN_CURSO;
                RS[s].operador = -1; // No hay un operador asociado. El estado se debe consultar en la L1D
            }
        }
    } /* endif */

    return RC;

} /* end fase_EX_1_memdatos */

/**
 * Implementa la fase de ejecución de los operadores de la ruta de datos.
 *
 * Avanza la ejecución de las operaciones que se están
 * ejecutando en cada operador.
 */
void fase_EX_2() {

    unsigned int oper;

    for (oper = 0; oper < NUM_OPERADORES; oper++) {
        if (Op[oper].ocupado) { /* Si el operador está ocupado */
            switch (Op[oper].tipo) {
                case OPER_ENTEROS:
                    fase_EX_2_enteros(oper);
                    break;
                case OPER_SUMREST:
                    fase_EX_2_sumrest(oper);
                    break;
                case OPER_MULTDIV:
                    fase_EX_2_multdiv(oper);
                    break;
                case OPER_DIRECCIONES:
                    fase_EX_2_direcciones(oper);
                    break;
                case OPER_MEMDATOS:
                    fase_EX_2_memdatos(oper);
                    break;
                default:
                    fprintf(stderr, "ERROR Oper (%s:%d): Operador no implementado\n",
                            __FILE__, __LINE__);
                    //exit(1);
                    break;
            }
        }
    }
}

void fase_EX_2_enteros(unsigned int oper) {

    marca_t s;
    uint64_t aux1, aux2;

    if (Op[oper].ciclo < Op[oper].Teval)
        Op[oper].ciclo++; /* Si quedan ciclos por realizar */

    s = Op[oper].estacion;

    /*** VISUALIZACIÓN ****/
    sprintf(p_str, "E%u", Op[oper].ciclo);
    muestra_fase(p_str, RS[s].orden);
    /**********************/

    if (Op[oper].ciclo == Op[oper].Teval) {
        RS[s].estado = FINALIZADA;
        Op[oper].ocupado = NO;

        RS[s].condicion = NO; // Por defecto
        RS[s].resultado.int_d = 0; // Por defecto

        /* Cálculo del resultado */
        switch (RS[s].OP) {
            case RV32I_ADD:
            case RV32I_ADDI:
                RS[s].resultado.int_d = RS[s].V1.int_d + RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case RV32I_SUB:
                RS[s].resultado.int_d = RS[s].V1.int_d - RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case RV32I_AND:
            case RV32I_ANDI:
                RS[s].resultado.int_d = RS[s].V1.int_d & RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case RV32I_OR:
            case RV32I_ORI:
                RS[s].resultado.int_d = RS[s].V1.int_d | RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case RV32I_XOR:
            case RV32I_XORI:
                RS[s].resultado.int_d = RS[s].V1.int_d ^ RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case RV32I_SRA:
            case RV64I_SRAI:
                RS[s].resultado.int_d = RS[s].V1.int_d >> (RS[s].V2.int_d & 0x3f);
                RS[s].control = RESULT;
                break;
            case RV32I_SLL:
            case RV64I_SLLI:
                RS[s].resultado.int_d = RS[s].V1.int_d << (RS[s].V2.int_d & 0x3f);
                RS[s].control = RESULT;
                break;
            case RV32I_SRL:
            case RV64I_SRLI:
                aux1 = (uxword) RS[s].V1.int_d;
                aux2 = (uxword) RS[s].V2.int_d & 0x3f;
                RS[s].resultado.int_d = aux1 >> aux2;
                RS[s].control = RESULT;
                break;
            case RV32I_SLT:
            case RV32I_SLTI:
                RS[s].resultado.int_d = (RS[s].V1.int_d < RS[s].V2.int_d);
                RS[s].control = RESULT;
                break;
            case RV32I_SLTU:
            case RV32I_SLTIU:
                RS[s].resultado.int_d = ((uxword) RS[s].V1.int_d < RS[s].V2.int_d);
                RS[s].control = RESULT;
                break;
            case RV32I_LUI:
                RS[s].resultado.int_d = RS[s].V2.int_d << 12;
                RS[s].control = RESULT;
                break;
            case RV32I_AUIPC:
                RS[s].resultado.int_d = (RS[s].V1.int_d + (RS[s].V2.int_d << 12));
                RS[s].control = RESULT;
                break;
            case RV32I_BEQ:
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                RS[s].condicion = (RS[s].V1.int_d == RS[s].V2.int_d);
                RS[s].control = CONDICION;
                break;
            case RV32I_BNE:
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                RS[s].condicion = (RS[s].V1.int_d != RS[s].V2.int_d);
                RS[s].control = CONDICION;
                break;
            case RV32I_BLT:
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                RS[s].condicion = (RS[s].V1.int_d < RS[s].V2.int_d);
                RS[s].control = CONDICION;
                break;
            case RV32I_BLTU:
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                RS[s].condicion = ((uxword) RS[s].V1.int_d < (uxword) RS[s].V2.int_d);
                RS[s].control = CONDICION;
                break;
            case RV32I_BGE:
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                RS[s].condicion = (RS[s].V1.int_d >= RS[s].V2.int_d);
                RS[s].control = CONDICION;
                break;
            case RV32I_BGEU:
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                RS[s].condicion = ((uxword) RS[s].V1.int_d >= (uxword) RS[s].V2.int_d);
                RS[s].control = CONDICION;
                break;
            case RV32I_JAL:
                // No pasa por EX
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                //RS[s].resultado.int_d = RS[s].PC + 4; // R31
                //RS[s].condicion = 1; // Salta
                break;
            case RV32I_JALR:
                RS[s].resultado.int_d = RS[s].V1.int_d + RS[s].desplazamiento;
                //RS[s].resultado.int_d = RS[s].PC + 4; // R31
                //RS[s].condicion = 1; // Salta
                RS[s].condicion = SI;
                RS[s].control = DIRECCION;
                break;
            case RV64I_ADDIW:
            case RV64I_ADDW:
                RS[s].resultado.int_d = (word) RS[s].V1.int_d + RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case RV64I_SUBW:
                RS[s].resultado.int_d = (word) RS[s].V1.int_d - RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case RV64I_SLLIW:
            case RV64I_SLLW:
                RS[s].resultado.int_d = (word) RS[s].V1.int_d << (RS[s].V2.int_d & 0x1f);
                RS[s].control = RESULT;
                break;
            case RV64I_SRLIW:
            case RV64I_SRLW:
                aux1 = (uword) RS[s].V1.int_d;
                aux2 = (uword) RS[s].V2.int_d & 0x1f;
                RS[s].resultado.int_d = (word) aux1 >> aux2;
                RS[s].control = RESULT;
                break;
            case RV64I_SRAIW:
            case RV64I_SRAW:
                RS[s].resultado.int_d = (word) RS[s].V1.int_d >> (RS[s].V2.int_d & 0x1f);
                RS[s].control = RESULT;
                break;
            default:
                fprintf(stderr, "ERROR Oper Enteros (%s:%d): Operación %s (%d) no implementada\n",
                        __FILE__, __LINE__, instruction_list[RS[s].OP].nemo, RS[s].OP);
                //exit(1);
        } /* endswitch */
    } /* endif */

}

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
            return 0x1 << 4;
        else if (clase == FP_NORMAL)
            return 0x1 << 5;
        else if (clase == FP_SUBNORMAL)
            return 0x1 << 6;
        else if (clase == FP_ZERO)
            return 0x1 << 7;
        else if (clase == FP_NAN)
            return 0x1 << 9;
    }
    // Para eliminar warnings
    return 0;
}

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

void fase_EX_2_sumrest(unsigned int oper) {

    marca_t s;
    int s1, s2;

    if (Op[oper].ciclo < Op[oper].Teval)
        Op[oper].ciclo++; /* Si quedan ciclos por realizar */

    s = Op[oper].estacion;

    /*** VISUALIZACIÓN ****/
    sprintf(p_str, "A%u", Op[oper].ciclo);
    muestra_fase(p_str, RS[s].orden);
    /**********************/

    if (Op[oper].ciclo == Op[oper].Teval) {
        RS[s].estado = FINALIZADA;
        RS[s].condicion = NO;

        Op[oper].ocupado = NO;

        RS[s].control = RESULT;
        /* Cálculo del resultado */
        switch (RS[s].OP) {
            case RV32F_FADD_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = RS[s].V1.fp_s + RS[s].V2.fp_s;
                break;
            case RV32F_FSUB_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = RS[s].V1.fp_s - RS[s].V2.fp_s;
                break;
            case RV32F_FSGNJ_S:
                if (RS[s].V2.fp_s >= 0)
                    RS[s].resultado.fp_s = fabsf(RS[s].V1.fp_s);
                else
                    RS[s].resultado.fp_s = -fabsf(RS[s].V1.fp_s);
                break;
            case RV32F_FSGNJN_S:
                if (RS[s].V2.fp_s >= 0)
                    RS[s].resultado.fp_s = -fabsf(RS[s].V1.fp_s);
                else
                    RS[s].resultado.fp_s = fabsf(RS[s].V1.fp_s);
                break;
            case RV32F_FSGNJX_S:
                s1 = (RS[s].V1.fp_s >= 0.0);
                s2 = (RS[s].V2.fp_s >= 0.0);
                if (s1 ^ s2)
                    RS[s].resultado.fp_s = -fabsf(RS[s].V1.fp_s);
                else
                    RS[s].resultado.fp_s = fabsf(RS[s].V1.fp_s);
                break;
            case RV32F_FMIN_S:
                if (RS[s].V1.fp_s < RS[s].V2.fp_s)
                    RS[s].resultado.fp_s = RS[s].V1.fp_s;
                else
                    RS[s].resultado.fp_s = RS[s].V2.fp_s;
                break;
            case RV32F_FMAX_S:
                if (RS[s].V1.fp_s > RS[s].V2.fp_s)
                    RS[s].resultado.fp_s = RS[s].V1.fp_s;
                else
                    RS[s].resultado.fp_s = RS[s].V2.fp_s;
                break;
            case RV32F_FCVT_W_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.int_d = (word) RS[s].V1.fp_s;
                break;
            case RV32F_FCVT_WU_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.int_d = (xword) ((uword) RS[s].V1.fp_s);
                break;
            case RV32F_FMV_X_W:
                RS[s].resultado.int_d = RS[s].V1.fp_s;
                break;
            case RV32F_FMV_W_X:
                RS[s].resultado.fp_s = RS[s].V1.int_d && 0xFFFFFFFF;
                break;
            case RV32F_FEQ_S:
                RS[s].resultado.int_d = (RS[s].V1.fp_s == RS[s].V2.fp_s);
                break;
            case RV32F_FLT_S:
                RS[s].resultado.int_d = (RS[s].V1.fp_s < RS[s].V2.fp_s);
                break;
            case RV32F_FLE_S:
                RS[s].resultado.int_d = (RS[s].V1.fp_s <= RS[s].V2.fp_s);
                break;
            case RV32F_FCLASS_S:
                RS[s].resultado.int_d = fp_classify(RS[s].V1.fp_s);
                break;
            case RV32F_FCVT_S_W:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = (float) RS[s].V1.int_d;
                break;
            case RV32F_FCVT_S_WU:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = (float) (uword) RS[s].V1.int_d;
                break;
            case RV32D_FADD_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = RS[s].V1.fp_d + RS[s].V2.fp_d;
                break;
            case RV32D_FSUB_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = RS[s].V1.fp_d - RS[s].V2.fp_d;
                break;
            case RV32D_FSGNJ_D:
                if (RS[s].V2.fp_d >= 0)
                    RS[s].resultado.fp_d = fabs(RS[s].V1.fp_d);
                else
                    RS[s].resultado.fp_d = -fabs(RS[s].V1.fp_d);
                break;
            case RV32D_FSGNJN_D:
                if (RS[s].V2.fp_d >= 0)
                    RS[s].resultado.fp_d = -fabs(RS[s].V1.fp_d);
                else
                    RS[s].resultado.fp_d = fabs(RS[s].V1.fp_d);
                break;
            case RV32D_FSGNJX_D:
                s1 = (RS[s].V1.fp_d >= 0.0);
                s2 = (RS[s].V2.fp_d >= 0.0);
                if (s1 ^ s2)
                    RS[s].resultado.fp_d = -fabs(RS[s].V1.fp_d);
                else
                    RS[s].resultado.fp_d = fabs(RS[s].V1.fp_d);
                break;
            case RV32D_FMIN_D:
                if (RS[s].V1.fp_d < RS[s].V2.fp_d)
                    RS[s].resultado.fp_d = RS[s].V1.fp_d;
                else
                    RS[s].resultado.fp_d = RS[s].V2.fp_d;
                break;
            case RV32D_FMAX_D:
                if (RS[s].V1.fp_d > RS[s].V2.fp_d)
                    RS[s].resultado.fp_d = RS[s].V1.fp_d;
                else
                    RS[s].resultado.fp_d = RS[s].V2.fp_d;
                break;
            case RV32D_FEQ_D:
                RS[s].resultado.int_d = (RS[s].V1.fp_d == RS[s].V2.fp_d);
                break;
            case RV32D_FLT_D:
                RS[s].resultado.int_d = (RS[s].V1.fp_d < RS[s].V2.fp_d);
                break;
            case RV32D_FLE_D:
                RS[s].resultado.int_d = (RS[s].V1.fp_d <= RS[s].V2.fp_d);
                break;
            case RV32D_FCLASS_D:
                RS[s].resultado.int_d = fp_classify(RS[s].V1.fp_d);
                break;
            case RV32D_FCVT_S_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = (float) RS[s].V1.fp_d;
                break;
            case RV32D_FCVT_D_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = (double) RS[s].V1.fp_s;
                break;
            case RV32D_FCVT_W_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.int_d = (xword) (word) RS[s].V1.fp_d;
                break;
            case RV32D_FCVT_WU_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.int_d = (xword) (uword) RS[s].V1.fp_d;
                break;
            case RV32D_FCVT_D_W:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = (double) RS[s].V1.int_d;
                break;
            case RV32D_FCVT_D_WU:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = (double) ((uword) RS[s].V1.int_d);
                break;
            case RV64F_FCVT_L_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.int_d = (dword) RS[s].V1.fp_s;
                break;
            case RV64F_FCVT_LU_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.int_d = (udword) RS[s].V1.fp_s;
                break;
            case RV64F_FCVT_S_L:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = (float) RS[s].V1.int_d;
                break;
            case RV64F_FCVT_S_LU:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = (float) ((uxword) RS[s].V1.int_d);
                break;
            case RV64D_FCVT_L_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.int_d = (dword) RS[s].V1.fp_d;
                break;
            case RV64D_FCVT_LU_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.int_d = (udword) RS[s].V1.fp_d;
                break;
            case RV64D_FMV_X_D:
                RS[s].resultado.int_d = RS[s].V1.fp_d;
                break;
            case RV64D_FCVT_D_L:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = (double) RS[s].V1.int_d;
                break;
            case RV64D_FCVT_D_LU:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = (double) ((udword) RS[s].V1.int_d);
                break;
            case RV64D_FMV_D_X:
                RS[s].resultado.fp_d = RS[s].V1.int_d;
                break;
            case OP_FP_ADD_PS:
                RS[s].resultado.fp_ps[Lo] = RS[s].V1.fp_ps[Lo] + RS[s].V2.fp_ps[Lo];
                RS[s].resultado.fp_ps[Hi] = RS[s].V1.fp_ps[Hi] + RS[s].V2.fp_ps[Hi];
                break;
            case OP_FP_SUB_PS:
                RS[s].resultado.fp_ps[Lo] = RS[s].V1.fp_ps[Lo] - RS[s].V2.fp_ps[Lo];
                RS[s].resultado.fp_ps[Hi] = RS[s].V1.fp_ps[Hi] - RS[s].V2.fp_ps[Hi];
                break;
            default:
                fprintf(stderr, "ERROR FP Oper SumRest (%s:%d): Operación %s (%d) no implementada\n",
                        __FILE__, __LINE__, instruction_list[RS[s].OP].nemo, RS[s].OP);
                //exit(1);
        } /* endswitch */
    } /* endif */

}

void fase_EX_2_multdiv(unsigned int oper) {

    marca_t s;

    if (Op[oper].ciclo < Op[oper].Teval)
        Op[oper].ciclo++; /* Si quedan ciclos por realizar */

    s = Op[oper].estacion;

    /*** VISUALIZACIÓN ****/
    sprintf(p_str, "M%u", Op[oper].ciclo);
    muestra_fase(p_str, RS[s].orden);
    /**********************/

    if (Op[oper].ciclo == Op[oper].Teval) {
        RS[s].estado = FINALIZADA;
        Op[oper].ocupado = NO;
        RS[s].condicion = NO;


        /*** Estadisticas ***/
        // Se actualizan en Commit
        //estat.flops++;
        RS[s].control = RESULT;
        /* Cálculo del resultado */
        switch (RS[s].OP) {
            case RV32M_MUL:
#if (XLEN == 32)
                RS[s].resultado.int_d = ((dword) RS[s].V1.int_d * (dword) RS[s].V2.int_d) & UINT32_MAX;
#else
                RS[s].resultado.int_d = ((qword) RS[s].V1.int_d * (qword) RS[s].V2.int_d) & UINT64_MAX;
#endif
                break;
            case RV32M_MULH:
#if (XLEN == 32)
                RS[s].resultado.int_d = ((((dword) RS[s].V1.int_d * (dword) RS[s].V2.int_d)) >> XLEN) & UINT32_MAX;
#else
                RS[s].resultado.int_d = ((((qword) RS[s].V1.int_d * (qword) RS[s].V2.int_d)) >> XLEN) & UINT64_MAX;
#endif
                break;
            case RV32M_MULHU:
#if (XLEN == 32)
                RS[s].resultado.int_d = ((udword) (((udword) RS[s].V1.int_d * (udword) RS[s].V2.int_d)) >> XLEN) & UINT32_MAX;
#else
                RS[s].resultado.int_d =
                        ((uqword) (((uqword) RS[s].V1.int_d * (uqword) RS[s].V2.int_d)) >> XLEN) & UINT64_MAX;
#endif
                break;
            case RV32M_MULHSU:
#if (XLEN == 32)
                RS[s].resultado.int_d = ((((dword) RS[s].V1.int_d * (udword) RS[s].V2.int_d)) >> XLEN) & UINT32_MAX;
#else
                RS[s].resultado.int_d = ((((qword) RS[s].V1.int_d * (qword) RS[s].V2.int_d)) >> XLEN) & UINT64_MAX;
#endif
                break;
            case RV32M_DIV:
                RS[s].resultado.int_d = RS[s].V1.int_d / RS[s].V2.int_d;
                break;
            case RV32M_DIVU:
                RS[s].resultado.int_d = ((uxword) RS[s].V1.int_d / (uxword) RS[s].V2.int_d);
                break;
            case RV32M_REM:
                RS[s].resultado.int_d = RS[s].V1.int_d % RS[s].V2.int_d;
                break;
            case RV32M_REMU:
                RS[s].resultado.int_d = ((udword) RS[s].V1.int_d % (udword) RS[s].V2.int_d);
                break;
            case RV32F_FMUL_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = (RS[s].V1.fp_s * RS[s].V2.fp_s);
                /* Falta los rounding modes */
                break;
            case RV32F_FDIV_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = (RS[s].V1.fp_s / RS[s].V2.fp_s);
                break;
            case RV32F_FSQRT_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = sqrt(RS[s].V1.fp_s);
                break;
            case RV32F_FMADD_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = (RS[s].V1.fp_s * RS[s].V2.fp_s) + RS[s].V3.fp_s;
                break;
            case RV32F_FMSUB_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = (RS[s].V1.fp_s * RS[s].V2.fp_s) - RS[s].V3.fp_s;
                break;
            case RV32F_FNMADD_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = -(RS[s].V1.fp_s * RS[s].V2.fp_s) - RS[s].V3.fp_s;
                break;
            case RV32F_FNMSUB_S:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_s = -(RS[s].V1.fp_s * RS[s].V2.fp_s) + RS[s].V3.fp_s;
                break;
            case RV32D_FMUL_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = (RS[s].V1.fp_d * RS[s].V2.fp_d);
                break;
            case RV32D_FDIV_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = (RS[s].V1.fp_d / RS[s].V2.fp_d);
                break;
            case RV32D_FSQRT_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = sqrt(RS[s].V1.fp_d);
                break;
            case RV32D_FMADD_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = (RS[s].V1.fp_d * RS[s].V2.fp_d) + RS[s].V3.fp_d;
                break;
            case RV32D_FMSUB_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = (RS[s].V1.fp_d * RS[s].V2.fp_d) - RS[s].V3.fp_d;
                break;
            case RV32D_FNMADD_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = -(RS[s].V1.fp_d * RS[s].V2.fp_d) - RS[s].V3.fp_d;
                break;
            case RV32D_FNMSUB_D:
                config_Redondeo(RS[s].redondeo);
                RS[s].resultado.fp_d = -(RS[s].V1.fp_d * RS[s].V2.fp_d) + RS[s].V3.fp_d;
                break;
            case RV64M_MULW:
                RS[s].resultado.int_d = ((dword) RS[s].V1.int_d * (dword) RS[s].V2.int_d) & UINT32_MAX;
                break;
            case RV64M_DIVW:
                RS[s].resultado.int_d = (dword) RS[s].V1.int_d / RS[s].V2.int_d;
                break;
            case RV64M_DIVUW:
                RS[s].resultado.int_d = (dword) ((udword) RS[s].V1.int_d / (udword) RS[s].V2.int_d);
                break;
            case RV64M_REMW:
                RS[s].resultado.int_d = (dword) RS[s].V1.int_d % RS[s].V2.int_d;
                break;
            case RV64M_REMUW:
                RS[s].resultado.int_d = (dword) ((udword) RS[s].V1.int_d % (udword) RS[s].V2.int_d);
                break;
            case OP_FP_MUL_PS:
                RS[s].resultado.fp_ps[Lo] = RS[s].V1.fp_ps[Lo] * RS[s].V2.fp_ps[Lo];
                RS[s].resultado.fp_ps[Hi] = RS[s].V1.fp_ps[Hi] * RS[s].V2.fp_ps[Hi];
                break;
            case OP_FP_DIV_PS:
                RS[s].resultado.fp_ps[Lo] = RS[s].V1.fp_ps[Lo] / RS[s].V2.fp_ps[Lo];
                RS[s].resultado.fp_ps[Hi] = RS[s].V1.fp_ps[Hi] / RS[s].V2.fp_ps[Hi];
                break;
            default:
                fprintf(stderr, "ERROR FP Oper MultDiv (%s:%d): Operación %s (%d) no implementada\n",
                        __FILE__, __LINE__, instruction_list[RS[s].OP].nemo, RS[s].OP);
                //exit(1);
        } /* endswitch */
    } /* endif */

}

void fase_EX_2_direcciones(unsigned int oper) {

    marca_t s;
    region_t r;

    /*** Cálculo de la dirección efectiva */

    if (Op[oper].ciclo < Op[oper].Teval)
        Op[oper].ciclo++; /* Si quedan ciclos por realizar */

    s = Op[oper].estacion;

    /*** VISUALIZACIÓN ****/
    if (Op[oper].Teval > 1) {
        sprintf(p_str, "AC%u", Op[oper].ciclo);
    } else {
        sprintf(p_str, "AC");
    }
    muestra_fase(p_str, RS[s].orden);
    /**********************/

    if (Op[oper].ciclo == Op[oper].Teval) {
        Op[oper].ocupado = NO;

        RS[s].direccion = RS[s].V1.int_d + RS[s].desplazamiento;

        if (ES_ESTACION(s, BUFFER_CARGA)) {
            LB[s].estado = PREPARADA;
            if ((LB[s].OP == RV32I_LB) || (LB[s].OP == RV32I_LBU)) {
                if (!esta_alineada_dir_byte(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            } else if ((LB[s].OP == RV32I_LH) || (LB[s].OP == RV32I_LHU)) {
                if (!esta_alineada_dir_half(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            } else if ((LB[s].OP == RV32I_LW) || (LB[s].OP == RV64I_LWU)) {
                if (!esta_alineada_dir_word(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            } else if (LB[s].OP == RV64I_LD) {
                if (!esta_alineada_dir_dword(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            } else if (LB[s].OP == RV32F_FLW) {
                if (!esta_alineada_dir_float(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            } else if (LB[s].OP == RV32D_FLD) {
                if (!esta_alineada_dir_double(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            }
            r = dir_a_region(mmem, LB[s].direccion);

            if (!es_region_datos(r)) {
                // fnDebug("Excepción: LOAD Ciclo = " F_CICLO " dir = %ld\n", Ciclo, RS[s].direccion);
                LB[s].excepcion = EXC_OUT_MEM_DAT;
                LB[s].resultado.int_d = LB[s].excepcion;
                LB[s].estado = FINALIZADA;
                LB[s].control = EXCEPCION;
            }
        } else {
            SB[s].estado = PREPARADA;
            if (SB[s].OP == RV32I_SB) {
                if (!esta_alineada_dir_byte(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                }
            } else if (SB[s].OP == RV32I_SH) {
                if (!esta_alineada_dir_half(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                }
            } else if (SB[s].OP == RV32I_SW) {
                if (!esta_alineada_dir_word(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                }
            } else if (SB[s].OP == RV64I_SD) {
                if (!esta_alineada_dir_dword(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                }
            } else if (SB[s].OP == RV32D_FSD) {
                if (!esta_alineada_dir_double(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                }
            } else if (SB[s].OP == OP_FP_S_PS) {
                if (!esta_alineada_dir_double(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                }
            } else if (SB[s].OP == RV32F_FSW) {
                if (!esta_alineada_dir_float(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                }
            }
            r = dir_a_region(mmem, SB[s].direccion);

            if (!es_region_datos(r)) {
                SB[s].excepcion = EXC_OUT_MEM_DAT;
            }
        }

    } /* endif */

}

void fase_EX_2_memdatos(unsigned int oper) {

    marca_t s;
    boolean dirty;

    // TODO: De momento no está claro cómo realizar la visualización para el avance de la operación de memoria en L1D
    if (!L1D.mem.activa) {
        // Comportamiento de la L1D perfecta

        if (Op[oper].ciclo < Op[oper].Teval)
            Op[oper].ciclo++; /* Si quedan ciclos por realizar */

        s = Op[oper].estacion;

        /*** VISUALIZACIÓN ****/
        /*
            if (ES_ESTACION(s, BUFFER_CARGA)) {
                sprintf(p_str, "L%d", Op[oper].ciclo);
            } else {
                sprintf(p_str, "S%d", Op[oper].ciclo);
            }
         */
        sprintf(p_str, "L%u", Op[oper].ciclo);
        muestra_fase(p_str, RS[s].orden);
        /**********************/

        if (Op[oper].ciclo == Op[oper].Teval) {
            if (ES_ESTACION(s, BUFFER_CARGA)) {

                LB[s].estado = FINALIZADA;
                Op[oper].ocupado = NO;
                LB[s].condicion = NO;

                RS[s].control = RESULT;
                /* Cálculo del resultado */
                switch (RS[s].OP) {
                    case RV64I_LD:
                        LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_dword, &dirty);
                        break;
                    case RV32D_FLD:
                        LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_double, &dirty);
                        break;
                    case RV32F_FLW:
                        LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_float, &dirty);
                        break;
                    case OP_FP_L_PS:
                        LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_float_ps, &dirty);
                        break;
                    case RV32I_LB:
                        LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_byte, &dirty);
                        break;
                    case RV32I_LBU:
                        LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_byte, &dirty);
                        LB[s].resultado.int_d = 0xFF & LB[s].resultado.int_d;
                        break;
                    case RV32I_LH:
                        LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_half, &dirty);
                        break;
                    case RV32I_LHU:
                        LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_half, &dirty);
                        LB[s].resultado.int_d = 0xFFFF & LB[s].resultado.int_d;
                        break;
                    case RV32I_LW:
                        LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_word, &dirty);
                        LB[s].resultado.int_d = (xword) LB[s].resultado.int_d;
                        break;
                    case RV64I_LWU:
                        LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_word, &dirty);
                        LB[s].resultado.int_d = 0xFFFFFFFF & LB[s].resultado.int_d;
                        break;
                    default:
                        fprintf(stderr, "ERROR Oper Load (%s:%d): Operación %s (%d) no implementada\n",
                                __FILE__, __LINE__, instruction_list[RS[s].OP].nemo, RS[s].OP);
                        //exit(1);
                } /* endswitch */

            } else {
                Op[oper].ocupado = NO;

                SB[s].ocupado = NO; /* El operador libera la estación de reserva */
                //SB[s].ocupado = LIBERANDOSE; /* El operador libera la estación de reserva */
                SB[s].excepcion = EXC_NONE;
                SB[s].condicion = NO;

                /* Escritura al final del último ciclo */
                switch (RS[s].OP) {
                    case RV64I_SD:
                        escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_dword, SI);
                        break;
                    case RV32F_FSW:
                        escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_float, SI);
                        break;
                    case RV32D_FSD:
                        escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_double, SI);
                        break;
                    case OP_FP_S_PS:
                        escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_float_ps, SI);
                        break;
                    case RV32I_SW:
                        escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_word, SI);
                        break;
                    case RV32I_SH:
                        escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_half, SI);
                        break;
                    case RV32I_SB:
                        escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_byte, SI);
                        break;
                    default:
                        fprintf(stderr, "ERROR Oper Store (%s:%d): Operación %s (%d) no implementada\n",
                                __FILE__, __LINE__, instruction_list[RS[s].OP].nemo, RS[s].OP);
                        //exit(1);
                } /* endswitch */
            } /* endif */
        } /* endif */
    }
}

/**
 * Actualiza el estado de una operación cuando la petición en la cache L1D ha concluido
 * @param nivel
 * @param oper_id
 * @param datos
 * @param tipos
 * @param tam
 */
void operacion_finalizada_L1D(
        operacion_nivel_t * oper_sig,
        void *datos,
        tipo_t *tipo_datos,
        boolean *dirty_datos
) {
    estacion_t *p_RS = (estacion_t *) oper_sig->oper_data;

    if (oper_sig->tipo == OP_LECT) {
        p_RS->estado = FINALIZADA;
        p_RS->condicion = NO;
        p_RS->control = RESULT;

        /* Cálculo del resultado */
        switch (p_RS->OP) {
            case RV64I_LD:
                p_RS->resultado.int_d = *(xword *) datos;
                break;
            case RV32D_FLD:
                p_RS->resultado.fp_d = *(double *) datos;
                break;
            case RV32F_FLW:
                p_RS->resultado.fp_s = *(float *) datos;
                break;
            case OP_FP_L_PS:
                memcpy(&p_RS->resultado.fp_ps, datos, 2 * sizeof(float));
                break;
            case RV32I_LB:
                p_RS->resultado.int_d = *(byte *) datos;
                break;
            case RV32I_LBU:
                p_RS->resultado.int_d = *(byte *) datos;
                p_RS->resultado.int_d &= 0xFF;
                break;
            case RV32I_LH:
                p_RS->resultado.int_d = *(half *) datos;
                break;
            case RV32I_LHU:
                p_RS->resultado.int_d = *(half *) datos;
                p_RS->resultado.int_d &= 0xFFFF;
                break;
            case RV32I_LW:
                p_RS->resultado.int_d = *(word *) datos;
                break;
            case RV64I_LWU:
                p_RS->resultado.int_d = *(word *) datos;
                p_RS->resultado.int_d &= 0xFFFFFFFF;
                break;
            default:
                fprintf(stderr, "ERROR Oper Load (%s:%d): Operación %s (%d) no implementada\n",
                        __FILE__, __LINE__, instruction_list[p_RS->OP].nemo, p_RS->OP);
                //exit(1);
        } /* endswitch */

    } else {
        p_RS->ocupado = NO; /* El operador libera la estación de reserva */
        p_RS->excepcion = EXC_NONE;
        p_RS->condicion = NO;

        /* Escritura al final del último ciclo */
        switch (p_RS->OP) {
            case RV64I_SD:
                *(xword *) datos = (xword) p_RS->V2.int_d;
                mem_etiqueta_datos(tipo_datos, dirty_datos, 0, t_dword, SI);
                break;
            case RV32F_FSW:
                *(float *) datos = (float) p_RS->V2.fp_s;
                mem_etiqueta_datos(tipo_datos, dirty_datos, 0, t_float, SI);
                break;
            case RV32D_FSD:
                *(double *) datos = (double) p_RS->V2.fp_d;
                mem_etiqueta_datos(tipo_datos, dirty_datos, 0, t_double, SI);
                break;
            case OP_FP_S_PS:
                memcpy(datos, p_RS->V2.fp_ps, 2 * sizeof(float));
                mem_etiqueta_datos(tipo_datos, dirty_datos, 0, t_float_ps, SI);
                break;
            case RV32I_SW:
                *(word *) datos = (word) p_RS->V2.int_d;
                mem_etiqueta_datos(tipo_datos, dirty_datos, 0, t_word, SI);
                break;
            case RV32I_SH:
                *(half *) datos = (half) p_RS->V2.int_d;
                mem_etiqueta_datos(tipo_datos, dirty_datos, 0, t_half, SI);
                break;
            case RV32I_SB:
                *(byte *) datos = (byte) p_RS->V2.int_d;
                mem_etiqueta_datos(tipo_datos, dirty_datos, 0, t_byte, SI);
                break;
            default:
                fprintf(stderr, "ERROR Oper Store (%s:%d): Operación %s (%d) no implementada\n",
                        __FILE__, __LINE__, instruction_list[p_RS->OP].nemo, p_RS->OP);
                //exit(1);
        } /* endswitch */
    } /* endif */

}


