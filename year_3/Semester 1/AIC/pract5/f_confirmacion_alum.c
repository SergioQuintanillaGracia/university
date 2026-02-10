/**
 * @file f_confirmacion.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene la fase de confirmación del algoritmo de Tomasulo con especulación
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

/* IMPLEMENTACIÓN OCULTA */

#define f_confirmacion_alum_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "instrucciones.h"
#include "prediccion.h"
#include "presentacion.h"
#include "algoritmo.h"
#include "tipos.h"
#include "depurar.h"
#include "riscv_syscall.h"
#include "registros.h"

/* IMPLEMENTACIÓN OCULTA */

static void estat_FLOPS(int codop) {
    if (inst_fp_load(codop)) return;
    estat.flops++;
    if (inst_fp_madd(codop)) estat.flops++;
}

/***************************************************************
 *
 * Func: fase_COM
 *
 **************************************************************/

void fase_COM_alum() {
    char aux[128];

    /*************************************/
    RB_cab = RB_inicio; /* Información administrativa */
    /*************************************/
    if (!terminando && RB[RB_inicio].ocupado &&
        (RB[RB_inicio].completado ||
         (inst_store(RB[RB_inicio].OP) && SB[RB[RB_inicio].dest].estado == PREPARADA))) {

        /* IMPLEMENTACIÓN OCULTA */

        /*** Estadísticas ***/
        estat.instrucciones++;

        /*** Atención de las excepciones */

        /* IMPLEMENTACIÓN OCULTA */

        imprime_instruccion_color(aux, RB[RB_inicio].PC, NO);

        /* IMPLEMENTACIÓN OCULTA */

        /* Si hay excepción, lo convertimos en ECALL */
        if (RB[RB_inicio].excepcion != EXC_NONE) {
            RB[RB_inicio].OP = RV32I_ECALL;
            /*** VISUALIZACIÓN ****/
            muestra_fase_excepcion("C", RB[RB_inicio].orden);
            /**********************/
        } else {
            /*** VISUALIZACIÓN ****/
            muestra_fase("C", RB[RB_inicio].orden);
            /**********************/
        }

        /*** Confirmación de la instrucción */

        // En la práctica 5 solo se muestran las de C.F.
        if (inst_ecall(RB[RB_inicio].OP)) {
            /*** LLAMADAS AL SISTEMA ***/

            /* IMPLEMENTACIÓN OCULTA */

        } else if (inst_salto(RB[RB_inicio].OP)) {
            /*** INSTRUCCIONES DE SALTO ***/

            /* IMPLEMENTACIÓN OCULTA */

        } else if (inst_store(RB[RB_inicio].OP)) {
            /*** ALMACENAMIENTOS EN MEMORIA ***/
            /*** Confirma la instrucción de escritura */
            SB[RB[RB_inicio].dest].confirm = SI;
        } else if (inst_to_Rint(RB[RB_inicio].OP)) {
            /*** OPERACIONES ENTERAS CON ESCRITURA EN REGISTROS */

            /* IMPLEMENTACIÓN OCULTA */

        } else if (inst_to_Rfp(RB[RB_inicio].OP)) {
            /*** OPERACIONES DE COMA FLOTANTE CON ESCRITURA EN REGISTROS */
            tipo_t tipo = t_double; // No debería ser necesario
            if (inst_fp_d(RB[RB_inicio].OP))
                tipo = t_double;
            else if (inst_fp_s(RB[RB_inicio].OP))
                tipo = t_float;
            else if (inst_simd(RB[RB_inicio].OP))
                tipo = t_float_ps;

            marca_fp_reg(RB[RB_inicio].dest, tipo, SI);

            /* Escritura del registro de destino en Rfp y liberación, en su caso */

            /* INSERTAR CÓDIGO */
            // We delete the mark in the register file if the mark there is the same as
            // our mark

            // 1) Write the value to the register in Rfp
            Rfp[RB[RB_inicio].dest].valor = RB[RB_inicio].valor;

            // 2) Delete the mark if it is our mark
            if (Rfp[RB[RB_inicio].dest].rob == RB_inicio) {
                Rfp[RB[RB_inicio].dest].rob = MARCA_NULA;
            }

            /*** Estadísticas ***/
            estat_FLOPS(RB[RB_inicio].OP);
        } else {
            fprintf(stderr, "ERROR (%s:%d): Operacion %d no implementada\n",
                    __FILE__, __LINE__, RB[RB_inicio].OP);
            //exit(1);
        } /* endswitch */

        /* Actualizar el reorder buffer */
        RB[RB_inicio].ocupado = NO;
        RB[RB_inicio].excepcion = EXC_NONE;
        RB_inicio = (RB_inicio + 1) % CONFIG(tam_reorder);
        RB_long--;
    } /* endif */
} /* end fase_COM */
