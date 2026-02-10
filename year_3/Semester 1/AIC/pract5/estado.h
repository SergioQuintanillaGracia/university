/**
 * @file estado.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    A hash table to maintain the previous state
 *    It is needed for the json format that shows only incremental info.
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef ESTADO_H
#define ESTADO_H

typedef struct table_value_s {
    char *key;
    char *text;
    char *class;
    struct table_value_s *next;
} table_value_t;

typedef struct table_state_s {
    int size;
    table_value_t **table;
} table_state_t;


#ifndef CPROTO

#include "estado_api.h"

#endif

#endif
