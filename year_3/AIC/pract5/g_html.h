/**
 * @file g_html.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Soporte para escribir HTML muy sencillo
 *    En este caso, las etiquetas con hijos no pueden tener texto
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef g_html_H
#define g_html_H

/***** Includes ***********************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "g_string.h"

/***** Defines ************************************************/

#define DEFAULT_HTML_BUFFER 4096

#define G_HTML_INIT {.root = NULL}
#define G_HTML_DECL(VAR) g_html_t VAR __attribute__ ((__cleanup__(g_html_destroy))) = G_HTML_INIT

/***** Macros *************************************************/

#define PTR_BASE(TAG) ((T_BASE)(TAG))
#define PTR_SINGLE(TAG) ((T_SINGLE)(TAG))
#define PTR_COMPOSITE(TAG) ((T_COMPOSITE)(TAG))

#define H_INIT_SINGLE_TAG(TAG, NAME) do { h_init_base_tag(PTR_BASE(TAG), NAME, single_tag); g_str_init(&(TAG)->text); } while(0)
#define H_INIT_COMPOSITE_TAG(TAG, NAME) do { \
    h_init_base_tag(PTR_BASE(TAG), NAME, composite_tag); \
    (TAG)->children = NULL;                  \
    (TAG)->last = NULL;                      \
    } while(0)

#define H_SINGLE(TAG) h_new_single_tag( # TAG)
#define H_COMPOSITE(TAG) h_new_composite_tag( # TAG)

#define H_ATTR_COMPOSITE(TAG, ATTR) \
    ({T_COMPOSITE _composite = h_new_composite_tag( # TAG) ; \
      h_set_attr(PTR_BASE(_composite), ATTR); _composite; })

#define H_ADD_SINGLE(PARENT, ITEM) PTR_SINGLE(h_add_item(PARENT, PTR_BASE(H_SINGLE(ITEM))))
#define H_ADD_COMPOSITE(PARENT, ITEM) PTR_COMPOSITE(h_add_item(PARENT, PTR_BASE(H_COMPOSITE(ITEM))))

#define H_ADD_ATTR_COMPOSITE(PARENT, ITEM, ATTR) PTR_COMPOSITE(h_add_item(PARENT, PTR_BASE(H_ATTR_COMPOSITE(ITEM, ATTR))))

#define H_ADD_SINGLE_TEXT(PARENT, ITEM, TEXT) h_add_text(H_ADD_SINGLE(PARENT, ITEM), (TEXT))
#define H_ADD_SINGLE_ATTR(PARENT, ITEM, ATTR) h_set_attr(PTR_BASE(H_ADD_SINGLE(PARENT, ITEM)), (ATTR))
#define H_ADD_SINGLE_ATTR_TEXT(PARENT, ITEM, ATTR, TEXT) h_add_attr_text(H_ADD_SINGLE(PARENT, ITEM), (ATTR), (TEXT))

#define H_GET_SINGLE_TEXT(TAG) (&(TAG)->text)

/***** Types **************************************************/

typedef enum {
    single_tag,
    composite_tag
} h_tag_type_t;

typedef struct {
    h_tag_type_t type;
    const char *name;
    g_string_t attr;
} h_tag_base_t;

typedef struct h_tag_single_struct {
    h_tag_base_t base;
    g_string_t text;
} h_tag_single_t;

typedef struct h_composite_item_struct {
    h_tag_base_t *item;
    struct h_composite_item_struct *next;
} h_composite_item_t;

typedef struct h_tag_composite_struct {
    h_tag_base_t base;
    h_composite_item_t *children;
    h_composite_item_t *last;
} h_tag_composite_t;

typedef struct {
    h_tag_composite_t *root;
} g_html_t;

// Shortcuts
typedef h_tag_base_t * T_BASE;
typedef h_tag_single_t * T_SINGLE;
typedef h_tag_composite_t * T_COMPOSITE;

/***** Prototypes *********************************************/

#ifndef CPROTO

#include "g_html_api.h"

#endif

#endif
