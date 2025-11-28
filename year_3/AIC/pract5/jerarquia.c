/**
 * @file jerarquia.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las funciones de acceso a la jerarquía de memoria comunes a todos los niveles
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define jerarquia_C

#include <memory.h>
#include <stdlib.h>

#include "main.h"
#include "tipos.h"

#include "memoria-cache.h"
#include "memoria-principal.h"

#include "g_string.h"
#include "g_html.h"
#include "g_list.h"
#include "presentacion-html.h"

#include "jerarquia.h"

/* -- Variables -- */

char fase_instruc_jerarquia[TAG_STR] = "";

interfaz_nivel_t interfaces_jerarquia[num_tipo_niveles_memoria] = {
        { // nivel_tipo_cache
                .obtiene_config = obtiene_config_memoria_cache,
                .inicia_operacion = inicia_oper_mem_cache,
                .actualiza_estado = actualiza_estado_mem_cache,
                .vuelca_datos = vuelca_datos_cache,
                .muestra_estado = muestra_estado_memoria_cache
        },
        { // nivel_tipo_memoria_principal
                .obtiene_config = obtiene_config_memoria_principal,
                .inicia_operacion = inicia_oper_mem_principal,
                .actualiza_estado = actualiza_estado_mem_principal,
                .vuelca_datos = vuelca_datos_mem_principal,
                .muestra_estado = muestra_estado_memoria_principal
        }
};

/* -- Functions -- */

/**
 * Inicializa un nivel de la jerarquía de memoria genérico
 */
void inicializa_nivel_jerarquia(
        nivel_jerarquia_memoria_t *nivel,
        config_operaciones_t op_config,  /**< Configuración del buffer de operaciones */
        size_t size_oper
) {
    unsigned int i;
    void *operaciones;
    operacion_nivel_t *oper;

    nivel->activa = SI;

    /** Operaciones */
    nivel->oper_config = op_config;
    nivel->oper_ciclo = 0;
    nivel->siguiente = NULL;

    QUEUE_INIT(nivel->oper_pendientes, nivel->ultima_pendiente);
    QUEUE_INIT(nivel->oper_libres, nivel->ultima_libre);

    /* Inicializa las operaciones de caché */
    operaciones = calloc(op_config.max_oper, size_oper);

    foreach(i, op_config.max_oper) {
        oper = operaciones + i * size_oper;
        oper->parent = nivel;
        oper->tipo = OP_NULA;
        oper->indice = i;

        QUEUE_APPEND_ITEM(operacion_nivel_t, nivel->oper_libres, nivel->ultima_libre, oper);
    }

    memset(&nivel->estad, 0, sizeof(estadisticas_memoria_t));
}

/**
 * Mira si hay operaciones pendientes en este nivel
 * @param nivel
 */
boolean
hay_operaciones_pendientes(
        NIVEL_MEM_PTR nivel
) {
    return (nivel->oper_pendientes != NULL) ? SI : NO;
}

/**
 * Mira si hay operaciones libres en este nivel y se puede comenzar una nueva en este ciclo
 * @param nivel
 */
boolean
hay_operaciones_libres(
        NIVEL_MEM_PTR nivel
) {
    // Mirar si hay operaciones libres y se puede lanzar otra operación en este ciclo
    return (nivel->oper_libres != NULL && nivel->oper_ciclo < nivel->oper_config.max_oper_ciclo) ? SI : NO;
}

/**
 * Busca un hueco en el buffer de operaciones libres y lo reserva
 * @param nivel
 * @return Un puntero a la operación disponible
 */
operacion_nivel_t *
reserva_operacion(
        NIVEL_MEM_PTR nivel
) {
    operacion_nivel_t *oper = NULL;

    if (hay_operaciones_libres(nivel)) {
        // Incrementa el número de operaciones lanzadas en este ciclo
        nivel->oper_ciclo++;
        // Mueve la operación a la lista de pendientes
        oper = QUEUE_EXTRACT_ITEM(operacion_nivel_t, nivel->oper_libres, nivel->ultima_libre);
        QUEUE_APPEND_ITEM(operacion_nivel_t, nivel->oper_pendientes, nivel->ultima_pendiente, oper);
    }

    return oper;
}

/**
 * Libera una operación y la añade a la lista de operaciones libres
 * @param nivel
 * @param oper
 * @return
 */
void
libera_operacion(
        NIVEL_MEM_PTR nivel,
        operacion_nivel_t *oper
) {
    // Mueve la operación a la lista de disponibles
    QUEUE_DELETE_ITEM(operacion_nivel_t, nivel->oper_pendientes, nivel->ultima_pendiente, oper);
    QUEUE_APPEND_ITEM(operacion_nivel_t, nivel->oper_libres, nivel->ultima_libre, oper);
}

/**
 * Inicializa una operación lanzada en un nivel
 * @param oper
 * @param src
 * @param tipo
 * @param dir
 * @param tam
 * @param orden
 * @param callback
 * @param oper_data
 */
void inicia_operacion_nivel(
        operacion_nivel_t *oper,
        nivel_jerarquia_memoria_t *src,
        tipo_operacion_memoria_t tipo,
        dword dir,
        size_t tam,
        ciclo_t orden,
        operacion_finalizada_fn callback,
        void *oper_data
) {
    oper->src = src;
    oper->tipo = tipo;
    oper->estado = EST_OP_PENDIENTE;
    oper->direccion = dir;
    oper->tam = tam;
    oper->orden = orden;
    oper->inicio = Ciclo; // Ciclo actual
    oper->ciclos = 0;

    oper->finaliza_operacion = callback;
    oper->oper_data = oper_data;

    estado_operacion(oper, "Pendiente");
}

void suma_estadisticas(
        estadisticas_accesos_t *seccion,
        operacion_nivel_t *oper
) {
    seccion->num_accesos++;
    seccion->num_aciertos += (oper->acierto) ? 1 : 0;
    seccion->num_ciclos += Ciclo - oper->inicio + 1;
}

void actualiza_estadisticas(
        operacion_nivel_t *oper
) {
    estadisticas_memoria_t *estad = &oper->parent->estad;

    suma_estadisticas(&estad->totales, oper);

    if (oper->tipo == OP_LECT) {
        suma_estadisticas(&estad->lectura, oper);
    } else {
        suma_estadisticas(&estad->escritura, oper);
    }
}

/**
 * Muestra Las estadísticas de este nivel de la jerarquía en formato texto
 * @param nivel
 * @param output
 */
void
muestra_estadisticas_nivel_txt(
        NIVEL_MEM_PTR nivel,
        g_string_t *output
) {
    g_str_printf(output, "Nivel: %s\n", nivel->nombre);

    g_str_copy(output, "  En lectura:   ");
    muestra_estadistica_tipo_txt(&nivel->estad.lectura, output);
    g_str_copy(output, "  En escritura: ");
    muestra_estadistica_tipo_txt(&nivel->estad.escritura, output);
    g_str_copy(output, "  Totales:      ");
    muestra_estadistica_tipo_txt(&nivel->estad.totales, output);
}

/**
 * Muestra las estadísticas de un tipo concreto en formato texto
 * @param stat
 * @param output
 */
void
muestra_estadistica_tipo_txt(
        estadisticas_accesos_t *stat,
        g_string_t *output
) {
    if (stat->num_accesos) {
        g_str_printf(output, "Accesos: %6d", stat->num_accesos);
        g_str_printf(output, " | Aciertos: %6d", stat->num_aciertos);
        g_str_printf(output, " | Tasa Aciertos: %6.2f %%",
                     100.0 * (double) stat->num_aciertos / (double) stat->num_accesos);
        g_str_printf(output, " | Tiempo Acceso: %6.2f\n",
                     (double) stat->num_ciclos / (double) stat->num_accesos);
    } else {
        g_str_copy(output, "Sin accesos\n");
    }
}

/**
 * Muestra Las estadísticas de este nivel de la jerarquía en formato HTML
 * @param nivel
 * @param output
 */
void
muestra_estadisticas_nivel_html(
        NIVEL_MEM_PTR nivel,
        T_COMPOSITE table
) {
    I_INIT_PRINT();

    T_COMPOSITE tr;

    tr = H_ADD_COMPOSITE(table, tr);
    H_ADD_SINGLE_TEXT(tr, td, nivel->nombre);

    muestra_estadistica_tipo_html(&nivel->estad.lectura, tr);
    muestra_estadistica_tipo_html(&nivel->estad.escritura, tr);
    muestra_estadistica_tipo_html(&nivel->estad.totales, tr);
}

/**
 * Muestra las estadísticas de un tipo concreto en formato HTML
 * @param stat
 * @param output
 */
void
muestra_estadistica_tipo_html(
        estadisticas_accesos_t *stat,
        T_COMPOSITE tr
) {
    I_INIT_PRINT();

    if (stat->num_accesos) {
        H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%d", stat->num_accesos));
        H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%d", stat->num_aciertos));
        H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%.2f %%",
                                           100.0 * (double) stat->num_aciertos / (double) stat->num_accesos));
        H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%.2f",
                                           (double) stat->num_ciclos / (double) stat->num_accesos));
    } else {
        H_ADD_SINGLE_ATTR_TEXT(tr, td, "colspan=4", "-");
    }
}

/**
 * Imprime el estado de las estructuras de coma flotante
 */
void
imprime_memoria_html() {
    char filename[LONG_STR];
    G_HTML_DECL(g_html);
    G_STR_DECL(content);

    I_INIT_PRINT();

    T_COMPOSITE html, head, body;
    T_SINGLE single;

    sprintf(filename, "memoria" F_LCICLO ".html", Ciclo);
    if (html_merge == SI) {
        fprintf(f_salida, "'%s':`", filename);
    } else {
        f_salida = fopen(filename, "w");
        if (f_salida == NULL) {
            fprintf(stderr, "Error creando %s\n", filename);
            exit(1);
        }
    }

    // Inicializa el buffer para ahorrar mallocs posteriores
    g_str_alloc(&content, DEFAULT_HTML_BUFFER);

    /*** HTML INTRO ***/

    html = g_html_init(&g_html, "html");

    head = H_ADD_COMPOSITE(html, head);
    H_ADD_SINGLE_TEXT(head, title, I_PRINTF("Memoria: %s. Ciclo " F_CICLO, nombre_fich, Ciclo));
    H_ADD_SINGLE_ATTR(head, link, "rel='stylesheet' href='aic-style.css'");

    body = H_ADD_COMPOSITE(html, body);

    // BODY

    // NAVIGATION
    H_ADD_SINGLE_ATTR_TEXT(body, a, "href='index.html'", "INICIO");
    H_ADD_SINGLE_ATTR_TEXT(body, a, "href='final.html'", "FINAL");

    if (Ciclo > 10) {
        H_ADD_SINGLE_ATTR_TEXT(body, a, I_PRINTF("href='memoria" F_LCICLO ".html'", Ciclo - 10), "[-10]");
        H_ADD_SINGLE_TEXT(body, span, "&nbsp;");
    } else {
        H_ADD_SINGLE_TEXT(body, span, "[-10]&nbsp;");
    }

    if (Ciclo > 5) {
        H_ADD_SINGLE_ATTR_TEXT(body, a, I_PRINTF("href='memoria" F_LCICLO ".html'", Ciclo - 5), "[-5]");
        H_ADD_SINGLE_TEXT(body, span, "&nbsp;");
    } else {
        H_ADD_SINGLE_TEXT(body, span, "[-5]&nbsp;");
    }

    if (Ciclo > 1) {
        H_ADD_SINGLE_ATTR_TEXT(body, a, I_PRINTF("href='memoria" F_LCICLO ".html'", Ciclo - 1), "[-1]");
        H_ADD_SINGLE_TEXT(body, span, "&nbsp;");
    } else {
        H_ADD_SINGLE_TEXT(body, span, "[-1]&nbsp;");
    }

    if (!final) {
        H_ADD_SINGLE_ATTR_TEXT(body, a, I_PRINTF("href='memoria" F_LCICLO ".html'", Ciclo + 1), "[+1]");
        H_ADD_SINGLE_TEXT(body, span, "&nbsp;");
        H_ADD_SINGLE_ATTR_TEXT(body, a, I_PRINTF("href='memoria" F_LCICLO ".html'", Ciclo + 5), "[+5]");
        H_ADD_SINGLE_TEXT(body, span, "&nbsp;");
        H_ADD_SINGLE_ATTR_TEXT(body, a, I_PRINTF("href='memoria" F_LCICLO ".html'", Ciclo + 10), "[+10]");
        H_ADD_SINGLE_TEXT(body, span, "&nbsp;");
    } else {
        H_ADD_SINGLE_TEXT(body, span, "[+1]&nbsp;");
        H_ADD_SINGLE_TEXT(body, span, "[+5]&nbsp;");
        H_ADD_SINGLE_TEXT(body, span, "[+10]&nbsp;");
    }

    H_ADD_SINGLE_ATTR_TEXT(body, a, I_PRINTF("href='estado" F_LCICLO ".html'", Ciclo), "Estado");
    H_ADD_SINGLE_TEXT(body, span, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");

    H_ADD_SINGLE_ATTR_TEXT(body, a, I_PRINTF("href='crono" F_LCICLO ".html'", Ciclo), "Crono");
    H_ADD_SINGLE_TEXT(body, span, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");

    H_ADD_SINGLE_ATTR_TEXT(body, a, I_PRINTF("href='predictor" F_LCICLO ".html'", Ciclo), "BTB");
    H_ADD_SINGLE_TEXT(body, span, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");

    H_ADD_SINGLE_TEXT(body, span, "Memoria");
    H_ADD_SINGLE_TEXT(body, span, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");

    H_ADD_SINGLE_TEXT(body, tt, I_PRINTF("<b>Programa: %s</b>", nombre_fich));
    H_ADD_SINGLE_TEXT(body, span, I_PRINTF("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>Ciclo: " F_CICLO "</b><br>", Ciclo));
    H_ADD_SINGLE_TEXT(body, p, "Estado al final del ciclo");

    // CONTENT

    /*** Jerarquía de memoria ***/
    single = H_ADD_SINGLE_TEXT(body, div, "\n");
    for (int i = NUM_ELEM_MEMORIA - 1; i >= 0; i--) {
        if (!elementos_memoria[i]->activa) continue;

        g_str_clear(&content);

        INVOCA_INTERFAZ_MEMORIA(elementos_memoria[i], muestra_estado, formato_html, &content);
        if (h_get_compact_mode()) {
            g_str_copy(H_GET_SINGLE_TEXT(single), g_str_string(&content));
        } else {
            g_str_indent(H_GET_SINGLE_TEXT(single), g_str_string(&content), 2, 2);
        }
    }

    // FOOTER
    H_ADD_SINGLE_TEXT(body, address, "Arquitectura e Ingenier&iacute;a de Computadores");
    g_html_flush(&g_html, f_salida);

    if (html_merge == SI) {
        fputs("`,", f_salida);
    } else {
        fclose(f_salida);
    } /* endif */

} /* end imprime_estado */
