/**
 * @file f_busqueda.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene la fase de búsqueda de la ruta de datos
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define f_busqueda_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "instrucciones.h"
#include "codop.h"
#include "prediccion.h"
#include "presentacion.h"
#include "memoria.h"
#include "tipos.h"
#include "jerarquia.h"

/**
 * Actualiza el estado de una operación cuando la petición en la cache L1I ha concluido
 * @param oper
 * @param datos
 * @param tipos
 * @param dirty
 */
static
void finaliza_oper_L1I(
        operacion_nivel_t *oper_sig,
        void *datos,
        tipo_t *tipo_datos,
        boolean *dirty_datos
) {
    operacion_busqueda_t *busqueda = (operacion_busqueda_t *) oper_sig->oper_data;

    // TODO: Revisar!!!

    // Copia los datos desde la cache L1I
    memcpy(busqueda->buffer.mem.m_word, datos, oper_sig->tam);

    // Procede a cerrar la operación
    busqueda->estado = EST_OP_TERMINADA;

    M_DEBUG(F_CICLO ": [%s] (%d) terminada\n", Ciclo, oper_sig->parent->nombre, oper_sig->indice);

}

/**
 * Solicita una operación de lectura a la jerarquía de memoria
 * @param busqueda
 * @param PC
 */
void fase_IF_1(
        operacion_busqueda_t *busqueda,
        dword PC,
        ciclo_t orden
) {
    unsigned int m;
    boolean result;

    if (terminando) return;

    foreach (m, CONFIG(num_vias.issue)) {
        busqueda->PC[m] = PC + (dword) (m * sizeof(word));
    }

    if (PTR_NIVEL(L1I)->activa) {
        switch (busqueda->estado) {
            case EST_OP_PENDIENTE:
                // Comienza una búsqueda nueva descartando la de ciclos anteriores
                foreach (m, CONFIG(num_vias.issue)) {
                    busqueda->buffer.mem.m_word[m] = INST_UNKNOWN; // Instrucción inválida
                }
                // No hay break, para realizar la petición a L1I
            case EST_OP_BLOQUEADA:
                // Si la fase IF se ha cancelado o hay una excepción activa,
                //   no se manda la petición al nivel inferior
                if (Cancelar_Activa || Excepcion_Activa)
                    break;

                // La operación no ha sido aceptada por la L1I
                result = INICIA_OPERACION_MEMORIA(PTR_NIVEL(L1I), OP_LECT, NULL,
                                                  PC, CONFIG(num_vias.issue) * sizeof(word),
                                                  orden,
                                                  finaliza_oper_L1I,
                                                  busqueda);

                if (result) {
                    busqueda->estado = EST_OP_ACCEDIENDO;
                } else {
                    busqueda->estado = EST_OP_BLOQUEADA;
                    // El siguiente nivel no acepta la petición
                    // -> se vuelve a pedir en el ciclo siguiente
                }
                break;
            case EST_OP_ACCEDIENDO:
                // La operación se ha aceptado por el nivel inferior, pero todavía no está disponible
                break;
            case EST_OP_TERMINADA:
                // La operación ha terminado en un ciclo anterior, pero no se han consumido
                //  todas las instrucciones. Así pues, no hay que volver a buscar las instrucciones.
                //  Se usan las que ya se habían buscado.
                break;
            default:
                fprintf(stderr, "IMPL: Estado de la búsqueda inválido (%d).\n", busqueda->estado);
                exit(1);
        }
    } else {
        // si la cache L1I es perfecta, obtiene directamente los índices de las instrucciones
        busqueda->estado = EST_OP_TERMINADA;
        foreach (m, CONFIG(num_vias.issue)) {
            busqueda->buffer.mem.m_word[m] = indice_instruccion(mmem, busqueda->PC[m]);
        }
    }

}

/***************************************************************
 *
 * Func: fase_IF_2
 *
 **************************************************************/

void fase_IF_2() {

    boolean salto;
    boolean prediccion;
    dword destino;
    prediccion_t pred_otros;
    instruccion_t inst;

    if (terminando && !Cancelar_Activa) return;

    memset(&IF_ISS_1, 0, sizeof(IF_ISS_1));

    Control_1.orden = Control_2.orden + 1;

    IF_ISS_1.PC = Control_2.NPC;
    IF_ISS_1.orden = Control_2.orden;
    IF_ISS_1.excepcion = mem_chequea_direccion(mmem, Control_2.NPC, t_code);

    if (Control_2.indice == INST_UNKNOWN) {
        IF_ISS_1.ignorar = SI;
    } else {
        if (IF_ISS_1.excepcion == EXC_NONE) {
            /* Obtiene la instrucción del buffer de L1I */
            inst = obtiene_instruccion(mmem, Control_2.indice);

            IF_ISS_1.instruccion = inst.valor;
            IF_ISS_1.codop = inst.codop; // Para los predictores
            strcpy(IF_ISS_1.etiqueta, inst.etiqueta);

        } else {
            /* Excepción en el acceso a la memoria de instrucciones */
            IF_ISS_1.codop = OP_TRAP; /* Cancela la instrucción */
        } /* endif */
    }

    /*** PREDICCIÓN ************/

    /* Almacenamos máscaras actuales */
    if (!Cancelar_Activa && !IFstall) {
        salto = obtener_prediccion(Control_2.NPC, Control_2.orden, &prediccion, &destino, &pred_otros);
        //fnDebug("IF -- -- Ciclo %d Orden %d PC %d pred %d dest %d salto %d\n", Ciclo, Control_2.orden, Control_2.NPC, prediccion, destino, salto);
        if (salto && prediccion) {
            /*** Es un salto y predice que salta */
            //IF_ISS_1.prediccion = SI;
            IF_ISS_1.prediccion = prediccion;
            Control_1.NPC = destino;

            /* Predictor hibrido y mascara global */
            IF_ISS_1.pred_data = pred_otros;
            IF_ISS_1.pred_data.dir_destino = destino; // Destino ofrecido por el predictor

            /*** Mascaras ***/

            //fnDebug("Pre PC=%d BHSR1 %d, Pred=%d\n", Control_2.NPC, BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)], prediccion);
            BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)] =
                    (BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)] << 1) | IF_ISS_1.prediccion;
            BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)] =
                    BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)] & (pwrtwo(CONFIG(predic.n_bits_BHSR1)) - 1);

            BHSR2[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR2)] =
                    (BHSR2[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR2)] << 1) | IF_ISS_1.prediccion;
            BHSR2[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR2)] =
                    BHSR2[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR2)] & (pwrtwo(CONFIG(predic.n_bits_BHSR2)) - 1);
            //fnDebug("POst PC=%d BHSR1 %d, Pred=%d\n", Control_2.NPC, BHSR1[Control_2.NPC % CONFIG(CONFIG(predic.tam_tabla_BHSR1))], prediccion);

            /*** Estadísticas ***/
            estat.saltos_accedidos++;
            //fnDebug("+++ IF Ciclo=%lld Encontrados=%d Accedidos=%d\n", Ciclo, estat.saltos_encontrados, estat.saltos_accedidos);

            //fnDebug("+++ IF -- -- Ciclo:%d Orden:%d PC:%d pred:%s NPC:%d\n", Ciclo, Control_2.orden, Control_2.NPC, IF_ISS_1.prediccion ? "SI" : "NO", Control_1.NPC);
            //fnDebug("Accedidos++ Ciclo=%lld\n", Ciclo);
        } else {
            /*** O no es un salto, o es un salto y predice que no salta */
            IF_ISS_1.prediccion = NO;
            Control_1.NPC = Control_2.NPC + 4;

            if (inst_salto(IF_ISS_1.codop)) {

                /* Predictor hibrido y máscara global */
                IF_ISS_1.pred_data = pred_otros;
                IF_ISS_1.pred_data.dir_destino = destino; // 0 si no estaba en la tabla

                /*** Máscaras ***/
                //fnDebug("Pre PC=%d BHSR1 %d, Pred=%d\n", Control_2.NPC, BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)], prediccion);
                BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)] =
                        (BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)] << 1) | IF_ISS_1.prediccion;
                BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)] =
                        BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)] & (pwrtwo(CONFIG(predic.n_bits_BHSR1)) - 1);

                BHSR2[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR2)] =
                        (BHSR2[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR2)] << 1) | IF_ISS_1.prediccion;
                BHSR2[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR2)] =
                        BHSR2[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR2)] & (pwrtwo(CONFIG(predic.n_bits_BHSR2)) - 1);
                //fnDebug("POst PC=%d BHSR1 %d, Pred=%d\n", Control_2.NPC, BHSR1[Control_2.NPC % CONFIG(predic.tam_tabla_BHSR1)], prediccion);

                estat.saltos_accedidos++;
                //fnDebug("+++ IF Ciclo=%lld Encontrados=%d Accedidos=%d\n", Ciclo, estat.saltos_encontrados, estat.saltos_accedidos);

                //fnDebug("+++ IF -- -- Ciclo:%d Orden:%d PC:%d pred:%s NPC:%d\n", Ciclo, Control_2.orden, Control_2.NPC, IF_ISS_1.prediccion ? "SI" : "NO", Control_1.NPC);
            }
        } /* endif */
    } /* endif */

    /*** VISUALIZACIÓN ****/
    if (Control_2.indice != INST_UNKNOWN) {
        init_instruc(Control_2.NPC, Control_2.orden, NULL);
    } else {
        init_instruc(Control_2.NPC, Control_2.orden, "unknown");
    }

    if (!IFhide) {
        if (!Cancelar_Activa && !Excepcion_Activa) {
            char fase[SHORT_STR];
            if (!IFstall) {
                sprintf(fase, "IF<small>%s</small>", fase_instruc_jerarquia);
            } else {
                sprintf(fase, "if<small>%s</small>", fase_instruc_jerarquia);
            }

            if (IF_ISS_1.excepcion == EXC_NONE) {
                muestra_fase(fase, Control_2.orden);
            } else {
                muestra_fase_excepcion(fase, Control_2.orden);
            }
        } else {
            if (IF_ISS_1.excepcion == EXC_NONE) {
                muestra_fase("X", Control_2.orden);
            } else {
                muestra_fase_excepcion("X", Control_2.orden);
            }
        }
    }
    /**********************/

} /* end fase_IF_2 */

