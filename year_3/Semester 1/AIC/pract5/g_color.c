/**
 * @file g_color.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Añade soporte de colores a las cadenas genéricas
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define g_color_C

/*** Includes *************************************************/

#include "g_string.h"
#include "g_color.h"

/*** Defines **************************************************/

#define FALSE 0
#define TRUE 1

/*** Variables ************************************************/

static int color_enabled = TRUE;

/*** Funciones ************************************************/

void
g_color_enable() {
    color_enabled = TRUE;
}

void
g_color_disable() {
    color_enabled = FALSE;
}

int
g_color_is_enabled() {
    return color_enabled;
}

void
g_color_set(g_string_t *str, char* color) {
    if (!g_color_is_enabled()) return;
    g_str_printf(str, "\e[%sm", color);
}

void
g_color_rgb(g_string_t *str, char * color, int r, int g, int b) {
    if (!g_color_is_enabled()) return;
    g_str_printf(str, "\e[%s;2;%d;%d;%dm", color, r, g, b);
}

void
g_color_reset(g_string_t *str) {
    if (!g_color_is_enabled()) return;
    g_str_copy(str, "\e[0m");
}

void
g_color_default(g_string_t *str) {
    if (!g_color_is_enabled()) return;
    g_str_printf(str, "\e[%s;%sm", GC_FG_DEFAULT, GC_BG_DEFAULT);
}
