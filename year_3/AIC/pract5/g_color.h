/**
 * @file g_color.h
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

#ifndef g_color_H
#define g_color_H

/***** Includes ***********************************************/

#include "g_string.h"

/***** Defines ************************************************/

#define GC_BOLD             "1"
#define GC_DIM              "2"
#define GC_UNDERLINE        "4"
#define GC_BLINK            "5"
#define GC_REVERSE          "7"

// For RGB sequences
#define GC_FG_COLOR         "38"
#define GC_BG_COLOR         "48"

#define GC_FG_DEFAULT       "39"
#define GC_BG_DEFAULT       "49"

// Standard colors
#define GC_FG_BLACK         "30"
#define GC_FG_RED           "31"
#define GC_FG_GREEN         "32"
#define GC_FG_YELLOW        "33"
#define GC_FG_BLUE          "34"
#define GC_FG_MAGENTA       "35"
#define GC_FG_CYAN          "36"
#define GC_FG_LIGHTGRAY     "37"

#define GC_BG_BLACK         "40"
#define GC_BG_RED           "41"
#define GC_BG_GREEN         "42"
#define GC_BG_YELLOW        "43"
#define GC_BG_BLUE          "44"
#define GC_BG_MAGENTA       "45"
#define GC_BG_CYAN          "46"
#define GC_BG_LIGHTGRAY     "47"

#define GC_FG_DARKGRAY      "90"
#define GC_FG_LIGHTRED      "91"
#define GC_FG_LIGHTGREEN    "92"
#define GC_FG_LIGHTYELLOW   "93"
#define GC_FG_LIGHTBLUE     "94"
#define GC_FG_LIGHTMAGENTA  "95"
#define GC_FG_LIGHTCYAN     "96"
#define GC_FG_WHITE         "97"

#define GC_BG_DARKGRAY      "100"
#define GC_BG_LIGHTRED      "101"
#define GC_BG_LIGHTGREEN    "102"
#define GC_BG_LIGHTYELLOW   "103"
#define GC_BG_LIGHTBLUE     "104"
#define GC_BG_LIGHTMAGENTA  "105"
#define GC_BG_LIGHTCYAN     "106"
#define GC_BG_WHITE         "107"

/***** Macros *************************************************/

#define GC_ESC(CODE) ((g_color_is_enabled()) ? ("\e[" CODE "m") : "")
#define GC_STYLE(STYLE) GC_ESC(GC_ ## STYLE)
#define FG_COLOR(COLOR) GC_ESC(GC_FG_ ## COLOR)
#define BG_COLOR(COLOR) GC_ESC(GC_BG_ ## COLOR)
#define GC_RESET GC_ESC("0")

/***** Prototypes *********************************************/

#ifndef CPROTO

#include "g_color_api.h"

#endif

#endif
