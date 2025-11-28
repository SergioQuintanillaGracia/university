/**
 * @file enlazador.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Definiciones para el enlazar de código
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef enlazador_H
#define enlazador_H

/***** Includes ***********************************************/

#include "memoria.h"
#include "simbolos.h"

/***** Types **************************************************/

typedef struct {
    tabla_simbolos_t ts;

    mapa_memoria_t memoria;
} fichero_objeto_t;

typedef struct {
    fichero_objeto_t * l_objs;
    int n_objs;

    simbolo_t * simbolos;

    mapa_memoria_t memoria;
} programa_t;

/***** Prototypes *********************************************/

#ifndef CPROTO
#include "enlazador_api.h"
#endif

#endif
