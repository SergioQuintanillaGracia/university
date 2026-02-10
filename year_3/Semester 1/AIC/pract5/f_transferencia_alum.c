/**
 * @file f_transferencia.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene la fase de transferencia por el bus común de datos para el
 *      algoritmo de Tomasulo con especulación.
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

/* IMPLEMENTACIÓN OCULTA */

#define f_transferencia_alum_C

/***** Includes ***********************************************/

#include "main.h"
#include "presentacion.h"
#include "algoritmo.h"

/* IMPLEMENTACIÓN OCULTA */

/**
 * Fase de transferencia por el bus común de datos
 */
void fase_WB_alum() {
    marca_t i, s;

    ciclo_t orden;

    /*** VISUALIZACIÓN ****/
    for (i = 0; i < CONFIG(tam_estaciones); i++) {
        if (RS[i].ocupado && RS[i].estado == FINALIZADA) {
            muestra_fase("-", RS[i].orden);
        } /* endif */
    }     /* endif */
    /**********************/

    /* IMPLEMENTACIÓN OCULTA */

    /*** Busca RS con resultados disponibles */

    orden = MAX_ORDEN;
    s = 0;

    for (i = 0; i < CONFIG(tam_estaciones); i++) {
        if (RS[i].ocupado && RS[i].estado == FINALIZADA && RS[i].orden < orden) {
            s = i;
            orden = RS[i].orden;
        } /* endif */
    }     /* endif */

    if (orden >= MAX_ORDEN) return; /* No hay ninguna RS con resultados disponibles */

    /*** Volcado de resultados en el BUS */
    BUS.condicion = RS[s].condicion;
    BUS.control = RS[s].control;
    BUS.valor = RS[s].resultado;  // We place the result in the bus
    BUS.codigo = RS[s].rob;  // We place the mark in the bus

    /*** Libera la RS */
    /* Los STORES no llegan aquí porque el estado nunca es FINALIZADA */
    RS[s].ocupado = NO;  // During WB, we release the reservation station

    /*** VISUALIZACIÓN ****/
    RS[s].estado = PENDIENTE;
    if (BUS.control != EXCEPCION) {
        muestra_fase("WB", RS[s].orden);
    } else {
        muestra_fase_excepcion("WB", RS[s].orden);
    }
    /**********************/

    /*** Lectura de resultados */
    RB[BUS.codigo].completado = SI;

    /* Reorder buffer */
    switch (BUS.control) {
        case DIRECCION:
            RB[BUS.codigo].direccion = BUS.valor.int_d;
            RB[BUS.codigo].condicion = BUS.condicion;
            return;
        case CONDICION:
            RB[BUS.codigo].condicion = BUS.condicion;
            return;
        case EXCEPCION:
            RB[BUS.codigo].excepcion = BUS.valor.int_d;
            return;
        default:
            RB[BUS.codigo].valor = BUS.valor;
    }

    /* Estaciones de reserva */

    /* IMPLEMENTACIÓN OCULTA */

    /* Para todas las estaciones de reserva, buffers de lectura y escritura */
    for (s = PRIMERA_ESTACION_RESERVA; s <= ULTIMA_ESTACION_RESERVA; s++) {
        /* Comprobar operando fuente 1 */
        if (RS[s].Q1 == BUS.codigo) {  // If the value placed over the bus is the one we are waiting for
            /* INSERTAR CÓDIGO */
            RS[s].V1 = BUS.valor;  // Get the value
            RS[s].Q1 = MARCA_NULA;  // Set the mark to null
        } /* endif */

        /* Comprobar operando fuente 2 */
        // Same logic as for operand 1
        if (RS[s].Q2 == BUS.codigo) {
            /* INSERTAR CÓDIGO */
            RS[s].V2 = BUS.valor;  // Get the value
            RS[s].Q2 = MARCA_NULA;  // Set the mark to null
        }

    } /* endfor */

    /* IMPLEMENTACIÓN OCULTA */

} /* end fase_WB */
