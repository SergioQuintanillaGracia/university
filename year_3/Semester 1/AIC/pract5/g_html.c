/**
 * @file g_html.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Soporte para imprimir HTML
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define g_html_C

/***** Includes ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tipos.h"
#include "g_list.h"
#include "g_html.h"

/*** Variables ************************************************/

static boolean html_compact = NO;

/*** Funciones ************************************************/

void h_set_compact_mode(
        boolean compact
) {
    html_compact = compact;
}

boolean
h_get_compact_mode() {
    return html_compact;
}

void
h_init_base_tag(
        h_tag_base_t *tag,
        const char *name,
        h_tag_type_t type
) {
    memset(tag, 0, sizeof(h_tag_base_t));
    tag->name = name;
    tag->type = type;
    g_str_init(&tag->attr);
}

T_SINGLE
h_new_single_tag(
        const char *name
) {
    T_SINGLE tag = malloc(sizeof(h_tag_single_t));
    H_INIT_SINGLE_TAG(tag, name);

    return tag;
}

T_COMPOSITE
h_new_composite_tag(
        const char *name
) {
    T_COMPOSITE tag = malloc(sizeof(h_tag_composite_t));
    H_INIT_COMPOSITE_TAG(tag, name);

    return tag;
}

T_BASE
h_set_attr(
        T_BASE tag,
        const char *attr
) {
    g_str_clear(&tag->attr);
    g_str_copy(&tag->attr, " ");
    g_str_copy(&tag->attr, attr);

    return tag;
}

T_BASE
h_add_attr(
        T_BASE tag,
        const char *attr
) {
    g_str_printf(&tag->attr, " %s", attr);

    return tag;
}

T_SINGLE
h_add_text(
        T_SINGLE tag,
        const char *text
) {
    g_str_copy(&tag->text, text);

    return tag;
}

T_SINGLE
h_add_attr_text(
        T_SINGLE tag,
        const char *attr,
        const char *text
) {
    g_str_printf(&PTR_BASE(tag)->attr, " %s", attr);
    g_str_copy(&tag->text, text);

    return tag;
}

T_BASE
h_add_item(
        T_COMPOSITE parent,
        T_BASE item
) {
    h_composite_item_t *child = malloc(sizeof(h_composite_item_t));
    child->item = item;

    QUEUE_APPEND_ITEM(h_composite_item_t, parent->children, parent->last, child);

    return item;
}

void
h_tag_flush(
        T_BASE tag,
        g_string_t *str
) {
    h_composite_item_t *child;
    G_STR_DECL(g_children);

    char *nl;

    nl = (html_compact) ? "" : "\n";

    switch (tag->type) {
        case single_tag:
            g_str_printf(str, "<%s%s", tag->name, g_str_string(&tag->attr));
            if (g_str_length(&PTR_SINGLE(tag)->text) > 0) {
                g_str_printf(str, ">%s</%s>%s", g_str_string(&PTR_SINGLE(tag)->text), tag->name, nl);
            } else {
                g_str_printf(str, " />%s", nl);
            }
            break;
        case composite_tag:
            g_str_printf(str, "<%s%s>%s", tag->name, g_str_string(&tag->attr), nl);
            for (child = PTR_COMPOSITE(tag)->children; child != NULL; child = child->next) {
                if (html_compact) {
                    h_tag_flush(child->item, str);
                } else {
                    g_str_clear(&g_children);
                    h_tag_flush(child->item, &g_children);
                    g_str_indent(str, g_str_string(&g_children), 2, 2);
                }
            }
            g_str_printf(str, "</%s>%s", tag->name, nl);
            break;
    }
}

void
h_destroy_base_tag(T_BASE tag) {
    g_str_destroy(&tag->attr);
}

void
h_destroy_single_tag(
        T_SINGLE tag
) {
    h_destroy_base_tag(PTR_BASE(tag));
    g_str_destroy(&tag->text);
    free(tag);
}

void
h_destroy_composite_tag(
        T_COMPOSITE tag
) {
    h_destroy_base_tag(PTR_BASE(tag));

    h_composite_item_t *p_item;
    h_composite_item_t *p_current;

    p_item = tag->children;
    while (p_item != NULL) {
        p_current = p_item;
        p_item = p_current->next;
        h_destroy_tag(p_current->item);
        free(p_current);
    }

    free(tag);
}

void
h_destroy_tag(
        T_BASE tag
) {
    switch (tag->type) {
        case single_tag:
            h_destroy_single_tag(PTR_SINGLE(tag));
            break;
        case composite_tag:
            h_destroy_composite_tag(PTR_COMPOSITE(tag));
            break;
    }
}

T_COMPOSITE
g_html_init(
        g_html_t *html,
        const char *name
) {
    html->root = h_new_composite_tag(name);

    return html->root;
}

void
g_html_flush(
        g_html_t *html,
        FILE *output
) {
    G_STR_DECL(html_str);

    if (html->root != NULL) {
        g_str_alloc(&html_str, DEFAULT_HTML_BUFFER);

        h_tag_flush(PTR_BASE(html->root), &html_str);

        g_str_flush(output, &html_str);
    }
}

void
g_html_destroy(
        g_html_t *html
) {
    if (html->root != NULL) {
        h_destroy_composite_tag(html->root);
    }
}
