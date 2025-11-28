/**
 * @file presentacion.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las funciones necesarias para presentar la
 *    información sobre la ejecución y el estado del procesador
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define presentacion_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memoria.h>
#include <enlazador.h>
#include <simbolos.h>

#include "tipos.h"
#include "main.h"
#include "codop.h"
#include "instrucciones.h"
#include "presentacion.h"
#include "g_string.h"

typedef enum {
    MF_OCIOSO = 0,
    MF_ESCRITURA = 1,
    MF_LECTURA = 2
} modo_firmas_t;

#define LONG_FIRMA 20480
#define FIRMA_DEL_FICHERO "<Fichero de firmas> AIC"

/***** Variables Globales **************************************/

presentacion_interface_t presentacion = {
        .imprime_inicio_f = imprime_inicio_null,
        .imprime_final_f = imprime_final_null,
        .inicio_ciclo_f = inicio_ciclo_null,
        .fin_ciclo_f = fin_ciclo_null,
        .imprime_estado_f = imprime_estado_null,
        .imprime_crono_f = imprime_crono_null,
        .imprime_predictor_f = imprime_predictor_null,
        .imprime_memoria_f = imprime_memoria_null,
        .init_instruc_f = init_instruc_null,
        .muestra_fase_f = muestra_fase_null,
        .muestra_datos_f = muestra_datos_null
};

/***** Variables Locales **************************************/

static FILE *f_firmas;
static modo_firmas_t modo_firmas = MF_OCIOSO;
static char linea_firma[LONG_FIRMA];

/***** Macros *************************************************/

/***** Funciones Locales **************************************/

/* reverse:  reverse string s in place */
void reverse(char s[]) {
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[], int radix) {
    int i, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n; /* make n positive */
    i = 0;
    do { /* generate digits in reverse order */
        s[i++] = n % radix + '0'; /* get next digit */
    } while ((n /= radix) > 0); /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

/***************************************************************
 *
 * Func: crea_firma
 *
 * Description:
 *   Crea un fichero de firmas
 *
 **************************************************************/

void crea_firma
        (
                char *fichero
        ) {
    /*************************************/
    /*  Function body                    */
    /*************************************/

    f_firmas = fopen(fichero, "w");
    if (f_firmas == NULL) {
        fprintf(stderr, "Error creando %s\n", fichero);
        exit(1);
    } /* endif */

    modo_firmas = MF_ESCRITURA;
    fprintf(f_firmas, FIRMA_DEL_FICHERO);

} /* end crea_firma */

/***************************************************************
 *
 * Func: abre_firma
 *
 * Description:
 *   Abre un fichero de firmas ya existente
 *
 **************************************************************/

void abre_firma(
        char *fichero
) {

    char *lin;

    f_firmas = fopen(fichero, "r");
    if (f_firmas == NULL) {
        fprintf(stderr, "Error abriendo %s\n", fichero);
        exit(1);
    } /* endif */

    modo_firmas = MF_LECTURA;

    /*** Comprueba que es un fichero de firmas ***/

    lin = fgets(linea_firma, LONG_FIRMA, f_firmas);
    if (lin == NULL) {
        fprintf(stderr, "Error leyendo el fichero de firmas\n");
        exit(1);
    } /* endif */

    printf(" Leido %s\n", FIRMA_DEL_FICHERO);
    /* Comprueba que la firma es correcta */
    if (strcmp(linea_firma, FIRMA_DEL_FICHERO "\n")) {
        fprintf(stderr, "ERROR. El fichero NO es un fichero de firmas.\n");
        exit(1);
    } /* endif */

} /* end abre_firma */

/***************************************************************
 *
 * Func: ignora_firma
 *
 * Description:
 *   Pasa a modo ocioso
 *
 **************************************************************/

void ignora_firma() {
    modo_firmas = MF_OCIOSO;

} /* end ignora_firma */

/***************************************************************
 *
 * Func: nueva_firma
 *
 * Description:
 *   Comienza la firma de un nuevo ciclo
 *
 **************************************************************/

void nueva_firma(
) {

    char *tok;
    char s_ciclo[8];

    sprintf(s_ciclo, F_CICLO, Ciclo);

    switch (modo_firmas) {
        case MF_LECTURA: {
            char *lin;
            lin = fgets(linea_firma, LONG_FIRMA, f_firmas);
            if (lin == NULL) {
                fprintf(stderr, "Error leyendo el fichero de firmas\n");
                exit(1);
            } /* endif */

            /* Comprueba que el ciclo es el correcto */
            tok = strtok(linea_firma, "@");
            if (strcmp(tok, s_ciclo)) {
                fprintf(stderr, "ERROR. "
                                "El ciclo actual (%s) no coincide con el ciclo del fichero de firmas (%s).\n"
                                "Seguramente no deberías haber alcanzado este ciclo\n", s_ciclo, tok);
                exit(1);
            } /* endif */
        }
            break; /* endcase */
        case MF_ESCRITURA: {
            fprintf(f_firmas, "\n%s@", s_ciclo);
        }
            break; /* endcase */
        default:
            break;
    } /* endswitch */

} /* end nueva_firma */

/***************************************************************
 *
 * Func: procesa_firma
 *
 * Description:
 *   MF_ESCRITURA: Añade un trozo de la firma correspondiente al ciclo actual.
 *   MF_LECTURA: Comprueba si uno de los campos de la firma es correcto.
 *
 *   En cualquier caso devuelve la cadena que se debe insertar
 *   como valor.
 *
 **************************************************************/

char *procesa_firma(
        char *campo,
        char *valor
) {

    char *tok;
    static char msg[128];

    strcpy(msg, valor);

    switch (modo_firmas) {
        case MF_LECTURA:
            tok = strtok(NULL, "$");
            if (tok == NULL) {
                fprintf(stderr, "%s: %s\n", campo, valor);
                fprintf(stderr, "Error leyendo el fichero de firmas: %d\n", __LINE__);
                exit(1);
            } /* endif */
            if (strcmp(tok, campo)) {
                fprintf(stderr, "ERROR. "
                                "Los nombres de los campos no coinciden con los del fichero de firmas.\n"
                                "Probablemente exista un error en la configuración del simulador\n");
                exit(1);
            } /* endif */

            tok = strtok(NULL, "@");
            if (tok == NULL) {
                fprintf(stderr, "%s: %s\n", campo, valor);
                fprintf(stderr, "Error leyendo el fichero de firmas: %d\n", __LINE__);
                exit(1);
            } /* endif */
            if (strcmp(tok, valor)) {
                if (!strcmp(valor, "&nbsp;"))
                    strcpy(msg, "<i><font color=\"red\"><b> ??? </b></font></i>");
                    /*
                    else if (!strcmp(valor, "")) // Campos alfanuméricos
                        strcpy(msg, "<i><font color=\"red\"><b> ??? </b></font></i>");
                     */
                else
                    sprintf(msg, "<i><font color=\"red\"><b>%s</b></font></i>", valor);

                error_firma = SI;
                return msg;
            } /* endif */
            break;
        case MF_ESCRITURA:
            fprintf(f_firmas, "%s$%s@", campo, valor);
            break;
        default:
            break;
    } /* endswitch */

    return msg;

} /* end procesa_firma */

/***************************************************************
 *
 * Func: cierra_firma
 *
 * Description:
 *   Cierra el fichero de firmas
 *
 **************************************************************/

void cierra_firma() {

    fprintf(f_firmas, "\n");
    fclose(f_firmas);

} /* end cierra_firma */

void color_etiqueta_memdatos(
        char *color,
        dword direccion
) {
    int index;

    ts_busca_direccion(programa.simbolos, direccion, &index);

    static char *paleta[15] = {
            "red", "darkcyan", "orange", "limegreen", "deeppink", "royalblue", "brown", "seagreen",
            "lightcoral", "magenta", "steelblue", "salmon", "mediumseagreen", "darkorange", "orchid"
    };

    strcpy(color, paleta[index % 15]);

}

/***************************************************************
 *
 * Func: imprime_instruccion
 *
 * Parametros:
 *	instruc  (char *) ;
 *
 * Descripción:
 *   Imprime la intrucción localizada en el PC sobre la
 *   cadena de caracteres 'instruc'
 *
 **************************************************************/

void __imprime_instruccion(
        char *instruc,
        dword PC,
        char *file,
        int line
) {

    if (PC % 4 != 0) {
        fprintf(stderr, "%s:%d ERROR: Dirección incorrecta al imprimir una instrucción (" F_DWORD ")\n", file, line,
                PC);
    }

    if (!en_region(mmem, PC, r_text)) {
        sprintf(instruc, "&lt;nop&gt;");
        return;
    } /* endif */

    imprime_instruccion_color(instruc, PC, SI);

} /* end imprime_instruccion */

/***************************************************************
 *
 * Func: imprime_codigo
 *
 * Description:
 *   Obtiene el nombre de una estación de reserva
 *
 **************************************************************/

void imprime_codigo(
        marca_t s,
        char *nom
) {

    if (/* s >= INICIO_RS_ENTEROS && */ /* Es siempre cierto en está
                                           implementación */
            s <= FIN_RS_ENTEROS) {
        sprintf(nom, "e%d", s - INICIO_RS_ENTEROS + 1);
    } else if (s >= INICIO_RS_SUMREST &&
               s <= FIN_RS_SUMREST) {
        sprintf(nom, "a%d", s - INICIO_RS_SUMREST + 1);
    } else if (s >= INICIO_RS_MULTDIV &&
               s <= FIN_RS_MULTDIV) {
        sprintf(nom, "m%d", s - INICIO_RS_MULTDIV + 1);
    } else if (s >= INICIO_BUFFER_CARGA &&
               s <= FIN_BUFFER_CARGA) {
        sprintf(nom, "l%d", s - INICIO_BUFFER_CARGA + 1);
    } else if (s >= INICIO_BUFFER_ALMACEN &&
               s <= FIN_BUFFER_ALMACEN) {
        sprintf(nom, "s%d", s - INICIO_BUFFER_ALMACEN + 1);
    } else {
        nom[0] = '\0'; /* Invalido o marca nula */
    } /* endif */

} /* end imprime_codigo */

/***************************************************************
 *
 * Func: imprime_operacion
 *
 * Description:
 *   Obtiene el nombre de una estación de reserva
 *
 **************************************************************/

void imprime_operacion(
        codop_t c,
        char *nom
) {

    sprintf(nom, "%s", PRINTED(c));

} /* end imprime_operacion */

/***************************************************************
 *
 * Func: imprime_CONFIG(predic.tipo_predictor)
 *
 * Description:
 **************************************************************/

void imprime_tipo_predictor(
        char *aux
) {

    // TODO: Faltan tipos en el switch
    switch (CONFIG(predic.tipo_predictor)) {
        case BTB_1_BIT:
            sprintf(aux, "BTB 1 bit");
            break;
        case BTB_2_BIT_H:
            sprintf(aux, "BTB 2 bits con histéresis");
            break;
        case BTB_2_BIT_S:
            sprintf(aux, "BTB 2 bits con saturación");
            break;
        case BTB_n_BIT_H:
            sprintf(aux, "Local, %d bits con histéresis", CONFIG(predic.n_bits_pred_local));
            break;
        case BTB_n_BIT_S:
            sprintf(aux, "Local, %d bits con saturación", CONFIG(predic.n_bits_pred_local));
            break;
        case HYBRID_n_BIT_H:
            sprintf(aux, "Híbrido, %d sel -> %d bits con histéresis + BHSR [ %d | %d ] bits",
                    CONFIG(predic.n_bits_selector), CONFIG(predic.n_bits_pred_local), CONFIG(predic.n_bits_BHSR1),
                    CONFIG(predic.n_bits_BHSR2));
            break;
        case HYBRID_n_BIT_S:
            sprintf(aux, "Híbrido, %d sel -> %d bits con saturación + BHSR [ %d | %d ] bits",
                    CONFIG(predic.n_bits_selector), CONFIG(predic.n_bits_pred_local), CONFIG(predic.n_bits_BHSR1),
                    CONFIG(predic.n_bits_BHSR2));
            break;
        case PERFECTO:
            sprintf(aux, "Perfecto");
            break;
        case CLARIVIDENTE:
            sprintf(aux, "Clarividente");
            break;
        case PREDICT_NOT_TAKEN:
            sprintf(aux, "Predict-Not-Taken");
            break;
        case TWO_LEVEL_n_BIT_H:
            sprintf(aux, "2 niveles, %d bits con histéresis + BHSR %d bits", CONFIG(predic.n_bits_pred_local),
                    CONFIG(predic.n_bits_BHSR1));
            break;
        case TWO_LEVEL_n_BIT_S:
            sprintf(aux, "2 niveles, %d bits con saturación + BHSR %d bits", CONFIG(predic.n_bits_pred_local),
                    CONFIG(predic.n_bits_BHSR1));
            break;
    }

}

void imprime_dato(
        zona_datos_t *datos,
        unsigned int i,
        char *output
) {
    switch (datos->tipos[i]) {
        case t_byte:
            /* Imprime también el carácter */
            sprintf(output, "%hhX %c", datos->mem.m_byte[indice_byte(i)], datos->mem.m_byte[indice_byte(i)]);
            break;
        case t_half:
            sprintf(output, "%hd", datos->mem.m_half[indice_half(i)]);
            break;
        case t_word:
            sprintf(output, "%d", datos->mem.m_word[indice_word(i)]);
            break;
        case t_dword:
            sprintf(output, F_DWORD, datos->mem.m_dword[indice_dword(i)]);
            break;
        case t_float:
            sprintf(output, "%.2f", datos->mem.m_float[indice_float(i)]);
            break;
        case t_double:
            sprintf(output, "%.2f", datos->mem.m_double[indice_double(i)]);
            break;
        default:
            strcpy(output, "&nbsp;");
    }

}

void valor2str(char *dst, valor_t dato, tipo_t tipo_dato) {
    switch (tipo_dato) {
        case t_double:
            sprintf(dst, "%.2lf", dato.fp_d);
            break;
        case t_float:
            sprintf(dst, "%.2f", dato.fp_s);
            break;
        case t_float_ps:
            sprintf(dst, "%.2f|%.2f", dato.fp_ps[Hi], dato.fp_ps[Lo]);
            break;
        case t_char:
            sprintf(dst, "%c", (char) dato.int_d);
            break;
        case t_ninguno:
            strcpy(dst, "&nbsp;");
            break;
        default:
            sprintf(dst, F_DWORD, dato.int_d);
            break;
    }
}

void FIRMA_valor(char *cadena, int i, valor_t dato, tipo_t tipo_dato) {

    char aux[SHORT_STR];

    valor2str(aux, dato, tipo_dato);

    FIRMA_2_1(cadena, i, aux);
}

/**
 * Obtiene el tipo de una instrucción en función del código de operación
 *
 * @param codop
 * @return
 */
tipo_t obtener_tipo(codop_t codop) {
    if (inst_fp_d(codop))
        return (t_double);
    if (inst_fp_s(codop))
        return (t_float);
    if (inst_simd(codop))
        return (t_float_ps);
    return (t_dword);
}

/***************************************************************
 *
 * Func: imprime_config
 *
 * Desc: Imprime la configuración del simulador
 *
 **************************************************************/

void imprime_config() {

    G_STR_DECL(info);
    char aux[128];

    fprintf(stdout, "\n*** CONFIGURACIÓN ***\n\n");

    fprintf(stdout, "  Grado superescalar: \n");
    fprintf(stdout, "        ISSUE:        %d\n", CONFIG(num_vias.issue));
    fprintf(stdout, "        BUSES:        %d\n", CONFIG(num_vias.bus));
    fprintf(stdout, "        COMMIT:       %d\n", CONFIG(num_vias.commit));
    fprintf(stdout, "  Reorder Buffer:     %d\n", CONFIG(tam_reorder));
    fprintf(stdout, "  E.R. Suma/Resta:    %d\n", CONFIG(tam_rs.sumrest));
    fprintf(stdout, "  E.R. Mult/Div:      %d\n", CONFIG(tam_rs.multdiv));
    fprintf(stdout, "  E.R. Enteros:       %d\n", CONFIG(tam_rs.enteros));
    fprintf(stdout, "  Buffer carga:       %d\n", CONFIG(tam_rs.carga));
    fprintf(stdout, "  Buffer almacen.:    %d\n", CONFIG(tam_rs.almacen));
    imprime_tipo_predictor(aux);
    fprintf(stdout, "  Tipo predictor:     %s\n", aux);
    fprintf(stdout, "  Buffer predictor:   %d\n", CONFIG(predic.tam_buffer_predic));
    fprintf(stdout, "  Registros:          %d\n", TAM_REGISTROS);

    /*** Tiempos de evaluación ***/

    fprintf(stdout, "\n");

#define INFO_OPER(OP) CONFIG(OP.num_inicio), CONFIG(OP.teval), ((CONFIG(OP.num_inicio) != CONFIG(OP.num_oper)) ? "SEGM" : "CONV")

    fprintf(stdout, "                    Cantidad   Latencia  Tipo\n");
    fprintf(stdout, "                    --------   --------  ----\n");
    fprintf(stdout, "  Oper. Suma/Resta:  %3d         %3d     %s\n", INFO_OPER(sumrest));
    fprintf(stdout, "  Oper. Mult/Div:    %3d         %3d     %s\n", INFO_OPER(multdiv));
    fprintf(stdout, "  Oper. Enteros:     %3d         %3d     %s\n", INFO_OPER(enteros));
    if (!L1D.mem.activa) {
        fprintf(stdout, "  Oper. Memoria:     %3d         %3d     %s\n", INFO_OPER(memdatos));
    }

    fprintf(stdout, "\n");

    /*** Jerarquía de memoria ***/
    for (int i = NUM_ELEM_MEMORIA - 1; i >= 0; i--) {
        INVOCA_INTERFAZ_MEMORIA(elementos_memoria[i], obtiene_config, formato_txt, &info);
    }

    g_str_flush(stdout, &info);

    fprintf(stdout, "\n");

} /* end imprime_config */

void imprime_mapa_memoria() {
    int i;

    fprintf(stdout, "*** MEMORIA ***\n\n");

    fprintf(stdout, "begin  end    size  region\n");
    fprintf(stdout, "------ ------ ----- -------------------------\n");
    for (i = 0; i < num_regiones; i++) {
        if (tam_region(mmem, i) > 0) {
            fprintf(stdout, "%06" PRIx64 "-%06" PRIx64 " %05" PRIx64 " [%s]\n",
                    mmem->regiones[i].inicio,
                    mmem->regiones[i].final,
                    tam_region(mmem, i),
                    mmem->regiones[i].nombre);
            for (int i_obj = 0; i_obj < programa.n_objs; ++i_obj) {
                fichero_objeto_t *obj = &programa.l_objs[i_obj];
                if (tam_region(&obj->memoria, i) > 0) {
                    fprintf(stdout, "%06" PRIx64 "-%06" PRIx64 " %05" PRIx64 "     [<%s> : %s]\n",
                            obj->memoria.regiones[i].inicio,
                            obj->memoria.regiones[i].final,
                            tam_region(&obj->memoria, i),
                            obj->ts.fichero,
                            obj->memoria.regiones[i].nombre);
                }
            }
        }
    }
    fprintf(stdout, "\n");
}

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
            fprintf(f_dump, "%08" PRIx64 " %08x ; " F_DWORD " %d \n", d, dump, d, dump);
        }
    }

    fprintf(f_dump, "; === .text  ===\n");
    for (i = inicio_region(mmem, r_text); i < final_region(mmem, r_text); i += 4) {
        inst = lee_mem_instruc(mmem, i);
        dump = inst.valor;
        fn_riscv_decode(i, dump, &fmt, &opcode, &rs1, &rs2, &rs3, &rd, &rm, &imm,
                        nemotecnico, texto);
        fprintf(f_dump, "%08" PRIx64 " %08x ; %s\n", i, dump, texto);

    }

    fclose(f_dump);
}

/***************************************************************
 *
 * Func: imprime_estadisticas
 *
 * Desc: Imprime las estadísticas del programa
 *
 **************************************************************/

void imprime_estadisticas() {

    fprintf(stdout, "\n*** ESTADISTICAS ***\n\n");

    fprintf(stdout, "Ciclos:              %4ld\n", estat.ciclos);
    fprintf(stdout, "Instrucciones:       %4ld\n", estat.instrucciones);
    fprintf(stdout, "CPI / IPC:           %1.2f / %1.2f\n", (float) estat.ciclos / estat.instrucciones,
            (float) estat.instrucciones / estat.ciclos);
    fprintf(stdout, "Ops C.F:             %4ld\n", estat.flops);
    fprintf(stdout, "Ops C.F./ciclo:      %1.2f\n", (float) estat.flops / estat.ciclos);
    fprintf(stdout, "Saltos:\n");
    fprintf(stdout, "    Accedidos:       %4ld\n", estat.saltos_accedidos);
    fprintf(stdout, "    Encontrados:     %4ld\n", estat.saltos_encontrados);
    fprintf(stdout, "    Ejecutados:      %4ld\n", estat.saltos_ejecutados);
    fprintf(stdout, "    Acertados:       %4ld\n", estat.saltos_acertados);
    fprintf(stdout, "\n");
    fprintf(stdout, "Ocupación de recursos:\n");
    fprintf(stdout, "               Max / TAM  %% max  %% med\n");

#define PRINT_ESTAT(label, campo) \
do { \
    int size = FIN_##campo - INICIO_##campo +1;\
    fprintf(stdout, "  " label " %3d / %3d   (%3u%% - %3u%%)\n", max_util.campo, (size), (max_util.campo * 100 / (size)), (aver_util.campo * 100 / (Ciclo * size)));\
} while(0)

    PRINT_ESTAT("RS Enteros: ", RS_ENTEROS);
    PRINT_ESTAT("RS S/R C.F.:", RS_SUMREST);
    PRINT_ESTAT("RS M/D C.F.:", RS_MULTDIV);
    PRINT_ESTAT("RS Carga:   ", BUFFER_CARGA);
    PRINT_ESTAT("RS Almac.:  ", BUFFER_ALMACEN);

    PRINT_ESTAT("Op Enteros: ", OP_ENTEROS);
    PRINT_ESTAT("Op S/R C.F.:", OP_SUMREST);
    PRINT_ESTAT("Op M/D C.F.:", OP_MULTDIV);
    PRINT_ESTAT("Op Mem (AC):", OP_DIRECCIONES);
    PRINT_ESTAT("Op Mem:     ", OP_MEMDATOS);
    PRINT_ESTAT("RB:         ", REORDER);

    if (JERARQUIA_ACTIVA()) {
        fprintf(stdout, "\nJerarquía de memoria:\n\n");
        G_STR_DECL(info);

        /*** Jerarquía de memoria ***/
        for (int i = NUM_ELEM_MEMORIA - 1; i >= 0; i--) {
            if (!elementos_memoria[i]->activa) continue;

            muestra_estadisticas_nivel_txt(elementos_memoria[i], &info);
            g_str_flush(stderr, &info);
        }
    }

    fprintf(stdout, "\n\n");

} /* end imprime_estadisticas */

void imprime_inicio_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void imprime_final_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void inicio_ciclo_null(ciclo_t ciclo) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void fin_ciclo_null(ciclo_t ciclo) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void imprime_estado_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void imprime_crono_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void imprime_predictor_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void imprime_memoria_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void init_instruc_null(dword PC, ciclo_t orden, const char *overwrite) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void muestra_fase_null(char *fase, ciclo_t orden, boolean exception) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void muestra_datos_null(region_t r, dword from, dword to, mem_status_t status) {
    /* Esta función de presentación no hace nada intencionadamente */
}

