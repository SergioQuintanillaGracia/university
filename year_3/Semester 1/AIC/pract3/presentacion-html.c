
/**************************************************************************
 *
 * Departamento de Informatica de Sistemas y Computadores (DISCA)
 * Universidad Politecnica de Valencia.
 *
 * Author:	Sergio Sáez (ssaez@disca.upv.es)
 *		Pedro López (plopez@disca.upv.es)
 *
 * File: presentacion-html.c
 *
 * Description: Contiene las funciones necesarias para presentar la
 *    informacion sobre la ejecucion del algoritmo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *************************************************************************/

#define presentacion_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>



/***** Definiciones Externas **********************************/

#include "tipos.h"
#include "main.h"
#include "instrucciones.h"
#include "control.h"

#include "datapath.h"
#include "presentacion-html.h"
#include "depurar.h"
#include "memoria.h"
#include "registros.h"


#ifdef MAX
#undef MAX
#endif
#define MAX(x,y) (((x)>(y)) ? (x) : (y))

#define S_READ "&#8592"
#define S_WRITE "&#8594"
#define S_READWRITE "&#8646"


/***** Variables Globales *************************************/

/***** Variables Locales **************************************/

FILE * f_salida_html;
static FILE * f_consola;

static ciclo_t primera_instruccion = 1;
static ciclo_t primer_ciclo = 1;
//static short maxima_columna_visible = MAX_CICLOS_INIC;

int MAX_INSTRUC = _MAX_INSTRUC;
int MAX_CICLOS_INIC = _MAX_CICLOS_INIC;
int MAX_CICLOS = _MAX_CICLOS;

static short maxima_columna_visible;

/*** Variables ************************************************/

char p_str[256]; /* Cadena auxiliar */

linea_crono_t crono[_MAX_INSTRUC];
/* Instrucciones visibles en el
   cronograma */


/***** Funciones Locales **************************************/

/**
 * Modifica algunos parámetros para pantalas pequeñas
 *
 */
void init_presentacion() {
    if (small_screen_flag) {
        MAX_INSTRUC = _MAX_INSTRUC_s;
        MAX_CICLOS_INIC = _MAX_CICLOS_INIC_s;
        MAX_CICLOS = _MAX_CICLOS_s;
        maxima_columna_visible = MAX_CICLOS_INIC;
    }
}


/**
 * Imprime la intro HTML incluyendo el archivo css
 *
 * @param titulo titulo de la pagina
 */
static void imprime_w3css(char* titulo) {

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /*** HTML INTRO ***/

    fprintf(f_salida_html, "<!DOCTYPE html>\n");
    fprintf(f_salida_html,
            "<html lang=\"es-ES\">\n<head>\n<title>%s:%s. Ciclo %u</title>\n",
            titulo, nombre_fich, Ciclo);
    fprintf(f_salida_html,
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
    fprintf(f_salida_html,
            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n");
    fprintf(f_salida_html,
            "<link rel=\"stylesheet\" href=\"https://www.w3schools.com/lib/w3.css\">\n");
    fprintf(f_salida_html, "<style>\n");
    fprintf(f_salida_html, "html, body {\n");
    fprintf(f_salida_html, "    font-family:\"%s\", Times, serif;\n", FONT_NAME);
    fprintf(f_salida_html, "    font-size:%dpt;\n", FONT_SIZE);
    fprintf(f_salida_html, "    line-height:%s;\n", LINE_HEIGHT);
    fprintf(f_salida_html, "}\n");
    fprintf(f_salida_html, " .field { display: inline-block; font-size: 80%%; height: 16px; "
                      "text-align: center; padding: 4px 0px 0px 0px; border: 1px solid black}\n");
    fprintf(f_salida_html, " .texto { display: inline-block;  width: 50px; }\n");

    fprintf(f_salida_html, "</style>\n");
    fprintf(f_salida_html, "</head>\n<body>\n\n");

}


/**
 * Imprime el final HTML con css
 *
 */
static void imprime_footer_css() {

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /*** HTML FINAL ***/
    fprintf(f_salida_html, "<!-- Final -->\n");
    fprintf(f_salida_html, "<div class=\"w3-row w3-container w3-light-grey\">\n");
    fprintf(f_salida_html, "<address>\n");
    fprintf(f_salida_html, "Arquitectura e Ingeniería de Computadores<br>\n");
    fprintf(f_salida_html, "Departamento de Informática de Sistemas y Computadores (DISCA)<br>\n");
    fprintf(f_salida_html, "Universitat Politècnica de València<br>\n");
    fprintf(f_salida_html, "</address>\n");
    fprintf(f_salida_html, "</div>\n\n");

    fprintf(f_salida_html, "</body>\n</html>\n");
}

/**
 * Imprime la barra de navegación, utilizando CSS
 *
 */
static void imprime_nav1_css() {


    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Barra de navegación */
    fprintf(f_salida_html, "<!-- Barra de navegacion CSS -->\n");
    fprintf(f_salida_html, "<div class=\"w3-bar w3-top w3-light-grey\">\n");


    //sprintf(_msg, "Getenv %s\n", getenv("MIPS_HOME"));
    //DEBUG(__func__);

    if (home_link_flag) {
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s\">HOME</a>\n", HOME_LINK);
    }
    fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"index.html\">INICIO</a>\n");
    fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button\" href=\"final.html\">FINAL</a>\n");
    if (salida == html) {
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button\" href=\"result001.html\">Estado</a>\n");
    }

    fprintf(f_salida_html, "</div>\n\n");
}

/**
 * Imprime la barra de navegación con estado ciclo a ciclo, utilizando CSS
 *
 * @param prefijo del archivo
 */
static void imprime_nav2_css(char* archivo) {

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Barra de navegación */
    fprintf(f_salida_html, "<!-- Barra de navegacion CSS -->\n");
    fprintf(f_salida_html, "<div class=\"w3-bar w3-top w3-light-grey\" id='nav_bar'>\n");
    if (home_link_flag) {
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s\">HOME</a>\n", HOME_LINK);
    }
    fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"index.html\">INICIO</a>\n");
    fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"final.html\">FINAL</a>\n");

    if (Ciclo > 100)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-100]</a>\n", archivo,
                Ciclo - 100);
    else
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-100]</a>\n", archivo,
                Ciclo);

    if (Ciclo > 10)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-10]</a>\n", archivo,
            Ciclo - 15);
    else
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-10]</a>\n", archivo,
            Ciclo);

    if (Ciclo > 5)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-5]</a>\n", archivo,
            Ciclo - 5);
    else
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-5]</a>\n", archivo,
            Ciclo);
    if (Ciclo > 1)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-1]</a>\n", archivo,
            Ciclo - 1);
    else
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-1]</a>\n", archivo,
            Ciclo);

    if (!final)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[+1]</a>\n", archivo,
            Ciclo + 1);

    if (!final)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[+5]</a>\n", archivo,
            Ciclo + 5);

    if (!final)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[+10]</a>\n", archivo,
            Ciclo + 10);

    if (!final)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[+100]</a>\n", archivo,
                Ciclo + 100);

    fprintf(f_salida_html, "</div>\n\n");
}

/**
 * Imprime la configuracion del simulador
 *
 */
static void imprime_conf() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[SHORT_STR];

    /*************************************/
    /*  Function body                    */
    /*************************************/


    /*** Configuración ***/

    fprintf(f_salida_html, "<!-- Configuración -->\n");

    fprintf(f_salida_html, "<table>\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td colspan=\"2\" bgcolor=\"%s\" align=\"center\">Configuración</td>\n", BG_HEADER);
    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Parámetro</td>\n", BG_HEADER_COL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Valor</td>\n", BG_HEADER_COL2);
    fprintf(f_salida_html, "</tr>\n");

    /*** Nombre programa ***/
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Programa</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%s</td>\n", BG_VAL2, nombre_fich);
    fprintf(f_salida_html, "</tr>\n");

    /*** Tipo procesador ***/
    switch (tipo_procesador) {
        case segm:
            sprintf(aux, "%s", "Segmentado");
            break;
        case no_segm_monociclo:
            sprintf(aux, "%s", "No segm. - u.c. monociclo");
            break;
        case no_segm_multiciclo:
            sprintf(aux, "%s", "No segm. - u.c. multiciclo");
    };

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Tipo de procesador</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%s</td>\n", BG_VAL2, aux);
    fprintf(f_salida_html, "</tr>\n");

    /*** Riesgos datos ***/
    switch (solucion_riesgos_datos) {
        case ninguno:
            sprintf(aux, "%s", "Ninguno");
            break;
        case parada:
            sprintf(aux, "%s", "Stalls");
            break;
        case cortocircuito:
            sprintf(aux, "%s", "Forwarding");
    };

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Riesgos de datos</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%s</td>\n", BG_VAL2, aux);
    fprintf(f_salida_html, "</tr>\n");

    switch (solucion_riesgos_control) {
        case ds3:
            sprintf(aux, "%s", "Ninguno (Lat=3)");
            break;
        case ds2:
            sprintf(aux, "%s", "Ninguno (Lat=2)");
            break;
        case ds1:
            sprintf(aux, "%s", "Ninguno (Lat=1)");
            break;
        case pnt3:
            sprintf(aux, "%s", "Predict-not-taken (Lat=3)");
            break;
        case pnt2:
            sprintf(aux, "%s", "Predict-not-taken (Lat=2)");
            break;
        case pnt1:
            sprintf(aux, "%s", "Predict-not-taken (Lat=1)");
            break;
        case stall3:
            sprintf(aux, "%s", "Stalls (Lat=3)");
            break;
        case stall2:
            sprintf(aux, "%s", "Stalls (Lat=2)");
            break;
        case stall1:
            sprintf(aux, "%s", "Stalls (Lat=1)");
    };

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Riesgos de control&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%s</td>\n", BG_VAL2, aux);
    fprintf(f_salida_html, "</tr>\n\n");

    /*** Registros ***/
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Registros</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, TAM_REGISTROS);
    fprintf(f_salida_html, "</tr>\n\n");

    /*** Latencias ***/
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Lat. L/S&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, FP_LS_LAT);
    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Lat. FP ADD&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, FP_ADD_LAT);
    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Lat. FP MUL&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, FP_MUL_LAT);
    fprintf(f_salida_html, "</tr>\n\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Lat. FP CVT&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, FP_CVT_LAT);
    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Lat. FP MISC&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, FP_MISC_LAT);
    fprintf(f_salida_html, "</tr>\n");



    fprintf(f_salida_html, "</table>\n\n");
}

/**
 * Desplaza la zona visible un ciclo hacia la izquierda
 *
 */
static void desplazamiento_horizontal() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    for (i = 0; i < MAX_INSTRUC; i++) {
        for (j = 0; j < MAX_CICLOS - 1; j++)
            strcpy(crono[i].fase[j], crono[i].fase[j + 1]);

        /* Borra la última columna */
        strcpy(crono[i].fase[j], "");
    } /* endfor */

    /*** Actualiza los desplazamientos iniciales ***/

    primer_ciclo++;

} /* end desplazamiento_horizontal */


/**
 * Desplaza la zona visible un ciclo hacia abajo
 *
 */
static void desplazamiento_vertical() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j, desp;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Averigua si la línea se va a quedar vacia al principio */
    for (desp = 0;
            desp < MAX_CICLOS &&
            !strlen(crono[1].fase[desp]); desp++);
    if (desp == MAX_CICLOS)
        desp = 1; /* La línea está completamente vacia. Avanzamos sólo un
                                 * ciclo */


    for (i = 0; i < MAX_INSTRUC - 1; i++) {
        strcpy(crono[i].inst, crono[i + 1].inst);
        strcpy(crono[i].PC, crono[i + 1].PC);

        for (j = 0; j < MAX_CICLOS - desp; j++)
            strcpy(crono[i].fase[j], crono[i + 1].fase[j + desp]);

        /* Borra las últimas columnas */
        for (j = MAX_CICLOS - desp; j < MAX_CICLOS; j++)
            strcpy(crono[i].fase[j], "");
    } /* endfor */

    /* Borra la última fila */
    crono[MAX_INSTRUC - 1].inst[0] = '\0';
    crono[MAX_INSTRUC - 1].PC[0] = '\0';
    for (j = 0; j < MAX_CICLOS; j++)
        crono[MAX_INSTRUC - 1].fase[j][0] = '\0';

    /*** Actualiza los desplazamientos iniciales ***/

    primer_ciclo += desp;
    primera_instruccion++;
    maxima_columna_visible -= desp;

} /* end desplazamiento_vertical */


/**
 * Imprime el nombre de la señal, si está activada
 *
 * @param senyal SI/NO
 * @param nombre nombre de la señal
 */
static void muestra_senyal(boolean senyal, char* nombre) {
    char aux[SHORT_STR];

    if (senyal == SI)
        sprintf(aux, "%s", nombre);
    else
        sprintf(aux, "&nbsp;");
    fprintf(f_salida_html, "<td width=%d>%s</td>\n", SENYALES_W, aux);
}

/**
 * Muestra la etapa de cada instrucción en el cronograma
 *
 * @param fase Etapa
 * @param orden Orden de la instruccion
 */
static void muestra_fase(
        char * fase,
        ciclo_t orden
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    int o, c;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    if (Ciclo >= primer_ciclo + MAX_CICLOS)
        desplazamiento_horizontal();

    o = orden - primera_instruccion;
    c = Ciclo - primer_ciclo;

    if (o >= 0 && c >= 0) {
        strcpy(crono[o].fase[c], fase);
        if (c + 1 > maxima_columna_visible)
            maxima_columna_visible = c + 1;
    } /* endif */

} /* end muestra_fase */


/**
 * Imprime el estado de los registros enteros en una columna
 *
 */
static void imprime_int_regs_col() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j;

    char col_ID[20], col_WB[20];
    char vstr[SHORT_STR];
    char aux2[LONG_STR];

    char reg_name[SHORT_STR];


    valor_t valor_reg;
    tipo_t tipo_dato;
    boolean dirty;


    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Banco de registros enteros */

    // Colores
    color_instruccion(col_ID, IF_ID.iPC);
    color_instruccion(col_WB, iWB.iPC);

    fprintf(f_salida_html, "<!-- Registros enteros en columna -->\n");
    fprintf(f_salida_html, "<table>\n");


    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_HEADER);
    fprintf(f_salida_html, "  <td align=\"center\" colspan=2>Registros int</td>\n");
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Registro</td>\n", BG_DIR1);
    fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">Valor</td>\n", REG_W, BG_DIR2);
    fprintf(f_salida_html, "</tr>\n");

    i = 0; // NO hace falta, pero así reutilizo el código
    j = 0;
    while (j < TAM_REGISTROS) {
        fprintf(f_salida_html, "<tr>\n");
        /*		fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                                                "R%d&nbsp;&nbsp;</td>\n", BG_VAL1, i+j); */
        valor_reg = lee_int_reg(i + j, &tipo_dato, &dirty);
        switch (tipo_dato) {
            case t_byte:
            case t_char:
                // Valor
                sprintf(vstr, F_DWORD "(%c)", valor_reg.int_d, (char) valor_reg.int_d);
                if (dirty) {
                    sprintf(aux2, "<td align=\"center\" bgcolor=\"%s\">%s</td>\n", BG_VAL2, vstr);
                } else {
                    sprintf(aux2, "<td align=\"center\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                }
                break;
            default:
                if (valor_reg.int_d <= MAX_WORD) {
                    sprintf(vstr, F_DWORD, valor_reg.int_d);
                } else {
                    sprintf(vstr, "%" PRIx64, valor_reg.int_d);
                }
                // Valor
                if (dirty) {
                    sprintf(aux2, "<td align=\"center\" bgcolor=\"%s\">%s</td>\n", BG_VAL2, vstr);
                } else {
                    sprintf(aux2, "<td align=\"center\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                }
                break;
        } /* endswitch */


        sprintf(reg_name, "x%d (%s)&nbsp;&nbsp;", i + j, nombre_int_reg[i + j]);
        
        // El registro se escribe
        if (hay_destino_WB() && (iWB.IR.rd == (i + j))) {
            // Y tambien se lee
            if ((hay_fuente1_ID() && (IF_ID.IR.rs1 == (i + j))) ||
                    (hay_fuente2_ID() && (IF_ID.IR.rs2 == (i + j)))) {
                // Rx
                //fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                //        S_READWRITE " R%d&nbsp;&nbsp;</td>\n", BG_RW, i + j);
                // Lo mostramos del color WB
                fprintf(f_salida_html, "  <td align=\"right\" style=\"color: white;\" bgcolor=\"%s\">"
                        S_READWRITE " %s</td>\n", col_WB, reg_name);
                //fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">" S_READWRITE " %s</td>\n", BG_RW, reg_name);
                // Valor
                fprintf(f_salida_html, "%s\n", aux2);
                // Solo se escribe
            } else {
                // Rx
                //fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                //        S_WRITE " R%d&nbsp;&nbsp;</td>\n", col_WB, i + j);
                fprintf(f_salida_html, "  <td align=\"right\" style=\"color: white;\"  bgcolor=\"%s\">"
                        S_WRITE " %s</td>\n", col_WB, reg_name);
                // Valor
                fprintf(f_salida_html, "%s\n", aux2);
            }
            // El registro se lee
        } else if ((hay_fuente1_ID() && (IF_ID.IR.rs1 == (i + j))) ||
                (hay_fuente2_ID() && (IF_ID.IR.rs2 == (i + j))) ||
                (hay_fuente2_ID() && (IF_ID.IR.rs2 == (i + j)))) {
            // Rx
            //fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
            //        S_READ " R%d&nbsp;&nbsp;</td>\n", col_ID, i + j);
            fprintf(f_salida_html, "  <td align=\"right\" style=\"color: white;\"  bgcolor=\"%s\">"
                    S_READ " %s</td>\n", col_ID, reg_name);
            // Valor
            fprintf(f_salida_html, "%s\n", aux2);
            // Ni se lee ni se escribe
        } else {
            // Rx
            //fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
            //        "R%d&nbsp;&nbsp;</td>\n", BG_VAL1, i + j);
            fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                    " %s</td>\n", BG_VAL1, reg_name);
            // Valor
            fprintf(f_salida_html, "%s\n", aux2);
        }

        fprintf(f_salida_html, "</tr>\n");
        j = j + 1;
    } /* while */

    fprintf(f_salida_html, "</table>\n\n");

} /* end imprime_int_regs_col */


/**
 * Imprime el estado de los registros enteros en una columna
 *
 */
static void imprime_fp_regs_col() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j;

    char col_ID[20], col_WB[20], col_CMP_WB[20];
    char aux1[SHORT_STR];
    char aux2[MEDIUM_STR];
    char aux3[LONG_STR];

    char reg_name[SHORT_STR];
 
    valor_t valor_fp_reg;
    //valor_t valor_int_reg;
    tipo_t tipo_dato;
    boolean dirty;


    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Banco de registros fp */

    // Colores
    color_instruccion(col_ID, IF_ID.iPC);
    color_instruccion(col_WB, FP_WB.iPC);
    color_instruccion(col_CMP_WB, FP_CMP_WB.iPC);

    fprintf(f_salida_html, "<!-- Registros fp en columna -->\n");
    fprintf(f_salida_html, "<table>\n");


    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_HEADER);
    fprintf(f_salida_html, "  <td align=\"center\" colspan=3>Registros fp</td>\n");
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Registro</td>\n", BG_DIR1);
    //  fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">double</td>\n", REG_W, BG_DIR2);
    fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">Hi</td>\n", REG_W, BG_DIR2);
    fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">Lo</td>\n", REG_W, BG_DIR2);
    fprintf(f_salida_html, "</tr>\n");

    i = 0; // NO hace falta, pero así reutilizo el código
    j = 0;
    while (j < TAM_REGISTROS) {
        fprintf(f_salida_html, "<tr>\n");
        /*		fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                                                "F%d&nbsp;&nbsp;</td>\n", BG_VAL1, i+j); */
        valor_fp_reg = lee_fp_reg(i + j, &tipo_dato, &dirty);
        switch (tipo_dato) {
            case t_double:
            case t_ninguno:
                // Valor
                sprintf(aux1, "%6.1lf", valor_fp_reg.fp_d);
                if (dirty) {
                    sprintf(aux3, "<td align=\"center\" colspan=\"2\" bgcolor=\"%s\">%s</td>\n", BG_VAL2, aux1);
                } else {
                    sprintf(aux3, "<td align=\"center\" colspan=\"2\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, aux1);
                }
                break;
            case t_float:
                sprintf(aux1, "%6.1f", 0.0);
                if (dirty) {
                    sprintf(aux2, "<td align=\"right\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, aux1);
                } else {
                    sprintf(aux2, "<td align=\"right\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, aux1);
                }
                sprintf(aux1, "%6.1f", valor_fp_reg.fp_s);
                if (dirty) {
                    sprintf(aux3, "%s<td align=\"right\" bgcolor=\"%s\">%s</td>\n", aux2, BG_VAL2, aux1);
                } else {
                    sprintf(aux3, "%s<td align=\"right\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", aux2, BG_VAL2, TEXT_GRAY, aux1);
                }
                break;
            case t_float_ps:
                sprintf(aux1, "%6.1f", valor_fp_reg.fp_ps[Hi]);
                if (dirty) {
                    sprintf(aux2, "<td align=\"right\" bgcolor=\"%s\">%s</td>\n", BG_VAL2, aux1);
                } else {
                    sprintf(aux2, "<td align=\"right\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, aux1);
                }
                sprintf(aux1, "%6.1f", valor_fp_reg.fp_ps[Lo]);
                if (dirty) {
                    sprintf(aux3, "%s<td align=\"right\" bgcolor=\"%s\">%s</td>\n", aux2, BG_VAL2, aux1);
                } else {
                    sprintf(aux3, "%s<td align=\"right\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", aux2, BG_VAL2, TEXT_GRAY, aux1);
                }
                break;
            default:
                fnError("Imprimir FP regs con tipo de datos no FP\n");
        }
        /*
        // Valor
        sprintf(aux1, "%6.1lf", Rfp[i + j].valor.fpdouble);
        copy_trunc_str(aux2, aux1, 6);
        color_tipo(t_double, valor_double, aux2, Rfp_tipo[i + j]);

        sprintf(aux1, "%6.1f", Rfp[i + j].valor.fpsingle[Hi]);
        copy_trunc_str(aux2, aux1, 6);
        color_tipo(t_float_ps, valor_single_hi, aux2, Rfp_tipo[i + j]);

        sprintf(aux1, "%6.1f", Rfp[i + j].valor.fpsingle[Lo]);
        copy_trunc_str(aux2, aux1, 6);
        color_float(valor_single_lo, aux2, Rfp_tipo[i + j]);
         */

        sprintf(reg_name, "f%d (%s)&nbsp;&nbsp;", i + j, nombre_fp_reg[i + j]);
        
        // El registro se escribe
        if (hay_destino_FP_WB() && (FP_WB.IR.rd == (i + j))) {
            // Y tambien se lee
            if ((hay_fuente1_FP_ID() && (IF_ID.IR.rs1 == (i + j))) ||
                    (hay_fuente2_FP_ID() && (IF_ID.IR.rs2 == (i + j)))) {
                // Lo mostramos del color WB
                fprintf(f_salida_html, "  <td align=\"right\" style=\"color: white;\" bgcolor=\"%s\">"
                        S_READWRITE " %s</td>\n", col_WB, reg_name);
                //fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">" S_READWRITE " F%d&nbsp;&nbsp;</td>\n", BG_RW, i + j);

                // Valor

                fprintf(f_salida_html, "%s", aux3);


                // Solo se escribe
            } else {
                // Fx
                fprintf(f_salida_html, "  <td align=\"right\" style=\"color: white;\" bgcolor=\"%s\">"
                        S_WRITE " %s</td>\n", col_WB, reg_name);
                // Valor
                fprintf(f_salida_html, "%s", aux3);

            }
            // El registro se lee
        } else if ((hay_fuente1_FP_ID() && (IF_ID.IR.rs1 == (i + j))) ||
                (hay_fuente2_FP_ID() && (IF_ID.IR.rs2 == (i + j))) ||
                (hay_fuente3_FP_ID() && (IF_ID.IR.rs3 == (i + j)))) {
            // Fx
            fprintf(f_salida_html, "  <td align=\"right\" style=\"color: white;\" bgcolor=\"%s\">"
                    S_READ " %s</td>\n", col_ID, reg_name);
            // Valor
            fprintf(f_salida_html, "%s", aux3);

            // Ni se lee ni se escribe
        } else {
            // Fx
            fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                    "%s</td>\n", BG_VAL1, reg_name);
            // Valor
            fprintf(f_salida_html, "%s", aux3);

        }

        fprintf(f_salida_html, "</tr>\n");
        j = j + 1;
    } /* while */


    fprintf(f_salida_html, "</table>\n\n");
    //fprintf(f_salida_html, "</div>\n");

} /* end imprime_fp_regs_col */

/**
 * Comprueba si una direccion de byte se encuentra en el rango de una
 * direccion de palabra
 * 
 * @param dir_palabra 
 * @param dir
 */
static int en_rango(dword dir_palabra, dword dir) {
    return ((dir >= dir_palabra) && (dir <= (dir_palabra + sizeof (word) - 1)));
}

/**
 * Muestra la etapa de cada instrucción en el cronograma
 *
 * @param fase Fase
 * @param orden Orden de la instruccion
 */
static void imprime_Dmem_col() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    dword i;
    dword d;
    dword next;
    char aux[SHORT_STR];

    char col_M[20], col_LS[20];

    valor_t dato;
    tipo_t tipo;
    //char vstr_col[SHORT_STR];
    char vstr[SHORT_STR];
    boolean dirty;


    /*************************************/
    /*  Function body                    */
    /*************************************/
    // Colores
    // Colores
    color_instruccion(col_M, EX_MEM.iPC);
    color_instruccion(col_LS, FP_LS[FP_LS_LAT].iPC);


    /*** Contenido de la memoria de datos ***/
    for (region_t r = 0; r < num_regiones; r++) {
        if (!es_region_datos(r) || tam_region(mmem, r) == 0)
            continue;

        fprintf(f_salida_html, "<table>\n");

        fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_HEADER);
        fprintf(f_salida_html, " <td align=\"center\" colspan=5>Memoria de Datos. Region %d</td>\n", r);
        fprintf(f_salida_html, "</tr>\n");
        fprintf(f_salida_html, "<tr>\n");
        fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Dirección</td>\n", BG_DIR1);
        for (i = 0; i < 4; i++) {
            fprintf(f_salida_html, "  <td bgcolor=\"%s\" width=\"%d\" align=\"center\">+%ld</td>\n", BG_DIR2, MEM_W, i);
        } /* endif */
        fprintf(f_salida_html, " </tr>\n");

        next = 0;
        for (d = inicio_region(mmem, r); d < final_region(mmem, r); d++) {

            i = dir_a_offset(mmem, d, r);



            if (d % sizeof (word) == 0) {
                // Falta ver si se lee o escribe
                fprintf(f_salida_html, " <tr bgcolor=\"%s\">\n", BG_VAL2);
                te_etiqueta(aux, d);
                //color_etiqueta_memdatos(color, i);

                if (inst_int_load(EX_MEM.IR.codop) && en_rango(d, EX_MEM.ALUout)) {
                    // LOAD
                    fnDebug(NO, "LOAD dir pal=%ld dir=%ld\n", d, EX_MEM.ALUout);
                    fprintf(f_salida_html, "  <td align=\"right\" style=\"color: white;\" bgcolor=\"%s\">"
                            S_READ " %s&nbsp;&nbsp;</td>\n", col_M, aux);
                } else if (inst_int_store(EX_MEM.IR.codop) && en_rango(d, EX_MEM.ALUout)) {
                    // STORE
                    fnDebug(NO, "STORE dir pal=%ld dir=%ld\n", d, EX_MEM.ALUout);
                    fprintf(f_salida_html, "  <td align=\"right\" style=\"color: white;\" bgcolor=\"%s\">"
                            S_WRITE " %s&nbsp;&nbsp;</td>\n", col_M, aux);
                } else if (inst_fp_load(FP_LS[FP_LS_LAT].IR.codop) && (FP_LS[FP_LS_LAT].iPC != -1) && en_rango(d, FP_LS[FP_LS_LAT].address)) {
                    // LOAD
                    fnDebug(NO, "LOAD FP dir pal=%ld dir=%ld codop=%d\n", d, FP_LS[FP_LS_LAT].address, FP_LS[FP_LS_LAT].IR.codop);
                    fprintf(f_salida_html, "  <td align=\"right\" style=\"color: white;\" bgcolor=\"%s\">"
                            S_READ " %s&nbsp;&nbsp;</td>\n", col_LS, aux);
                } else if (inst_fp_store(FP_LS[FP_LS_LAT].IR.codop) && (FP_LS[FP_LS_LAT].iPC != -1) && en_rango(d, FP_LS[FP_LS_LAT].address)) {
                    // STORE
                    fnDebug(NO, "STORE FP dir pal=%ld dir=%ld\n", d, FP_LS[FP_LS_LAT].address);
                    fprintf(f_salida_html, "  <td align=\"right\" style=\"color: white;\" bgcolor=\"%s\">"
                            S_WRITE " %s&nbsp;&nbsp;</td>\n", col_LS, aux);
                } else {
                    fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                            "%s&nbsp;&nbsp;</td>\n", BG_VAL1, aux);
                }
            }

            // printf("Region %d Abs %d Offset %d \n", r, d, i);
            if (i == next) {
                tipo = md_tipo[r][i];
                //printf("%d,%hhd\n", i, dato.int_b);

                switch (tipo) {
                    case t_char:
                    case t_byte:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);
                        sprintf(vstr, F_BYTE"(%c)", (unsigned char) dato.int_d, (char) dato.int_d);
                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }
                        //FIRMA_2_2("MD%d", i, "%hhX", MD.m_byte[dir_byte(i)]);
                        next = i + sizeof (byte);
                        break;
                    case t_half:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);

                        sprintf(vstr, F_HALF, (half) dato.int_d);
                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"2\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"2\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }
                        //FIRMA_2_2("MD%d", i, "%hd", MD.m_half[dir_half(i)]);
                        next = i + sizeof (half);
                        break;
                    case t_word:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);

                        sprintf(vstr, F_WORD, (word) dato.int_d);
                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"4\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"4\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }
                        //FIRMA_2_2("MD%d", i, "%d", MD.m_word[dir_word(i)]);
                        next = i + sizeof (word);
                        break;
                    case t_dword:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);

                        if (dato.int_d <= MAX_WORD) {
                            sprintf(vstr, F_DWORD, dato.int_d);
                        } else {
                            sprintf(vstr, "%" PRIx64, dato.int_d);
                        }

                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" rowspan=\"2\" colspan=\"4\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" rowspan=\"2\" colspan=\"4\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }
                        //FIRMA_2_2("MD%d", i, F_DWORD, MD.m_dword[dir_dword(i)]);
                        next = i + sizeof (dword);
                        break;
                    case t_float:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);

                        sprintf(vstr, "%5.1f", dato.fp_s);
                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"4\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"4\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }
                        //FIRMA_2_2("MD%d", i, "%.2f", MD.m_float[dir_float(i)]);
                        next = i + sizeof (float);
                        break;
                    case t_double:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);

                        sprintf(vstr, "%5.1lf", dato.fp_d);
                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" rowspan=\"2\" colspan=\"4\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" rowspan=\"2\" colspan=\"4\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }

                        //FIRMA_2_2("MD%d", i, "%.2f", MD.m_double[dir_double(i)]);
                        next = i + sizeof (double);
                        break;
                    default:
                        fprintf(f_salida_html, "  <td>&nbsp;</td>\n");
                        next = i + 1;
                } /* endswitch */
            } /* endif */
            if ((i + 1) % sizeof (word) == 0) {

                fprintf(f_salida_html, " </tr>\n");
            } /* endif */
        } /* endif */
        fprintf(f_salida_html, "</table>\n\n");
    }
}

/**
 * Imprime el estado de la memoria de instrucciones en una columna
 *
 * @param format_mode full/compact/none Mostrar codificacion en campos
 */
static void imprime_Imem_col(modo_formato_t format_mode) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i = 0;
    char aux[LONG_STR];
    G_STR_DECL(formato);


    /*************************************/
    /*  Function body                    */
    /*************************************/


    /*** Contenido de la memoria de instrucciones ***/

    fprintf(f_salida_html, "<!-- Mem Instr en columna -->\n");
    fprintf(f_salida_html, "<table>\n");

    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_HEADER);
    fprintf(f_salida_html, "  <td align=\"center\" colspan=2>Memoria de Instrucciones</td>\n");
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Dirección</td>\n", BG_DIR1);
    fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">Instrucciones</td>\n", INSTR_CRONO_W, BG_DIR2);
    if (format_mode!=format_none)
        fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">Formato</td>\n", 400, BG_DIR2);
    fprintf(f_salida_html, "</tr>\n");

    for (i = inicio_region(mmem, r_text); i < final_region(mmem, r_text); i += 4) {

        fprintf(f_salida_html, " <tr>\n");
        te_etiqueta(aux, i);
        fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                "%s&nbsp;&nbsp;</td>\n", BG_VAL1, aux);
        imprime_instruccion_color(aux, i, SI);
        fprintf(f_salida_html, "  <td bgcolor=\"%s\">%s</td>\n", BG_VAL2, aux);
            if (format_mode != format_none) {
                imprime_formato(&formato, i, format_mode);
                fprintf(f_salida_html,
                        "  <td bgcolor=\"#eeeeee\" nowrap='nowrap'>%s</td>\n", g_str_string(&formato));
            }
            fprintf(f_salida_html, "</tr>\n");
    } /* endfor */

    fprintf(f_salida_html, "</table>\n\n");
}

/**
 * Imprime las etapas del procesador
 *
 */
static void imprime_etapas() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i = 0;
    char aux[SHORT_STR];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /*** Muestra etapas ***/

    fprintf(f_salida_html, "<!-- Etapas -->\n");
    fprintf(f_salida_html, "<table>\n");

    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);
    fprintf(f_salida_html, "<td width=%d bgcolor=\"white\">&nbsp;</td>\n", NOMBRE_ETAPAS_W);
    fprintf(f_salida_html, "<td width=%d align=center>--/IF</td>\n", ETAPAS_W);
    fprintf(f_salida_html, "<td width=%d align=center>IF/ID</td>\n", ETAPAS_W);
    fprintf(f_salida_html, "<td width=%d align=center>ID/EX</td>\n", ETAPAS_W);
    fprintf(f_salida_html, "<td width=%d align=center>EX/MEM</td>\n", ETAPAS_W);
    fprintf(f_salida_html, "<td width=%d align=center>MEM/WB</td>\n", ETAPAS_W);
    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">Instruc.</td>\n", BG_OPER);

    imprime_instruccion_color(aux, PC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    imprime_instruccion_color(aux, IF_ID.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    //imprime_instruccion_color(aux, ID_EX.iPC, SI);
    imprime_instruccion_color(aux, FP_LS_LAT == 2 && ID_EX.iPC == -1 ? ID_FP_LS.iPC : ID_EX.iPC, SI); // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    //imprime_instruccion_color(aux, EX_MEM.iPC, SI);
    imprime_instruccion_color(aux, FP_LS_LAT == 2 && EX_MEM.iPC == -1 ? FP_LS[2].iPC : EX_MEM.iPC, SI); // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    //imprime_instruccion_color(aux, MEM_WB.iPC, SI);
    imprime_instruccion_color(aux, FP_LS_LAT == 2 && MEM_WB.iPC == -1 ? FP_LS_WB.iPC : MEM_WB.iPC, SI); // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    fprintf(f_salida_html, "</tr>\n");


    /* Load/Store FP */
    if (FP_LS_LAT > 2) { // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion
        fprintf(f_salida_html, "<!-- LOAD/STORE -->\n");
        fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

        fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
        fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
        fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
        for (i = 1; i <= FP_LS_LAT; i++) {
            fprintf(f_salida_html, "<td width=%d align=center>L/S%d</td>\n", ETAPAS_W, i);
        }
        fprintf(f_salida_html, "<td width=%d align=center>FP_LS/WB</td>\n", ETAPAS_W);

        fprintf(f_salida_html, "</tr>\n");

        fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
        fprintf(f_salida_html, "<td bgcolor=\"%s\">L/S (FP)</td>\n", BG_OPER);

        fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
        fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

        imprime_instruccion_color(aux, ID_FP_LS.iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
        for (i = 2; i <= FP_LS_LAT; i++) {
            imprime_instruccion_color(aux, FP_LS[i].iPC, SI);
            fprintf(f_salida_html, "<td>%s</td>\n", aux);
        }

        //if inst_load_fp(LS_WB.IR.codop) {
        imprime_instruccion_color(aux, FP_LS_WB.iPC, SI);
        //} else {
        //    imprime_instruccion_color(aux, -1, SI);
        //}
        fprintf(f_salida_html, "<td>%s</td>\n", aux);

        fprintf(f_salida_html, "</tr>\n");
    } /* endif (FP_LS_LAT > 2) */

    /* Sumador FP */
    fprintf(f_salida_html, "<!-- FP ADD-->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 1; i <= FP_ADD_LAT; i++) {
        fprintf(f_salida_html, "<td width=%d align=center>A%d</td>\n", ETAPAS_W, i);
    }
    fprintf(f_salida_html, "<td width=%d align=center>FP WB</td>\n", ETAPAS_W);

    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">Sum (FP)</td>\n", BG_OPER);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

    imprime_instruccion_color(aux, ID_FP_ADD.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    for (i = 2; i <= FP_ADD_LAT; i++) {
        imprime_instruccion_color(aux, FP_ADD[i].iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
    }

    imprime_instruccion_color(aux, FP_ADD_WB.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);

    fprintf(f_salida_html, "</tr>\n");

    /* Multiplicador FP */

    fprintf(f_salida_html, "<!-- FP MUL -->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 1; i <= FP_MUL_LAT; i++) {
        fprintf(f_salida_html, "<td width=%d align=center>M%d</td>\n", ETAPAS_W, i);
    }
    fprintf(f_salida_html, "<td width=%d align=center>FP WB</td>\n", ETAPAS_W);

    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">Mul (FP)</td>\n", BG_OPER);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

    imprime_instruccion_color(aux, ID_FP_MUL.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    for (i = 2; i <= FP_MUL_LAT; i++) {

        imprime_instruccion_color(aux, FP_MUL[i].iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
    }

    imprime_instruccion_color(aux, FP_MUL_WB.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);

    fprintf(f_salida_html, "</tr>\n");

    /* Comparacion FP */

    fprintf(f_salida_html, "<!-- FP CMP -->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 1; i <= FP_CMP_LAT; i++) {
        fprintf(f_salida_html, "<td width=%d align=center>C%d</td>\n", ETAPAS_W, i);
    }
    fprintf(f_salida_html, "<td width=%d align=center>INT WB</td>\n", ETAPAS_W);

    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">Cmp (FP)</td>\n", BG_OPER);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

    imprime_instruccion_color(aux, ID_FP_CMP.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    for (i = 2; i <= FP_CMP_LAT; i++) {
        imprime_instruccion_color(aux, FP_CMP[i].iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
    }

    imprime_instruccion_color(aux, FP_CMP_WB.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);

    fprintf(f_salida_html, "</tr>\n");

    /* Multiplicacion INT */

    fprintf(f_salida_html, "<!-- INT MUL -->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 1; i <= FP_MUL_LAT; i++) {
        fprintf(f_salida_html, "<td width=%d align=center>M%d</td>\n", ETAPAS_W, i);
    }
    fprintf(f_salida_html, "<td width=%d align=center>INT WB</td>\n", ETAPAS_W);

    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">Mul (Int)</td>\n", BG_OPER);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

    imprime_instruccion_color(aux, ID_INT_MUL.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    for (i = 2; i <= FP_MUL_LAT; i++) {
        imprime_instruccion_color(aux, INT_MUL[i].iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
    }

    imprime_instruccion_color(aux, INT_MUL_WB.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);

    fprintf(f_salida_html, "</tr>\n");

    /* CVT FP */

    fprintf(f_salida_html, "<!-- FP CVT -->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 1; i <= FP_CVT_LAT; i++) {
        fprintf(f_salida_html, "<td width=%d align=center>Y%d</td>\n", ETAPAS_W, i);
    }
    fprintf(f_salida_html, "<td width=%d align=center>FP WB</td>\n", ETAPAS_W);

    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">CVT (>FP)</td>\n", BG_OPER);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

    imprime_instruccion_color(aux, ID_FP_CVT.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    for (i = 2; i <= FP_CVT_LAT; i++) {
        imprime_instruccion_color(aux, FP_CVT[i].iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
    }

    imprime_instruccion_color(aux, FP_CVT_WB.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);

    fprintf(f_salida_html, "</tr>\n");

    /* MISC */

    fprintf(f_salida_html, "<!-- MISC -->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 1; i <= FP_MISC_LAT; i++) {
        fprintf(f_salida_html, "<td width=%d align=center>X%d</td>\n", ETAPAS_W, i);
    }
    fprintf(f_salida_html, "<td width=%d align=center>INT WB</td>\n", ETAPAS_W);

    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">MISC(>Int)</td>\n", BG_OPER);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

    imprime_instruccion_color(aux, ID_FP_MISC.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    for (i = 2; i <= FP_MISC_LAT; i++) {

        imprime_instruccion_color(aux, FP_MISC[i].iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
    }

    imprime_instruccion_color(aux, FP_MISC_WB.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);

    fprintf(f_salida_html, "</tr>\n");



    fprintf(f_salida_html, "</table>\n\n");
    fprintf(f_salida_html, "<br>\n");

} /* end imprime_etapas */

/**
 * Imprime las señales activas del procesador
 *
 */
static void imprime_senyales() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/


    /*************************************/
    /*  Function body                    */
    /*************************************/


    /*** Señales ***/

    //    fprintf(f_salida_html, "<tr bgcolor=\"white\"><td colspan=\"6\">&nbsp;</td></tr>\n");

    fprintf(f_salida_html, "<!-- Señales activas -->\n");
    fprintf(f_salida_html, "<table>\n");

    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_VAL); // Contenido de la tabla
    fprintf(f_salida_html, "<td width=%d bgcolor=\"%s\">Señales</td>\n", NOMBRE_ETAPAS_W, BG_SIG);

    muestra_senyal(IFstall, "IFstall");
    muestra_senyal(IDstall, "IDstall");
    muestra_senyal(RAW, "RAW");
    muestra_senyal(WAW, "WAW");
    muestra_senyal(estructural, "WBconflict");
    muestra_senyal(0, " ");
    muestra_senyal(0, " ");


    fprintf(f_salida_html, "</tr>\n");

    /*** Mas Señales ***/

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"#ffffff\">&nbsp;</td>\n");

    muestra_senyal(IFnop, "IFnop");
    muestra_senyal(IDnop, "IDnop");
    muestra_senyal(EXnop, "EXnop");
    muestra_senyal(0, " ");
    muestra_senyal(0, " ");
    muestra_senyal(0, " ");
    muestra_senyal(0, " ");


    fprintf(f_salida_html, "</tr>\n");


    /*** Mas Señales ***/

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"#ffffff\">&nbsp;</td>\n");

    muestra_senyal((MEMaIDcomp_i || MEMaIDcomp_s), "MEMaIDcomp");
    muestra_senyal((MEMaEXalu_i || MEMaEXalu_s), "MEMaEX");
    muestra_senyal((MEMaEXcomp_i || MEMaEXcomp_s), "MEMaEXcomp");
    muestra_senyal(MEMaFP_LS1_s, FP_LS_LAT == 2 ? "MEMaEXs" : "MEMaFP_LS1s");
    muestra_senyal((MEMaiM1_i || MEMaiM1_s), "MEMaiM1");
    muestra_senyal( MEMaFP_CVT1_s,"MEMaFP_CVT1");
    muestra_senyal(0, " ");

    fprintf(f_salida_html, "</tr>\n");

    /*** Mas Señales ***/

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"#ffffff\">&nbsp;</td>\n");

    muestra_senyal(WBaMEM, "WBaMEM ");
    muestra_senyal((WBaEXalu_i || WBaEXalu_s), "WBaEX");
    muestra_senyal((WBaEXcomp_i || WBaEXcomp_s), "WBaEXcomp");
    muestra_senyal(WBaEXmem, "WBaEXmem ");
    muestra_senyal(WBaFP_LS1_s, FP_LS_LAT == 2 ? "WBaEX" : "WBaFP_LS1");
    muestra_senyal(WBaFP_CVT1_s, "WBaFP_CVT1");
    muestra_senyal((WBaiM1_s || WBaiM1_i), "WBaiM1");

    fprintf(f_salida_html, "</tr>\n");

    /*** Mas Señales ***/

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"#ffffff\">&nbsp;</td>\n");

    muestra_senyal(FP_WBaFP_LS1_i, FP_LS_LAT == 2 ? "WBFaEXmem" : "WBFaLS1i");
    muestra_senyal(FP_WBaFP_LS2_i, FP_LS_LAT == 2 ? "WBFaMEM" : "WBFaLS2i");
    muestra_senyal((FP_WBaADD1_s || FP_WBaADD1_i), "WBFaA1");
    muestra_senyal((FP_WBaMUL1_s || FP_WBaMUL1_i), "WBFaM1");
    muestra_senyal((FP_WBaCMP1_s || FP_WBaCMP1_i), "WBFaC1");
    muestra_senyal(0, " ");
    muestra_senyal(0, " ");


    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "</table>\n\n");

} /* end imprime_senyales */

/**
 * Imprime el cronograma de ejecución
 *
 * @param mum_instrucciones Num instrucciones del cronograma
 * @param archivo Nombre del archivo html
 */
static void imprime_dit(int num_instrucciones, char* archivo) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short x, y;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    fprintf(f_salida_html, "<!-- Diagrama instrucciones - tiempo  -->\n");
    fprintf(f_salida_html, "<table id='chrono_element'>\n");

    /*** Cronograma de ejecución ***/

    fprintf(f_salida_html, "  <!-- T-Cronograma -->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\" align=\"center\">\n", BG_HEADER);
    fprintf(f_salida_html, "  <td width=\"%d\">PC</td>\n", PC_CRONO_W);
    fprintf(f_salida_html, "  <td width=\"%d\">Instrucci&oacute;n</td>\n", INSTR_CRONO_W);
    fprintf(f_salida_html, "  <!-- T-Linea -->\n");
    fprintf(f_salida_html, "  <!-- T-PC %s -->\n", "PC");
    fprintf(f_salida_html, "  <!-- T-Instr %s -->\n", "Instruccion");
    for (x = 0; x < MAX(MAX_CICLOS_INIC, maxima_columna_visible); x++) {
        fprintf(f_salida_html, "  <td width=\"%d\"><a href=\"%s%03u.html\">%u</a></td>\n",
                FASE_CRONO_W, archivo, primer_ciclo + x, primer_ciclo + x);
        fprintf(f_salida_html, "  <!-- T-Ciclo-H %u %u -->\n", x + primer_ciclo, x + primer_ciclo);
    }
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "  <!-- T-Fin-Linea -->\n");

    for (y = 0; y < num_instrucciones; y++) {
        if (y % 2 == 0)
            fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_VAL1);
        else
            fprintf(f_salida_html, "<tr bgcolor=\"%s\" align=\"center\">\n", BG_VAL2);
        fprintf(f_salida_html, "  <!-- T-Linea -->\n");

        if (!strlen(crono[y].inst)) {
            if (y % 2 == 0) {
                fprintf(f_salida_html,
                        "  <td bgcolor=\"%s\">&nbsp;</td>\n", BG_DIT1);
                fprintf(f_salida_html,
                        "  <td bgcolor=\"%s\">&nbsp;</td>\n", BG_DIT1);
            } else {
                fprintf(f_salida_html,
                        "  <td bgcolor=\"%s\">&nbsp;</td>\n", BG_DIT2);
                fprintf(f_salida_html,
                        "  <td bgcolor=\"%s\">&nbsp;</td>\n", BG_DIT2);
            }
        } else {
            if (y % 2 == 0) {
                fprintf(f_salida_html,
                        "  <td align=\"left\" bgcolor=\"%s\">%s</td>\n", BG_DIT1,
                        crono[y].PC);
                fprintf(f_salida_html,
                        "  <td align=\"left\" bgcolor=\"%s\">%s</td>\n", BG_DIT1,
                        crono[y].inst);
            } else {
                fprintf(f_salida_html,
                        "  <td align=\"left\" bgcolor=\"%s\">%s</td>\n", BG_DIT2,
                        crono[y].PC);
                fprintf(f_salida_html,
                        "  <td align=\"left\" bgcolor=\"%s\">%s</td>\n", BG_DIT2,
                        crono[y].inst);
            }
            fprintf(f_salida_html, "  <!-- T-PC %s -->\n", crono[y].PC);
            fprintf(f_salida_html, "  <!-- T-Instr  %s -->\n", crono[y].inst);
        }

        for (x = 0; x < MAX(MAX_CICLOS_INIC, maxima_columna_visible); x++) {
            fprintf(f_salida_html, "  <td align=\"center\">");

            if (strlen(crono[y].fase[x])) {
                fprintf(f_salida_html, "%s", crono[y].fase[x]);
                fprintf(f_salida_html, "</td>\n");
                fprintf(f_salida_html, "  <!-- T-Ciclo-D %d %s -->\n", x + 1, crono[y].fase[x]);
            } else {

                fprintf(f_salida_html, "&nbsp;");
                fprintf(f_salida_html, "</td>\n");
                fprintf(f_salida_html, "  <!-- T-Ciclo-D %d %s -->\n", x + 1, "BLANCO");
            }
            fprintf(f_salida_html, "</td>\n");
        } /* endfor */
        fprintf(f_salida_html, "</tr>\n");
        fprintf(f_salida_html, "  <!-- T-Fin-Linea -->\n");
    } /* endfor */

    fprintf(f_salida_html, "</table><br>\n\n");
    fprintf(f_salida_html, "  <!-- T-Fin-Cronograma -->\n");
} /* end imprime_dit */




/***** Funciones Exportables **********************************/



void vuelca_memoria(
        programa_t *prog
) {

    dword i;
    dword d;
    uword dump = 0;
    valor_t dato;
    boolean dirty;

    instruccion_t inst;
    int opcode, rs1, rs2, rs3, rd, rm, imm;
    formato_t fmt;
    char nemotecnico[LONG_STR];
    char texto[LONG_STR];

    FILE *f_dump;

    char dump_name[PATH_MAX + 5];

    sprintf(dump_name, "%s.dump", prog->l_objs->ts.fichero);
    f_dump = fopen(dump_name, "w");

    fprintf(stderr, "Escribiendo el mapa de memoria en: %s\n", dump_name);

    for (region_t r = 0; r < num_regiones; r++) {
        if (!es_region_datos(r) || tam_region(mmem, r) == 0)
            continue;
        fprintf(f_dump, "; === .data ===\n");
        for (d = inicio_region(mmem, r); d < final_region(mmem, r); d += 4) {
            i = dir_a_offset(mmem, d, r);
            dato = mem_lee_region_datos(mmem, r, i, t_word, &dirty);
            dump = dato.int_d;
            fprintf(f_dump, "%08lx %08x ; %ld %d \n", d, dump, d, dump);
        }
    }

    fprintf(f_dump, "; === .text  ===\n");
    for (i = inicio_region(mmem, r_text); i < final_region(mmem, r_text); i += 4) {
        inst = lee_mem_instruc(mmem, i);
        dump = inst.valor;
        fn_riscv_decode(i, dump, &fmt, &opcode, &rs1, &rs2, &rs3, &rd, &rm, &imm,
                        nemotecnico, texto);
        fprintf(f_dump, "%08lx %08x ; %s\n", i, dump, texto);

    }

    fclose(f_dump);
}


/**
 * Inicializa una instrucción en el cronograma
 *
 */
void init_instruc(
        word PC,
        ciclo_t orden
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    int o;
    char aux[SHORT_STR];
    char dir[SHORT_STR];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    if (orden == primera_instruccion + MAX_INSTRUC)
        desplazamiento_vertical();

    o = orden - primera_instruccion;

    te_etiqueta(dir, PC);
    imprime_instruccion_color(aux, PC, SI);
    // sprintf(crono[o].inst, "%s: %s", PC, aux);
    sprintf(crono[o].PC, "%s", dir);
    sprintf(crono[o].inst, "%s", aux);

} /* end init_instruc */


/**
 *  Actualiza el cronograma con el estado de las fases
 *
 */
void actualiza_crono() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    int i;
    char aux[SHORT_STR];
    char fase[SHORT_STR];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(fase, "%s", "IF");
    if (IFstall) {
        //        sprintf(fase, "<i>%s</i>", "if");
        sprintf(fase, "%s", "if");
    }
    if (IFnop) {
        sprintf(aux, "%s", fase);
        //sprintf(fase,"<b>%s</b>",aux);
        //sprintf(fase,"<font color=\"red\">%s</font>", aux);
        sprintf(fase, "%s", aux);
    }
    muestra_fase(fase, orden);

    if (IF_ID.orden != -1) {
        sprintf(fase, "%s", "ID");
        if (IDstall) {
            //            sprintf(fase, "<i>%s</i>", "id");
            sprintf(fase, "%s", "id");
        }
        if (IDnop) {
            sprintf(aux, "%s", fase);
            //sprintf(fase,"<b>%s</b>",aux);
            //sprintf(fase,"<font color=\"red\">%s</font>", aux);
            sprintf(fase, "%s", aux);
        }
        muestra_fase(fase, IF_ID.orden);
    }

    if (ID_EX.orden != -1)
        muestra_fase("EX", ID_EX.orden);

    if (EX_MEM.orden != -1)
        muestra_fase("ME", EX_MEM.orden);

    //    if (MEM_WB.orden != -1)
    //        muestra_fase("WB", MEM_WB.orden);

    if (ID_FP_LS.orden != -1)
        // muestra_fase("L1", ID_LS.orden);
        muestra_fase(FP_LS_LAT == 2 ? "EX" : "L1", ID_FP_LS.orden); // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion

    for (i = 2; i <= FP_LS_LAT; i++) {
        if (FP_LS[i].orden != -1) {
            sprintf(aux, "L%d", i);
            //muestra_fase(aux, LS[i].orden);
            muestra_fase(FP_LS_LAT == 2 && i == 2 ? "ME" : aux, FP_LS[i].orden); // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion
        }
    }

    if (ID_FP_ADD.orden != -1)
        muestra_fase("A1", ID_FP_ADD.orden);

    for (i = 2; i <= FP_ADD_LAT; i++) {
        if (FP_ADD[i].orden != -1) {
            sprintf(aux, "A%d", i);
            muestra_fase(aux, FP_ADD[i].orden);
        }
    }

    if (ID_FP_CMP.orden != -1)
        muestra_fase("C1", ID_FP_CMP.orden);

    for (i = 2; i <= FP_CMP_LAT; i++) {
        if (FP_CMP[i].orden != -1) {
            sprintf(aux, "C%d", i);
            muestra_fase(aux, FP_CMP[i].orden);
        }
    }


    if (ID_FP_MUL.orden != -1)
        muestra_fase("M1", ID_FP_MUL.orden);

    for (i = 2; i <= FP_MUL_LAT; i++) {
        if (FP_MUL[i].orden != -1) {
            sprintf(aux, "M%d", i);
            muestra_fase(aux, FP_MUL[i].orden);
        }
    }

    if (ID_INT_MUL.orden != -1)
        muestra_fase("M1", ID_INT_MUL.orden);

    for (i = 2; i <= FP_MUL_LAT; i++) {
        if (INT_MUL[i].orden != -1) {
            sprintf(aux, "M%d", i);
            muestra_fase(aux, INT_MUL[i].orden);
        }
    }

    if (ID_FP_MISC.orden != -1)
        muestra_fase("X1", ID_FP_MISC.orden);

    for (i = 2; i <= FP_MISC_LAT; i++) {
        if (FP_MISC[i].orden != -1) {
            sprintf(aux, "X%d", i);
            muestra_fase(aux, FP_MISC[i].orden);
        }
    }

    if (ID_FP_CVT.orden != -1)
        muestra_fase("Y1", ID_FP_CVT.orden);

    for (i = 2; i <= FP_CVT_LAT; i++) {
        if (FP_CVT[i].orden != -1) {
            sprintf(aux, "Y%d", i);
            muestra_fase(aux, FP_CVT[i].orden);
        }
    }

    if (FP_WB.orden != -1)
        muestra_fase("WB", FP_WB.orden);

    if (iWB.orden != -1)
        muestra_fase("WB", iWB.orden);



} /* end actualiza_crono */


/**
 * Imprime crono, etapas, señales, regs y mem 
 *
 */
void imprime_css() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[MAX_TEXTO_CONSOLA];


    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Para pantallas pequeñas, caben 10 instrucciones en el dit */

    sprintf(aux, "result%03u.html", Ciclo);
    if (html_merge == SI) {
        fprintf(f_salida_html, "'%s':`", aux);
    } else {
        f_salida_html = fopen(aux, "w");
        if (f_salida_html == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */


    /*** HTML INTRO ***/

    imprime_w3css("Resultados");

    /* Barra de navegación */
    imprime_nav2_css("result");
    fprintf(f_salida_html, "<br><br><br>\n");

    if (html_merge)
      fprintf(f_salida_html,
	      "<div class='w3-container w3-cell'>"
	      "<div class='w3-container'>"
	      "Ver en otra ventana:"
	      "<button id='chrono'>Cronograma</button>"
	      "<button id='path'>Ruta</button>"
	      "<button id='state'>Estado</button>"
	      "</div>"
	      "</div>"
	      "<br>"
	      );

    /** CSS - responsive general */
    fprintf(f_salida_html, "<div class=\"w3-responsive\">\n");

    fprintf(f_salida_html, "<!-- Cronograma y señales -->\n");

    /** CSS - bloque izquierdo */
    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");

    /** CSS - Bloque cronograma **/
    fprintf(f_salida_html, "<div class=\"w3-container\">\n");
    //    fprintf(f_salida_html, "<div class=\"w3-col w3-container\" style=\"width:60%%\">\n");

    /*** Cronograma de ejecución ***/

    if (SHOW_DIT) imprime_dit(MAX_INSTRUC, "result");
    fprintf(f_salida_html, "</div>\n");
    fprintf(f_salida_html, "<br>\n");



    /*** CSS - Bloque Datapath ***/
    fprintf(f_salida_html, "<div class=\"w3-container w3-raw\" id='path_element'>\n");

    if ((!hay_float) && (!hay_double) && (!hay_imul)) {
        if (SHOW_DATAPATH)
            dibuja_datapath();
    } else {
        if (SHOW_DATAPATH) {
            imprime_etapas();
            imprime_senyales();
        }
    }
    fprintf(f_salida_html, "</div>\n");
    fprintf(f_salida_html, "<br>\n");

    /*** CSS - Consola */
    if (strlen(CONSOLA) > 0) {
        if (SHOW_CONSOLE) {
            fprintf(f_salida_html, "<div class=\"w3-container w3-raw w3-black w3-text-green\">\n");
            fprintf(f_salida_html, "<!-- CONSOLA -->\n");
            fprintf(f_salida_html, "<br>\n");
            fprintf(f_salida_html, "CONSOLA:\n");
            fprintf(f_salida_html, "<br>\n");
            fprintf(f_salida_html, "%s\n\n", CONSOLA);
            fprintf(f_salida_html, "<br><br>\n");
            fprintf(f_salida_html, "</div>\n");
            fprintf(f_salida_html, "<br>\n");
        }
    }

    /* CSS - Log */
    if (strlen(LOG) > 0) {
        if (SHOW_LOG) {
            fprintf(f_salida_html, "<div class=\"w3-raw w3-container w3-light-grey\">\n");
            fprintf(f_salida_html, "<!-- LOG -->\n");
            fprintf(f_salida_html, "<br>\n");
            fprintf(f_salida_html, "LOG:\n<br>");
            fprintf(f_salida_html, "%s\n<br>", LOG);
            fprintf(f_salida_html, "<br>\n");
            fprintf(f_salida_html, "</div>\n\n");
            fprintf(f_salida_html, "<br>\n");
        }
    }

    /*** CSS - Fin bloque izquierdo */
    fprintf(f_salida_html, "</div>\n");
    fprintf(f_salida_html, "<!-- Fin Cronograma y señales -->\n\n");


    /*** CSS - Bloque derecho */

    fprintf(f_salida_html, "<!-- Resto: Reg Int, Reg FP, Memoria -->\n");
    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\" id='state_element'>\n");


    /*** Registros int ***/
    //fprintf(f_salida_html, "<div class=\"w3-container w3-cell \">\n");
    fprintf(f_salida_html, "<div class=\"w3-cell\">\n");
    //    fprintf(f_salida_html, "<div class=\"w3-col w3-container\" style=\"width:12%%\">\n");
    if (SHOW_REG)
        imprime_int_regs_col();
    fprintf(f_salida_html, "</div>\n");

    if ((hay_float) || (hay_double)) {
        /*** Registros fp ***/
        //fprintf(f_salida_html, "<div class=\"w3-container w3-cell \">\n");

        fprintf(f_salida_html, "<div class=\"w3-cell\">\n");
        //    fprintf(f_salida_html, "<div class=\"w3-col w3-container\" style=\"width:12%%\">\n");
        if (SHOW_REG)
            imprime_fp_regs_col();
        fprintf(f_salida_html, "</div>\n");
    }

    /*** Memoria ***/
    //   fprintf(f_salida_html, "<div class=\"w3-container w3-cell \">\n");
    fprintf(f_salida_html, "<div class=\"w3-cell\">\n");
    //    fprintf(f_salida_html, "<div class=\"w3-col w3-container\" style=\"width:12%%\">\n");
    if (SHOW_MEM)
        imprime_Dmem_col();
    fprintf(f_salida_html, "</div>\n\n");

    /** CSS - Fin bloque derecho */
    fprintf(f_salida_html, "</div>\n");
    fprintf(f_salida_html, "<!-- Fin Resto -->\n\n");
    fprintf(f_salida_html, "</div>\n");

    /** CSS - Fin */
    //fprintf(f_salida_html, "<br>\n");
    fprintf(f_salida_html, "</div>\n");

    /** CSS - Fin responsive */
    //fprintf(f_salida_html, "<br>\n");
    fprintf(f_salida_html, "</div>\n");


    /*** Final ***/
    imprime_footer_css();
    fprintf(f_salida_html, "</div>\n");

    if (html_merge == SI) {
        fputs("`,", f_salida_html);
    } else {

        fclose(f_salida_html);
    } /* endif */


} /* end imprime_css */


/**
 * Imprime la configuracion del simulador
 *
 */
void imprime_inicio_css() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[SHORT_STR];


    /*************************************/
    /*  Function body                    */
    /*************************************/



    /* Si solo hay enteros, ajustar dit, dibujar datapath */

    if (html_merge == SI) {
        sprintf(aux, "%s.htm", "index");
    } else {
        sprintf(aux, "index.html");
    } /* endif */

    f_salida_html = fopen(aux, "w");
    if (f_salida_html == NULL) {

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
                "\n"
                "</style>\n"
                "\n"
                "<script>\n"
                "\n"
                "var files={\n"
                "'index.html':`",
                f_salida_html);
    } /* endif */

    /*** HTML INTRO ***/

    imprime_w3css("Inicio");

    /* Barra de navegación */
    imprime_nav1_css();
    fprintf(f_salida_html, "<br><br><br>\n");



    /*** Configuración ***/
    fprintf(f_salida_html, "<div class=\"w3-row w3-container\">\n");
    imprime_conf();
    fprintf(f_salida_html, "<br>\n");
    fprintf(f_salida_html, "</div>\n\n");



    /*** Memorias ***/
    fprintf(f_salida_html, "<div class=\"w3-row\">\n");

    /*** Contenido de la memoria de datos ***/
    fprintf(f_salida_html, "<div class=\"w3-cell w3-container\">\n");


    imprime_Dmem_col();

    fprintf(f_salida_html, "</div>\n");

    /*** Contenido de la memoria de instrucciones ***/
    fprintf(f_salida_html, "<div class=\"w3-cell w3-container\">\n");

    imprime_Imem_col(show_format);
    fprintf(f_salida_html, "</div>\n");
    fprintf(f_salida_html, "</div>\n");


    /*** Final ***/
    imprime_footer_css();

    if (html_merge == SI) {
        fputs("`,", f_salida_html);
    } else {

        fclose(f_salida_html);
    } /* endif */


} /* end imprime_inicio */


/**
 * Imprime la configuracion y resultados finales
 *
 */
void imprime_final_css() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[MAX_TEXTO_CONSOLA];


    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(aux, "final.html");
    if (html_merge == SI) {
        fprintf(f_salida_html, "'%s':`", aux);
    } else {
        f_salida_html = fopen(aux, "w");
        if (f_salida_html == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */


    /*** HTML INTRO ***/

    imprime_w3css("Final");

    /* Barra de navegación */
    imprime_nav1_css();
    fprintf(f_salida_html, "<br><br><br>\n");


    fprintf(f_salida_html, "<div class=\"w3-row w3-container\">\n");

    /*** Resultados de la ejecucion ***/

    fprintf(f_salida_html, "<table>\n");
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "<td colspan=\"5\" bgcolor=\"%s\" align=\"center\">Resultados</td>\n", BG_HEADER);
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_DIR1);
    fprintf(f_salida_html, "  <td>Ciclos&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>Instrucciones&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>CPI&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>Op. CF&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>Op. CF/Ciclo&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_DIR2);
    fprintf(f_salida_html, "  <td>%ld</td>\n", estat.ciclos);
    fprintf(f_salida_html, "  <td>%ld</td>\n", estat.instrucciones);
    fprintf(f_salida_html, "  <td>%1.2f</td>\n", (float) estat.ciclos / estat.instrucciones);
    fprintf(f_salida_html, "  <td>%ld</td>\n", estat.flops);
    fprintf(f_salida_html, "  <td>%1.2f</td>\n", (float) estat.flops / estat.ciclos);
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "</table><br>\n");
    fprintf(f_salida_html, "</div>\n\n");


    /*** Configuración ***/
    fprintf(f_salida_html, "<div class=\"w3-row w3-container\">\n");
    imprime_conf();
    fprintf(f_salida_html, "<br>\n");
    fprintf(f_salida_html, "</div>\n\n");



    fprintf(f_salida_html, "<div class=\"w3-row w3-container\">\n");


    /* Banco de registros enteros */

    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");
    imprime_int_regs_col();
    fprintf(f_salida_html, "</div>\n\n");

    /* Banco de registros fp */

    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");
    imprime_fp_regs_col();
    fprintf(f_salida_html, "</div>\n\n");


    /*** Contenido de la memoria de datos ***/

    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");
    imprime_Dmem_col();
    fprintf(f_salida_html, "</div>\n\n");


    /*** Contenido de la memoria de instrucciones ***/

    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");
    imprime_Imem_col(format_none);
    fprintf(f_salida_html, "</div>\n\n");

    fprintf(f_salida_html, "</div>\n\n");
    fprintf(f_salida_html, "<br>\n");

    /* Consola */

    if (SHOW_CONSOLE) {
        fprintf(f_salida_html, "<div class=\"w3-raw w3-container w3-black w3-text-green\">\n");
        fprintf(f_salida_html, "<br>\n");
        fprintf(f_salida_html, "CONSOLA:\n<br>");
        f_consola = fopen(console_name, "r");
        while (fgets(aux, MAX_TEXTO_CONSOLA, f_consola) != NULL) {
            fprintf(f_salida_html, "%s<br>\n", aux);
        }
        fclose(f_consola);
        fprintf(f_salida_html, "\n<br>\n");

        fprintf(f_salida_html, "</div>\n\n");
        fprintf(f_salida_html, "<br>\n");
    }

    /*** Final ***/
    imprime_footer_css();

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
	      "      iframedoc.writeln(files[file]);\n"
	      "      iframedoc.close();\n"
	      "   }\n"
	      "   else\n"
	      "   {\n"
	      "      alert('Cannot inject dynamic contents into iframe.');\n"
	      "   }\n"
	      "}\n"
	      "\n"
	      "var child_windows = {\n"
	      "    'chrono': { title: 'Cronograma', features: 'width=800, height=400'},\n"
	      "    'path': { title: 'Ruta de datos', features: 'width=800, height=600'},\n"
	      "    'state': { title: 'Estado', features: 'width=800, height=800'},\n"
	      "};\n"
	      "\n"
	      "function update_child_window(id) {\n"
	      "    if (!child_windows[id].window || child_windows[id].window.closed) return;\n"
	      "\n"
	      "    var document = child_windows[id].window.document;\n"
	      "\n"      
	      "    document.removeEventListener('click', check_click);\n"
	      "    document.open();\n"
	      "    document.write(`<!DOCTYPE html>\n"
	      "       <html lang='es-ES'>\n"
	      "       <head>\n"
	      "       <meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>\n"
	      "       <meta name='viewport' content='width=device-width, initial-scale=1'>\n"
	      "       <link rel='stylesheet' href='https://www.w3schools.com/lib/w3.css'>\n"
	      "       <style>\n"
	      "       html, body {\n"
	      "       font-family:'Courier', Times, serif;\n"
	      "       font-size:10pt;\n"
	      "       line-height:100%;\n"
	      "       }\n"
	      "       .field { display: inline-block; font-size: 80%; height: 16px; text-align: center; padding: 4px 0px 0px 0px; border: 1px solid black}\n"
	      "       .texto { display: inline-block;  width: 50px; }\n"
	      "       </style>\n"
	      "       </head>\n"
	      "       <body>\n"
	      "    `);\n"
	      "\n"
	      "    var iframedoc = get_iframedoc();"
	      "\n"
	      "    var nav_bar = iframedoc.getElementById('nav_bar');\n"
	      "    if (nav_bar) document.body.appendChild(nav_bar.cloneNode(true));\n"
	      "\n"
	      "    document.write('<br><br><br>');\n"
	      "\n"
	      "    var element = iframedoc.getElementById(id+'_element');\n"
	      "    if (element) document.body.appendChild(element.cloneNode(true));\n"
	      "\n"
	      "    document.write('</body></html>');\n"
	      "    document.close();\n"
	      "    document.title = iframedoc.title;\n"
	      "    document.addEventListener('click', check_click);\n"
	      "}\n"
	      "\n"  
	      "function check_click(event)\n"
	      "{\n"
	      "    switch (event.target.tagName) {\n"      
	      "    case 'A':\n"
	      " var iframedoc = get_iframedoc();\n"
	      "\n"
	      "	event.preventDefault();\n"
	      "	iframedoc.removeEventListener('click', check_click);\n"
	      "	load_file(event.target.href.split('/').pop());\n"
	      "	iframedoc.addEventListener('click', check_click);\n"
	      "\n"
	      "	for (var id in child_windows) update_child_window(id);\n"
	      "\n"
	      "	break;\n"
	      "\n"	
	      "    case 'BUTTON':\n"
	      "	var id = event.target.id;\n"
	      "\n"	
	      "	child_windows[id].window = window.open('', child_windows[id].title, child_windows[id].features);\n"
	      "	update_child_window(id);\n"
	      "    }\n"
	      "}\n"
	      "\n"
	      "function init_page()\n"
	      "{\n"
	      "   var iframedoc = get_iframedoc();\n"
	      "\n"
	      "   document.title = iframedoc.title;\n"
	      "   iframedoc.addEventListener('click', check_click);\n"
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
	      f_salida_html);
    } /* endif */

    fclose(f_salida_html);

} /* end imprime_final_css */
