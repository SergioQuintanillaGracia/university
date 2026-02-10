/**
 * @file g_options.c
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

/***** Includes ***********************************************/

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <libgen.h>

#include "g_string.h"
#include "g_color.h"
#include "g_options.h"

/***** Variables **********************************************/


/***** Macros *************************************************/

#define G_ADD_ITEM(TYPE, HEADER, ITEM) \
do {                                   \
    TYPE ** p_item;                    \
    p_item = &(HEADER);                \
    while (*p_item != NULL) {          \
        p_item = &((*p_item)->next);   \
    }                                  \
    *p_item = (TYPE *)(ITEM);          \
} while (0)

/***** Functions **********************************************/

/**
 * Muestra el mensaje de error y termina el programa
 * @param opts
 * @param show_help
 * @param format
 * @param ...
 */
void g_opt_fatal_error(
        g_options_t *opts,
        show_help_t show_mode,
        const char *param,
        const char *format, ...
) {
    va_list ap;

    switch (show_mode) {
        case show_help:
            g_opt_show_help(opts);
            break;
        case show_option:
            g_opt_show_option(opts, param);
            break;
        case show_header:
            g_str_flush(stderr, &opts->header);
            break;
    }
    fprintf(stderr, "\n");

    fprintf(stderr, FG_COLOR(RED));
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, GC_RESET);

    if (show_mode == show_header) {
        fprintf(stderr, "\n\nTry '%s -h' for help!\n",
                basename(opts->argv[0]));
    } else {
        fprintf(stderr, "\n");
    }

    exit(1);

} /* end g_opt_fatal_error */

/**
 * Creates a new section for grouping options
 * @param title
 * @return The new section
 */
static g_section_t *
g_opt_create_section(const char *title) {
    g_section_t *section;

    section = malloc(sizeof(g_section_t));
    memset(section, 0, sizeof(g_section_t));
    section->title = title;

    return section;
}

/**
 * Creates a new option
 * @param title
 * @return The new option
 */
static g_base_option_t *
g_opt_create_option(
        const char *opt_short,
        const char *opt_long,
        const char *description,
        const char *parameter,
        g_option_type_t type,
        size_t option_size
) {
    g_base_option_t *option;

    option = malloc(option_size);
    memset(option, 0, option_size);

    option->opt_short = opt_short;
    option->opt_long = opt_long;
    option->description = description;
    option->type = type;
    option->parameter = parameter;
    option->provided = NO;

    return option;
}

static
char *
g_opt_get_option_signature(
        g_base_option_t *option
) {
    static char signature[128];

    if (strlen(option->opt_short) == 0) {
        sprintf(signature, "--%s", option->opt_long);
    } else if (strlen(option->opt_long) == 0) {
        sprintf(signature, "-%s", option->opt_short);
    } else {
        sprintf(signature, "-%s|--%s", option->opt_short, option->opt_long);
    }

    if (option->type != flag_type) {
        size_t s = strlen(signature);
        sprintf(&signature[s], " <%s>", option->parameter);
    }

    return signature;
}

static
char *
g_opt_get_option_line(
        g_base_option_t *option
) {
    static char signature[128];

    if (strlen(option->opt_short) == 0) {
        sprintf(signature, "    --%s", option->opt_long);
    } else if (strlen(option->opt_long) == 0) {
        sprintf(signature, "-%s", option->opt_short);
    } else {
        sprintf(signature, "-%s, --%s", option->opt_short, option->opt_long);
    }

    if (option->type != flag_type) {
        size_t s = strlen(signature);
        sprintf(&signature[s], " <%s>", option->parameter);
    }

    return signature;
}

static void
g_opt_add_option(
        g_section_t *section,
        g_base_option_t *option
) {
    G_ADD_ITEM(g_base_option_t, section->options, option);
}

/**
 * Initializes a set of options
 * @param options
 * @return The default section
 */
g_section_t *
g_opt_init(
        g_options_t *options,
        int argc,
        char *argv[]
) {
    memset(options, 0, sizeof(g_options_t));
    options->argc = argc;
    options->argv = argv;

    g_str_init(&options->header);
    g_str_init(&options->usage);
    g_str_init(&options->authors);
    g_str_init(&options->info);

    // Default section has no title
    g_opt_add_section(options, "");

    return options->sections;
}

/**
 * Añade una nueva sección de opciones
 * @param options
 * @param title
 * @return La nueva sección
 */
g_section_t *
g_opt_add_section(
        g_options_t *options,
        const char *title
) {
    g_section_t *section;

    section = g_opt_create_section(title);
    section->parent = options;

    G_ADD_ITEM(g_section_t, options->sections, section);

    return section;
}

#define DEFINE_ADD_OPTION_FUNCTION(TYPE, NAME) \
static g_## NAME ##_option_t *                 \
g_opt_add_## NAME(                             \
        g_section_t *section,                  \
        char *opt_short,                       \
        char *opt_long,                        \
        char *description,                     \
        TYPE *p_value                          \
) {                                            \
    g_## NAME ##_option_t *option;             \
                                               \
    option = g_opt_create_## NAME(opt_short, opt_long, description); \
    option->p_value = p_value;                 \
    g_opt_add_option(section, (g_base_option_t *)option);            \
                                               \
    return option;                             \
}

#define DEFINE_ADD_MANDATORY_OPTION_FUNCTION(TYPE, NAME) \
g_## NAME ##_option_t *                                  \
g_opt_add_mandatory_## NAME(                             \
        g_section_t *section,                            \
        char *opt_short,                                 \
        char *opt_long,                                  \
        char *description,                               \
        TYPE *p_value                                    \
) {                                                      \
    g_## NAME ##_option_t *option;                       \
                                                         \
    option = g_opt_add_## NAME(section, opt_short, opt_long, description, p_value); \
    option->common.required = SI;                        \
    return option;                                       \
}

#define DEFINE_ADD_OPTIONAL_OPTION_FUNCTION(TYPE, NAME) \
g_## NAME ##_option_t *                                 \
g_opt_add_optional_## NAME(                             \
        g_section_t *section,                           \
        char *opt_short,                                \
        char *opt_long,                                 \
        char *description,                              \
        TYPE *p_value,                                  \
        TYPE default_value                              \
) {                                                     \
    g_## NAME ##_option_t *option;                      \
                                                        \
    option = g_opt_add_## NAME(section, opt_short, opt_long, description, p_value); \
    option->common.required = NO;                       \
    if (p_value != NULL) *p_value = default_value;      \
    return option;                                      \
}

DEFINE_ADD_OPTION_FUNCTION(boolean, flag)

DEFINE_ADD_OPTION_FUNCTION(int, integer)

DEFINE_ADD_OPTION_FUNCTION(float, float)

DEFINE_ADD_OPTION_FUNCTION(char*, string)

DEFINE_ADD_OPTION_FUNCTION(int, enum)

DEFINE_ADD_OPTION_FUNCTION(void, custom)

DEFINE_ADD_MANDATORY_OPTION_FUNCTION(int, integer)

DEFINE_ADD_MANDATORY_OPTION_FUNCTION(float, float)

DEFINE_ADD_MANDATORY_OPTION_FUNCTION(char*, string)

DEFINE_ADD_MANDATORY_OPTION_FUNCTION(int, enum)

DEFINE_ADD_MANDATORY_OPTION_FUNCTION(void, custom)

DEFINE_ADD_OPTIONAL_OPTION_FUNCTION(boolean, flag)

DEFINE_ADD_OPTIONAL_OPTION_FUNCTION(int, integer)

DEFINE_ADD_OPTIONAL_OPTION_FUNCTION(float, float)

DEFINE_ADD_OPTIONAL_OPTION_FUNCTION(char*, string)

DEFINE_ADD_OPTIONAL_OPTION_FUNCTION(int, enum)

g_custom_option_t *
g_opt_add_optional_custom(
        g_section_t *section,
        char *opt_short,
        char *opt_long,
        char *description,
        void *p_value,
        void *default_value,
        size_t size
) {
    g_custom_option_t *option;

    option = g_opt_add_custom(section, opt_short, opt_long, description, p_value);
    option->common.required = NO;
    memcpy(p_value, default_value, size);

    return option;
}

static g_enum_case_t *
g_opt_create_enum_case(
        const char *label,
        const char *description,
        int value
) {
    g_enum_case_t *enum_case = malloc(sizeof(g_enum_case_t));

    memset(enum_case, 0, sizeof(g_enum_case_t));

    enum_case->label = label;
    enum_case->description = description;
    enum_case->value = value;

    return enum_case;
}

g_enum_case_t *
g_opt_add_enum_case(
        g_enum_option_t *opt_enum,
        const char *label,
        const char *description,
        int value
) {
    g_enum_case_t *enum_case = g_opt_create_enum_case(label, description, value);

    G_ADD_ITEM(g_enum_case_t, opt_enum->cases, enum_case);

    return enum_case;
}


static size_t
g_opt_compute_option_line_size(
        g_options_t *options
) {
    g_section_t *p_section;
    g_base_option_t *p_option;
    size_t maxlength = 0;

    for (p_section = options->sections; p_section != NULL; p_section = p_section->next) {
        for (p_option = p_section->options; p_option != NULL; p_option = p_option->next) {
            size_t length = strlen(g_opt_get_option_line(p_option));
            maxlength = (length > maxlength) ? length : maxlength;
        }
    }

    return maxlength;
}

/**
 * Show program info
 * @param options
 */
void
g_opt_show_about(
        g_options_t *options
) {
    const int tab = 4;
    G_STR_DECL(info);

    // Avoids future reallocs
    g_str_alloc(&info, 2048);

    // Show program header
    g_str_flush(stderr, &options->header);
    fprintf(stderr, "\n");
    g_str_flush(stderr, &options->brief);

    // Show authors
    g_str_printf(&info, "\n%sAutores:%s\n", FG_COLOR(YELLOW), GC_RESET);
    g_str_indent(&info, g_str_string(&options->authors), tab, 0);

    // Show info
    g_str_printf(&info, "\n%sDescripción:%s\n", FG_COLOR(YELLOW), GC_RESET);
    g_str_indent(&info, g_str_string(&options->info), tab, 0);

    g_str_flush(stderr, &info);
}

static g_base_option_t *
g_opt_find_option(
        g_options_t *options,
        const char *param
) {
    g_section_t *p_section;
    g_base_option_t *p_option;

    boolean is_long = g_str_starts_with(param, "--");

    for (p_section = options->sections; p_section != NULL; p_section = p_section->next) {
        for (p_option = p_section->options; p_option != NULL; p_option = p_option->next) {
            if (is_long) {
                if (strcmp(&param[2], p_option->opt_long) == 0)
                    return p_option;
            } else {
                if (strlen(param) == 2 && param[1] == p_option->opt_short[0])
                    return p_option;
            }
        }
    }

    return NULL;
}

void
g_opt_print_option(
        g_base_option_t *p_option,
        g_string_t *help,
        const int tab,
        size_t maxlength
) {
    g_enum_case_t *p_case;
    G_STR_DECL(opt_help);

    g_str_printf(&opt_help, "%s%s%-*s%s%*s%s",
                 FG_COLOR(GREEN), (p_option->required) ? GC_STYLE(BOLD) : "",
                 maxlength, g_opt_get_option_line(p_option),
                 GC_RESET, tab, "",
                 p_option->description);
    g_str_indent(help, g_str_string(&opt_help), maxlength + 2 * tab, tab);
    if (!g_str_ends_with(p_option->description, "\n")) {
        g_str_copy(help, "\n");
    }

    if (p_option->type == enum_type) {
        // Print enum options
        for (p_case = ((g_enum_option_t *) p_option)->cases; p_case != NULL; p_case = p_case->next) {
            g_str_repeat(help, ' ', maxlength + 3 * tab);
            g_color_set(help, GC_BOLD);
            g_str_printf(help, "%s: ", p_case->label);
            g_color_reset(help);
            g_str_copy(help, p_case->description);
            if (!g_str_ends_with(p_case->description, "\n")) {
                g_str_copy(help, "\n");
            }
        }
    }

    if (!p_option->required) {
        switch (p_option->type) {
            case flag_type:
                break;
            case integer_type:
                g_str_repeat(help, ' ', maxlength + 3 * tab);
                g_str_printf(help, "Default: %d\n",
                             *((g_integer_option_t *) p_option)->p_value);
                break;
            case float_type:
                g_str_repeat(help, ' ', maxlength + 3 * tab);
                g_str_printf(help, "Default: %.2f\n",
                             *((g_float_option_t *) p_option)->p_value);
                break;
            case string_type:
                if (*((g_string_option_t *) p_option)->p_value == NULL) break;

                g_str_repeat(help, ' ', maxlength + 3 * tab);
                g_str_printf(help, "Default: %s\n",
                             *((g_string_option_t *) p_option)->p_value);
                break;
            case enum_type:
                for (p_case = ((g_enum_option_t *) p_option)->cases; p_case != NULL; p_case = p_case->next) {
                    if (p_case->value == *((g_enum_option_t *) p_option)->p_value) {
                        g_str_repeat(help, ' ', maxlength + 2 * tab);
                        g_str_printf(help, "Default: %s%s%s\n",
                                     GC_STYLE(BOLD), p_case->label, GC_RESET);
                        break;
                    }
                }
                break;
            case custom_type:
                break;
        }
    }
}

/**
 * Show the help information for a given set options
 * @param options
 */
void
g_opt_show_help(
        g_options_t *options
) {
    const int tab = 4;
    g_section_t *p_section;
    g_base_option_t *p_option;

    G_STR_DECL(help);

    size_t maxlength = g_opt_compute_option_line_size(options);

    // Avoids future reallocs
    g_str_alloc(&help, 2048);

    // Show program header
    g_str_flush(stderr, &options->header);

    // Show usage
    g_str_printf(&help, "\n%sUso:%s\n", FG_COLOR(YELLOW), GC_RESET);
    g_str_indent(&help, g_str_string(&options->usage), tab, 0);
    g_str_flush(stderr, &help);

    // Show Options
    g_str_printf(&help, "\n\n%sOpciones:%s\n", FG_COLOR(YELLOW), GC_RESET);

    for (p_section = options->sections; p_section != NULL; p_section = p_section->next) {

        // Section header
        if (strlen(p_section->title) > 0) {
            g_str_copy(&help, "\n");
            g_str_repeat(&help, ' ', tab);
            g_str_printf(&help, "%s--- %s ---%s\n\n",
                         FG_COLOR(BLUE), p_section->title, GC_RESET);
        }

        for (p_option = p_section->options; p_option != NULL; p_option = p_option->next) {
            g_opt_print_option(p_option, &help, tab, maxlength);

            // Print this option
            g_str_flush(stderr, &help);
        }
    }

    g_str_flush(stderr, &help);
}

/**
 * Show the help information for a given set options
 * @param options
 */
void
g_opt_show_option(
        g_options_t *options,
        const char *param
) {
    const int tab = 4;
    g_base_option_t *p_option;

    G_STR_DECL(help);
    G_STR_DECL(opt_help);

    size_t maxlength = g_opt_compute_option_line_size(options);

    // Avoids future reallocs
    g_str_alloc(&help, 2048);

    // Show program header
    g_str_flush(stderr, &options->header);

    // Show usage
    g_str_printf(&help, "\n%sUso:%s\n", FG_COLOR(YELLOW), GC_RESET);
    g_str_indent(&help, g_str_string(&options->usage), tab, 0);
    g_str_flush(stderr, &help);

    // Show Options
    g_str_printf(&help, "\n\n%sOpción:%s\n", FG_COLOR(YELLOW), GC_RESET);

    p_option = g_opt_find_option(options, param);
    if (p_option != NULL) {
        g_opt_print_option(p_option, &help, tab, maxlength);

        // Print this option
        g_str_flush(stderr, &help);
    } else {
        g_opt_error_help(options, "Option '%s' not found!", param);
    }

    g_str_flush(stderr, &help);
}

/**
 * Parse the command line arguments for a given set of options
 * @param options
 * @param argn
 * @param argc
 * @param argv
 * @return
 */
int
g_opt_parse(
        g_options_t *options,
        int argn
) {
    int rc;
    boolean has_parameter;
    g_section_t *p_section;
    g_base_option_t *p_option;

    options->i_arg = argn;

    while (options->i_arg < options->argc) {
        char *opt = options->argv[options->i_arg];
        if (!g_str_starts_with(opt, "-")) {
            // No more options
            break;
        }

        p_option = g_opt_find_option(options, opt);
        if (p_option == NULL) {
            g_opt_error_help(options, "Option '%s' not found!", opt);
        }

        has_parameter = (options->i_arg + 1 < options->argc);
        if (p_option->type != flag_type) {
            if (has_parameter) {
                options->param = options->argv[options->i_arg + 1];
            } else {
                g_opt_error_option(options, opt, "Option '%s' requires a parameter", opt);
            }
        } else {
            options->param = NULL;
        }

        switch (p_option->type) {
            case flag_type:
                do {
                    g_flag_option_t *p_flag = (g_flag_option_t *) p_option;
                    if (p_flag->p_value != NULL) {
                        if (p_option->provided) {
                            g_opt_error_option(options, opt, "Option '%s' does not allow multiple values",
                                               opt);
                        }

                        *p_flag->p_value = !(*p_flag->p_value);
                    }
                } while (0);
                break;
            case integer_type:
                do {
                    g_integer_option_t *p_integer = (g_integer_option_t *) p_option;

                    options->i_arg++;
                    if (p_integer->p_value != NULL) {
                        if (p_option->provided) {
                            g_opt_error(options, "Option '%s' does not allow multiple values",
                                        opt);
                        }
                        rc = sscanf(options->param, "%d", p_integer->p_value);
                        if (rc != 1) {
                            g_opt_error_option(options, opt,
                                               "Option '%s' parameter is expected to be an integer number",
                                               opt);
                        }
                    } else if (p_option->handler == NULL) {
                        g_opt_error(options,
                                    "IMPL: Option '%s' allows multiple values. Handler is mandatory!",
                                    opt);
                    }
                } while (0);

                break;
            case float_type:
                do {
                    g_float_option_t *p_float = (g_float_option_t *) p_option;

                    options->i_arg++;
                    if (p_float->p_value != NULL) {
                        if (p_option->provided) {
                            g_opt_error(options, "Option '%s' does not allow multiple values",
                                        opt);
                        }
                        rc = sscanf(options->param, "%f", p_float->p_value);
                        if (rc != 1) {
                            g_opt_error_option(options, opt,
                                               "Option '%s' parameter is expected to be a real number",
                                               opt);
                        }
                    } else if (p_option->handler == NULL) {
                        g_opt_error(options,
                                    "IMPL: Option '%s' allows multiple values. Handler is mandatory!",
                                    opt);
                    }
                } while (0);

                break;
            case string_type:
                do {
                    g_string_option_t *p_string = (g_string_option_t *) p_option;

                    options->i_arg++;
                    if (p_string->p_value != NULL) {
                        if (p_option->provided) {
                            g_opt_error(options, "Option '%s' does not allow multiple values",
                                        opt);
                        }
                        *p_string->p_value = options->param;
                    } else if (p_option->handler == NULL) {
                        g_opt_error(options,
                                    "IMPL: Option '%s' allows multiple values. Handler is mandatory!",
                                    opt);
                    }
                } while (0);

                break;
            case enum_type:
                do {
                    g_enum_option_t *p_enum = (g_enum_option_t *) p_option;

                    options->i_arg++;
                    g_enum_case_t *p_case;
                    if (p_enum->p_value != NULL) {
                        if (p_option->provided) {
                            g_opt_error(options, "Option '%s' does not allow multiple values",
                                        opt);
                        }
                        for (p_case = p_enum->cases; p_case != NULL; p_case = p_case->next) {
                            if (strcmp(options->param, p_case->label) == 0) {
                                *p_enum->p_value = p_case->value;
                                break;
                            }
                        }
                        if (p_case == NULL) {
                            g_opt_error_option(options, opt, "Invalid value '%s' for option '%s'",
                                               options->param, opt);
                        }
                    } else if (p_option->handler == NULL) {
                        g_opt_error(options,
                                    "IMPL: Option '%s' allows multiple values. Handler is mandatory!",
                                    opt);
                    }
                } while (0);
                break;
            case custom_type:
                do {
                    g_custom_option_t *p_custom = (g_custom_option_t *) p_option;

                    options->i_arg++;
                    if (p_custom->p_value != NULL) {
                        if (p_option->provided) {
                            g_opt_error(options, "Option '%s' does not allow multiple values",
                                        opt);
                        }
                    }

                    // The actual work is performed by the mandatory handler
                    if (p_option->handler == NULL) {
                        g_opt_error(options,
                                    "IMPL: Option '%s' is user defined. Handler is mandatory!",
                                    opt);
                        exit(1);
                    }
                } while (0);
                break;
        }

        if (p_option->handler != NULL) {
#ifndef LIBRISCV
            p_option->handler(options, p_option);
#else
            (*(p_option->handler))(options, p_option);
#endif
        }

        // Correct option has been found
        p_option->provided = SI;

        options->i_arg++;
    }

    // Check if all required parameters have benn provided

    for (p_section = options->sections; p_section != NULL; p_section = p_section->next) {
        for (p_option = p_section->options; p_option != NULL; p_option = p_option->next) {
            if (p_option->required && !p_option->provided) {
                g_opt_error(options, "Option '%s' is required!",
                            g_opt_get_option_signature(p_option));
            }
        }
    }

    return options->i_arg;
}
