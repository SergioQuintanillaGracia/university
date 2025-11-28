/**
 * @file presentacion-html.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Definiciones para el módulo de presentación en formato HTML
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef presentacion_html_H
#define presentacion_html_H

/***** Includes ***********************************************/

#include "tipos.h"
#include "presentacion.h"

/***** Types **************************************************/

typedef char n_fase_t[64]; /* Nombre de la fase */
typedef char n_instruccion_t[128]; /* Instrucción */

/** Una linea del cronograma */
typedef struct {
    dword PC;
    n_instruccion_t inst;       /**< Instrucción */
    n_fase_t fase[MAX_CICLOS];  /**< Fases de la instrucción */
    ciclo_t min;                /**< Primer ciclo utilizado */
    ciclo_t max;                /**< Último ciclo utilizado */
} linea_crono_t;

/***** Prototypes *********************************************/

#ifndef CPROTO
#include "presentacion_html_api.h"
#endif

#ifndef presentacion_html_C  /*** Privado ***/

extern linea_crono_t crono[MAX_INSTRUC];
/* Instrucciones visibles en el
   cronograma */

extern boolean html_merge;

extern presentacion_interface_t presentacion_html;

extern FILE *f_salida;

#endif

#endif
