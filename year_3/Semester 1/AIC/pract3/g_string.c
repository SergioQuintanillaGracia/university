/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author:  Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: g_string.c
 *
 * Description:
 *    Módulo principal del simulador
 *
 *************************************************************************/

#define g_string_C

/***** Includes ***********************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "g_string.h"


// #define G_STR_DEBUG(...) fprintf(stderr, __VA_ARGS__)
#define G_STR_DEBUG(...) do { } while(0)

/*** Funciones ************************************************/

/**
 * Comprueba si lo que se quiere escribir en el buffer cabe.
 * Si no cabe, se lleva a cabo un realloc()
 * Si el realloc() falla, devuelve 0 y el buffer actual se libera.
 *
 * @param g_str
 * @param required_size
 * @return
 */
static size_t g_str_alloc(g_string_t *g_str, size_t required_size) {
    unsigned int blocks;
    void *p;

    if (required_size <= g_str->size) {
        return g_str->size;
    }

    if (g_str->size > 0) {
        G_STR_DEBUG("Resizing a buffer from %zu to %zu bytes\n",
                    g_str->index, required_size);
    } else {
        G_STR_DEBUG("Allocating a buffer for a string of %zu bytes\n",
                    required_size);
    }

    /* We round up to the next block size */
    blocks = required_size / G_BLOCK_SIZE;
    blocks += (required_size % G_BLOCK_SIZE) ? 1 : 0;

    g_str->size = blocks * G_BLOCK_SIZE;
    p = realloc(g_str->buffer, g_str->size);
    if (p == NULL) {
        g_str->size = 0;
        free(g_str->buffer);
    }
    g_str->buffer = p;

    return g_str->size;
}

/**
 * Inicializa la estructura g_string_t
 *
 * @param g_str
 */
void g_str_init(g_string_t *g_str) {
    g_str->buffer = NULL;
    g_str->size = 0;
    g_str->index = 0;
}

/**
 * Destruye la estructura g_string_t liberando la memoria
 *
 * @param g_str
 */
void g_str_destroy(g_string_t *g_str) {
    G_STR_DEBUG("Destroying a string of size %zu\n", g_str->index);

    free(g_str->buffer);
    g_str_init(g_str);
}

/**
 * Realiza un sprintf sobre el buffer interno
 *
 * @param g_str
 * @param g_mode Modo de escritura.
 *      G_MODE_BEGIN: sobreescribe la cadena.
 *      G_MODE_END: concatena con el contenido previo.
 * @param format Formato de escritura del printf
 * @param ... Parámetros adicionales según el formato
 * @return Longitud de la cadena final alojada en el buffer
 */
size_t g_str_printf(g_string_t *g_str, g_mode_t g_mode, const char *format, ...) {
    int n = 0;
    size_t size = 0;
    va_list ap;
    size_t offset;

    /* Determine required size */
    va_start(ap, format);
    n = vsnprintf(NULL, 0, format, ap);
    va_end(ap);

    if (n <= 0)
        return 0;

    switch (g_mode) {
        case g_mode_begin:
        case g_mode_new:
            offset = 0;
            break;
        case g_mode_end:
        case g_mode_concat:
            offset = g_str->index;
            break;
        default:
            fprintf(stderr, "Error: Invalid g_mode (%d)", g_mode);
            abort();
    }

    /* One extra byte for '\0' */
    size = (size_t) n + 1;
    if (g_str_alloc(g_str, offset + size) == 0)
        return 0;

    va_start(ap, format);
    n = vsnprintf(g_str->buffer + offset, size, format, ap);
    va_end(ap);

    g_str->index = offset + n;

    return g_str->index;
}

/**
 * Copia la cadena suministrada en el buffer
 *
 * @param g_str
 * @param g_mode Modo de escritura.
 *      G_MODE_BEGIN: sobreescribe la cadena.
 *      G_MODE_END: concatena con el contenido previo.
 * @param src
 * @return Longitud de la cadena final alojada en el buffer
 */
size_t g_str_copy(g_string_t *g_str, g_mode_t g_mode, const char *src) {
    size_t size = 0;
    size_t offset;
    size_t n = strlen(src);

    switch (g_mode) {
        case g_mode_begin:
        case g_mode_new:
            offset = 0;
            break;
        case g_mode_end:
        case g_mode_concat:
            offset = g_str->index;
            break;
        default:
            fprintf(stderr, "Error: Invalid g_mode (%d)", g_mode);
            abort();
    }

    /* One extra byte for '\0' */
    size = (size_t) n + 1;
    if (g_str_alloc(g_str, offset + size) == 0)
        return 0;

    strcpy(g_str->buffer + offset, src);
    g_str->index = offset + size - 1;

    return g_str->index;
}

/**
 * Escribe el contenido del buffer en un fichero.
 *
 * @param file
 * @param g_str
 * @return Número de bytes que se han escrito
 */
size_t g_str_write(FILE *file, g_string_t *g_str) {
    size_t n;

    n = fwrite(g_str->buffer, 1, g_str->index, file);

    return n;
}

/**
 * Escribe el contenido del buffer en un fichero y
 *   destruye el contenido del buffer.
 *
 * @param file
 * @param g_str
 * @return Número de bytes que se han escrito
 */
size_t g_str_flush(FILE *file, g_string_t *g_str) {
    size_t n;

    n = g_str_write(file, g_str);
    g_str_destroy(g_str);

    return n;
}