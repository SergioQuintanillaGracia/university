/**
 * @file presentacion-html.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las funciones de presentación en formato HTML
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define presentacion_html_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memoria.h>
#include <tipos.h>

#include "main.h"
#include "instrucciones.h"
#include "registros.h"
#include "jerarquia.h"

#include "presentacion.h"
#include "html-style.h"
#include "presentacion-html.h"
#include "g_html.h"

/***** Variables globales *************************************/

presentacion_interface_t presentacion_html = {
        .imprime_inicio_f = imprime_inicio_html,
        .imprime_final_f = imprime_final_html,
        .inicio_ciclo_f = inicio_ciclo_null,
        .fin_ciclo_f = fin_ciclo_null,
        .imprime_estado_f = imprime_estado_html,
        .imprime_crono_f = imprime_crono_html,
        .imprime_predictor_f = imprime_predictor_html,
        .imprime_memoria_f = imprime_memoria_html,
        .init_instruc_f = init_instruc_html,
        .muestra_fase_f = muestra_fase_html,
        .muestra_datos_f = muestra_datos_null
};

linea_crono_t crono[MAX_INSTRUC];
/* Instrucciones visibles en el cronograma */

boolean html_merge = NO;

FILE *f_salida;

/***** Variables locales **************************************/

static ciclo_t primera_instruccion = 1;
static ciclo_t primer_ciclo = 1;
static short maxima_columna_visible = MAX_CICLOS_INIC;

/***** Definiciones Externas **********************************/

/***************************************************************
 *
 * Func: imprime_estado_predictor
 *
 * Description:
 *   Obtiene el estado del predictor
 *
 **************************************************************/

static void imprime_estado_predictor_html(
        char *nom,
        estado_predic_t e
) {
    char aux[LONG_STR];

    itobin(e, aux, CONFIG(predic.n_bits_pred_local));
    switch (e) {
        case NO_SALTA:
            sprintf(nom, "<font color=\"orange\">(%s) NO Salta</font>", aux);
            break;
        case NO_SALTA_UN_FALLO:
            sprintf(nom, "<font color=\"orange\">(%s) NO Salta (1 fallo)</font>", aux);
            break;
        case SALTA_UN_FALLO:
            sprintf(nom, "<font color=\"green\">(%s) Salta (1 fallo)</font>", aux);
            break;
        case SALTA:
            sprintf(nom, "<font color=\"green\">(%s) Salta</font>", aux);
            break;
        default:
            sprintf(nom, "?");
            break;
    } /* endswitch */

} /* end imprime_estado_predictor */

/***************************************************************
 *
 * Func: imprime_estado_predictor
 *
 * Description:
 *   Obtiene el estado del predictor
 *
 **************************************************************/

static void imprime_estado_predictor_n_html(
        char *nom,
        int e,
        int n_bits
) {
    char aux[LONG_STR];

    if (n_bits == 0)
        sprintf(nom, "<font color=\"orange\">--</font>");
    else {
        itobin(e, aux, n_bits);
        if (e <= pwrtwo(n_bits - 1) - 1) {
            sprintf(nom, "<font color=\"orange\">%s (%d) NO Salta</font>", aux, e);
        } else {
            sprintf(nom, "<font color=\"green\">%s (%d) Salta</font>", aux, e);
        }
    }

}/* end imprime_estado_predictor_n */

static void imprime_estado_BHSR(
        char *nom,
        int e,
        int n_bits
) {
    char aux[LONG_STR];

    if (n_bits == 0)
        sprintf(nom, "<font color=\"orange\">--</font>");
    else {
        itobin(e, aux, n_bits);
        if (e <= pwrtwo(n_bits - 1) - 1) {
            sprintf(nom, "<font color=\"orange\">%s (%d)</font>", aux, e);
        } else {
            sprintf(nom, "<font color=\"green\">%s (%d)</font>", aux, e);
        }
    }

} /* end imprime_estado_predictor_n */

/***************************************************************
 *
 * Func: imprime_estado_selector
 *
 * Description:
 *   Obtiene el estado del predictor
 *
 **************************************************************/

static void imprime_estado_selector_n_html(
        char *nom,
        int e
) {

    char aux[LONG_STR];

    itobin(e, aux, CONFIG(predic.n_bits_selector));
    if (e <= pwrtwo(CONFIG(predic.n_bits_selector) - 1) - 1) {
        sprintf(nom, "<font color=\"orange\">%s (%d) Pred1</font>", aux, e);
    } else {
        sprintf(nom, "<font color=\"green\">%s (%d) Pred2</font>", aux, e);

    }

}/* end imprime_estado_predictor_n */

/***************************************************************
 *
 * Func: desplazamiento_horizontal
 *
 * Description:
 *   Desplaza la zona visible un ciclo hacia la izquierda
 *
 **************************************************************/

static void desplazamiento_horizontal() {

    short i, j;

    for (i = 0; i < MAX_INSTRUC; i++) {
        for (j = 0; j < MAX_CICLOS - 1; j++)
            strcpy(crono[i].fase[j], crono[i].fase[j + 1]);

        /* Borra la Última columna */
        strcpy(crono[i].fase[j], "");
    } /* endfor */

    /*** Actualiza los desplazamientos iniciales ***/

    primer_ciclo++;

} /* end desplazamiento_horizontal */

/***************************************************************
 *
 * Func: desplazamiento_vertical
 *
 * Description:
 *   Desplaza la zona visible un ciclo hacia la izquierda
 *
 **************************************************************/

static void desplazamiento_vertical() {

    short i, j, desp;

    desp = MAX_CICLOS;

    for (i = 1; i < MAX_INSTRUC; i++) {
        if (crono[i].min > primer_ciclo) {
            desp = MIN(desp, crono[i].min - primer_ciclo);
        } else {
            for (j = 0; j < MAX_CICLOS; j++) {
                if (strlen(crono[i].fase[j]) > 0) {
                    desp = MIN(desp, j);
                    break;
                }
            }
        }
    }

    for (i = 0; i < MAX_INSTRUC - 1; i++) {
        crono[i].PC = crono[i + 1].PC;
        crono[i].min = crono[i + 1].min;
        crono[i].max = crono[i + 1].max;
        strcpy(crono[i].inst, crono[i + 1].inst);

        for (j = 0; j < MAX_CICLOS; j++) {
            if (j < MAX_CICLOS - desp) {
                strcpy(crono[i].fase[j], crono[i + 1].fase[j + desp]);
            } else {
                strcpy(crono[i].fase[j], "");
            }
        }
    }

    /* Borra la última fila */
    strcpy(crono[MAX_INSTRUC - 1].inst, "");
    for (j = 0; j < MAX_CICLOS; j++) {
        strcpy(crono[MAX_INSTRUC - 1].fase[j], "");
    }
    crono[MAX_INSTRUC - 1].min = MAX_CICLOS;
    crono[MAX_INSTRUC - 1].max = 0;

    /*** Actualiza los desplazamientos iniciales ***/

    primer_ciclo += desp;
    primera_instruccion++;
    maxima_columna_visible -= desp;

} /* end desplazamiento_vertical */

/***************************************************************
 *
 * Func: imprime_config_html
 *
 * Description:
 **************************************************************/

static void imprime_config_html() {

    char aux[LONG_STR];

    G_STR_DECL(info);

    fprintf(f_salida, "<table>\n");
    fprintf(f_salida, " <tr valign=\"top\"><td>\n");

    /*** Estructuras ***/

    fprintf(f_salida, "<table class='config'>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <th>Estructura</th>\n");
    fprintf(f_salida, "  <th class='value'>Número</th>\n");
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Num v&iacuteas Issue</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(num_vias.issue));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Num v&iacuteas Writeback</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(num_vias.bus));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Num v&iacuteas Commit</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(num_vias.commit));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Registros</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", TAM_REGISTROS);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Reorder&nbsp;Buffer</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(tam_reorder));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>E.R.&nbsp;Suma/Resta</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(tam_rs.sumrest));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>E.R.&nbsp;Multiplicación/División</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(tam_rs.multdiv));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>E.R.&nbsp;Enteros</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(tam_rs.enteros));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Buffers de carga</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(tam_rs.carga));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Buffers de almacen.</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(tam_rs.almacen));
    fprintf(f_salida, " </tr>\n");

    imprime_tipo_predictor(aux);
    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Tipo&nbsp;de&nbsp;predictor</td>\n");
    fprintf(f_salida, "  <td class='value'>%s</td>\n", aux);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Buffer&nbsp;predictor</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(predic.tam_buffer_predic));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, "</table><br>\n");


    fprintf(f_salida, "  </td><td>\n");

    /*** Tiempos de evaluación ***/

    fprintf(f_salida, "<table class='config'>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <th>Operador</th>\n");
    fprintf(f_salida, "  <th class='value'>Cantidad</th>\n");
    fprintf(f_salida, "  <th class='value'>Latencia</th>\n");
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Suma/Resta</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(sumrest.num_oper));
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(sumrest.teval));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Multiplicación/División</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(multdiv.num_oper));
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(multdiv.teval));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Enteros</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(enteros.num_oper));
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(enteros.teval));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td class='header'>Memoria de datos</td>\n");
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(memdatos.num_oper));
    fprintf(f_salida, "  <td class='value'>%d</td>\n", CONFIG(memdatos.teval));
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, "</table><br>\n");

    fprintf(f_salida, "<td>\n");
    /*** Jerarquía de memoria ***/
    for (int i = NUM_ELEM_MEMORIA - 1; i >= 0; i--) {
        g_str_clear(&info);
        INVOCA_INTERFAZ_MEMORIA(elementos_memoria[i], obtiene_config, formato_html, &info);
        fprintf(f_salida, "%s<br>\n", g_str_string(&info));
    }
    fprintf(f_salida, "</td>\n");

    fprintf(f_salida, "  </td></tr>\n");
    fprintf(f_salida, "</table><br>\n");

}


/***** Funciones Exportables **********************************/

/***************************************************************
 *
 * Func: init_instruc
 *
 * Description:
 *   Inicializa una instrucción en el cronograma
 *
 **************************************************************/

void init_instruc_html(
        dword PC,
        ciclo_t orden,
        const char *overwrite
) {

    int o;
    char texto[LONG_STR];

    if (orden == primera_instruccion + MAX_INSTRUC)
        desplazamiento_vertical();

    o = orden - primera_instruccion;
    if (overwrite == NULL) {
        imprime_instruccion_color(texto, PC, SI);
    } else {
        strcpy(texto, overwrite);
    }
    crono[o].PC = PC;

    strcpy(crono[o].inst, texto);

} /* end init_instruc */

/***************************************************************
 *
 * Func: muestra_fase
 *
 * Description:
 *   Muestra la fase de cada instrucción en el cronograma
 *
 **************************************************************/

void muestra_fase_html(
        char *fase,
        ciclo_t orden,
        boolean exception
) {

    int o, c;

    if (Ciclo >= primer_ciclo + MAX_CICLOS)
        desplazamiento_horizontal();

    o = orden - primera_instruccion;
    c = Ciclo - primer_ciclo;

    crono[o].min = MIN(crono[o].min, Ciclo);
    crono[o].max = MAX(crono[o].max, Ciclo);

    if (o >= 0 && c >= 0) {
        if (!exception) {
            strcpy(crono[o].fase[c], fase);
        } else {
            sprintf(crono[o].fase[c], "<font color='red'>%s</font>", fase);
        }
        if (c + 1 > maxima_columna_visible)
            maxima_columna_visible = c + 1;
    } /* endif */

} /* end muestra_fase */

/***************************************************************
 *
 * Func: imprime_DMem
 *
 * Desc: Imprime la memoria de datos
 *
 **************************************************************/

static void imprime_DMem_html() {

    dword i;
    dword d;
    dword next;

    char aux[LONG_STR];
    char color[LONG_STR];

    nueva_firma(); /*** FIRMAS ***/

    for (region_t r = 0; r < num_regiones; r++) {
        if (!es_region_datos(r) || tam_region(mmem, r) == 0)
            continue;

        fprintf(f_salida, "<table>\n");

        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"50\" align=\"center\">Dirección</td>\n");
        for (i = 0; i < sizeof(dword); i++) {
            fprintf(f_salida, "  <td width=\"20\" align=\"center\">+" F_DWORD "</td>\n", i);
        } /* endif */
        fprintf(f_salida, " </tr>\n");

        next = 0;
        for (d = inicio_region(mmem, r); d < final_region(mmem, r); d++) {

            i = dir_a_offset(mmem, d, r);

            if (d % sizeof(dword) == 0) {
                fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
                te_etiqueta(aux, d);
                color_etiqueta_memdatos(color, d);
                fprintf(f_salida, "  <td class='address' bgcolor=\"#ddeeff\">"
                                  "<span style='color: %s; margin: 0 0.5em'>%s</span></td>\n", color, aux);
            } /* endif */

            if (i == next) {
                switch (mmem->datos[r].tipos[i]) {
                    case t_byte:
                        FIRMA_2_2("MD"
                                          F_DWORD, i, "%hhX", mmem->datos[r].mem.m_byte[indice_byte(i)]);
                        /* Imprime también el caracter */
                        sprintf(aux, "%s %c", vstr, mmem->datos[r].mem.m_byte[indice_byte(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\">%s</td>\n", aux);
                        next = i + sizeof(byte);
                        break;
                    case t_half:
                        FIRMA_2_2("MD"
                                          F_DWORD, i, "%hd", mmem->datos[r].mem.m_half[indice_half(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\" colspan=\"2\">%s</td>\n", vstr);
                        next = i + sizeof(half);
                        break;
                    case t_word:
                        FIRMA_2_2("MD"
                                          F_DWORD, i, "%d", mmem->datos[r].mem.m_word[indice_word(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\" colspan=\"4\">%s</td>\n", vstr);
                        next = i + sizeof(word);
                        break;
                    case t_dword:
                        FIRMA_2_2("MD"
                                          F_DWORD, i, F_DWORD, mmem->datos[r].mem.m_dword[indice_dword(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\" colspan=\"8\">%s</td>\n", vstr);
                        next = i + sizeof(dword);
                        break;
                    case t_float:
                        FIRMA_2_2("MD"
                                          F_DWORD, i, "%.2f", mmem->datos[r].mem.m_float[indice_float(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\" colspan=\"4\">%s</td>\n", vstr);
                        next = i + sizeof(float);
                        break;
                    case t_double:
                        FIRMA_2_2("MD"
                                          F_DWORD, i, "%.2f", mmem->datos[r].mem.m_double[indice_double(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\" colspan=\"8\">%s</td>\n", vstr);
                        next = i + sizeof(double);
                        break;
                    default:
                        fprintf(f_salida, "  <td>&nbsp;</td>\n");
                        next = i + 1;
                } /* endswitch */
            } /* endif */
        } /* endif */

        fprintf(f_salida, "</table><br>\n");

    }
}

/***************************************************************
 *
 * Func: imprime_Imem_col
 *
 * Desc: Imprime el estado de la memoria de instrucciones en una columna
 *
 **************************************************************/

static void imprime_IMem_html() {

    short i = 0;
    char aux[SHORT_STR];
    G_STR_DECL(formato);

    /*** Contenido de la memoria de instrucciones ***/

    fprintf(f_salida, "<!-- Mem Instr en columna -->\n");
    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"75\" align=\"left\" nowrap='nowrap'>Dirección</td>\n");
    fprintf(f_salida, "  <td width=\"200\" align=\"left\" nowrap='nowrap'>Instrucciones</td>\n");
    if (OPTION(show_format) != format_none) {
        fprintf(f_salida, "  <td aling='center' nowrap='nowrap'>Formato</td>\n");
    }
    fprintf(f_salida, " </tr>\n");

    for (i = inicio_region(mmem, r_text); i < final_region(mmem, r_text); i += 4) {
        fprintf(f_salida, " <tr>\n");
        te_etiqueta(aux, i);
        fprintf(f_salida, "  <td class='address'>%s</td>\n", aux);
        imprime_instruccion(aux, i);
        fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" nowrap='nowrap'>%s</td>\n", aux);
        if (OPTION(show_format) != format_none) {
            imprime_formato(&formato, i);
            fprintf(f_salida,
                    "  <td bgcolor=\"#eeeeee\" nowrap='nowrap'>%s</td>\n", g_str_string(&formato));
        }
        fprintf(f_salida, " </tr>\n");
    }

    fprintf(f_salida, "</table>\n");
}

/***************************************************************
 *
 * Func: imprime_inicio
 *
 * Desc: Imprime el cronograma de ejecución
 *
 **************************************************************/

void imprime_inicio_html() {

    short i;

    char aux[LONG_STR];

    primera_instruccion = 1;
    primer_ciclo = 1;
    maxima_columna_visible = MAX_CICLOS_INIC;

    for (i = 0; i < MAX_INSTRUC; i++) {
        memset(&crono[i], 0, sizeof(linea_crono_t));
        crono[i].min = MAX_CICLOS;
        crono[i].max = 0;
    } /* endfor */

    if (html_merge == SI) {
        //        sprintf(aux, "%s.htm", nombre_fich);
        sprintf(aux, "%s.htm", "index");
    } else {
        sprintf(aux, "index.html");
    } /* endif */

    /** Genera la hoja de estilos 'aic-style.css' */
    escribe_aic_style_css();
    // Existe una variable 'aic_style_css_string' por si hiciera falta para la versión compacta

    f_salida = fopen(aux, "w");
    if (f_salida == NULL) {
        fprintf(stderr, "Error creando %s\n", aux);
        exit(1);
    } /* endif */

    if (html_merge == SI) {
        fputs("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
              "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
              "<head>\n"
              "<title>HTML merge</title>\n"
              "<!--\n"
              "     Iframe occupies the whole body\n"
              "-->\n"
              "<style type=\"text/css\">\n"
              "  body, html\n"
              "  {\n"
              "     margin: 0; padding: 0; height: 100%; overflow: hidden;\n"
              "  }\n"
              "\n"
              "  #content\n"
              "  {\n"
              "     position:absolute; left: 0; right: 0; bottom: 0; top: 0px;\n"
              "  }\n"
              " .field { display: inline-block; font-size: 90%%; height: 16px; "
              "text-align: center; padding: 4px 0px 0px 0px; border: 1px solid black}\n"
              "\n"
              "</style>\n"
              "\n"
              "<script>\n"
              "\n"
              "css_file=`",
              f_salida);
        fputs(aic_style_css_string, f_salida);
        fputs("`;\n"
              "var files={\n"
              "'index.html':`",
              f_salida);
    } /* endif */

    /*** HTML INTRO ***/

    fprintf(f_salida,
            "<html><head><title>riscV-especulacion: %s. Ciclo " F_CICLO "</title>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida, "<link rel='stylesheet' href='aic-style.css' />\n");
    fprintf(f_salida, "</head>\n");
    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (FLAG(depuracion)) {
        fprintf(f_salida, "<a href=\"estado%03d.html\">Estado</a>\n", 1);

        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"crono%03d.html\">Cronograma</a>\n", 1);

        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"predictor%03d.html\">BTB</a>\n", 1);

        if (JERARQUIA_ACTIVA()) {
            fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
            fprintf(f_salida, "<a href=\"memoria%03d.html\">Memoria</a>\n", 1);
        }
    }
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "<br>\n");
    fprintf(f_salida, "<br>\n");

    /*** Datos de la implementación ***/

    imprime_config_html();

    /*** Memorias ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr  valign=\"top\" bgcolor=\"#ddeeff\">\n");
    fprintf(f_salida, "  <td>Memoria&nbsp;de&nbsp;datos</td>\n");
    fprintf(f_salida, "  <td>Memoria&nbsp;de&nbsp;instrucciones</td>\n");
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr valign=\"top\"><td>\n");

    /*** Contenido de la memoria de datos ***/

    imprime_DMem_html();

    fprintf(f_salida, "  </td><td>\n");

    /*** Contenido de la memoria de instrucciones ***/

    imprime_IMem_html();

    fprintf(f_salida, "  </td></tr>\n");
    fprintf(f_salida, "</table><br>\n");

    fprintf(f_salida, "<address>Arquitectura e Ingeniería de Computadores</address>\n");
    fprintf(f_salida, "</body></html>\n");

    if (html_merge == SI) {
        fputs("`,", f_salida);
    } else {
        fclose(f_salida);
    } /* endif */

} /* end imprime_inicio */

/***************************************************************
 *
 * Func: imprime_final
 *
 * Desc: Imprime el cronograma de ejecución
 *
 **************************************************************/

void imprime_final_html() {

    short i;

    char aux[LONG_STR];

    sprintf(aux, "final.html");
    if (html_merge == SI) {
        fprintf(f_salida, "'%s':`", aux);
    } else {
        f_salida = fopen(aux, "w");
        if (f_salida == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */

    /*** HTML INTRO ***/

    fprintf(f_salida,
            "<html><head><title>riscV-especulacion: %s. Ciclo " F_CICLO "</title>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida, "<link rel='stylesheet' href='aic-style.css' />\n");
    fprintf(f_salida, "</head>\n");
    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (FLAG(depuracion)) {
        fprintf(f_salida, "<a href=\"estado%03d.html\">Estado</a>\n", 1);

        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"crono%03d.html\">Cronograma</a>\n", 1);

        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"predictor%03d.html\">BTB</a>\n", 1);

        if (JERARQUIA_ACTIVA()) {
            fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
            fprintf(f_salida, "<a href=\"memoria%03d.html\">Memoria</a>\n", 1);
        }
    }
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "<br>\n");
    fprintf(f_salida, "<br>\n");

    /*** Resultados de la ejecucion ***/

    fprintf(f_salida, "<table>\n");
    fprintf(f_salida, "<tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td>Ciclos</td>\n");
    fprintf(f_salida, "  <td>Instrucciones</td>\n");
    fprintf(f_salida, "  <td>CPI / IPC</td>\n");
    fprintf(f_salida, "  <td>Op. CF</td>\n");
    fprintf(f_salida, "  <td>Op. CF/ciclo</td>\n");
    fprintf(f_salida, "  <td>Saltos buscados</td>\n");
    fprintf(f_salida, "  <td>Saltos en BTB</td>\n");
    fprintf(f_salida, "  <td>Saltos ejecutados (C)</td>\n");
    fprintf(f_salida, "  <td>Saltos acertados</td>\n");

    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "<tr bgcolor=\"#eeeeee\">\n");
    fprintf(f_salida, "  <td>%ld</td>\n", estat.ciclos);
    fprintf(f_salida, "  <td>%ld</td>\n", estat.instrucciones);
    fprintf(f_salida, "  <td>%1.2f / %1.2f</td>\n", (float) estat.ciclos / estat.instrucciones,
            (float) estat.instrucciones / estat.ciclos);
    fprintf(f_salida, "  <td>%ld</td>\n", estat.flops);
    fprintf(f_salida, "  <td>%1.2f</td>\n", (float) estat.flops / estat.ciclos);
    fprintf(f_salida, "  <td>%ld </td>\n", estat.saltos_accedidos);
    fprintf(f_salida, "  <td>%ld (%2.1f%%)</td>\n", estat.saltos_encontrados,
            (float) (estat.saltos_encontrados * 100) / estat.saltos_accedidos);
    fprintf(f_salida, "  <td>%ld (%2.1f%%)</td>\n", estat.saltos_ejecutados,
            (float) (estat.saltos_ejecutados * 100) / estat.instrucciones);
    fprintf(f_salida, "  <td>%ld (%2.1f%%)</td>\n", estat.saltos_acertados,
            (float) (estat.saltos_acertados * 100) / estat.saltos_ejecutados);
    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "</table><br>\n");

    /*** Resultados de la ejecución: Ocupación ***/

    fprintf(f_salida, "<table>\n");
    fprintf(f_salida, "<tr bgcolor=\"#ccddff\">\n");

    fprintf(f_salida, "  <td colspan=\"11\" align=\"center\">Ocupación de recursos </td>\n");
    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "<tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td>RS Ent.</td>\n");
    fprintf(f_salida, "  <td>RS S/R</td>\n");
    fprintf(f_salida, "  <td>RS M/D</td>\n");
    fprintf(f_salida, "  <td>RS Carga</td>\n");
    fprintf(f_salida, "  <td>RS Almac.</td>\n");
    fprintf(f_salida, "  <td>Op Ent.</td>\n");
    fprintf(f_salida, "  <td>Op S/R</td>\n");
    fprintf(f_salida, "  <td>Op M/D</td>\n");
    fprintf(f_salida, "  <td>Op AC</td>\n");
    fprintf(f_salida, "  <td>Op Mem.</td>\n");
    fprintf(f_salida, "  <td>RB</td>\n");
    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "<tr bgcolor=\"#eeeeee\">\n");

    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.RS_ENTEROS,
            (max_util.RS_ENTEROS * 100 / CONFIG(tam_rs.enteros)));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.RS_SUMREST,
            (max_util.RS_SUMREST * 100 / CONFIG(tam_rs.sumrest)));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.RS_MULTDIV,
            (max_util.RS_MULTDIV * 100 / CONFIG(tam_rs.multdiv)));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.BUFFER_CARGA,
            (max_util.BUFFER_CARGA * 100 / CONFIG(tam_rs.carga)));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.BUFFER_ALMACEN,
            (max_util.BUFFER_ALMACEN * 100 / CONFIG(tam_rs.almacen)));

    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.OP_ENTEROS,
            (max_util.OP_ENTEROS * 100 / CONFIG(enteros.num_oper)));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.OP_SUMREST,
            (max_util.OP_SUMREST * 100 / CONFIG(sumrest.num_oper)));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.OP_MULTDIV,
            (max_util.OP_MULTDIV * 100 / CONFIG(multdiv.num_oper)));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.OP_DIRECCIONES,
            (max_util.OP_DIRECCIONES * 100 / CONFIG(direcciones.num_oper)));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.OP_MEMDATOS,
            (max_util.OP_MEMDATOS * 100 / CONFIG(memdatos.num_oper)));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.REORDER, (max_util.REORDER * 100 / CONFIG(tam_reorder)));

    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "</table><br>\n");

    /*** Estadísticas de la jerarquía de memoria ***/

    if (JERARQUIA_ACTIVA()) {
        G_STR_DECL(content);

        T_COMPOSITE table, tr;

        table = H_ATTR_COMPOSITE(table, "class='stats'");

        tr = H_ADD_COMPOSITE(table, tr);
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "rowspan=2", "Nivel");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "colspan=4", "Lecturas");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "colspan=4", "Escrituras");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "colspan=4", "Totales");

        tr = H_ADD_COMPOSITE(table, tr);
        H_ADD_SINGLE_TEXT(tr, th, "Accesos");
        H_ADD_SINGLE_TEXT(tr, th, "Aciertos");
        H_ADD_SINGLE_TEXT(tr, th, "Tasa Aciertos");
        H_ADD_SINGLE_TEXT(tr, th, "Tiempo Acceso");

        H_ADD_SINGLE_TEXT(tr, th, "Accesos");
        H_ADD_SINGLE_TEXT(tr, th, "Aciertos");
        H_ADD_SINGLE_TEXT(tr, th, "Tasa Aciertos");
        H_ADD_SINGLE_TEXT(tr, th, "Tiempo Acceso");

        H_ADD_SINGLE_TEXT(tr, th, "Accesos");
        H_ADD_SINGLE_TEXT(tr, th, "Aciertos");
        H_ADD_SINGLE_TEXT(tr, th, "Tasa Aciertos");
        H_ADD_SINGLE_TEXT(tr, th, "Tiempo Acceso");

        for (i = NUM_ELEM_MEMORIA - 1; i >= 0; i--) {
            if (!elementos_memoria[i]->activa) continue;

            muestra_estadisticas_nivel_html(elementos_memoria[i], table);
        }

        h_tag_flush(PTR_BASE(table), &content);
        g_str_flush(f_salida, &content);

        h_destroy_tag(PTR_BASE(table));
        fprintf(f_salida, "<br>\n");
    }

    /*** Datos de la implementación ***/

    imprime_config_html();

    /*** Memorias ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr  valign=\"top\" bgcolor=\"#ddeeff\">\n");
    fprintf(f_salida, "  <td>Memoria&nbsp;de&nbsp;datos</td>\n");
    fprintf(f_salida, "  <td>Memoria&nbsp;de&nbsp;instrucciones</td>\n");
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr valign=\"top\"><td>\n");

    /*** Contenido de la memoria de datos ***/

    imprime_DMem_html();

    fprintf(f_salida, "  </td><td>\n");

    /*** Contenido de la memoria de instrucciones ***/

    imprime_IMem_html();

    fprintf(f_salida, "  </td></tr>\n");
    fprintf(f_salida, "</table><br>\n");

    fprintf(f_salida, "<address>Arquitectura e Ingeniería de Computadores</address>\n");
    fprintf(f_salida, "</body></html>\n");

    if (html_merge == SI) {
        fputs("`};\n"
              "\n"
              "function get_iframedoc()\n"
              "{\n"
              "   var iframe = document.getElementById('iframe');\n"
              "   var iframedoc = iframe.document;\n"
              "      if (iframe.contentDocument)\n"
              "      {\n"
              "         iframedoc = iframe.contentDocument;\n"
              "      }\n"
              "      else if (iframe.contentWindow)\n"
              "      {\n"
              "         iframedoc = iframe.contentWindow.document;\n"
              "      }\n"
              "   return iframedoc;\n"
              "}\n"
              "\n"
              "//\n"
              "// loads file on iframe\n"
              "//\n"
              "function load_file(file)\n"
              "{\n"
              "   if (!files[file])\n"
              "   {\n"
              "      return;\n"
              "   }\n"
              "\n"
              "   var iframedoc = get_iframedoc();\n"
              "\n"
              "   if (iframedoc)\n"
              "   {\n"
              "      iframedoc.open();\n"
              "      //iframedoc.writeln(files[file]);\n"
              "      iframedoc.writeln(files[file].replace('<html>','<html><style>'+css_file+'</style>'));\n"
              "      iframedoc.close();\n"
              "   }\n"
              "   else\n"
              "   {\n"
              "      alert('Cannot inject dynamic contents into iframe.');\n"
              "   }\n"
              "}\n"
              "\n"
              "function check_anchor_click(event)\n"
              "{\n"
              "   if (event.target.tagName !== 'A')\n"
              "   {\n"
              "      return;\n"
              "   }\n"
              "   event.preventDefault();\n"
              "\n"
              "   var iframedoc = get_iframedoc();\n"
              "\n"
              "   iframedoc.removeEventListener('click', check_anchor_click);\n"
              "   load_file(event.target.href.split('/').pop());\n"
              "   iframedoc.addEventListener('click', check_anchor_click);\n"
              "}\n"
              "\n"
              "function init_page()\n"
              "{\n"
              "   var iframedoc = get_iframedoc();\n"
              "\n"
              "   document.title = iframedoc.title;\n"
              "   iframedoc.addEventListener('click', check_anchor_click);\n"
              "}\n"
              "\n"
              "</script>\n"
              "</head>\n"
              "<!--\n"
              "     loads 'index.html' on first load\n"
              "-->\n"
              "<body onLoad='load_file(\"index.html\")'>\n"
              "\n"
              "<div id=\"content\">\n"
              "   <iframe id=\"iframe\" width=\"100%\" height=\"100%\" frameborder=\"0\" src=\"about:blank\" onLoad=\"init_page()\" />\n"
              "</div>\n"
              "\n"
              "</body>\n"
              "</html>\n",
              f_salida);
    } /* endif */

    fclose(f_salida);

} /* end imprime_final */

static void imprime_Registros_html() {

    int i;
    boolean dirty;
    valor_t valor_int_reg, valor_fp_reg;
    tipo_t tipo_dato;

    nueva_firma(); /*** FIRMAS ***/

    fprintf(f_salida, "<table>\n");

    /* Banco de registros de coma flotante */

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        fprintf(f_salida, "  <td width=\"75\" align=\"center\">f%d<br>(%s)</td>\n", i, nombre_fp_reg[i]);
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"#ddeeff\">rob</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        if (Rfp[i].rob != MARCA_NULA) {
            FIRMA_2_2("F%d.rob", i, "#&nbsp;%d", Rfp[i].rob);
            fprintf(f_salida,
                    "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);
        } else {
            FIRMA_2_1("F%d.rob", i, "&nbsp;");
            fprintf(f_salida, "  <td>%s</td>\n", vstr);
        } /* endif */
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"#ddeeff\">valor</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        valor_fp_reg = lee_fp_reg(i, &tipo_dato, &dirty);
        FIRMA_valor("F%d.valor", i, valor_fp_reg, tipo_dato);
        if (Rfp[i].rob != MARCA_NULA) {
            fprintf(f_salida,
                    "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);
        } else {
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    /* Banco de registros de enteros */

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        fprintf(f_salida, "  <td width=\"75\" align=\"center\">x%d<br>(%s)</td>\n", i, nombre_int_reg[i]);
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"#ddeeff\">rob</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        if (Rint[i].rob != MARCA_NULA) {
            FIRMA_2_2("x%d.rob", i, "#&nbsp;%d", Rint[i].rob);
            fprintf(f_salida,
                    "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);
        } else {
            FIRMA_2_1("x%d.rob", i, "&nbsp;");
            fprintf(f_salida, "  <td>%s</td>\n", vstr);
        } /* endif */
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"#ddeeff\">valor</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        valor_int_reg = lee_int_reg(i, &tipo_dato, &dirty);

        FIRMA_valor("R%d.valor", i, valor_int_reg, tipo_dato);
        if (Rint[i].rob != MARCA_NULA) {
            fprintf(f_salida,
                    "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);
        } else {
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, "</table><br>\n");
}

static void imprime_ROB_html() {

    int i;

    inst_types_t inst_type;

    char aux[LONG_STR];
    char aux2[LONG_STR * 2];
    char *clase;

    nueva_firma(); /*** FIRMAS ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"25\">Cod</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"40\">Ocup.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"125\">Instrucción</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"50\">Complet.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"100\">Destino</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"75\">Valor</td>\n");
    //fprintf(f_salida, "  <td align=\"center\" width=\"75\">Cond.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"75\">Pred.</td>\n");
    //fprintf(f_salida, "  <td align=\"center\" width=\"75\">Direc.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Exc.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">PC</td>\n");
    fprintf(f_salida, " </tr>\n");

    for (i = 0; i < CONFIG(tam_reorder); i++) {
        if (!RB[i].ocupado && RB[i].orden > 0) {
            clase = "old";
        } else {
            clase = "normal";
        }

        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\" class=\"%s\">\n", clase);

        if (i == RB_inicio)
            fprintf(f_salida,
                    "  <td bgcolor=\"#ffddaa\" align=\"center\">%d</td>\n", i);
        else
            fprintf(f_salida,
                    "  <td bgcolor=\"#ddeeff\" align=\"center\">%d</td>\n", i);

        if (RB[i].ocupado || (!RB[i].ocupado && RB[i].orden > 0)) {
            inst_type = TIPOS(RB[i].OP);

            FIRMA_2_1("ROB%d.ocupado", i, (RB[i].ocupado) ? "SI" : "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            imprime_instruccion_color(aux, RB[i].PC, SI);
            FIRMA_2_1("ROB%d.inst", i, aux);
            fprintf(f_salida, "  <td align=\"left\" nowrap=\"nowrap\">%s</td>\n", vstr);

            FIRMA_2_1("ROB%d.estado", i, (RB[i].completado) ? "SI" : "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


            if (inst_store(RB[i].OP)) {
                imprime_codigo(RB[i].dest, aux);
                FIRMA_2_1("ROB%d.dest", i, aux);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                FIRMA_2_1("ROB%d.valor", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("ROB%d.condicion", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("ROB%d.pred", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                // FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
                // fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            } else if (inst_salto(RB[i].OP)) {
                // Dest
                if inst_jal_jalr(RB[i].OP) {
                    // SI es jal*, ponemos "X,direccion"
                    if (RB[i].completado) {
                        te_etiqueta(aux, RB[i].direccion);
                        sprintf(aux2, "x"
                                      F_DWORD
                                      ",%s", RB[i].dest, aux);
                    } else {
                        sprintf(aux2, "x"
                                      F_DWORD, RB[i].dest);
                    }
                    FIRMA_2_1("ROB%d.dir", i, aux2);
                    //FIRMA_2_2("ROB%d.dest", i, F_DWORD, 31);
                    //fprintf(f_salida, "  <td align=\"center\">%s%s</td>\n", (inst_int(RB[i].OP)) ? "R" : "F", vstr);
                } else if inst_jalr(RB[i].OP) {
                    if (RB[i].completado) {
                        te_etiqueta(aux, RB[i].direccion);
                        FIRMA_2_1("ROB%d.dir", i, aux);
                    } else {
                        FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
                    }
                } else { // Resto de saltos
                    te_etiqueta(aux, RB[i].direccion);
                    FIRMA_2_1("ROB%d.dir", i, aux);
                }
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                // [Valor,condicion] prediccion
                // Saltos condicionales
                if (RB[i].completado) {
                    FIRMA_2_1("ROB%d.cond", i, (RB[i].condicion) ? "Salta" : "No salta");
                } else {
                    FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
                }
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);
                FIRMA_2_1("ROB%d.pred", i, (RB[i].prediccion) ? "Salta" : "No salta");
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);
            } else if (RB[i].OP != RV32I_ECALL) {
                //FIRMA_2_2("ROB%d.dest", i, F_DWORD, RB[i].dest);
                if inst_to_Rint(RB[i].OP) {
                    sprintf(aux, "x" F_DWORD " (%s)", RB[i].dest, nombre_int_reg[RB[i].dest]);
                    FIRMA_2_2("ROB%d.dest", i, "%s", aux);
                } else {
                    sprintf(aux, "f" F_DWORD " (%s)", RB[i].dest, nombre_fp_reg[RB[i].dest]);
                    FIRMA_2_2("ROB%d.dest", i, "%s", aux);
                }
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                //fprintf(f_salida, "  <td align=\"center\">%s%s</td>\n", (inst_to_Rint(RB[i].OP)) ? "x" : "f", vstr);

                if (RB[i].completado) {
                    FIRMA_valor("ROB%d.valor", i, RB[i].valor, inst_type.result);
                } else {
                    FIRMA_2_1("ROB%d.valor", i, "&nbsp");
                }
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                //FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("ROB%d.pred", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else {
                FIRMA_2_1("ROB%d.dest", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("ROB%d.valor", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("ROB%d.pred", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            if (RB[i].excepcion != EXC_NONE)
                FIRMA_2_2("ROB%d.exc", i, "%d", RB[i].excepcion);
            else
                FIRMA_2_1("ROB%d.exc", i, "&nbsp;");

            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            te_etiqueta(aux, RB[i].PC);
            FIRMA_2_1("ROB%d.pc", i, aux);
            fprintf(f_salida, "  <td align=\"left\" nowrap='nowrap'>%s</td>\n", vstr);
        } else {

            FIRMA_2_1("ROB%d.ocupado", i, "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.inst", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.estado", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.dest", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.valor", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            //FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
            //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.pred", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            //FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
            //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.exc", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.pc", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */

        fprintf(f_salida, " </tr>\n");
    } /* endfor */

    fprintf(f_salida, "</table>\n");

}

static void imprime_RS(int inicio_RS, int fin_RS) {

    marca_t s;
    inst_types_t inst_type;

    char aux[LONG_STR];
    char *clase;

    int BL, BE;
    int op1, op2, op3;

    BL = (inicio_RS == INICIO_BUFFER_CARGA);
    BE = (inicio_RS == INICIO_BUFFER_ALMACEN);
    op1 = 1;
    op2 = (inicio_RS != INICIO_BUFFER_CARGA);
    op3 = (inicio_RS == INICIO_RS_MULTDIV);

    nueva_firma(); /*** FIRMAS ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"25\">Cod</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"40\">Busy</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Op</td>\n");
    if (op1) {
        fprintf(f_salida, "  <td align=\"center\" width=\"25\">Q1</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"75\">V1</td>\n");
    }
    if (op2) {
        fprintf(f_salida, "  <td align=\"center\" width=\"25\">Q2</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"75\">V2</td>\n");
    }
    if (op3) {
        fprintf(f_salida, "  <td align=\"center\" width=\"25\">Q3</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"75\">V3</td>\n");
    }
    if (BL | BE) {
        fprintf(f_salida, "  <td align=\"center\" width=\"50\">Desp(etiq)</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"50\">Dirección</td>\n");
    }
    if (BE) {
        fprintf(f_salida, "  <td align=\"center\" width=\"50\">Conf</td>\n");
    } else {
        fprintf(f_salida, "  <td align=\"center\" width=\"25\">rob</td>\n");
        fprintf(f_salida, "  <td align=\"center\">Resultado</td>\n");
    }
    if (!BL && !BE) {
        fprintf(f_salida, "  <td align=\"center\">Estado</td>\n");
    }
    fprintf(f_salida, " </tr>\n");

    for (s = inicio_RS;
         s <= fin_RS; s++) {

        if (!RS[s].ocupado && RS[s].orden > 0) {
            clase = "old";
        } else {
            clase = "normal";
        }

        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\" class=\"%s\">\n", clase);

        imprime_codigo(s, aux);
        FIRMA_2_1("RS%d.cod", s, aux);
        fprintf(f_salida, "  <td bgcolor=\"#ddeeff\" align=\"center\">%s</td>\n", vstr);

        if (RS[s].ocupado || (!RS[s].ocupado && RS[s].orden > 0)) {
            inst_type = TIPOS(RS[s].OP);

            FIRMA_2_1("RS%d.ocupado", s, (RS[s].ocupado) ? "SI" : "NO");
            fprintf(f_salida, "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);
            //printf("Debug presentacion: Ciclo %d Estacion %d Ocupado %s Orden %d Op %d\n", Ciclo, s, vstr, RS[s].orden, RS[s].OP);

            imprime_operacion(RS[s].OP, aux);
            FIRMA_2_1("RS%d.op", s, aux);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            if (op1) {
                if (RS[s].Q1 != MARCA_NULA) {
                    FIRMA_2_2("RS%d.Q1", s, "#&nbsp;%d", RS[s].Q1);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_1("RS%d.V1", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);
                } else {
                    FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);
                    FIRMA_valor("RS%d.V1", s, RS[s].V1, inst_type.Rfte1);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                } /* endif */
            }
            if (op2) {
                if (RS[s].Q2 != MARCA_NULA) {
                    FIRMA_2_2("RS%d.Q2", s, "#&nbsp;%d", RS[s].Q2);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_1("RS%d.V2", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);
                } else {
                    FIRMA_2_1("RS%d.Q2", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);
                    FIRMA_valor("RS%d.V2", s, RS[s].V2, inst_type.Rfte2);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                } /* endif */
            }
            if (op3) {
                if (RS[s].Q3 != MARCA_NULA) {
                    FIRMA_2_2("RS%d.Q3", s, "#&nbsp;%d", RS[s].Q3);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_1("RS%d.V3", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);
                } else {
                    FIRMA_2_1("RS%d.Q3", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);

                    FIRMA_valor("RS%d.V3", s, RS[s].V3, inst_type.Rfte3);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                } /* endif */
            }
            if (BL | BE) {
                if (strlen(RS[s].etiqueta)) {
                    FIRMA_2_3("RS%d.desp", s, F_DWORD " [%s]", RS[s].desplazamiento, RS[s].etiqueta);
                } else {
                    FIRMA_2_2("RS%d.desp", s, F_DWORD, RS[s].desplazamiento);
                }
                fprintf(f_salida, "  <td align='center' nowrap='nowrap'>%s</td>\n", vstr);

                if (RS[s].estado != PENDIENTE) {
                    te_etiqueta_desp(aux, RS[s].direccion);
                    FIRMA_2_1("RS%d.dir", s, aux);

                    if (RS[s].excepcion == EXC_NONE || RS[s].ocupado == NO) {
                        fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%ld [%s]</td>\n",
                                RS[s].direccion, vstr);
                    } else {
                        fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap' style='color:red'>%ld [%s]</td>\n",
                                RS[s].direccion, vstr);
                    }
                } else {
                    FIRMA_2_1("RS%d.dir", s, "&nbsp;");
                    if (RS[s].excepcion == EXC_NONE || RS[s].ocupado == NO) {
                        fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);
                    } else {
                        fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap' style='color:red'>%s</td>\n",
                                vstr);
                    }
                }
            }
            if (BE) {
                FIRMA_2_2("RS%d.conf", s, "%s", (RS[s].confirm) ? "SI" : "NO");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else {
                FIRMA_2_2("RS%d.rob", s, "#&nbsp;%d", RS[s].rob);
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
            }

            switch (RS[s].estado) {
                case EN_CURSO:
                    if (!BE) {
                        FIRMA_2_1("RS%d.result", s, "&nbsp;");
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    }
                    if (!BL && !BE) {
                        FIRMA_2_3("RS%d.est", s, F_ESTADO, Op[RS[s].operador].ciclo, Op[RS[s].operador].Teval);
                    } else {
                        FIRMA_2_1("RS%d.est", s, "&nbsp;");
                    }
                    if (!BL && !BE) {
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    }
                    break;
                case FINALIZADA:
                    if (inst_branch(RS[s].OP)) {
                        FIRMA_2_1("RS%d.result", s, (RS[s].condicion) ? "sí" : "no");
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    } else if (!BE) {
                        FIRMA_valor("RS%d.result", s, RS[s].resultado, inst_type.result);
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    }
                    FIRMA_2_1("RS%d.est", s, "end");
                    if (!BL && !BE) {
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    }
                    break;
                default:
                    if (!BE) {
                        FIRMA_2_1("RS%d.result", s, "&nbsp;");
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    }
                    FIRMA_2_1("RS%d.est", s, "&nbsp;");
                    if (!BL && !BE) {
                        fprintf(f_salida, "  <td>%s</td>\n", vstr);
                    }
            }
        } else {
            FIRMA_2_1("RS%d.ocupado", s, "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.op", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            if (op1) {
                FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V1", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            if (op2) {
                FIRMA_2_1("RS%d.Q2", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V2", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            if (op3) {
                FIRMA_2_1("RS%d.Q3", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V3", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            if (BL | BE) {
                FIRMA_2_1("RS%d.desp", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.dir", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            if (BE) {
                FIRMA_2_1("RS%d.conf", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else {
                FIRMA_2_1("RS%d.rob", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.result", s, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            FIRMA_2_1("RS%d.est", s, "&nbsp;");
            if (!BL && !BE) {
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
        } /* endif */

        fprintf(f_salida, " </tr>\n");

    } /* endfor */

    fprintf(f_salida,
            "</table>\n");
}

static void imprime_RS_enteros_html() {

    imprime_RS(INICIO_RS_ENTEROS, FIN_RS_ENTEROS);

}

static void imprime_RS_Sum_FP_html() {

    imprime_RS(INICIO_RS_SUMREST, FIN_RS_SUMREST);

}

static void imprime_RS_Mul_FP_html() {

    imprime_RS(INICIO_RS_MULTDIV, FIN_RS_MULTDIV);

}

static void imprime_RS_FP_html() {

    imprime_RS_Sum_FP_html();
    imprime_RS_Mul_FP_html();
}

static void imprime_LB_html() {

    imprime_RS(INICIO_BUFFER_CARGA, FIN_BUFFER_CARGA);

}

static void imprime_SB_html() {

    imprime_RS(INICIO_BUFFER_ALMACEN, FIN_BUFFER_ALMACEN);

}

/***************************************************************
 *
 * Func: imprime_estado
 *
 * Desc: Imprime el estado de las estructuras de coma flotante
 *
 **************************************************************/

void imprime_estado_html() {

    char aux[LONG_STR];

    sprintf(aux, "estado" F_LCICLO ".html", Ciclo);
    if (html_merge == SI) {
        fprintf(f_salida, "'%s':`", aux);
    } else {
        f_salida = fopen(aux, "w");
        if (f_salida == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */

    //nueva_firma(); /*** FIRMAS ***/

    /*** HTML INTRO ***/

    fprintf(f_salida,
            "<html><head><title>Estado: %s. Ciclo " F_CICLO "</title>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida, "<link rel='stylesheet' href='aic-style.css' />\n");
    fprintf(f_salida, "</head>\n");
    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (Ciclo > 10)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[-10]</a>&nbsp;\n",
                Ciclo - 10);
    else
        fprintf(f_salida, "[-10]&nbsp;\n");
    if (Ciclo > 5)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[-5]</a>&nbsp;\n",
                Ciclo - 5);
    else
        fprintf(f_salida, "[-5]&nbsp;\n");
    if (Ciclo > 1)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[-1]</a>&nbsp;\n",
                Ciclo - 1);
    else
        fprintf(f_salida, "[-1]\n");

    if (!final)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[+1]</a>\n", Ciclo + 1);
    else
        fprintf(f_salida, "[+1]&nbsp;\n");
    if (!final)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[+5]</a>\n", Ciclo + 5);
    else
        fprintf(f_salida, "[+5]&nbsp;\n");
    if (!final)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[+10]</a>\n", Ciclo + 10);
    else
        fprintf(f_salida, "[+10]&nbsp;\n");

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<!a href=\"estado" F_LCICLO ".html\">Estado<!/a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">Crono</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">BTB</a>\n", Ciclo);

    if (JERARQUIA_ACTIVA()) {
        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"memoria" F_LCICLO ".html\">Memoria</a>\n", Ciclo);
    }

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<b>Ciclo: " F_CICLO "</b>\n", Ciclo);
    fprintf(f_salida, "<br>\n");

    fprintf(f_salida, "<p>Estado al final del ciclo\n");


    /*** ORGANIZACIÖN EN DOS COLUMNAS  */
    fprintf(f_salida, "<table>\n");
    fprintf(f_salida, "<tr valign=\"top\">\n");
    fprintf(f_salida, "<td>\n");

    /*** Bancos de registros ***/

    imprime_Registros_html();

    fprintf(f_salida, "<table bgcolor=\"white\">\n");
    fprintf(f_salida, " <tr valign=\"top\"><td>\n");

    /*** Reorder buffer ***/
    imprime_ROB_html();

    fprintf(f_salida, "  </td><td>\n");

    /*** Estaciones de reserva ***/

    /* Contenido de las estaciones de reserva */

    imprime_RS_enteros_html();
    imprime_RS_FP_html();


    fprintf(f_salida, "  </td></tr>\n");
    fprintf(f_salida, "</table><br>\n");

    fprintf(f_salida, "<table bgcolor=\"white\">\n");
    fprintf(f_salida, " <tr valign=\"top\"><td>\n");

    /*** Buffer de carga y almacenamiento ***/

    imprime_LB_html();

    fprintf(f_salida, "  </td>\n");
    fprintf(f_salida, "  <td>\n");

    imprime_SB_html();

    fprintf(f_salida, "  </td></tr>\n");
    fprintf(f_salida, "</table>\n");


    /*** SEGUNDA COLUMNA **********************************************/

    fprintf(f_salida, "</td>\n");
    fprintf(f_salida, "<td>\n");

    /*** Contenido de la memoria de datos ***/

    imprime_DMem_html();

    fprintf(f_salida, "</td>\n");
    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "</table>\n");

    fprintf(f_salida, "<address>Arquitectura e Ingeniería de Computadores</address>\n");
    fprintf(f_salida, "</body></html>\n");

    if (html_merge == SI) {
        fputs("`,", f_salida);
    } else {
        fclose(f_salida);
    } /* endif */

    if (error_firma) {

        fprintf(stderr, "ERROR. "
                        "Existen diferencias entre la ejecución actual y el fichero de firmas.\n"
                        "Analice el fichero de estado 'estado" F_LCICLO ".html'\n", Ciclo);
        exit(1);
    } /* endif */

} /* end imprime_estado */

/***************************************************************
 *
 * Func: imprime_crono
 *
 * Desc: Imprime el cronograma de ejecución
 *
 **************************************************************/

void imprime_crono_html() {

    char aux[LONG_STR];

    short x, y;
    char *color;

    sprintf(aux, "crono" F_LCICLO ".html", Ciclo);
    if (html_merge == SI) {
        fprintf(f_salida, "'%s':`", aux);
    } else {
        f_salida = fopen(aux, "w");
        if (f_salida == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */

    /*** HTML INTRO ***/

    fprintf(f_salida, "<html>\n");
    fprintf(f_salida, "<head>\n");
    fprintf(f_salida, "<title>Cronograma: %s. Ciclo " F_CICLO "</title>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida, "<link rel='stylesheet' href='aic-style.css' />\n");
    fprintf(f_salida, "</head>\n");

    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<div style='position:sticky; top:0; background-color:white; opacity:1; '>");
    fprintf(f_salida, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (Ciclo > 10)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[-10]</a>&nbsp;\n",
                Ciclo - 10);
    else
        fprintf(f_salida, "[-10]&nbsp;\n");
    if (Ciclo > 5)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[-5]</a>&nbsp;\n",
                Ciclo - 5);
    else
        fprintf(f_salida, "[-5]&nbsp;\n");
    if (Ciclo > 1)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[-1]</a>&nbsp;\n",
                Ciclo - 1);
    else
        fprintf(f_salida, "[-1]\n");

    if (!final)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[+1]</a>\n", Ciclo + 1);
    else
        fprintf(f_salida, "[+1]&nbsp;\n");
    if (!final)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[+5]</a>\n", Ciclo + 5);
    else
        fprintf(f_salida, "[+5]&nbsp;\n");
    if (!final)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[+10]</a>\n", Ciclo + 10);
    else
        fprintf(f_salida, "[+10]&nbsp;\n");
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">Estado</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<!a href=\"crono" F_LCICLO ".html\">Crono<!/a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">BTB</a>\n", Ciclo);

    if (JERARQUIA_ACTIVA()) {
        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"memoria" F_LCICLO ".html\">Memoria</a>\n", Ciclo);
    }

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<b>Ciclo: " F_CICLO "</b>\n", Ciclo);
    fprintf(f_salida, "</div>");
    fprintf(f_salida, "<br>\n");

    fprintf(f_salida, "<p>Estado al final del ciclo\n");

    /*** Cronograma de ejecución ***/

    fprintf(f_salida, "<style>th.sticky{position: sticky; top: 20; z-index: 10; background-color: #ccddff;}</style>\n");

    fprintf(f_salida, "<table class='crono'><thead>\n");
    fprintf(f_salida, "  <!-- T-Cronograma -->\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\" align=\"center\">\n");
    fprintf(f_salida, "  <th width=\"20\" class=\"sticky\">PC</th>\n");
    fprintf(f_salida, "  <th width=\"150\" class=\"sticky\">Instruc.</th>\n");
    fprintf(f_salida, "  <!-- T-Linea -->\n");
    fprintf(f_salida, "  <!-- T-PC %s -->\n", "PC");
    fprintf(f_salida, "  <!-- T-Instr %s -->\n", "Instruccion");
    for (x = 0; x < MAX(MAX_CICLOS_INIC, maxima_columna_visible); x++) {
        fprintf(f_salida,
                "  <th width=\"25\" class=\"sticky\"><a href=\"crono" F_LCICLO ".html\">" F_CICLO "</a></th>\n",
                x + primer_ciclo, x + primer_ciclo);
        fprintf(f_salida, "  <!-- T-Ciclo-H " F_CICLO " " F_CICLO " -->\n", x + primer_ciclo, x + primer_ciclo);
    }
    fprintf(f_salida, " </tr></thead><tbody>\n");
    fprintf(f_salida, "  <!-- T-Fin-Linea -->\n");

    /* Busca la primera línea del crono con una etapa activa */
    for (y = 0; y < MAX_INSTRUC; y++)
        for (x = 0; x < MAX(MAX_CICLOS_INIC, maxima_columna_visible); x++)
            if (strlen(crono[y].fase[x]))
                goto etapa_encontrada;
    etapa_encontrada:

    for (; y < MAX_INSTRUC; y++) {
        if (y % 2 == 0)
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\" align=\"center\">\n");
        else
            fprintf(f_salida, " <tr bgcolor=\"#dddddd\" align=\"center\">\n");
        fprintf(f_salida, "  <!-- T-Linea -->\n");


        color = (y % 2 == 0) ? "#ddeeff" : "#ccddff";
        if (!strlen(crono[y].inst)) {
            // PC
            fprintf(f_salida,
                    "  <td bgcolor=\"%s\">&nbsp;</td>\n", color);
            // Instruc
            fprintf(f_salida,
                    "  <td bgcolor=\"%s\">&nbsp;</td>\n", color);
        } else {
            te_etiqueta(aux, crono[y].PC);

            fprintf(f_salida,
                    "  <td class=\"address\" bgcolor=\"%s\">%s</td>\n",
                    color, aux);
            fprintf(f_salida, "  <!-- T-PC %s -->\n", aux);

            fprintf(f_salida,
                    "  <td nowrap=\"nowrap\" align=\"left\" bgcolor=\"%s\">%s</td>\n",
                    color, crono[y].inst);
            fprintf(f_salida, "  <!-- T-Instr  %s -->\n", crono[y].inst);
        }

        for (x = 0; x < MAX(MAX_CICLOS_INIC, maxima_columna_visible); x++) {
            fprintf(f_salida, "  <td>");

            if (strlen(crono[y].fase[x])) {
                fprintf(f_salida, "%s", crono[y].fase[x]);
                fprintf(f_salida, "  </td>\n");
                fprintf(f_salida, "  <!-- T-Ciclo-D %d %s -->\n", x + 1, crono[y].fase[x]);
            } else {
                fprintf(f_salida, "&nbsp;");
                fprintf(f_salida, "  </td>\n");
                fprintf(f_salida, "  <!-- T-Ciclo-D %d %s -->\n", x + 1, "BLANCO");
            }
        } /* endfor */
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, "  <!-- T-Fin-Linea -->\n");
    } /* endfor */
    fprintf(f_salida, "  <!-- T-Fin-Cronograma -->\n");
    fprintf(f_salida, "</tbody></table>\n");

    fprintf(f_salida, "<address>Arquitectura e Ingeniería de Computadores</address>\n");
    fprintf(f_salida, "</body></html>\n");

    if (html_merge == SI) {
        fputs("`,", f_salida);
    } else {

        fclose(f_salida);
    } /* endif */

} /* end imprime_crono */

/***************************************************************
 *
 * Func: imprime_predictor
 *
 * Desc: Imprime el estado del predictor
 *
 **************************************************************/

void imprime_predictor_html() {

#define IF_INDEX(BHSR) (IF_ISS_1.PC % CONFIG(predic.tam_tabla_##BHSR))
#define C_INDEX(BHSR) (RB[RB_inicio].PC % CONFIG(predic.tam_tabla_##BHSR))
#define IF_PRED(field) (IF_ISS_1.pred_data.field)
#define C_PRED(field)  (RB[RB_inicio].pred_data.field)
#define Salto_en_IF inst_salto(IF_ISS_1.codop)
#define Salto_en_C (inst_salto(RB[RB_inicio].OP) && (RB[RB_inicio].ocupado) && (RB[RB_inicio].completado))

    short i;

    char aux[LONG_STR];

    sprintf(aux, "predictor" F_LCICLO ".html", Ciclo);
    if (html_merge == SI) {
        fprintf(f_salida, "'%s':`", aux);
    } else {
        f_salida = fopen(aux, "w");
        if (f_salida == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */

    nueva_firma(); /*** FIRMAS ***/

    /*** HTML INTRO ***/

    fprintf(f_salida,
            "<html><head><title>Predictor: %s. Ciclo " F_CICLO "</title></head>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (Ciclo > 10)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[-10]</a>&nbsp;\n",
                Ciclo - 10);
    else
        fprintf(f_salida, "[-5]&nbsp;\n");
    if (Ciclo > 5)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[-5]</a>&nbsp;\n",
                Ciclo - 5);
    else
        fprintf(f_salida, "[-5]&nbsp;\n");
    if (Ciclo > 1)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[-1]</a>&nbsp;\n",
                Ciclo - 1);
    else
        fprintf(f_salida, "[-1]\n");

    if (!final)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[+1]</a>\n", Ciclo + 1);
    if (!final)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[+5]</a>\n", Ciclo + 5);
    if (!final)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[+10]</a>\n", Ciclo + 10);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">Estado</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">Crono</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<!a href=\"predictor" F_LCICLO ".html\">BTB<!/a>\n", Ciclo);

    if (JERARQUIA_ACTIVA()) {
        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"memoria" F_LCICLO ".html\">Memoria</a>\n", Ciclo);
    }

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<b>Ciclo: " F_CICLO "</b>\n", Ciclo);
    fprintf(f_salida, "<br>\n");

    fprintf(f_salida, "<p>Estado al final del ciclo\n");

    /*  Con propósitos de depuración. Funciona con 1 vía */
    if (CONFIG(num_vias.issue) == 1) {
        if (inst_salto(IF_ISS_1.codop)) {
            imprime_instruccion_color(aux, IF_ISS_1.PC, NO);
            fprintf(f_salida,
                    "<p>IF: PC= " F_DWORD "; Instr=%s; BHSR1[%d]=%d, pred1=%d, BHSR2[%d]=%d, pred2: %d, selector: %d &rarr; Prediccion: %d \n",
                    IF_ISS_1.PC, aux,
                    (word) IF_INDEX(BHSR1), IF_PRED(BHSR1), IF_PRED(pred1),
                    (word) IF_INDEX(BHSR2), IF_PRED(BHSR2), IF_PRED(pred2),
                    IF_PRED(selector), IF_ISS_1.prediccion);

            fprintf(f_salida, "<p>\n");

        }

        if (inst_salto(RB[RB_inicio].OP) && (RB[RB_inicio].ocupado) && (RB[RB_inicio].completado)) {
            imprime_instruccion_color(aux, RB[RB_inicio].PC, NO);
            //fprintf(f_salida, "<p><p>&rarr;C: PC= " F_DWORD "; Instr=%s\n", RB[RB_inicio].PC, aux);
            //fprintf(f_salida, "<p>BHSR1[%d] original: %d, pred1: %d; ", (word) RB[RB_inicio].PC % TAM_TABLA_BHSR1, RB[RB_inicio].pred_data.BHSR1, RB[RB_inicio].pred_data.pred1);
            //fprintf(f_salida, "BHSR2[%d] original %d, pred2: %d; ", (word) RB[RB_inicio].PC % TAM_TABLA_BHSR2, RB[RB_inicio].pred_data.BHSR2, RB[RB_inicio].pred_data.pred2);
            //fprintf(f_salida, "Selector: %d -> Prediccion: %d; Condicion: %d\n", RB[RB_inicio].pred_data.selector, RB[RB_inicio].prediccion, RB[RB_inicio].condicion);
            fprintf(f_salida, "<p>\n");
            fprintf(f_salida,
                    "<p>&rarr; C: PC="
                    F_DWORD
                    "; Instr=%s  (BHSR1[%d]=%d, BHSR2[%d]=%d, pred1=%d, pred2=%d, selector=%d, Prediccion=%d) &rarr; Condicion=%d; \n",
                    RB[RB_inicio].PC, aux,
                    (word) C_INDEX(BHSR2), C_PRED(BHSR1), C_PRED(pred1),
                    (word) C_INDEX(BHSR2), C_PRED(BHSR2), C_PRED(pred2),
                    C_PRED(selector), RB[RB_inicio].prediccion, RB[RB_inicio].condicion);
            fprintf(f_salida, "<p>\n");
        }
    }

    if ((CONFIG(predic.tipo_predictor) == CLARIVIDENTE) || (CONFIG(predic.tipo_predictor) == BTB_1_BIT) ||
        (CONFIG(predic.tipo_predictor) == BTB_2_BIT_H) || (CONFIG(predic.tipo_predictor) == BTB_2_BIT_S)) {

        /* Branch Target Buffer 1 o 2 bits */

        fprintf(f_salida, "<p>BTB\n");

        /* BTB */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PC (instr.)</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Estado</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Dir. Destino</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">(Ultimo acceso)</td>\n");
        fprintf(f_salida, " </tr>\n");


        for (i = 0; i < CONFIG(predic.tam_buffer_predic); i++) {
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");

            if (BTB[i].orden > 0) { /* Entrada ocupada */
                te_etiqueta(aux, BTB[i].PC);

                FIRMA_2_1("BTB%d.pc", i, aux);
                imprime_instruccion_color(aux, BTB[i].PC, SI);
                fprintf(f_salida, "  <td nowrap='nowrap'>%s (%s)</td>\n", vstr, aux);

                imprime_estado_predictor_html(aux, BTB[i].estado);
                FIRMA_2_1("BTB%d.estado", i, aux);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                te_etiqueta(aux, BTB[i].destino);

                FIRMA_2_1("BTB%d.destino", i, aux);
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_2("BTB%d.orden", i, F_CICLO, BTB[i].orden);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else {
                FIRMA_2_1("BTB%d.pc", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("BTB%d.estado", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                FIRMA_2_1("BTB%d.destino", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("BTB%d.orden", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            fprintf(f_salida, " </tr>\n");
        } /* endfor */

        fprintf(f_salida, "</table>\n");

        /* ======== TWO LEVEL ======= */
    } else if ((CONFIG(predic.tipo_predictor) == TWO_LEVEL_n_BIT_S) ||
               (CONFIG(predic.tipo_predictor) == TWO_LEVEL_n_BIT_H)) {

        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">BHSR1s: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">entry</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">BHSR1</td>\n");
        fprintf(f_salida, " </tr>\n");
        for (i = 0; i < CONFIG(predic.tam_tabla_BHSR1); i++) {
            if ((Salto_en_IF && (i == IF_INDEX(BHSR1))) || (Salto_en_C && (i == C_INDEX(BHSR1))))
                fprintf(f_salida, " <tr bgcolor=\"#ffddaa\">\n");
            else
                fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            fprintf(f_salida, "  <td nowrap='nowrap'>%d</td>\n", i);
            itobin(BHSR1[i], aux, CONFIG(predic.n_bits_BHSR1));
            FIRMA_2_1("BHSR1%d", i, aux);
            //            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s (0x%x)</td>\n", aux, BHSR1[i]);
            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s (%d)</td>\n", aux, BHSR1[i]);
            fprintf(f_salida, " </tr>\n");
        }
        fprintf(f_salida, "</table>\n");

        /* Tablas. BHSR en filas y PHT en columnas */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PHT1: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">BHSR1\\PHT</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            if ((Salto_en_IF && (col == IF_PRED(entrada))) || (Salto_en_C && (col == C_PRED(entrada))))
                fprintf(f_salida, "  <td bgcolor=\"#ffddaa\" width=\"150\">%d</td>\n", col);
            else
                fprintf(f_salida, "  <td width=\"150\">%d</td>\n", col);

        }
        fprintf(f_salida, " </tr>\n");
        for (int fila = 0; fila < pwrtwo(CONFIG(predic.n_bits_BHSR1)); fila++) {

            //if ((Salto_en_IF && (fila == IF_PRED.BHSR1)) || (Salto_en_C && (fila == C_PRED.BHSR1))) 
            //    fprintf(f_salida, " <tr bgcolor=\"#ffddaa\">\n");
            //else 
            //    fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            if ((Salto_en_IF && (fila == IF_PRED(BHSR1))) || (Salto_en_C && (fila == C_PRED(BHSR1))))
                fprintf(f_salida, "  <td bgcolor=\"#ffddaa\" width=\"150\">%d</td>\n", fila);
            else
                fprintf(f_salida, "  <td width=\"150\">%d</td>\n", fila);
            for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
                if (BTB[col].orden > 0) {
                    imprime_estado_predictor_n_html(aux, BTB[col].BHSR1[fila], CONFIG(predic.n_bits_pred_local));
                    FIRMA_2_1("BTB%d.BHSR1", fila, aux);
                } else
                    FIRMA_2_1("BTB%d.BHSR1", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            fprintf(f_salida, " </tr>\n");
        }
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        fprintf(f_salida, "  <td width=\"150\">PC (instr.)</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            if (BTB[col].orden > 0) {
                te_etiqueta(aux, BTB[col].PC);
                FIRMA_2_1("BTB%d.pc", col, aux);
                imprime_instruccion(aux, BTB[col].PC);
                fprintf(f_salida, "  <td align='center' nowrap='nowrap'>%s (%s)</td>\n", vstr, aux);
            } else {
                FIRMA_2_1("BTB%d.pc", col, "&nbsp;");
                fprintf(f_salida, "  <td align='center' nowrap='nowrap'>%s</td>\n", vstr);
            }
        }
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Dir. Destino</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            if (BTB[col].orden > 0) {
                te_etiqueta(aux, BTB[col].destino);
                FIRMA_2_1("BTB%d.destino", col, aux);
            } else
                FIRMA_2_1("BTB%d.destino", col, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);
        }
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">(Ultimo acceso)</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            if (BTB[col].orden > 0) {
                FIRMA_2_2("BTB%d.orden", col, F_CICLO, BTB[col].orden);
            } else
                FIRMA_2_1("BTB%d.orden", col, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        }
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, "</table>\n");

        /* Tablas. PHT en filas y BHSR en columnas */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PHT1: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PHT\\BHSR1</td>\n");
        for (int col = 0; col < pwrtwo(CONFIG(predic.n_bits_BHSR1)); col++) {
            if ((Salto_en_IF && (col == IF_PRED(BHSR1))) || (Salto_en_C && ((col == C_PRED(BHSR1)))))
                fprintf(f_salida, "  <td bgcolor=\"#ffddaa\" width=\"150\">%d</td>\n", col);
            else
                fprintf(f_salida, "  <td bgcolor=\"#ccddff\" width=\"150\">%d</td>\n", col);
        }
        fprintf(f_salida, "  <td width=\"150\">PC (instr.)</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Dir. Destino</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">(Ultimo acceso)</td>\n");
        fprintf(f_salida, " </tr>\n");
        for (int fila = 0; fila < CONFIG(predic.tam_buffer_predic); fila++) {
            //if ((Salto_en_IF && (fila == IF_PRED.entrada)) || (Salto_en_C && (fila == C_PRED.entrada)))
            //    fprintf(f_salida, " <tr bgcolor=\"#ffddaa\" >\n");
            //else
            //    fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            //fprintf(f_salida, "  <td width=\"150\">%d</td>\n", fila);
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            if ((Salto_en_IF && (fila == IF_PRED(entrada))) || (Salto_en_C && (fila == C_PRED(entrada))))
                fprintf(f_salida, "  <td bgcolor=\"#ffddaa\" width=\"150\">%d</td>\n", fila);
            else
                fprintf(f_salida, "  <td width=\"150\">%d</td>\n", fila);
            if (BTB[fila].orden > 0) {
                for (int col = 0; col < pwrtwo(CONFIG(predic.n_bits_BHSR1)); col++) {
                    imprime_estado_predictor_n_html(aux, BTB[fila].BHSR1[col], CONFIG(predic.n_bits_pred_local));
                    FIRMA_2_1("BTB%d.BHSR1", fila, aux);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                }
                te_etiqueta(aux, BTB[fila].PC);
                FIRMA_2_1("BTB%d.pc", fila, aux);
                imprime_instruccion(aux, BTB[fila].PC);
                fprintf(f_salida, "  <td align='center' nowrap='nowrap'>%s (%s)</td>\n", vstr, aux);

                te_etiqueta(aux, BTB[fila].destino);
                FIRMA_2_1("BTB%d.destino", fila, aux);
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_2("BTB%d.orden", fila, F_CICLO, BTB[fila].orden);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                fprintf(f_salida, " </tr>\n");
            } else {
                for (int col = 0; col < pwrtwo(CONFIG(predic.n_bits_BHSR1)); col++) {
                    FIRMA_2_1("PHT1%d.estado", fila, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                }
                FIRMA_2_1("PHT1%d.pc", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                FIRMA_2_1("PHT1%d.destino", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_1("PHT1%d.orden", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                fprintf(f_salida, " </tr>\n");
            }
        }
        fprintf(f_salida, "</table>\n");

        /* ====== HYBRID ====== */
    } else if ((CONFIG(predic.tipo_predictor) == HYBRID_n_BIT_S) || (CONFIG(predic.tipo_predictor) == HYBRID_n_BIT_H)) {
        /* Selector. Se muestra en el predictor híbrido */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">entry</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Selector</td>\n");
        fprintf(f_salida, " </tr>\n");
        for (int fila = 0; fila < CONFIG(predic.tam_buffer_predic); fila++) {
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            fprintf(f_salida, "  <td width=\"150\">%d</td>\n", fila);
            if (BTB[fila].orden > 0) {
                imprime_estado_selector_n_html(aux, BTB[fila].estado_selector);
                FIRMA_2_1("Selector%d", fila, aux);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else {
                FIRMA_2_1("Selector%d", fila, "");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            fprintf(f_salida, " </tr>\n");
        }


        fprintf(f_salida, "</table>\n");

        /* ====== BHSR1 ======= */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">BHSR1s: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">entry</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">BHSR1</td>\n");
        fprintf(f_salida, " </tr>\n");
        for (i = 0; i < CONFIG(predic.tam_tabla_BHSR1); i++) {
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            fprintf(f_salida, "  <td nowrap='nowrap'>%d</td>\n", i);
            imprime_estado_BHSR(aux, BHSR2[i], CONFIG(predic.n_bits_BHSR2));
            //itobin(BHSR2[i], aux, CONFIG(predic.n_bits_BHSR2));
            FIRMA_2_1("BHSR1%d", i, aux);
            //            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s (0x%x)</td>\n", aux, BHSR2[i]);
            //fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s (%d)</td>\n", aux, BHSR2[i]);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            fprintf(f_salida, " </tr>\n");
        }
        fprintf(f_salida, "</table>\n");

        /* Tablas. BHSR en filas y PHT en columnas */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PHT1: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">BHSR\\PHT</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            fprintf(f_salida, "  <td width=\"150\">%d</td>\n", col);
        }
        fprintf(f_salida, " </tr>\n");
        for (int fila = 0; fila < pwrtwo(CONFIG(predic.n_bits_BHSR1)); fila++) {

            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            fprintf(f_salida, "  <td width=\"150\">%d</td>\n", fila);
            for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
                if (BTB[col].orden > 0) {
                    imprime_estado_predictor_n_html(aux, BTB[col].BHSR1[fila], CONFIG(predic.n_bits_pred_local));
                    FIRMA_2_1("BTB%d.BHSR1", fila, aux);
                } else
                    FIRMA_2_1("BTB%d.BHSR1", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            fprintf(f_salida, " </tr>\n");
        }
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        fprintf(f_salida, "  <td width=\"150\">PC (instr.)</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            if (BTB[col].orden > 0) {
                te_etiqueta(aux, BTB[col].PC);
                FIRMA_2_1("BTB%d.pc", col, aux);
                imprime_instruccion(aux, BTB[col].PC);
                fprintf(f_salida, "  <td align='center' nowrap='nowrap'>%s (%s)</td>\n", vstr, aux);
            } else {
                FIRMA_2_1("BTB%d.pc", col, "&nbsp;");
                fprintf(f_salida, "  <td align='center' nowrap='nowrap'>%s</td>\n", vstr);
            }
        }
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Dir. Destino</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            if (BTB[col].orden > 0) {
                te_etiqueta(aux, BTB[col].destino);
                FIRMA_2_1("BTB%d.destino", col, aux);
            } else
                FIRMA_2_1("BTB%d.destino", col, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);
        }
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">(Ultimo acceso)</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            if (BTB[col].orden > 0) {
                FIRMA_2_2("BTB%d.orden", col, F_CICLO, BTB[col].orden);
            } else
                FIRMA_2_1("BTB%d.orden", col, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        }
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, "</table>\n");

        /* Tablas. PHT en filas y BHSR en columnas */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PHT1: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PHT\\BHSR</td>\n");
        for (int col = 0; col < pwrtwo(CONFIG(predic.n_bits_BHSR1)); col++) {
            fprintf(f_salida, "  <td width=\"150\">%d</td>\n", col);
        }
        fprintf(f_salida, "  <td width=\"150\">PC (instr.)</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Dir. Destino</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">(Ultimo acceso)</td>\n");
        fprintf(f_salida, " </tr>\n");
        for (int fila = 0; fila < CONFIG(predic.tam_buffer_predic); fila++) {
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            fprintf(f_salida, "  <td width=\"150\">%d</td>\n", fila);
            if (BTB[fila].orden > 0) {
                for (int col = 0; col < pwrtwo(CONFIG(predic.n_bits_BHSR1)); col++) {
                    imprime_estado_predictor_n_html(aux, BTB[fila].BHSR1[col], CONFIG(predic.n_bits_pred_local));
                    FIRMA_2_1("BTB%d.BHSR1", fila, aux);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                }
                te_etiqueta(aux, BTB[fila].PC);
                FIRMA_2_1("BTB%d.pc", fila, aux);
                imprime_instruccion(aux, BTB[fila].PC);
                fprintf(f_salida, "  <td align='center' nowrap='nowrap'>%s (%s)</td>\n", vstr, aux);

                te_etiqueta(aux, BTB[fila].destino);
                FIRMA_2_1("BTB%d.destino", fila, aux);
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_2("BTB%d.orden", fila, F_CICLO, BTB[fila].orden);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                fprintf(f_salida, " </tr>\n");
            } else {
                for (int col = 0; col < pwrtwo(CONFIG(predic.n_bits_BHSR1)); col++) {
                    FIRMA_2_1("PHT1%d.estado", fila, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                }
                FIRMA_2_1("PHT1%d.pc", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                FIRMA_2_1("PHT1%d.destino", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_1("PHT1%d.orden", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                fprintf(f_salida, " </tr>\n");
            }
        }
        fprintf(f_salida, "</table>\n");


        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">BHSR2s: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">entry</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">BHSR2</td>\n");
        fprintf(f_salida, " </tr>\n");
        for (i = 0; i < CONFIG(predic.tam_tabla_BHSR2); i++) {
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            fprintf(f_salida, "  <td nowrap='nowrap'>%d</td>\n", i);
            imprime_estado_BHSR(aux, BHSR2[i], CONFIG(predic.n_bits_BHSR2));
            //itobin(BHSR2[i], aux, CONFIG(predic.n_bits_BHSR2));
            FIRMA_2_1("BHSR2%d", i, aux);
            //            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s (0x%x)</td>\n", aux, BHSR2[i]);
            //fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s (%d)</td>\n", aux, BHSR2[i]);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            fprintf(f_salida, " </tr>\n");
        }
        fprintf(f_salida, "</table>\n");

        /* Tablas. BHSR en filas y PHT en columnas */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PHT1: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">BHSR2\\PHT</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            fprintf(f_salida, "  <td width=\"150\">%d</td>\n", col);
        }
        fprintf(f_salida, " </tr>\n");
        for (int fila = 0; fila < pwrtwo(CONFIG(predic.n_bits_BHSR2)); fila++) {

            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            fprintf(f_salida, "  <td width=\"150\">%d</td>\n", fila);
            for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
                if (BTB[col].orden > 0) {
                    imprime_estado_predictor_n_html(aux, BTB[col].BHSR2[fila], CONFIG(predic.n_bits_pred_local));
                    FIRMA_2_1("BTB%d.BHSR2", fila, aux);
                } else
                    FIRMA_2_1("BTB%d.BHSR2", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            fprintf(f_salida, " </tr>\n");
        }
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        fprintf(f_salida, "  <td width=\"150\">PC (instr.)</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            if (BTB[col].orden > 0) {
                te_etiqueta(aux, BTB[col].PC);
                FIRMA_2_1("BTB%d.pc", col, aux);
                imprime_instruccion(aux, BTB[col].PC);
                fprintf(f_salida, "  <td align='center' nowrap='nowrap'>%s (%s)</td>\n", vstr, aux);
            } else {
                FIRMA_2_1("BTB%d.pc", col, "&nbsp;");
                fprintf(f_salida, "  <td align='center' nowrap='nowrap'>%s</td>\n", vstr);
            }
        }
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Dir. Destino</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            if (BTB[col].orden > 0) {
                te_etiqueta(aux, BTB[col].destino);
                FIRMA_2_1("BTB%d.destino", col, aux);
            } else
                FIRMA_2_1("BTB%d.destino", col, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);
        }
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">(Ultimo acceso)</td>\n");
        for (int col = 0; col < CONFIG(predic.tam_buffer_predic); col++) {
            if (BTB[col].orden > 0) {
                FIRMA_2_2("BTB%d.orden", col, F_CICLO, BTB[col].orden);
            } else
                FIRMA_2_1("BTB%d.orden", col, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        }
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, "</table>\n");

        /* Tablas. PHT en filas y BHSR en columnas */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PHT1: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PHT\\BHSR2</td>\n");
        for (int col = 0; col < pwrtwo(CONFIG(predic.n_bits_BHSR2)); col++) {
            fprintf(f_salida, "  <td width=\"150\">%d</td>\n", col);
        }
        fprintf(f_salida, "  <td width=\"150\">PC (instr.)</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Dir. Destino</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">(Ultimo acceso)</td>\n");
        fprintf(f_salida, " </tr>\n");
        for (int fila = 0; fila < CONFIG(predic.tam_buffer_predic); fila++) {
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            fprintf(f_salida, "  <td width=\"150\">%d</td>\n", fila);
            if (BTB[fila].orden > 0) {
                for (int col = 0; col < pwrtwo(CONFIG(predic.n_bits_BHSR2)); col++) {
                    imprime_estado_predictor_n_html(aux, BTB[fila].BHSR2[col], CONFIG(predic.n_bits_pred_local));
                    FIRMA_2_1("BTB%d.BHSR2", fila, aux);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                }
                te_etiqueta(aux, BTB[fila].PC);
                FIRMA_2_1("BTB%d.pc", fila, aux);
                imprime_instruccion(aux, BTB[fila].PC);
                fprintf(f_salida, "  <td align='center' nowrap='nowrap'>%s (%s)</td>\n", vstr, aux);

                te_etiqueta(aux, BTB[fila].destino);
                FIRMA_2_1("BTB%d.destino", fila, aux);
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_2("BTB%d.orden", fila, F_CICLO, BTB[fila].orden);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                fprintf(f_salida, " </tr>\n");
            } else {
                for (int col = 0; col < pwrtwo(CONFIG(predic.n_bits_BHSR2)); col++) {
                    FIRMA_2_1("PHT1%d.estado", fila, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                }
                FIRMA_2_1("PHT1%d.pc", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                FIRMA_2_1("PHT1%d.destino", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_1("PHT1%d.orden", fila, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                fprintf(f_salida, " </tr>\n");
            }
        }
        fprintf(f_salida, "</table>\n");

    } else {
        /* Branch Target Buffer n bits */
        fprintf(f_salida, "<p>BTB\n");



        /* BTB */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PC (instr.)</td>\n");


        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Dir. Destino</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">(Ultimo acceso)</td>\n");
        fprintf(f_salida, " </tr>\n");

        for (i = 0; i < CONFIG(predic.tam_buffer_predic); i++) {
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");

            if (BTB[i].orden > 0) { /* Entrada ocupada */
                te_etiqueta(aux, BTB[i].PC);

                FIRMA_2_1("BTB%d.pc", i, aux);
                imprime_instruccion_color(aux, BTB[i].PC, SI);
                fprintf(f_salida, "  <td nowrap='nowrap'>%s (%s)</td>\n", vstr, aux);

                //imprime_estado_predictor(aux, BTB[i].estado);
                //FIRMA_2_1("BTB%d.estado", i, aux);
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);




                te_etiqueta(aux, BTB[i].destino);
                FIRMA_2_1("BTB%d.destino", i, aux);
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_2("BTB%d.orden", i, F_CICLO, BTB[i].orden);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else {
                FIRMA_2_1("BTB%d.pc", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("BTB%d.estado", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);




                FIRMA_2_1("BTB%d.destino", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("BTB%d.orden", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            fprintf(f_salida, " </tr>\n");
        } /* endfor */

        fprintf(f_salida, "</table>\n");

    }

    fprintf(f_salida, "<address>Arquitectura e Ingeniería de Computadores</address>\n");
    fprintf(f_salida, "</body></html>\n");

    if (html_merge == SI) {
        fputs("`,", f_salida);
    } else {
        fclose(f_salida);
    } /* endif */

    if (error_firma) {

        fprintf(stderr, "ERROR. "
                        "Existen diferencias entre la ejecución actual y el fichero de firmas.\n"
                        "Analice el fichero de estado 'estado" F_LCICLO ".html'\n", Ciclo);
        exit(1);
    } /* endif */

} /* end imprime_predictor */
