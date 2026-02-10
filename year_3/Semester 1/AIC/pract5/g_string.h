/**
 * @file g_string.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Soporte para un printf más genérico
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef g_string_H
#define g_string_H

/***** Includes ***********************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/***** Defines ************************************************/

#define G_BLOCK_SIZE 512

#define G_STR_INIT {.buffer = NULL, .type = normal_buffer, .size = 0, .index = 0}
#define G_STR_INLINE_INIT {.buffer = NULL, .type = inline_buffer, .size = 0, .index = 0}

#define G_STR_DECL(VAR) g_string_t VAR __attribute__ ((__cleanup__(g_str_destroy))) = G_STR_INIT
#define G_STR_INLINE_DECL(VAR) g_string_t VAR __attribute__ ((__cleanup__(g_str_destroy))) = G_STR_INLINE_INIT

/***** Macros *************************************************/

#define g_str_string(s) (((s)->index) ? (s)->buffer : "")
#define g_str_length(s) ((s)->index)

#define I_INIT_PRINT() G_STR_INLINE_DECL(g_inline)
#define I_CLEAR_PRINT() g_str_clear(&g_inline)
#define I_PRINTF(FORMAT, ...) g_str_printf(&g_inline, FORMAT, __VA_ARGS__)

/***** Types **************************************************/

typedef enum {
    normal_buffer,
    inline_buffer
} g_buffer_type_t;

typedef struct {
    char *buffer;
    g_buffer_type_t type;
    size_t size;
    size_t index;
} g_string_t;

/***** Prototypes *********************************************/

#ifndef CPROTO

#include "g_string_api.h"

#endif

#endif
