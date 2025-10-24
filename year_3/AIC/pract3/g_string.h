/**
 * @file g_string.h
 *
 * @copyright Departamento de Informática de Sistema y Computadores (DISCA)
 * @copyright Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez (ssaez@disca.upv.es)
 *
 * @brief Soporte para un printf más genérico
 */

#ifndef g_string_H
#define g_string_H

/***** Includes ***********************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/***** Defines ************************************************/

#define G_BLOCK_SIZE 512
#define G_STR_INIT {.buffer = NULL, .size = 0, .index = 0}
#define G_STR_DECL(var) g_string_t var __attribute__ ((__cleanup__(g_str_destroy))) = G_STR_INIT

/***** Macros *************************************************/

#define g_str_string(s) ((s)->buffer)
#define g_str_length(s) ((s)->index)

/***** Types **************************************************/

typedef enum {
    g_mode_begin,
    g_mode_new,
    g_mode_end,
    g_mode_concat
} g_mode_t;

typedef struct {
    char *buffer;
    size_t size;
    size_t index;
} g_string_t;

/***** Prototypes *********************************************/

#ifndef CPROTO

#include "g_string_api.h"

#endif

#endif
