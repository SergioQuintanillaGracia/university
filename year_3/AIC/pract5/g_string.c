/**
 * @file g_string.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Soporte para cadenas genéricos que aumentan de tamaño bajo demanda.
 *    Incluye un printf más genérico
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

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

int
g_str_starts_with(const char *haystack, const char *needle) {
    return (strncmp(haystack, needle, strlen(needle)) == 0);
}

int
g_str_ends_with(const char *haystack, const char *needle) {
    return (strcmp(haystack + strlen(haystack) - strlen(needle), needle) == 0);
}

/**
 * Inicializa la estructura g_string_t
 *
 * @param g_str
 */
void g_str_init(g_string_t *g_str) {
    g_str->buffer = NULL;
    g_str->type = normal_buffer;
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
 * Elimina el contenido del buffer, pero no libera la memoria
 *
 * @param g_str
 */
void g_str_clear(g_string_t *g_str) {
    G_STR_DEBUG("Removing buffer contents (%zu)\n", g_str->index);

    g_str->index = 0;
}

/**
 * Comprueba si lo que se quiere escribir en el buffer cabe.
 * Si no cabe, se lleva a cabo un realloc()
 * Si el realloc() falla, devuelve 0 y el buffer actual se libera.
 *
 * @param g_str
 * @param required_size
 * @return size_t El tamaño del buffer
 */
size_t g_str_alloc(g_string_t *g_str, size_t required_size) {
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
        if (g_str->buffer)
            free(g_str->buffer);
    }
    g_str->buffer = p;

    return g_str->size;
}

/**
 * Realiza un sprintf sobre el buffer interno
 *
 * @param g_str
 * @param format Formato de escritura del printf
 * @param ... Parámetros adicionales según el formato
 * @return Pointer to the string to print.
 *         If inline buffer, this string is this last one.
 *         The whole buffer otherwise.
 */
char *
g_str_printf(g_string_t *g_str, const char *format, ...) {
    int n = 0;
    va_list ap;
    size_t offset;
    char *str;

    /* Determine required size */
    va_start(ap, format);
    n = vsnprintf(NULL, 0, format, ap);
    va_end(ap);

    if (n <= 0)
        return NULL;

    // Inline adds size for the previous '\0' char
    offset = (g_str->type == inline_buffer) ? 1 : 0;

    /* One extra byte for '\0' */
    if (g_str_alloc(g_str, g_str->index + n + offset + 1) == 0)
        return NULL;

    str = (g_str->type == inline_buffer) ? g_str->buffer + g_str->index + 1 : g_str->buffer;

    va_start(ap, format);
    n = vsprintf(g_str->buffer + g_str->index + offset, format, ap);
    va_end(ap);

    g_str->index += n + offset;

    return str;
}

/**
 * Copia la cadena suministrada en el buffer
 *
 * @param g_str
 * @param src Cadena que se escribe en el buffer
 * @param length cuantos caracteres de la cadena se escriben
 * @return Longitud de la cadena final alojada en el buffer
 */
size_t g_str_n_copy(g_string_t *g_str, const char *src, size_t length) {

    /* One extra byte for '\0' */
    if (g_str_alloc(g_str, g_str->index + length + 1) == 0)
        return 0;

    strncpy(g_str->buffer + g_str->index, src, length);
    g_str->index += length;
    g_str->buffer[g_str->index] = '\0';

    return g_str->index;
}

/**
 * Copia la cadena suministrada en el buffer
 *
 * @param g_str
 * @param src Cadena que se escribe en el buffer
 * @return Longitud de la cadena final alojada en el buffer
 */
size_t g_str_copy(g_string_t *g_str, const char *src) {
    return g_str_n_copy(g_str, src, strlen(src));
}

/**
 * Repite un carácter 'times' veces en el buffer
 *
 * @param g_str
 * @param ch
 * @param times
 * @return Longitud de la cadena final alojada en el buffer
 */
size_t g_str_repeat(g_string_t *g_str, char ch, size_t times) {

    /* One extra byte for '\0' */
    if (g_str_alloc(g_str, g_str->index + times + 1) == 0)
        return 0;

    memset(g_str->buffer + g_str->index, ch, times);
    g_str->index += times;
    g_str->buffer[g_str->index] = '\0';

    return g_str->index;
}

/**
 * Copia la cadena suministrada en el buffer añadiendo 'indent' espacios
 *  al principio y al comienzo de cada línea ('\n')
 *
 * @param g_str
 * @param src Cadena que se escribe en el buffer
 * @param indent cuantos espacios hay que añadir al principio de cada línea
 * @param first cuantos espacios se añaden al principio de la primera línea
 *              un valor de 0 indica que es igual que para el resto de líneas
 * @return Longitud de la cadena final alojada en el buffer
 */
size_t g_str_indent(g_string_t *g_str, const char *src,
                    size_t indent, size_t first) {

    const char *p_end;
    const char *p_src = src;
    size_t length;
    size_t tab;

    tab = (first) ? first : indent;

    while ((p_end = strchr(p_src, '\n')) != NULL) {
        length = p_end - p_src;
        g_str_repeat(g_str, ' ', tab);
        g_str_n_copy(g_str, p_src, length);
        g_str_n_copy(g_str, "\n", 1);
        p_src = p_end + 1;
        tab = indent;
    }

    if (strlen(p_src) > 0) {
        g_str_repeat(g_str, ' ', tab);
        g_str_copy(g_str, p_src);
    }

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
    return fwrite(g_str->buffer, 1, g_str->index, file);
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
    g_str_clear(g_str);

    return n;
}