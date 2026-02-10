/**
 * @file g_options.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Soporte para el manejo de opciones por línea de órdenes
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef g_options_H
#define g_options_H

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdarg.h>

// Para no redefinir boolean
#include "tipos.h"

/***** Defines ************************************************/


/***** Macros *************************************************/

#define G_OPT_FIELD(OPTS, FIELD) (&((OPTS)->FIELD))
#define g_opt_header_str(OPTS) G_OPT_FIELD(OPTS, header)
#define g_opt_usage_str(OPTS) G_OPT_FIELD(OPTS, usage)
#define g_opt_authors_str(OPTS) G_OPT_FIELD(OPTS, authors)
#define g_opt_brief_str(OPTS) G_OPT_FIELD(OPTS, brief)
#define g_opt_info_str(OPTS) G_OPT_FIELD(OPTS, info)

#define G_CREATE_BASE_OPTION(SHORT, LONG, DESC, TYPE) \
(g_## TYPE ##_option_t *) g_opt_create_option(SHORT, LONG, DESC, # TYPE, TYPE ##_type, sizeof(g_## TYPE ##_option_t))

#define g_opt_create_flag(SHORT, LONG, DESC) G_CREATE_BASE_OPTION(SHORT, LONG, DESC, flag)
#define g_opt_create_integer(SHORT, LONG, DESC) G_CREATE_BASE_OPTION(SHORT, LONG, DESC, integer)
#define g_opt_create_float(SHORT, LONG, DESC) G_CREATE_BASE_OPTION(SHORT, LONG, DESC, float)
#define g_opt_create_string(SHORT, LONG, DESC) G_CREATE_BASE_OPTION(SHORT, LONG, DESC, string)
#define g_opt_create_enum(SHORT, LONG, DESC) G_CREATE_BASE_OPTION(SHORT, LONG, DESC, enum)
#define g_opt_create_custom(SHORT, LONG, DESC) G_CREATE_BASE_OPTION(SHORT, LONG, DESC, custom)

#define g_opt_set_parameter_name(OPTION, NAME)  do { ((g_base_option_t *)(OPTION))->parameter = (NAME); } while(0)
#define g_opt_set_handler(OPTION, HANDLER)      do { ((g_base_option_t *)(OPTION))->handler = (HANDLER); } while(0)

//#define g_opt_error_help(OPTIONS, FORMAT, ...) g_opt_fatal_error(OPTIONS, show_help, "", FORMAT __VA_OPT__(,) __VA_ARGS__)
//#define g_opt_error_option(OPTIONS, PARAM, FORMAT, ...) g_opt_fatal_error(OPTIONS, show_option, PARAM, FORMAT __VA_OPT__(,) __VA_ARGS__)
//#define g_opt_error(OPTIONS, FORMAT, ...) g_opt_fatal_error(OPTIONS, show_header, "", FORMAT __VA_OPT__(,) __VA_ARGS__)

#define g_opt_error_help(OPTIONS, ...) g_opt_fatal_error(OPTIONS, show_help, "", __VA_ARGS__)
#define g_opt_error_option(OPTIONS, PARAM, ...) g_opt_fatal_error(OPTIONS, show_option, PARAM, __VA_ARGS__)
#define g_opt_error(OPTIONS, ...) g_opt_fatal_error(OPTIONS, show_header, "", __VA_ARGS__)

/***** Types **************************************************/

typedef enum {
    show_help,
    show_option,
    show_header
} show_help_t;

struct g_options_struct;
struct g_base_option_struct;

typedef enum {
    flag_type,
    integer_type,
    float_type,
    string_type,
    enum_type,
    custom_type
} g_option_type_t;

#define HANDLER_INTERFACE(NAME) void NAME(struct g_options_struct *opts, struct g_base_option_struct *p_option)
#ifndef LIBRISCV
typedef HANDLER_INTERFACE((*g_opt_handler_ptr));
#else
typedef void (^g_opt_handler)(struct g_options_struct *opts, struct g_base_option_struct *p_option);
typedef g_opt_handler *g_opt_handler_ptr;
#endif

// #define OPTION_HANDLER(OPTION) HANDLER_INTERFACE(OPTION ## _handler)
// #define END_HANDLER(OPTION) g_opt_set_handler(OPTION, OPTION ## _handler);

#ifndef LIBRISCV
#define OPTION_HANDLER(OPTION) \
    auto HANDLER_INTERFACE(OPTION ## _handler); \
    g_opt_set_handler(OPTION, OPTION ## _handler); \
    HANDLER_INTERFACE(OPTION ## _handler)
#else
#define OPTION_HANDLER(OPTION) \
    auto void (^OPTION##_handler)(struct g_options_struct *opts, struct g_base_option_struct *p_option); \
    g_opt_set_handler(OPTION, &OPTION##_handler); \
    OPTION##_handler = ^(struct g_options_struct *opts, struct g_base_option_struct *p_option)
#endif

typedef struct g_base_option_struct {
    const char *opt_short;      // Short option. One character
    const char *opt_long;       // Long option. Use slashes to separate words
    const char *description;    // Option description. Multiple lines are allowed.
    const char *parameter;      // Name of the parameter, if any. Defaults to parameter type
    g_option_type_t type;       // Type of option
    boolean required;           // Indicates if this option is mandatory
    boolean provided;           // Indicates if this option has been provided in the command line

    g_opt_handler_ptr handler;     // Handler function

    struct g_base_option_struct *next;
} g_base_option_t;

// Specific options

#define DEFINE_OPTION_TYPE(TYPE, NAME) \
typedef struct {                       \
    g_base_option_t common;            \
    TYPE * p_value;                    \
} g_## NAME ##_option_t

DEFINE_OPTION_TYPE(boolean, flag);
DEFINE_OPTION_TYPE(int, integer);
DEFINE_OPTION_TYPE(float, float);
DEFINE_OPTION_TYPE(char*, string);
DEFINE_OPTION_TYPE(void*, custom);

typedef struct g_enum_case_struct {
    const char *label;
    int value;
    const char *description;
    struct g_enum_case_struct *next;
} g_enum_case_t;

typedef struct {
    g_base_option_t common;
    g_enum_case_t *cases;
    int *p_value;
} g_enum_option_t;

// Grouping structures

typedef struct g_section_struct {
    const char *title;
    g_base_option_t *options;
    struct g_options_struct *parent;
    struct g_section_struct *next;
} g_section_t;

typedef struct g_options_struct {
    int argc;
    char **argv;
    int i_arg;
    char * param;

    g_section_t *sections;

    g_string_t header;
    g_string_t usage;
    g_string_t authors;
    g_string_t brief;
    g_string_t info;
} g_options_t;

/***** Prototypes *********************************************/

#ifndef CPROTO

#include "g_options_api.h"

#endif

#endif
