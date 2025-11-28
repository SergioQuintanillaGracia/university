/**
 * @file presentacion-json.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Definiciones para el módulo de presentación en formato JSON
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef presentacion_json_H
#define presentacion_json_H

/***** Includes ***********************************************/

#include "tipos.h"

/***** Constants **********************************************/


/***** Types **************************************************/


/***** Prototypes *********************************************/

#ifndef CPROTO

#include "presentacion_json_api.h"

#endif

#ifdef presentacion_json_C  /*** Privado ***/

boolean json_merge = NO;

#else    /*** Público ***/

extern linea_crono_t crono[MAX_INSTRUC];
/* Instrucciones visibles en el
   cronograma */

extern boolean json_merge;

extern presentacion_interface_t presentacion_json;

#endif

#endif
