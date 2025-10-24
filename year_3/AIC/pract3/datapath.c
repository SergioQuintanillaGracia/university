/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *        Sergio Sáez (ssaez@disca.upv.es)
 *
 * Fichero: control.c
 *
 * Descripción:
 *   Contiene las funciones para dibujar la ruta de datos
 *
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "instrucciones.h"
#include "datapath.h"

#define imprime_valor(valor, maxval, cadena) { \
    if (abs(valor) < maxval) { \
        sprintf(cadena, "%ld", (long) valor); \
    } else { \
        if (valor>0) \
            sprintf(cadena, "++"); \
        else \
            sprintf(cadena, "--"); \
    } \
}

#define TAM_DIBUJO 2048


extern FILE *f_salida_html;

typedef struct {
    word ctrl;
    char color[20];
} muxctrl_t;

typedef struct {
    word x;
    word y;
} coord_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    char dibujo[TAM_DIBUJO];
} elem_1i1o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    coord_t in2;
    coord_t out2;
    char dibujo[TAM_DIBUJO];
} elem_2i2o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    coord_t in2;
    coord_t out2;
    coord_t in3;
    coord_t out3;
    char dibujo[TAM_DIBUJO];
} elem_3i3o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    coord_t in2;
    coord_t out2;
    coord_t in3;
    coord_t out3;
    coord_t in4;
    coord_t out4;
    char dibujo[TAM_DIBUJO];
} elem_4i4o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    coord_t in2;
    coord_t out2;
    coord_t in3;
    coord_t out3;
    coord_t in4;
    coord_t out4;
    coord_t in5;
    coord_t out5;
    char dibujo[TAM_DIBUJO];
} elem_5i5o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    coord_t in2;
    coord_t out2;
    coord_t in3;
    coord_t out3;
    coord_t in4;
    coord_t out4;
    coord_t in5;
    coord_t out5;
    coord_t in6;
    coord_t out6;
    char dibujo[TAM_DIBUJO];
} elem_6i6o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t in2;
    coord_t out1;
    char dibujo[TAM_DIBUJO];
} elem_2i1o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t in2;
    coord_t ctrl;
    coord_t out1;
    char dibujo[TAM_DIBUJO];
} mux2_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t in2;
    coord_t in3;
    coord_t ctrl;
    coord_t out1;
    char dibujo[TAM_DIBUJO];
} mux3_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t in2;
    coord_t in3;
    coord_t in4;
    coord_t out1;
    coord_t out2;
    char dibujo[TAM_DIBUJO];
} elem_4i2o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t in2;
    coord_t in3;
    coord_t in4;
    coord_t ctrl;
    coord_t out1;
    char dibujo[TAM_DIBUJO];
} mux4_t;

/***** Macros Locales ******************************************/

#define inicia_dibujo(figura) strcpy(figura, "")

#define dibuja(figura, format, ...) do { \
  char line[256];                   \
  sprintf(line, format, __VA_ARGS__); \
  strcat(figura, line);                \
} while(0)


/***************************************************************
 *
 * Func: asigna
 *
 * Desc: Asigna un valor a un punto
 *
 **************************************************************/

/*
static void asigna (coord_t* punto, int x, int y)
{
    (*punto).x = x;
    (*punto).y = y;
}
 */

/***************************************************************
 *
 * Func: escribe
 *
 * Desc: Escribe un texto en el dibujo
 *
 **************************************************************/

static void escribe(int x, int y, char *texto, char *tcolor, int tsize) {
    char *line2;
    char line1[SHORT_STR];
    int position;

    line2 = strchr(texto, '[');
    fprintf(f_salida_html, "<!-- Texto -->\n");
    if (line2 != NULL) {
        // Lo ponemos en dos lineas
        position = strlen(texto) - strlen(line2);
        strncpy(line1, texto, position - 1);
        line1[position - 1] = '\0';
        fprintf(f_salida_html, "<!-- linea1 -->\n");
        fprintf(f_salida_html,
                "<text x=\"%d\" y=\"%d\"\n     style=\"fill:%s;stroke:%s;font-family:Arial;font-size:%dpx;\">\n%s\n</text>\n",
                x, y, tcolor, "null", tsize, line1);
        fprintf(f_salida_html, "<!-- linea2 -->\n");
        fprintf(f_salida_html,
                "<text x=\"%d\" y=\"%d\"\n     style=\"fill:%s;stroke:%s;font-family:Arial;font-size:%dpx;\">\n%s\n</text>\n\n",
                x, y + 20, tcolor, "null", tsize, line2);
    } else
        fprintf(f_salida_html,
                "<text x=\"%d\" y=\"%d\"\n     style=\"fill:%s;stroke:%s;font-family:Arial;font-size:%dpx;\">\n%s\n</text>\n\n",
                x, y, tcolor, "null", tsize, texto);

}

/***************************************************************
 *
 * Func: genera_sum
 *
 * Desc: Dibuja el símbolo de la ALU
 *
 **************************************************************/

static void genera_sum(elem_2i1o_t *alu, int x1, int y1, int ancho, int alto, char *nombre, char *color, char *relleno,
                       char *tcolor, int input1, int input2, int output1) {
    int x2;
    int y2;
    char i1[5];
    char i2[5];
    char o1[5];

    x2 = x1 + ancho;
    y2 = y1 + alto;

    alu->pos1.x = x1;
    alu->pos1.y = y1;
    alu->pos2.x = x2;
    alu->pos2.y = y2;
    alu->ancho = ancho;
    alu->alto = alto;
    alu->in1.x = x1;
    alu->in1.y = y1 + alto / 6;
    alu->in2.x = x1;
    alu->in2.y = y2 - alto / 6;
    alu->out1.x = x2;
    alu->out1.y = y1 + alto / 2;
    inicia_dibujo(alu->dibujo);
    dibuja(alu->dibujo,
           "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\" \n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           x1, y1, x1, y1 + (int) (3 * alto / 8), x1 + (int) (ancho / 4), y1 + (int) alto / 2, x1,
           y1 + (int) (5 * alto / 8), x1, y1 + alto, x2, y2 - (int) alto / 4, x2, y1 + (int) alto / 4, x1, y1, relleno,
           color, trazo_grueso);
    dibuja(alu->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n</g>\n",
           x1 + ancho / 2 + 5, alu->out1.y + 15, color, "none", nombre);

    imprime_valor(input1, 10000, i1);
    imprime_valor(input2, 10000, i2);
    imprime_valor(output1, 10000, o1);
    dibuja(alu->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
           alu->in1.x + 5, alu->in1.y + 15, tcolor, "none", i1);
    dibuja(alu->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
           alu->in2.x + 5, alu->in2.y - 5, tcolor, "none", i2);
    dibuja(alu->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n",
           alu->out1.x - 5, alu->out1.y, tcolor, "none", o1);

    fprintf(f_salida_html, "<!-- SUM -->\n");
    fprintf(f_salida_html, "%s\n", alu->dibujo);
}


/***************************************************************
 *
 * Func: genera_alu
 *
 * Desc: Dibuja el símbolo de la ALU
 *
 **************************************************************/

static void genera_ALU(elem_2i1o_t *alu, int x1, int y1, int ancho, int alto, char *nombre, char *color, char *relleno,
                       char *tcolor, int input1, int input2, int output1) {
    int x2;
    int y2;
    char i1[5];
    char i2[5];
    char o1[5];
    char control[12];

    x2 = x1 + ancho;
    y2 = y1 + alto;

    alu->pos1.x = x1;
    alu->pos1.y = y1;
    alu->pos2.x = x2;
    alu->pos2.y = y2;
    alu->ancho = ancho;
    alu->alto = alto;
    alu->in1.x = x1;
    alu->in1.y = y1 + alto / 6;
    alu->in2.x = x1;
    alu->in2.y = y2 - alto / 6;
    alu->out1.x = x2;
    alu->out1.y = y1 + alto / 2;
    inicia_dibujo(alu->dibujo);
    dibuja(alu->dibujo,
           "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\" \n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           x1, y1, x1, y1 + (int) (3 * alto / 8), x1 + (int) (ancho / 4), y1 + (int) alto / 2, x1,
           y1 + (int) (5 * alto / 8), x1, y1 + alto, x2, y2 - (int) alto / 4, x2, y1 + (int) alto / 4, x1, y1, relleno,
           color, trazo_grueso);
    dibuja(alu->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n</g>\n",
           x1 + ancho / 2 + 5, alu->out1.y + 15, color, "none", nombre);
    imprime_valor(input1, 10000, i1);
    imprime_valor(input2, 10000, i2);
    imprime_valor(output1, 10000, o1);
    dibuja(alu->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
           alu->in1.x + 5, alu->in1.y + 15, tcolor, "none", i1);
    dibuja(alu->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
           alu->in2.x + 5, alu->in2.y - 10, tcolor, "none", i2);
    dibuja(alu->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n",
           alu->out1.x - 5, alu->out1.y, tcolor, "none", o1);
    if (inst_load(ID_EX.IR.codop) || inst_store(ID_EX.IR.codop))
        sprintf(control, "mem addr");
    else if ((latencia_salto2(solucion_riesgos_control) || latencia_salto3(solucion_riesgos_control)) &&
             inst_branch(ID_EX.IR.codop))
        sprintf(control, "branch addr");
    else
        sprintf(control, "alu op");
    dibuja(alu->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
           alu->pos1.x - 10, alu->pos1.y - 5, tcolor, "none", control);
    fprintf(f_salida_html, "<!-- ALU -->\n");
    fprintf(f_salida_html, "%s\n", alu->dibujo);
}

/***************************************************************
 *
 * Func: genera_MI
 *
 * Desc: Dibuja el símbolo de la memoria
 *
 **************************************************************/

static void genera_MI(elem_2i1o_t *mem, int x1, int y1, int ancho, int alto, char *tcolor, char *color, char *relleno) {
    int x2;
    int y2;
    char i1[50];
    char nombre[10] = "Mem Instr";
    char in1[10] = "Dir";
    char in2[10] = "";
    char out1[10] = "Inst";


    x2 = x1 + ancho;
    y2 = y1 + alto;

    mem->pos1.x = x1;
    mem->pos1.y = y1;
    mem->pos2.x = x2;
    mem->pos2.y = y2;
    mem->ancho = ancho;
    mem->alto = alto;
    mem->in1.x = x1;
    mem->in1.y = y1 + alto / 4;
    mem->in2.x = x1;
    mem->in2.y = y2 - alto / 4;
    mem->out1.x = x2;
    mem->out1.y = y1 + alto / 2;
    inicia_dibujo(mem->dibujo);
    dibuja(mem->dibujo,
           "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           x1, y1, x1, y1 + alto, x1 + ancho, y1 + alto, x1 + ancho, y1, x1, y1, relleno, color, trazo_grueso);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           x1 + ancho / 2, (int) y1 + 5 * alto / 8, color, "none", nombre);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n",
           mem->in1.x + 5, mem->in1.y, color, "none", in1);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n",
           mem->in2.x + 5, mem->in2.y, color, "none", in2);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n",
           mem->out1.x - 5, mem->out1.y, color, "none", out1);
    imprime_valor(PC, 10000, i1);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n",
           mem->in1.x + 25, mem->in1.y, tcolor, "none", i1);
    fprintf(f_salida_html, "<!-- MI -->\n");
    fprintf(f_salida_html, "%s\n", mem->dibujo);
}

/***************************************************************
 *
 * Func: genera_MD
 *
 * Desc: Dibuja el símbolo de la memoria
 *
 **************************************************************/

static void genera_MD(elem_2i1o_t *mem, int x1, int y1, int ancho, int alto, char *tcolor, char *color, char *relleno) {
    int x2;
    int y2;
    char i1[50];
    char i2[50];
    char o1[50];
    char nombre[10] = "Mem Datos";
    char in1[10] = "Dir";
    char in2[10] = "W";
    char out1[10] = "Dato";
    char control[10];

    x2 = x1 + ancho;
    y2 = y1 + alto;

    mem->pos1.x = x1;
    mem->pos1.y = y1;
    mem->pos2.x = x2;
    mem->pos2.y = y2;
    mem->ancho = ancho;
    mem->alto = alto;
    mem->in1.x = x1;
    mem->in1.y = y1 + alto / 4;
    mem->in2.x = x1;
    mem->in2.y = y2 - alto / 4;
    mem->out1.x = x2;
    mem->out1.y = y1 + alto / 2;
    inicia_dibujo(mem->dibujo);
    dibuja(mem->dibujo,
           "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           x1, y1, x1, y1 + alto, x1 + ancho, y1 + alto, x1 + ancho, y1, x1, y1, relleno, color, trazo_grueso);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           x1 + ancho / 2, (int) y1 + 7 * alto / 8, color, "none", nombre);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n",
           mem->in1.x + 5, mem->in1.y, color, "none", in1);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n",
           mem->in2.x + 5, mem->in2.y, color, "none", in2);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n",
           mem->out1.x - 5, mem->out1.y, color, "none", out1);
    if (inst_load(EX_MEM.IR.codop) || inst_store(EX_MEM.IR.codop)) {
        imprime_valor(MEM.dir, 10000, i1);
        dibuja(mem->dibujo,
               "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n",
               mem->in1.x + 25, mem->in1.y, tcolor, "none", i1);
    }
    strcpy(control, "");
    if (inst_load(EX_MEM.IR.codop)) {
        sprintf(control, "Read");
        imprime_valor(MEM_WBn.MEMout, 10000, o1);
        dibuja(mem->dibujo,
               "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n",
               mem->out1.x - 5, mem->out1.y + 15, tcolor, "none", o1);
    }
    if (inst_store(EX_MEM.IR.codop)) {
        sprintf(control, "Write");
        imprime_valor(MEM.data, 10000, i2);
        dibuja(mem->dibujo,
               "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n",
               mem->in2.x + 25, mem->in2.y, tcolor, "none", i2);
    }
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
           mem->pos1.x, mem->pos1.y - 5, tcolor, "none", control);
    fprintf(f_salida_html, "<!-- MD -->\n");
    fprintf(f_salida_html, "%s\n", mem->dibujo);
}

/***************************************************************
 *
 * Func: genera_elipse
 *
 * Desc: Dibuja una elipse
 *
 **************************************************************/

static void
genera_elipse(elem_1i1o_t *mem, int x1, int y1, int ancho, int alto, char *nombre, char *in1, char *out1, char *tcolor,
              char *color, char *relleno) {
    int x2;
    int y2;
    int cx, cy, rx, ry;
    char o1[5];

    x2 = x1 + ancho;
    y2 = y1 + alto;
    rx = ancho / 2;
    ry = alto / 2;
    cx = x1 + rx;
    cy = y1 + ry;

    mem->pos1.x = x1;
    mem->pos1.y = y1;
    mem->pos2.x = x2;
    mem->pos2.y = y2;
    mem->ancho = ancho;
    mem->alto = alto;
    mem->in1.x = x1;
    mem->in1.y = y1 + alto / 2;
    mem->out1.x = x2;
    mem->out1.y = y1 + alto / 2;
    inicia_dibujo(mem->dibujo);
    dibuja(mem->dibujo,
           "<g>\n  <ellipse cx=\"%d\" cy=\"%d\" rx=\"%d\" ry=\"%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           cx, cy, rx, ry, relleno, color, trazo_grueso);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           x1 + ancho / 2, mem->out1.y + 15, color, "none", nombre);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n",
           mem->in1.x, mem->in1.y, tcolor, "none", in1);
    dibuja(mem->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n",
           mem->out1.x, mem->out1.y, tcolor, "none", out1);

    if inst_imm(IF_ID.IR.codop) {
        imprime_valor(ID_EXn.Imm, 10000, o1);
        dibuja(mem->dibujo,
               "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n",
               mem->out1.x - 5, mem->out1.y, tcolor, "none", o1);
    }
    fprintf(f_salida_html, "<!-- Elipse -->\n");
    fprintf(f_salida_html, "%s\n", mem->dibujo);
}

/***************************************************************
 *
 * Func: genera_latch
 *
 * Desc: Dibuja un registro de 1 entrada
 *
 **************************************************************/

static void
genera_latch(elem_1i1o_t *reg, int x1, int y1, int ancho, int alto, char *nombre, char *in1, char *tcolor, char *color,
             char *relleno) {
    int x2;
    int y2;

    x2 = x1 + ancho;
    y2 = y1 + alto;

    reg->pos1.x = x1;
    reg->pos1.y = y1;
    reg->pos2.x = x2;
    reg->pos2.y = y2;
    reg->ancho = ancho;
    reg->alto = alto;
    reg->in1.x = x1;
    reg->in1.y = y1 + alto / 2;
    reg->out1.x = x2;
    reg->out1.y = y1 + alto / 2;
    inicia_dibujo(reg->dibujo);
    dibuja(reg->dibujo,
           "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           x1, y1, x1, y1 + alto, x1 + ancho, y1 + alto, x1 + ancho, y1, x1, y1, relleno, color, trazo_grueso);
    dibuja(reg->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           x1 + ancho / 2, y1 - 10, color, tcolor, nombre);
    dibuja(reg->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n</g>\n",
           reg->in1.x + ancho / 2, reg->in1.y - 2, tcolor, "none", in1);
    fprintf(f_salida_html, "<!-- LATCH -->\n");
    fprintf(f_salida_html, "%s\n", reg->dibujo);
}

/*
 void genera_latch2 (elem_2i2o_t* reg, int x1, int y1, int ancho, int alto, int i1_y, int i2_y)
 {
        int x2;
        int y2;

        x2 = x1+ancho;
        y2 = y1+alto;

        reg->pos1.x = x1;
        reg->pos1.y = y1;
        reg->pos2.x = x2;
        reg->pos2.y = y2;
        reg->ancho = ancho;
        reg->alto = alto;
        reg->in1.x = x1;
        reg->in1.y = i1_y;
        reg->out1.x = x2;
        reg->out1.y = i1_y;
        reg->in2.x = x1;
        reg->in2.y = i2_y;
        reg->out2.x = x2;
        reg->out2.y = i2_y;
 sprintf (reg->dibujo, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:purple;stroke-width:3\"/>\n", x1,y1,x1,y1+alto,x1+ancho,y1+alto,x1+ancho,y1,x1,y1,"none");
        fprintf (f_salida_html,reg->dibujo);
 }


 void genera_latch3 (elem_3i3o_t* reg, int x1, int y1, int ancho, int alto, int i1_y, int i2_y, int i3_y)
 {
        int x2;
        int y2;

        x2 = x1+ancho;
        y2 = y1+alto;

        reg->pos1.x = x1;
        reg->pos1.y = y1;
        reg->pos2.x = x2;
        reg->pos2.y = y2;
        reg->ancho = ancho;
        reg->alto = alto;
        reg->in1.x = x1;
        reg->in1.y = i1_y;
        reg->out1.x = x2;
        reg->out1.y = i1_y;
        reg->in2.x = x1;
        reg->in2.y = i2_y;
        reg->out2.x = x2;
        reg->out2.y = i2_y;
        reg->in3.x = x1;
        reg->in3.y = i3_y;
        reg->out3.x = x2;
        reg->out3.y = i3_y;
 sprintf (reg->dibujo, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:purple;stroke-width:3\"/>\n", x1,y1,x1,y1+alto,x1+ancho,y1+alto,x1+ancho,y1,x1,y1,"none");
        fprintf (f_salida_html,reg->dibujo);
 }

 void genera_latch4 (elem_4i4o_t* reg, int x1, int y1, int ancho, int alto, int i1_y, int i2_y, int i3_y, int i4_y, char* nombre, char* in1, char* in2, char* in3, char* in4, char* tcolor, char* color, char * relleno)
 {
        int x2;
        int y2;

        x2 = x1+ancho;
        y2 = y1+alto;

        reg->pos1.x = x1;
        reg->pos1.y = y1;
        reg->pos2.x = x2;
        reg->pos2.y = y2;
        reg->ancho = ancho;
        reg->alto = alto;
        reg->in1.x = x1;
        reg->in1.y = i1_y;
        reg->out1.x = x2;
        reg->out1.y = i1_y;
        reg->in2.x = x1;
        reg->in2.y = i2_y;
        reg->out2.x = x2;
        reg->out2.y = i2_y;
        reg->in3.x = x1;
        reg->in3.y = i3_y;
        reg->out3.x = x2;
        reg->out3.y = i3_y;
        reg->in4.x = x1;
        reg->in4.y = i4_y;
        reg->out4.x = x2;
        reg->out4.y = i4_y;
 sprintf (reg->dibujo, "<g>  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:3\"/>\n", x1,y1,x1,y1+alto,x1+ancho,y1+alto,x1+ancho,y1,x1,y1,relleno,color);
        dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\"\n    >%s</text>\n", x1+ancho/2, y1-10, color, "none", nombre);
        dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\"\n    >%s</text>\n", reg->in1.x+ancho/2, reg->in1.y-2, tcolor, "none", in1);
        dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\"\n    >%s</text>\n", reg->in2.x+ancho/2, reg->in2.y-2, tcolor, "none", in2);
        dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\"\n    >%s</text>\n", reg->in3.x+ancho/2, reg->in3.y-2, tcolor, "none", in3);
        dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\"\n    >%s</text>\n</g>\n", reg->in4.x+ancho/2, reg->in4.y-2, tcolor, "none", "B");
        fprintf (f_salida_html,reg->dibujo);
 }

 void genera_latch5 (elem_5i5o_t* reg, int x1, int y1, int ancho, int alto, int i1_y, int i2_y, int i3_y, int i4_y, int i5_y)
 {
        int x2;
        int y2;

        x2 = x1+ancho;
        y2 = y1+alto;

        reg->pos1.x = x1;
        reg->pos1.y = y1;
        reg->pos2.x = x2;
        reg->pos2.y = y2;
        reg->ancho = ancho;
        reg->alto = alto;
        reg->in1.x = x1;
        reg->in1.y = i1_y;
        reg->out1.x = x2;
        reg->out1.y = i1_y;
        reg->in2.x = x1;
        reg->in2.y = i2_y;
        reg->out2.x = x2;
        reg->out2.y = i2_y;
        reg->in3.x = x1;
        reg->in3.y = i3_y;
        reg->out3.x = x2;
        reg->out3.y = i3_y;
        reg->in4.x = x1;
        reg->in4.y = i4_y;
        reg->out4.x = x2;
        reg->out4.y = i4_y;
 reg->in5.x = x1;
        reg->in5.y = i5_y;
        reg->out5.x = x2;
        reg->out5.y = i5_y;
 sprintf (reg->dibujo, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:purple;stroke-width:3\"/>\n", x1,y1,x1,y1+alto,x1+ancho,y1+alto,x1+ancho,y1,x1,y1,"none");
        fprintf (f_salida_html,reg->dibujo);
 }
 */

/***************************************************************
 *
 * Func: genera_latch6
 *
 * Desc: Dibuja un registro de 6 entradas y 6 salidas
 *
 **************************************************************/

static void
genera_latch6(elem_6i6o_t *reg, int x1, int y1, int ancho, int alto, int i1_y, int i2_y, int i3_y, int i4_y, int i5_y,
              int i6_y, char *nombre, char *in1, char *in2, char *in3, char *in4, char *in5, char *in6, char *tcolor,
              char *color, char *relleno) {
    int x2;
    int y2;

    x2 = x1 + ancho;
    y2 = y1 + alto;

    reg->pos1.x = x1;
    reg->pos1.y = y1;
    reg->pos2.x = x2;
    reg->pos2.y = y2;
    reg->ancho = ancho;
    reg->alto = alto;
    reg->in1.x = x1;
    reg->in1.y = i1_y;
    reg->out1.x = x2;
    reg->out1.y = i1_y;
    reg->in2.x = x1;
    reg->in2.y = i2_y;
    reg->out2.x = x2;
    reg->out2.y = i2_y;
    reg->in3.x = x1;
    reg->in3.y = i3_y;
    reg->out3.x = x2;
    reg->out3.y = i3_y;
    reg->in4.x = x1;
    reg->in4.y = i4_y;
    reg->out4.x = x2;
    reg->out4.y = i4_y;
    reg->in5.x = x1;
    reg->in5.y = i5_y;
    reg->out5.x = x2;
    reg->out5.y = i5_y;
    reg->in6.x = x1;
    reg->in6.y = i6_y;
    reg->out6.x = x2;
    reg->out6.y = i6_y;
    inicia_dibujo(reg->dibujo);
    dibuja(reg->dibujo,
           "<g>  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           x1, y1, x1, y1 + alto, x1 + ancho, y1 + alto, x1 + ancho, y1, x1, y1, relleno, color, trazo_grueso);
    dibuja(reg->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           x1 + ancho / 2, y1 - 10, color, "none", nombre);
    dibuja(reg->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           reg->in1.x + ancho / 2, reg->in1.y - 4, tcolor, "none", in1);
    dibuja(reg->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           reg->in2.x + ancho / 2, reg->in2.y - 4, tcolor, "none", in2);
    dibuja(reg->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           reg->in3.x + ancho / 2, reg->in3.y - 4, tcolor, "none", in3);
    dibuja(reg->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           reg->in4.x + ancho / 2, reg->in4.y - 4, tcolor, "none", in4);
    dibuja(reg->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           reg->in5.x + ancho / 2, reg->in5.y - 4, tcolor, "none", in5);
    dibuja(reg->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n</g>\n",
           reg->in6.x + ancho / 2, reg->in6.y - 2, tcolor, "none", in6);
    fprintf(f_salida_html, "<!-- LATCH6 -->\n");
    fprintf(f_salida_html, "%s\n", reg->dibujo);
}

/***************************************************************
 *
 * Func: genera_regs
 *
 * Desc: Dibuja un banco de registros
 *
 **************************************************************/


static void
genera_regs(elem_4i2o_t *regs, int x1, int y1, int ancho, int alto, int in1, int in2, char *in12_col, int in3, int in4,
            char *in34_col, char *color, char *relleno) {
    int x2;
    int y2;
    char o1[8];
    char o2[8];

    x2 = x1 + ancho;
    y2 = y1 + alto;

    regs->pos1.x = x1;
    regs->pos1.y = y1;
    regs->pos2.x = x2;
    regs->pos2.y = y2;
    regs->ancho = ancho;
    regs->alto = alto;
    regs->in1.x = x1;
    regs->in1.y = y1 + alto / 6;
    regs->in2.x = x1;
    regs->in2.y = y1 + 2 * alto / 6;
    regs->in3.x = x1;
    regs->in3.y = y1 + 4 * alto / 6;
    regs->in4.x = x1;
    regs->in4.y = y1 + 5 * alto / 6;
    regs->out1.x = x2;
    regs->out1.y = y1 + alto / 4;
    regs->out2.x = x2;
    regs->out2.y = y1 + 3 * alto / 4;
    inicia_dibujo(regs->dibujo);
    dibuja(regs->dibujo,
           "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           x1, y1, x1, y1 + alto, x1 + ancho, y1 + alto, x1 + ancho, y1, x1, y1, relleno, color, trazo_grueso);
    dibuja(regs->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n",
           x1 + ancho / 2, (int) y1 + alto / 2, color, "none", "Regs");
    // Rfte1
    if inst_int_Rfte1(IF_ID.IR.codop)
        dibuja(regs->dibujo,
               "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    x%d\n</text>\n",
               regs->in1.x + 5, regs->in1.y, in12_col, "none", in1);
    // Rfte2
    if inst_int_Rfte2(IF_ID.IR.codop)
        dibuja(regs->dibujo,
               "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    x%d\n</text>\n",
               regs->in2.x + 5, regs->in2.y, in12_col, "none", in2);
    // Rfdst
    if inst_int_to_Rint(MEM_WB.IR.codop) {
        dibuja(regs->dibujo,
               "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    x%d\n</text>\n",
               regs->in3.x + 5, regs->in3.y, in34_col, "none", in3);
        if (in3 == 0)
            sprintf(o1, "%s", "");
        else imprime_valor(in4, 10000, o1);
        dibuja(regs->dibujo,
               "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n",
               regs->in4.x + 5, regs->in4.y, in34_col, "none", o1);
    }
    dibuja(regs->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n",
           regs->out1.x - 5, regs->out1.y, color, "none", "A");
    dibuja(regs->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n",
           regs->out1.x - 5, regs->out2.y, color, "none", "B");

    if inst_int_Rfte1(IF_ID.IR.codop) {
        imprime_valor(ID_EXn.Ra, 10000, o1)
    } else {
        sprintf(o1, "%s", "");
    }
    if inst_int_Rfte2(IF_ID.IR.codop) {
        imprime_valor(ID_EXn.Rb, 10000, o2);
    } else {
        sprintf(o2, "%s", "");
    }
    dibuja(regs->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n",
           regs->out1.x - 5, regs->out1.y - 15, in12_col, "none", o1);
    dibuja(regs->dibujo,
           "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n",
           regs->out2.x - 5, regs->out2.y + 15, in12_col, "none", o2);

    fprintf(f_salida_html, "<!-- REGs -->\n");
    fprintf(f_salida_html, "%s\n", regs->dibujo);
}

/***************************************************************
 *
 * Func: genera_mux2
 *
 * Desc: Dibuja un multiplexor de 2 entradas.
 * Fijamos la posición de la salida
 * El in3 es el de control
 *
 **************************************************************/

static void
genera_mux2(mux2_t *mux, int xi, int yo, int ancho, int alto, int selected, char *scolor, char *color, char *relleno) {
    int x1, x2, y1, y2;
    int radio;
    int lado;
    int xsel, ysel;

    x1 = xi;
    y1 = yo - alto / 2;
    //    x1 = xo;
    //    x1 = x2-ancho;

    //    y1 = yo-alto/2;
    //    y2 = y1+alto;
    x2 = x1 + ancho;
    y2 = y1 + alto;

    radio = ancho / 2;
    lado = alto - ancho;

    mux->pos1.x = x1;
    mux->pos1.y = y1;
    mux->pos2.x = x2;
    mux->pos2.y = y2;
    mux->ancho = ancho;
    mux->alto = alto;
    mux->in1.x = x1;
    mux->in1.y = y1 + radio;
    mux->in2.x = x1;
    mux->in2.y = y1 + radio + lado;
    mux->ctrl.x = x1 + radio;
    mux->ctrl.y = y1;
    mux->out1.x = x2;
    mux->out1.y = y1 + radio + lado / 2;

    if (selected == 0) {
        xsel = mux->in1.x;
        ysel = mux->in1.y;
    } else {
        xsel = mux->in2.x;
        ysel = mux->in2.y;
    }
    inicia_dibujo(mux->dibujo);
    dibuja(mux->dibujo,
           "<g>  <path d=\"M%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           x1, y1 + radio, radio, radio, x1 + ancho, y1 + radio, x1 + ancho, y1 + radio + lado, radio, radio, x1,
           y1 + radio + lado, x1, y1 + radio, relleno, color, trazo_grueso);
    dibuja(mux->dibujo, "<polyline points=\"%d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n</g>\n",
           xsel, ysel, mux->out1.x, mux->out1.y, "none", scolor, trazo_resaltado);
    fprintf(f_salida_html, "<!-- MUX2 -->\n");
    fprintf(f_salida_html, "%s\n", mux->dibujo);
}

/***************************************************************
 *
 * Func: genera_mux3
 *
 * Desc: Dibuja un multiplexor de 3 entradas // in4 es la de control
 *
 **************************************************************/

static void
genera_mux3(mux3_t *mux, int xi, int yo, int ancho, int alto, int selected, char *scolor, char *color, char *relleno) {
    /*
     int x2;
     int y2;
     int radio;
     int lado;

     x2 = x1+ancho;
     y2 = y1+alto;
     radio = ancho/2;
     lado = (alto-ancho)/2;
     */
    int x1, x2, y1, y2;
    int radio;
    int lado;
    int xsel = 0, ysel = 0;

    x1 = xi;
    y1 = yo - alto / 2;
    //    x1 = xo;
    //    x1 = x2-ancho;

    //    y1 = yo-alto/2;
    //    y2 = y1+alto;
    x2 = x1 + ancho;
    y2 = y1 + alto;

    radio = ancho / 2;
    lado = alto - ancho;

    mux->pos1.x = x1;
    mux->pos1.y = y1;
    mux->pos2.x = x2;
    mux->pos2.y = y2;
    mux->ancho = ancho;
    mux->alto = alto;
    mux->in1.x = x1;
    mux->in1.y = y1 + radio + 0 * (lado / 2);
    //    mux->in1.y = y1 + radio + lado / 4;
    mux->in2.x = x1;
    mux->in2.y = y1 + radio + 1 * (lado / 2);
    //  mux->in2.y = y1 + radio + 2 * lado / 4;
    mux->in3.x = x1;
    mux->in3.y = y1 + radio + 2 * (lado / 2);
    // mux->in3.y = y1 + radio + 3 * lado / 4;
    mux->ctrl.x = x1 + radio;
    mux->ctrl.y = y1;
    mux->out1.x = x2;
    mux->out1.y = y1 + radio + lado / 2;

    if (selected == 0) {
        xsel = mux->in1.x;
        ysel = mux->in1.y;
    } else if (selected == 1) {
        xsel = mux->in2.x;
        ysel = mux->in2.y;
    } else if (selected == 2) {
        xsel = mux->in3.x;
        ysel = mux->in3.y;
    }
    inicia_dibujo(mux->dibujo);
    dibuja(mux->dibujo,
           "<g>  <path d=\"M%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           x1, y1 + radio, radio, radio, x1 + ancho, y1 + radio, x1 + ancho, y1 + radio + lado, radio, radio, x1,
           y1 + radio + lado, x1, y1 + radio, relleno, color, trazo_grueso);
    dibuja(mux->dibujo, "<polyline points=\"%d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n</g>\n",
           xsel, ysel, mux->out1.x, mux->out1.y, "none", scolor, trazo_resaltado);
    fprintf(f_salida_html, "<!-- MUX3 -->\n");
    fprintf(f_salida_html, "%s\n", mux->dibujo);
}

/***************************************************************
 *
 * Func: genera_mux4
 *
 * Desc: Dibuja un multiplexor de 4 entradas // in5 es la de control
 *
 **************************************************************/

static void
genera_mux4(mux4_t *mux, int xi, int yo, int ancho, int alto, int selected, char *scolor, char *color, char *relleno) {
    /*
     int x2;
     int y2;
     int radio;
     int lado;

     x2 = x1+ancho;
     y2 = y1+alto;
     radio = ancho/2;
     lado = (alto-ancho)/2;
     */
    int x1, x2, y1, y2;
    int radio;
    int lado;
    int xsel, ysel;

    x1 = xi;
    y1 = yo - alto / 2;
    //    x1 = xo;
    //    x1 = x2-ancho;

    //    y1 = yo-alto/2;
    //    y2 = y1+alto;
    x2 = x1 + ancho;
    y2 = y1 + alto;

    radio = ancho / 2;
    lado = alto - ancho;

    mux->pos1.x = x1;
    mux->pos1.y = y1;
    mux->pos2.x = x2;
    mux->pos2.y = y2;
    mux->ancho = ancho;
    mux->alto = alto;
    mux->in1.x = x1;
    mux->in1.y = y1 + radio + 0 * (lado / 3);
    //    mux->in1.y = y1 + radio + lado / 5;
    mux->in2.x = x1;
    mux->in2.y = y1 + radio + 1 * (lado / 3);
    mux->in3.x = x1;
    mux->in3.y = y1 + radio + 2 * (lado / 3);
    mux->in4.x = x1;
    mux->in4.y = y1 + radio + 3 * (lado / 3);
    mux->ctrl.x = x1 + radio;
    mux->ctrl.y = y1;
    mux->out1.x = x2;
    mux->out1.y = y1 + radio + lado / 2;

    if (selected == 0) {
        xsel = mux->in1.x;
        ysel = mux->in1.y;
    } else if (selected == 1) {
        xsel = mux->in2.x;
        ysel = mux->in2.y;
    } else if (selected == 2) {
        xsel = mux->in3.x;
        ysel = mux->in3.y;
    } else {
        xsel = mux->in4.x;
        ysel = mux->in4.y;
    }
    inicia_dibujo(mux->dibujo);
    dibuja(mux->dibujo,
           "<g>  <path d=\"M%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
           x1, y1 + radio, radio, radio, x1 + ancho, y1 + radio, x1 + ancho, y1 + radio + lado, radio, radio, x1,
           y1 + radio + lado, x1, y1 + radio, relleno, color, trazo_grueso);
    dibuja(mux->dibujo, "<polyline points=\"%d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n</g>\n",
           xsel, ysel, mux->out1.x, mux->out1.y, "none", scolor, trazo_resaltado);
    fprintf(f_salida_html, "<!-- MUX4 -->\n");
    fprintf(f_salida_html, "%s\n", mux->dibujo);

    /*
     sprintf (mux->dibujo, "<path d=\"M%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\"\n     style=\"fill:%s;stroke:purple;stroke-width:1\"/>\n", x1, y1+radio, radio, radio, x1+ancho, y1+radio, x1+ancho, y1+alto, radio, radio, x1, y1+alto, x1, y1+radio,"none");
     fprintf (f_salida_html,mux->dibujo); */
}

/***************************************************************
 *
 * Func: conecta
 *
 * Desc: Dibuja la conexión entre dos puntos
 *
 **************************************************************/

static void conecta(coord_t punto1, coord_t punto2, char *color) {

    int x_medio;
    x_medio = punto1.x + (punto2.x - punto1.x) / 2;

    fprintf(f_salida_html, "<!-- conecta -->\n");
    fprintf(f_salida_html,
            "<polyline points=\"%d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n\n",
            punto1.x, punto1.y, x_medio, punto1.y, x_medio, punto2.y, punto2.x, punto2.y, "none", color, trazo_fino);
}

/***************************************************************
 *
 * Func: conecta_p
 *
 * Desc: Dibuja la conexión entre dos puntos
 *
 **************************************************************/

/*
static void conecta_p (coord_t punto1, coord_t punto2, int percent,  char* color)
{
    int ancho;
    int alto;

    ancho = punto2.x-punto1.x;
    alto = punto2.y-punto1.x;

    fprintf (f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", punto1.x,punto1.y,punto1.x+(int) ancho*percent/100,punto1.y,punto1.x+(int) ancho*percent/100,punto2.y, punto2.x, punto2.y,"none", color, trazo_fino) ;
}
 */

/***************************************************************
 *
 * Func: conecta_int
 *
 * Desc: Dibuja la conexión entre dos puntos
 *
 **************************************************************/

/*
static void conecta_int (coord_t punto1, coord_t punto2, int desp_xmedio, char* color)
{
    int x_medio;

    x_medio = punto1.x+desp_xmedio;

    fprintf (f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", punto1.x,punto1.y,x_medio,punto1.y,x_medio,punto2.y, punto2.x, punto2.y,"none", color, trazo_fino) ;
}
 */

/***************************************************************
 *
 * Func: conecta_p1_dx1_dy_dx2_p2
 *
 * Desc: Dibuja la conexión entre dos puntos p1 y p2: p1 -> rel_x, rel_y, rel_x -> p2
 *
 **************************************************************/

/*
static void conecta_p1_dx1_dy_dx2_p2 (coord_t punto1, coord_t punto2, int desp_xmedio1, int desp_ymedio, int desp_xmedio2, char* color)
{
    int x_medio1;
    int x_medio2;
    int y_medio1;

    x_medio1 = punto1.x+desp_xmedio1;
    x_medio2 = punto2.x+desp_xmedio2;
    y_medio1 = punto1.y+desp_ymedio;

    fprintf (f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", punto1.x,punto1.y,x_medio1,punto1.y,x_medio1,y_medio1,x_medio2,y_medio1,x_medio2, punto2.y ,punto2.x, punto2.y,"none", color, trazo_fino) ;
}
 */

/***************************************************************
 *
 * Func: conecta_p1_dx1_y_dx2_p2
 *
 * Desc: Dibuja la conexión entre dos puntos p1 y p2: p1 -> rel_x, abs_y, rel_x -> p2
 *
 **************************************************************/

static void
conecta_p1_dx1_y_dx2_p2(coord_t punto1, coord_t punto2, int desp_xmedio1, int y_medio1, int desp_xmedio2, char *color) {

    int x_medio1;
    int x_medio2;
    //    int y_medio1;

    x_medio1 = punto1.x + desp_xmedio1;
    x_medio2 = punto2.x + desp_xmedio2;
    //    y_medio1 = punto1.y+desp_ymedio;

    fprintf(f_salida_html, "<!-- conecta_p1_dx1_y_dx2_p2 -->\n");
    fprintf(f_salida_html,
            "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n\n",
            punto1.x, punto1.y, x_medio1, punto1.y, x_medio1, y_medio1, x_medio2, y_medio1, x_medio2, punto2.y,
            punto2.x, punto2.y, "none", color, trazo_fino);
}

/***************************************************************
 *
 * Func: conecta_p1_dx1_y1_dx2_y2_dx3_p2
 *
 * Desc: Dibuja la conexión entre dos puntos p1 y p2:
 * p1 -> rel_x, abs_y, rel_x, abs_y, rel_x -> p2
 *
 **************************************************************/
/*
static void
conecta_p1_dx1_y1_dx2_y2_dx3_p2(coord_t punto1, coord_t punto2, int desp_xmedio1, int y_medio1, int desp_xmedio2,
                                int y_medio2, int desp_xmedio3, char *color) {

    int x_medio1;
    int x_medio2;
    int x_medio3;
    //    int y_medio1;

    x_medio1 = punto1.x + desp_xmedio1;
    x_medio2 = x_medio1 + desp_xmedio2;
    x_medio3 = punto2.x + desp_xmedio3;
    //    y_medio1 = punto1.y+desp_ymedio;

    fprintf(f_salida_html, "<!-- conecta_p1_dx1_y_dx2_p2 -->\n");
    fprintf(f_salida_html,
            "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n\n",
            punto1.x, punto1.y, x_medio1, punto1.y, x_medio1, y_medio1, x_medio2, y_medio1, x_medio2, y_medio2,
            x_medio3, y_medio2, x_medio3, punto2.y, punto2.x, punto2.y, "none", color, trazo_fino);
}
*/
/***************************************************************
 *
 * Func: conecta_x
 *
 * Desc: Dibuja la conexión entre dos puntos p1 y p2 moviendo en x
 *
 **************************************************************/

/*
static void conecta_x (coord_t punto1, coord_t punto2, char* color)
{
    fprintf (f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", punto1.x,punto1.y,punto2.x,punto1.y,punto2.x, punto2.y,"none", color, trazo_fino) ;
}
 */

/***************************************************************
 *
 * Func: conecta_y
 *
 * Desc: Dibuja la conexión entre dos puntos p1 y p2, moviendo en y
 *
 **************************************************************/

/*
static void conecta_y (coord_t punto1, coord_t punto2, char* color)
{
    fprintf (f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", punto1.x,punto1.y,punto1.x,punto2.y, punto2.x, punto2.y,"none", color, trazo_fino) ;
}
 */

/***************************************************************
 *
 * Func: dibuja_datapath
 *
 * Desc: Dibuja el datapath. Necesita que este abierto f_salida_html
 *
 **************************************************************/

void dibuja_datapath() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];
    elem_1i1o_t RegPC, ext;

    elem_6i6o_t RegIF_ID, RegID_EX, RegEX_M, RegM_WB;
    elem_2i1o_t alu_IF_PC, alu_EX, alu_ID, zeroID, zeroEX, mem_instr, mem_datos;
    elem_4i2o_t regs;
    // Mux 4 entradas
    mux4_t mux4_EX_alu_s, mux4_EX_alu_i;
    // Mux 3 entradas
    mux3_t mux3_WB, mux3_ID_addr_s, mux3_EX_zero_s, mux3_EX_zero_i;
    // Mux 2 entradas
    mux2_t mux2_EX_alu_s, mux2_EX_alu_i, mux2_EX_store, mux2_MEM_store, mux2_pc;
    mux2_t mux2_ID_zero_s, mux2_ID_zero_i;

    muxctrl_t muxctrl; // para los MUX

    char col_IF[20], col_ID[20], col_EX[20], col_M[20], col_WB[20];

    /*
     char *	registro=
     "<symbol id=\"registro\">\n"
     "    <polygon points=\"0,0 20,0 20,60 0,60\" />\n"
     "</symbol>\n";

     char *	registro_intermedio=
     "<symbol id=\"registro_intermedio\">\n"
     "    <polygon points=\"0,0 20,0 20,200 0,200\" />\n"
     "</symbol>\n";

     char *	bancoreg=
     "<symbol id=\"bancoreg\">\n"
     "    <polygon points=\"0,0 40,0 40,80 0,80\" />\n"
     "</symbol>\n";

     char *	memoria=
     "<symbol id=\"memoria\">\n"
     "    <polygon points=\"0,0 40,0 40,80 0,80\" />\n"
     "</symbol>\n";

     char *	alu=
     "<symbol id=\"alu\">\n"
     "    <polyline points=\"0,0 0,40 10,50 0,60 0,100 40,80 40,30 0,0\" />\n"
     "</symbol>\n";


     char *	triangulo=
     "<symbol id=\"triangulo\">\n"
     "    <polyline points=\"10,2  60,2  35,52  10,2\""
     "    style=\"stroke:#006600; fill: #33cc33;\"/>\n"
     "</symbol>\n";
     */


    /*************************************/
    /*  Function body                    */
    /*************************************/


    /*** Datapath ***/

    // Colores
    color_instruccion(col_IF, PC);
    color_instruccion(col_ID, IF_ID.iPC);
    color_instruccion(col_EX, ID_EX.iPC);
    color_instruccion(col_M, EX_MEM.iPC);
    color_instruccion(col_WB, MEM_WB.iPC);


    /*
     sprintf(col_IF, "red");
     sprintf(col_ID, "blue");
     sprintf(col_EX,"green");
     sprintf(col_M, "magenta");
     sprintf(col_WB,"brown");
     */

    fprintf(f_salida_html, "<!-- Datapath en SVG -->\n");
    fprintf(f_salida_html,
            "<svg style='width: %s' viewbox=\"0 0 1100 600\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" shape-rendering=\"geometricPrecision\" >\n",
            SVG_WIDTH);
    //fprintf(f_salida_html, "<svg style='max-width: 820px' width=\"100%%\" viewbox=\"0 0 1100 600\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" shape-rendering=\"geometricPrecision\" >\n");
    //fprintf(f_salida_html, "<svg width=\"800\" viewbox=\"0 0 1100 650\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" shape-rendering=\"geometricPrecision\" >\n");
    //fprintf(f_salida_html, "<svg width=\"1100\" height=\"650\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" shape-rendering=\"geometricPrecision\" >\n");
    fprintf(f_salida_html, "<symbol id=\"datapath\">\n");

    imprime_instruccion_color(aux, PC, NO);
    escribe(PC_x + 0 * sep1x, eje_y_texto, aux, col_IF, 18);
    imprime_instruccion_color(aux, IF_ID.iPC, NO);
    escribe(IFID_x + 4 * sep1x, eje_y_texto, aux, col_ID, 18);
    imprime_instruccion_color(aux, ID_EX.iPC, NO);
    escribe(IDEX_x + 4 * sep1x, eje_y_texto, aux, col_EX, 18);
    imprime_instruccion_color(aux, EX_MEM.iPC, NO);
    escribe(EXM_x + 4 * sep1x, eje_y_texto, aux, col_M, 18);
    imprime_instruccion_color(aux, MEM_WB.iPC, NO);
    escribe(MWB_x + 3 * sep1x, eje_y_texto, aux, col_WB, 18);

    /****************/
    /*** Etapa IF ***/
    /****************/
    // PC
    genera_latch(&RegPC, PC_x, PC_y, reg_ancho, PC_alto, "", "PC", "black", "black", "lightgray");
    // MI
    genera_MI(&mem_instr, mem_instr_x, mem_instr_y, mem_ancho, mem_alto, col_IF, "black", "lightgray");
    // PC -> MI
    conecta(RegPC.out1, mem_instr.in1, col_IF);
    // Sum PC
    genera_sum(&alu_IF_PC, alu_IF_PC_x, alu_IF_PC_y, aluPC_ancho, aluPC_alto, "+", "black", "lightgray", col_IF, 4, PC,
               IF_IDn.NPC);
    //escribe(alu_IF_PC.in1.x - 20, alu_IF_PC.in1.y, "4", "black", 14);
    // PC -> Sum PC
    conecta(RegPC.out1, alu_IF_PC.in2, col_IF);
    // Señales
    escribe(PC_x, PC_y + PC_alto + 20, (IFstall ? "IFstall" : ""), "black", 14);

    // mux PC
    // muxctrl.ctrl = (SaltoEfectivo ? 0 : 2);
    muxctrl.ctrl = 2; // No salta
    if latencia_salto1(solucion_riesgos_control) {
        if (SaltoEfectivo) {
            muxctrl.ctrl = 0;
            sprintf(muxctrl.color, "%s", col_ID);
        } else {
            sprintf(muxctrl.color, "%s", col_IF);
        }
    } else if latencia_salto2(solucion_riesgos_control) {
        if (SaltoEfectivo) {
            muxctrl.ctrl = 0;
            sprintf(muxctrl.color, "%s", col_EX);
        } else {
            sprintf(muxctrl.color, "%s", col_IF);
        }
    } else {
        if (SaltoEfectivo) {
            muxctrl.ctrl = 0;
            sprintf(muxctrl.color, "%s", col_M);
        } else {
            sprintf(muxctrl.color, "%s", col_IF);
        }
    }
    //genera_mux2(&mux_pc, muxpc_x, muxpc_y, mux2_ancho, mux2_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");
    genera_mux2(&mux2_pc, muxpc_x, muxpc_y, mux2_ancho, mux2_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");
    // Sum Alu PC-> muxPC
    conecta_p1_dx1_y_dx2_p2(alu_IF_PC.out1, mux2_pc.in2, sep1x, mux2_pc.in2.y, -sep1x, col_IF);
    // Registro IF_ID
    genera_latch6(&RegIF_ID, IFID_x, IFID_y, reg_ancho, reg_alto, IFID_y_pc, IFID_y_npc, 0, 0, 0, mem_instr.out1.y,
                  "IF_ID", "pc", "npc", "", "", "", "IR", "red", "gray", "lightgray");
    // Señales
    escribe(IFID_x, IFID_y + reg_alto + 20, (IDstall ? "IDstall" : ""), "black", 14);

    // SumPC --> IF_ID.npc
    conecta_p1_dx1_y_dx2_p2(alu_IF_PC.out1, RegIF_ID.in2, sep1x, RegIF_ID.in2.y, 0, col_IF);
    // MI --> IF_ID.IR
    conecta(mem_instr.out1, RegIF_ID.in6, col_IF);
    // muxPC --> PC
    //conecta_p1_dx1_y_dx2_p2(mux_pc.out1, RegPC.in1, sep1x, alu_IF_PC.pos2.y + (RegPC.pos1.y - alu_IF_PC.pos2.y) / 2, -sep1x, col_IF);
    conecta_p1_dx1_y_dx2_p2(mux2_pc.out1, RegPC.in1, sep1x, RegPC.pos1.y - 5 * sep1y, -sep1x, col_IF);
    // PC --> IF_ID.pc
    conecta_p1_dx1_y_dx2_p2(RegPC.out1, RegIF_ID.in1, sep1x, RegIF_ID.in1.y, -sep1x, col_IF);


    /****************/
    /*** Etapa ID ***/
    /****************/
    // Banco de registros
    genera_regs(&regs, regs_x, regs_y, regs_ancho, regs_alto, IF_ID.IR.rs1, IF_ID.IR.rs2, col_ID, MEM_WB.IR.rd, WB.data,
                col_WB, "black", "lightgray");
    // Extension de signo
    genera_elipse(&ext, ext_x, ext_y, elipse_ancho, elipse_alto, "Ext Signo", "", "", col_ID, "black", "lightgray");
    // Control
    //genera_elipse (&ctrl, control_x, control_y, elipse_ancho,elipse_alto, "Control", "", "", "black", "black", "lightgray");

    // IF_ID.IR -> Rfte1
    conecta_p1_dx1_y_dx2_p2(RegIF_ID.out6, regs.in1, sep1x, regs.in1.y, 0, col_ID);
    // IF_ID.IR -> Rfte2
    conecta_p1_dx1_y_dx2_p2(RegIF_ID.out6, regs.in2, sep1x, regs.in2.y, 0, col_ID);
    // IF_ID.IR -> Extension signo
    conecta_p1_dx1_y_dx2_p2(RegIF_ID.out6, ext.in1, sep1x, ext.in1.y, 0, col_ID);
    // Control
    // conecta_p1_dx1_y_dx2_p2 (RegIF_ID.out3, ctrl.in1, sep1x, ctrl.in1.y, 0, col_ID);


    // ID_EX
    genera_latch6(&RegID_EX, IDEX_x, IDEX_y, reg_ancho, reg_alto, RegIF_ID.out1.y, RegIF_ID.out2.y, regs.out1.y,
                  regs.out2.y, ext.out1.y, ext.out1.y + 5 * sep1y, "ID_EX", "pc", "npc", "A", "B", "Imm", "IR",
                  "red", "gray", "lightgray");


    // IF_ID.IR -> ID_EX.IR
    conecta_p1_dx1_y_dx2_p2(RegIF_ID.out6, RegID_EX.in6, sep1x, RegID_EX.out6.y, 0, col_ID);
    // IF_ID.nPC -> ID_EX.nPC
    conecta(RegIF_ID.out2, RegID_EX.in2, col_ID);

    // Banco de registros A -> ID_EX
    conecta(regs.out1, RegID_EX.in3, col_ID);
    // Banco de registros A -> ID_EX
    conecta(regs.out2, RegID_EX.in4, col_ID);
    // Ext signo -> ID_EX
    conecta(ext.out1, RegID_EX.in5, col_ID);

    // Si ds1
    if latencia_salto1(solucion_riesgos_control) {
        // Cmp Zero
        genera_sum(&zeroID, zeroid_x, zeroid_y, zero_ancho, zero_alto, "cmp", "black", "lightgray",
                   ((inst_branch(IF_ID.IR.codop) && !IDstall) ? col_ID : "lightgray"), ID.compin1, ID.compin2,
                   ID.cond_out);
        // genera_latch(&zero_id, zeroid_x, zeroid_y, zero_ancho, zero_alto, "", "zero", "red", "black", "lightgray");
        // Sum calculo PC
        genera_sum(&alu_ID, alu_ID_x, alu_ID_y, aluPC_ancho, aluPC_alto, "+", "black", "lightgray",
                   ((inst_salto(IF_ID.IR.codop) && !IDstall) ? col_ID : "lightgray"), ID.sumin1, (word) IF_ID.IR.imm,
                   ID.dir);
        // Ext signo -> Sum calculo PC
        conecta_p1_dx1_y_dx2_p2(ext.out1, alu_ID.in2, sep1x, ext.pos1.y - sep1y, -3 * sep1x, col_ID);
        // Cmp Zero -> control mux PC (IF)
        conecta_p1_dx1_y_dx2_p2(zeroID.out1, mux2_pc.ctrl, sep1x, RegIF_ID.pos1.y - 5 * sep1y, 0, col_ID);
        // Sum calculo PC -> mux PC (IF)
        conecta_p1_dx1_y_dx2_p2(alu_ID.out1, mux2_pc.in1, sep1x, RegIF_ID.pos1.y - 4 * sep1y, -sep1x, col_ID);
        //conecta_p1_dx1_y1_dx2_y2_dx3_p2(regs.out1, mux_pc.in2, sep1x, regs.pos1.y - sep1y, -6 * sep1x, RegIF_ID.pos1.y - 5 * sep1y, -2 * sep1x, col_ID);

        // Si no hay cortocircuitos
        if (solucion_riesgos_datos != cortocircuito) {
            // Banco de registros A -> Cmp Zero
            conecta_p1_dx1_y_dx2_p2(regs.out1, zeroID.in1, sep1x, regs.pos1.y - sep1y, -2 * sep1x, col_ID);
            // Banco de registros B -> Cmp Zero
            conecta_p1_dx1_y_dx2_p2(regs.out2, zeroID.in2, 2 * sep1x, regs.pos1.y - 2 * sep1y, -sep1x, col_ID);
            // mux Sum calculo PC (para jalr)
            genera_mux3(&mux3_ID_addr_s, mux3_ID_addr_s_x, alu_ID.in1.y, mux3_ancho, mux3_alto,
                        !inst_jalr(IF_ID.IR.codop), col_ID, "black", "lightgray");
            // IF_ID.pc -> mux3_ID_addr_s
            //conecta(RegIF_ID.out1, mux3_ID_addr_s.in2,col_ID)
            conecta_p1_dx1_y_dx2_p2(RegIF_ID.out1, mux3_ID_addr_s.in2, sep1x, mux3_ID_addr_s.in2.y, 0, col_ID);
            // Banco de registros A -> mux sup zero
            conecta_p1_dx1_y_dx2_p2(regs.out1, mux3_ID_addr_s.in3, sep1x, regs.pos1.y - sep1y, -sep1x, col_ID);
            // mux Sum -> Sum calculo PC
            conecta(mux3_ID_addr_s.out1, alu_ID.in1, col_ID);
        } else {
            // mux ZERO en ID
            if (MEMaIDcomp_s) {
                muxctrl.ctrl = 0;
                sprintf(muxctrl.color, "%s", col_M);
            } else {
                muxctrl.ctrl = 1;
                sprintf(muxctrl.color, "%s", col_ID);
            }
            // mux sup Zero
            genera_mux2(&mux2_ID_zero_s, muxZeroID_x, zeroID.in1.y, mux2_ancho, mux2_alto, muxctrl.ctrl, muxctrl.color,
                        "black", "lightgray");
            // mux sup Zero -> Cmp Zero
            conecta(mux2_ID_zero_s.out1, zeroID.in1, col_ID);
            // Banco de registros A -> mux sup zero
            conecta_p1_dx1_y_dx2_p2(regs.out1, mux2_ID_zero_s.in2, sep1x, regs.pos1.y - sep1y, -2 * sep1x, col_ID);

            // mux ZERO en ID
            if (MEMaIDcomp_i) {
                muxctrl.ctrl = 0;
                sprintf(muxctrl.color, "%s", col_M);
            } else {
                muxctrl.ctrl = 2;
                sprintf(muxctrl.color, "%s", col_ID);
            }
            // mux inf Zero
            genera_mux2(&mux2_ID_zero_i, muxZeroID_x, zeroID.in2.y, mux2_ancho, mux2_alto, muxctrl.ctrl, muxctrl.color,
                        "black", "lightgray");
            // mux inf Zero -> Cmp Zero
            conecta(mux2_ID_zero_i.out1, zeroID.in2, col_ID);
            // Banco de registros B -> muz sup zero
            conecta_p1_dx1_y_dx2_p2(regs.out2, mux2_ID_zero_i.in2, 2 * sep1x, regs.pos1.y - 2 * sep1y, -sep1x, col_ID);
            // mux Sum calculo PC (para jalr)
            if (MEMaIDcomp_s) {
                muxctrl.ctrl = 0;
                sprintf(muxctrl.color, "%s", col_M);
            } else if inst_jalr(IF_ID.IR.codop) {
                muxctrl.ctrl = 2;
                sprintf(muxctrl.color, "%s", col_ID);
            } else {
                muxctrl.ctrl = 1;
                sprintf(muxctrl.color, "%s", col_ID);
            }
            genera_mux3(&mux3_ID_addr_s, mux3_ID_addr_s_x, alu_ID.in1.y, mux3_ancho, mux3_alto, muxctrl.ctrl, col_ID,
                        "black", "lightgray");
            // IF_ID.pc -> mux3_ID_addr
            conecta_p1_dx1_y_dx2_p2(RegIF_ID.out1, mux3_ID_addr_s.in2, sep1x, mux3_ID_addr_s.in2.y, 0, col_ID);
            // Banco de registros A -> mux sup zero
            conecta_p1_dx1_y_dx2_p2(regs.out1, mux3_ID_addr_s.in3, sep1x, regs.pos1.y - sep1y, -sep1x, col_ID);

            // Banco de registros A -> mux sup Zero -> mux Sum
            //conecta_p1_dx1_y_dx2_p2(mux3_ID_zero_s.out1, mux3_ID_addr_s.in1, sep1x, RegIF_ID.pos1.y - 2 * sep1y, -sep1x, col_ID);
            // mux Sum -> Sum calculo PC
            conecta(mux3_ID_addr_s.out1, alu_ID.in1, col_ID);
        }
    }


    // IF_ID.pc -> ID_EX.pc
    conecta(RegIF_ID.out1, RegID_EX.in1, col_ID);

    // Valor de mux_IF_PC.in1
    if (SaltoEfectivo) {
        if (latencia_salto1(solucion_riesgos_control)) {
            imprime_valor(ID.dir, 10000, aux);
            escribe(alu_IF_PC.pos2.x - 3 * sep1x, alu_IF_PC_y, aux, col_M, 14);
        } else if (latencia_salto2(solucion_riesgos_control)) {
            imprime_valor(EX_MEMn.ALUout, 10000, aux);
            escribe(alu_IF_PC.pos2.x - 3 * sep1x, alu_IF_PC_y, aux, col_M, 14);
        } else {
            imprime_valor(EX_MEM.ALUout, 10000, aux);
            escribe(alu_IF_PC.pos2.x - 3 * sep1x, alu_IF_PC_y, aux, col_M, 14);
        }
    }

    /****************/
    /*** Etapa EX ***/
    /****************/
    // ALU
    genera_ALU(&alu_EX, alu_x, alu_y, alu_ancho, alu_alto, "ALU", "black", "lightgray", col_EX, EX.aluin1, EX.aluin2,
               EX.aluout);

    // Si ds1
    if latencia_salto1(solucion_riesgos_control) {
        // EX_M, sin cond
        genera_latch6(&RegEX_M, EXM_x, EXM_y, reg_ancho, reg_alto, zeroEX.out1.y, RegID_EX.out2.y, alu_EX.out1.y,
                      RegID_EX.out5.y - sep1y, 0, RegID_EX.out6.y, "EX_MEM", "", "npc", "ALUout", "B", "", "IR", "red",
                      "gray", "lightgray");
    }

    // Si ds2
    if latencia_salto2(solucion_riesgos_control) {
        // Zero Cmp
        genera_sum(&zeroEX, ZeroEX_x, ZeroEX_y, zero_ancho, zero_alto, "cmp", "black", "lightgray",
                   (inst_branch(ID_EX.IR.codop) ? col_EX : "lightgray"), EX.compin1, EX.compin2, EX.cond_out);
        // EX_M, cond
        genera_latch6(&RegEX_M, EXM_x, EXM_y, reg_ancho, reg_alto, zeroEX.out1.y, RegID_EX.out2.y, alu_EX.out1.y,
                      RegID_EX.out5.y - sep1y, 0, RegID_EX.out6.y, "EX_MEM", "", "npc", "ALUout", "B", "", "IR", "red",
                      "gray", "lightgray");
        // Zero Cmp -> muxPC control (IF)
        conecta_p1_dx1_y_dx2_p2(zeroEX.out1, mux2_pc.ctrl, sep1x, RegID_EX.pos1.y - 4 * sep1y, 0, col_EX);
        // ALU out -> muxPC (IF)
        conecta_p1_dx1_y_dx2_p2(alu_EX.out1, mux2_pc.in1, sep1x, RegID_EX.pos1.y - 5 * sep1y, -sep1x, col_EX);
    }

    // Si pnt o ds3
    if latencia_salto3(solucion_riesgos_control) {
        // Zero Cmp
        genera_sum(&zeroEX, ZeroEX_x, ZeroEX_y, zero_ancho, zero_alto, "cmp", "black", "lightgray",
                   (inst_branch(ID_EX.IR.codop) ? col_EX : "lightgray"), EX.compin1, EX.compin2, EX.cond_out);
        // EX_M, con cond
        genera_latch6(&RegEX_M, EXM_x, EXM_y, reg_ancho, reg_alto, zeroEX.out1.y, RegID_EX.out2.y, alu_EX.out1.y,
                      RegID_EX.out5.y - sep1y, 0, RegID_EX.out6.y, "EX_MEM", "cond", "npc", "ALUout", "B", "", "IR",
                      "red", "gray", "lightgray");
        // Zero Cmp -> EX_M.cond
        conecta(zeroEX.out1, RegEX_M.in1, col_EX);
    }

    // Si no hay cortocircuitos
    if (solucion_riesgos_datos != cortocircuito) {
        if (!latencia_salto1(solucion_riesgos_control)) {

            // ID_EX.A -> CMP zero
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out3, zeroEX.in1, RegA_x, zeroEX.in1.y, 0, col_EX);
            // ID_EX.B -> CMP zero
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, zeroEX.in2, RegB_x, zeroEX.in2.y, 0, col_EX);
        } else { // Si latencia 1, no hay mux superior

        }
// mux ALUs
        if (inst_leePC(ID_EX.IR.codop)) {
            muxctrl.ctrl = 0;
        } else {
            muxctrl.ctrl = 1;
        }
        // mux ALUsup
        genera_mux2(&mux2_EX_alu_s, mux2s_x, alu_EX.in1.y, mux2_ancho, mux2_alto, muxctrl.ctrl, col_EX, "black",
                    "lightgray");
        // mux ALUsup -> ALU
        conecta(mux2_EX_alu_s.out1, alu_EX.in1, col_EX);
        // ID_EX.A -> mux ALUsup
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out3, mux2_EX_alu_s.in2, RegA_x, mux2_EX_alu_s.in2.y, 0, col_EX);
        // ID_EX.PC -> mux ALUsup
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out1, mux2_EX_alu_s.in1, NPC_x, mux2_EX_alu_s.in1.y, 0, col_EX);
        // mux ALUinf
        muxctrl.ctrl = (ID_EX.IR.tipo == FormatoR ? 0 : 1);
        // mux ALUinf
        genera_mux2(&mux2_EX_alu_i, mux2i_x, alu_EX.in2.y, mux2_ancho, mux2_alto, muxctrl.ctrl, col_EX, "black",
                    "lightgray");
        // mux ALUinf -> ALU
        conecta(mux2_EX_alu_i.out1, alu_EX.in2, col_EX);
        // ID_EX.B -> mux ALUinf
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, mux2_EX_alu_i.in1, RegB_x, mux2_EX_alu_i.in1.y, 0, col_EX);
        // ID_EX.Imm -> muxALUinf
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out5, mux2_EX_alu_i.in2, Imm_x, mux2_EX_alu_i.in2.y, 0, col_EX);
        // ID_EX.B -> EX/M.B
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, RegEX_M.in4, RegB_x, RegEX_M.in4.y, 0, col_EX);
    }

    // Si hay cortocircuitos
    if (solucion_riesgos_datos == cortocircuito) {
        // mux ALU sup
        muxctrl.ctrl = EX.muxALU_sup;
        switch (muxctrl.ctrl) {
            case 0:
            case 3:
                sprintf(muxctrl.color, "%s", col_EX);
                break;
            case 1:
                sprintf(muxctrl.color, "%s", col_M);
                break;
            case 2:
                sprintf(muxctrl.color, "%s", col_WB);
                break;
            default:
                sprintf(muxctrl.color, "%s", "white");
        }
        // mux ALUsup
        genera_mux4(&mux4_EX_alu_s, mux4s_x, alu_EX.in1.y, mux4_ancho, mux4_alto, muxctrl.ctrl, muxctrl.color, "black",
                    "lightgray");

        // mux ALU inf
        muxctrl.ctrl = EX.muxALU_inf;
        switch (muxctrl.ctrl) {
            case 0:
            case 3:
                sprintf(muxctrl.color, "%s", col_EX);
                break;
            case 1:
                sprintf(muxctrl.color, "%s", col_M);
                break;
            case 2:
                sprintf(muxctrl.color, "%s", col_WB);
                break;
            default:
                sprintf(muxctrl.color, "%s", "white");
        }
        // mux ALUinf
        genera_mux4(&mux4_EX_alu_i, mux4s_x, alu_EX.in2.y, mux4_ancho, mux4_alto, muxctrl.ctrl, muxctrl.color, "black",
                    "lightgray");

        // mux MEM en EX
        muxctrl.ctrl = (WBaEXmem ? 1 : 0);
        if (WBaEXmem) {
            sprintf(muxctrl.color, "%s", col_WB);
        } else {
            sprintf(muxctrl.color, "%s", col_EX);
        }
        genera_mux2(&mux2_EX_store, muxW_x, RegEX_M.in4.y, mux2_ancho, mux2_alto, muxctrl.ctrl, muxctrl.color, "black",
                    "lightgray");

        // mux ALUsup -> ALU
        conecta(mux4_EX_alu_s.out1, alu_EX.in1, col_EX);
        // mux ALUinf -> ALU
        conecta(mux4_EX_alu_i.out1, alu_EX.in2, col_EX);
        // ID_EX.A -> mux ALUsup
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out3, mux4_EX_alu_s.in4, RegA_x, mux4_EX_alu_s.in4.y, 0, col_EX);
        // ID_EX.B -> mux ALUinf
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, mux4_EX_alu_i.in1, RegB_x, mux4_EX_alu_i.in1.y, 0, col_EX);


        // ID_EX.pc -> mux ALUsup
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out1, mux4_EX_alu_s.in1, 3 * sep1x, mux4_EX_alu_s.in1.y, 0, col_EX);
        // ID_EX.Imm -> mux ALUinf
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out5, mux4_EX_alu_i.in4, Imm_x, mux4_EX_alu_i.in4.y, 0, col_EX);
        // Reg B -> Mem
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, mux2_EX_store.in1, RegB_x, mux2_EX_store.in1.y, 0, col_EX);
        // ID_EX.B -> EX/M.B
        conecta(mux2_EX_store.out1, RegEX_M.in4, col_EX);

        // mux sup ZERO
        if (!latencia_salto1(solucion_riesgos_control)) {
            // mux sup Zero
            if (WBaEXcomp_s) {
                muxctrl.ctrl = 2;
                sprintf(muxctrl.color, "%s", col_WB);
            } else if (MEMaEXcomp_s) {
                muxctrl.ctrl = 1;
                sprintf(muxctrl.color, "%s", col_M);
            } else {
                muxctrl.ctrl = 0;
                sprintf(muxctrl.color, "%s", col_EX);
            }
            // mux sup ZERO
            genera_mux3(&mux3_EX_zero_s, muxZeroEX_x, zeroEX.in1.y, mux3_ancho, mux3_alto, muxctrl.ctrl, muxctrl.color,
                        "black", "lightgray");
            // ID_EX.A -> muxZERO
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out3, mux3_EX_zero_s.in1, sep1x, mux3_EX_zero_s.in1.y, 0, col_EX);
            // mux Zero -> Cmp Zero
            conecta(mux3_EX_zero_s.out1, zeroEX.in1, col_EX);

            // mux inf Zero
            if (WBaEXcomp_i) {
                muxctrl.ctrl = 2;
                sprintf(muxctrl.color, "%s", col_WB);
            } else if (MEMaEXcomp_i) {
                muxctrl.ctrl = 1;
                sprintf(muxctrl.color, "%s", col_M);
            } else {
                muxctrl.ctrl = 0;
                sprintf(muxctrl.color, "%s", col_EX);
            }
            // mux ZERO s
            genera_mux3(&mux3_EX_zero_i, muxZeroEX_x, zeroEX.in2.y, mux3_ancho, mux3_alto, muxctrl.ctrl, muxctrl.color,
                        "black", "lightgray");
            // ID_EX.B -> mux inf ZERO
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, mux3_EX_zero_i.in1, 2 * sep1x, mux3_EX_zero_i.in1.y, 0, col_EX);
            // mux inf Zero -> Cmp Zero
            conecta(mux3_EX_zero_i.out1, zeroEX.in2, col_EX);
        }
    }

    // ID_EX.npc -> EX_M.npc
    conecta(RegID_EX.out2, RegEX_M.in2, col_EX);

    // ALUout -> EX_M.Aluout
    conecta(alu_EX.out1, RegEX_M.in3, col_EX);

    // ID_EX.IR -> EX_M.IR
    conecta(RegID_EX.out6, RegEX_M.in6, col_EX);

    /****************/
    /*** Etapa MEM ***/
    /****************/
    // MD
    genera_MD(&mem_datos, mem_datos_x, mem_datos_y, mem_ancho, mem_alto, col_M, "black", "lightgray");
    // EX_M.Aluout -> MD Dir
    conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mem_datos.in1, 3 * sep1x, mem_datos.in1.y, 0, col_M);
    // M_WB
    genera_latch6(&RegM_WB, MWB_x, MWB_y, reg_ancho, reg_alto, 0, RegEX_M.out2.y, mem_datos.pos1.y - 2 * sep1y,
                  mem_datos.out1.y, 0, RegEX_M.out6.y, "MEM_WB", "", "npc", "ALUout", "LMD", "", "IR", "red", "gray",
                  "lightgray");
    // MD Read > M_WB
    conecta(mem_datos.out1, RegM_WB.in4, col_M);
    // EX_M.ALUout -> M_WB
    conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, RegM_WB.in3, 3 * sep1x, RegM_WB.in3.y, 0, col_M);
    // EX_M.IR -> M_WB.IR
    conecta(RegEX_M.out6, RegM_WB.in6, col_M);
    // EX_M.npc -> M_WB.npc
    conecta(RegEX_M.out2, RegM_WB.in2, col_M);
    // Si no hay cortocircuitos
    if (solucion_riesgos_datos != cortocircuito) {
        // EX_M.B -> M Write
        conecta(RegEX_M.out4, mem_datos.in2, col_M);
    } else {
        // mux MEM en MEM
        muxctrl.ctrl = (WBaMEM ? 1 : 0);
        if (WBaMEM) {
            sprintf(muxctrl.color, "%s", col_WB);
        } else {
            sprintf(muxctrl.color, "%s", col_M);
        }
        // mux Mem
        genera_mux2(&mux2_MEM_store, muxmem_x, RegEX_M.in4.y + 5, mux2_ancho, mux2_alto, muxctrl.ctrl, muxctrl.color,
                    "black", "lightgray");
        // mux Mem -> Mem Write
        conecta(mux2_MEM_store.out1, mem_datos.in2, col_M);
        // EX_M.B -> mux Mem
        conecta(RegEX_M.out4, mux2_MEM_store.in1, col_M);
        // EX_M -> muxALUsup (EX) corto MaEX
        conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux4_EX_alu_s.in2, 3 * sep1x, RegEX_M.pos1.y - 3 * sep1y, -2 * sep1x,
                                col_M);
        // EX_M -> muxALUinf (EX) corto MaEX
        conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux4_EX_alu_i.in2, 3 * sep1x, RegEX_M.pos1.y - 3 * sep1y, -2 * sep1x,
                                col_M);
        if latencia_salto1(solucion_riesgos_control) {
            // EX_M -> mux Zero sup corto MaID
            conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux2_ID_zero_s.in1, 3 * sep1x, RegEX_M.pos1.y - 3 * sep1y, -sep1x,
                                    col_M);
            // EX_M -> mux Zero inf corto MaID
            conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux2_ID_zero_i.in1, 3 * sep1x, RegEX_M.pos1.y - 3 * sep1y, -sep1x,
                                    col_M);
            // EX_M -> mux ALUPC_ID sup corto MaID
            conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux3_ID_addr_s.in1, 3 * sep1x, RegEX_M.pos1.y - 3 * sep1y, -sep1x,
                                    col_M);
        } else {
            // EX_M -> mux Zero sup corto MaEX
            conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux3_EX_zero_s.in2, 3 * sep1x, RegEX_M.pos1.y - 3 * sep1y, -2 * sep1x,
                                    col_M);
            // EX_M -> mux Zero sup corto MaEX
            conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux3_EX_zero_i.in2, 3 * sep1x, RegEX_M.pos1.y - 3 * sep1y, -2 * sep1x,
                                    col_M);
        }
    }

    if latencia_salto3(solucion_riesgos_control) {
        // EX_M.cond -> mux PC (IF) control
        conecta_p1_dx1_y_dx2_p2(RegEX_M.out1, mux2_pc.ctrl, 2*sep1x, RegEX_M.pos1.y - 4 * sep1y, 0, col_M);
        // EX_M.Aluout -> mux PC (IF) Dir
        conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux2_pc.in1, 3 * sep1x, RegEX_M.pos1.y - 3 * sep1y, -sep1x, col_M);
    }
    // Valor de ALUout
    if (EX_MEM.iPC != -1) {
        imprime_valor(EX_MEM.ALUout, 10000, aux);
    } else {
        sprintf(aux, "%s", "");
    }
    escribe(RegM_WB.in3.x - 4 * sep1x, RegM_WB.in3.y - 1 * sep1y, aux, col_M, 14);


    /****************/
    /*** Etapa WB ***/
    /****************/
    // mux WB
    if (inst_jal_jalr(MEM_WB.IR.codop))
        muxctrl.ctrl = 0;
    else if (inst_int_load(MEM_WB.IR.codop))
        muxctrl.ctrl = 2;
    else
        muxctrl.ctrl = 1;
    // mux WB
    genera_mux3(&mux3_WB, muxwb_x, mem_datos.out1.y - sep1y, mux3_ancho, mux3_alto, muxctrl.ctrl, col_WB, "black",
                "lightgray");
    // Valor de WB.data
    if inst_to_Rint(MEM_WB.IR.codop) {
        imprime_valor(WB.data, 10000, aux);
        escribe(mux3_WB.pos2.x, mux3_WB.pos1.y, aux, col_WB, 14);
    }
    // M_WB.npc -> muxWB
    conecta_p1_dx1_y_dx2_p2(RegM_WB.out2, mux3_WB.in1, 0, RegM_WB.out2.y, -sep1x, col_WB);
    // M_WB.Aluout -> muxWB
    conecta(RegM_WB.out3, mux3_WB.in2, col_WB);
    // M_WB.Mem -> muxWB
    conecta(RegM_WB.out4, mux3_WB.in3, col_WB);
    // M_WB.IR.Rdst -> Banco Reg Rdst
    conecta_p1_dx1_y_dx2_p2(RegM_WB.out6, regs.in3, sep1x, RegM_WB.pos2.y + sep1y, -sep1x, col_WB);
    // M_WB -> Banco Reg W
    conecta_p1_dx1_y_dx2_p2(mux3_WB.out1, regs.in4, sep1x, RegM_WB.pos2.y + 2 * sep1y, -2 * sep1x, col_WB);

    if (solucion_riesgos_datos == cortocircuito) {
        // M_WB -> muxALUsup (EX) corto WBaEX
        conecta_p1_dx1_y_dx2_p2(mux3_WB.out1, mux4_EX_alu_i.in3, sep1x, RegM_WB.pos2.y + 2 * sep1y, -sep1x, col_WB);
        // M_WB -> muxALUinf (EX) corto WBaEX
        conecta_p1_dx1_y_dx2_p2(mux3_WB.out1, mux4_EX_alu_s.in3, sep1x, RegM_WB.pos2.y + 2 * sep1y, -sep1x, col_WB);
        // M_WB -> muxW mem (MEM) corto WBaMEM
        conecta_p1_dx1_y_dx2_p2(mux3_WB.out1, mux2_MEM_store.in2, sep1x, RegM_WB.pos2.y + 2 * sep1y, -sep1x, col_WB);
        // M_WB -> muxW EXmem (MEM) corto WBaExmem
        conecta_p1_dx1_y_dx2_p2(mux3_WB.out1, mux2_EX_store.in2, sep1x, RegM_WB.pos2.y + 2 * sep1y, -sep1x, col_WB);

        /* Corto WB a ID. No es necesario si lectura y escritura simult�neas */
        /*
         if (solucion_riesgos_control == ds1) {
         conecta_p1_dx1_y_dx2_p2 (mux3_WB.out1, mux3_zero_ID.in2, sep1x, RegEX_M.pos1.y-6*sep1y, -2*sep1x, col_WB);
         }
         */
        if (!latencia_salto1(solucion_riesgos_control)) {
            // WB_EX -> mux Zero sup corto WBaEX
            conecta_p1_dx1_y_dx2_p2(mux3_WB.out1, mux3_EX_zero_s.in3, sep1x, RegM_WB.pos2.y + 2 * sep1y, -sep1x,
                                    col_WB);
            // WB_EX -> mux Zero inf corto WBaEX
            conecta_p1_dx1_y_dx2_p2(mux3_WB.out1, mux3_EX_zero_i.in3, sep1x, RegM_WB.pos2.y + 2 * sep1y, -sep1x,
                                    col_WB);
        }
    }

    fprintf(f_salida_html, "</symbol> \n");
    fprintf(f_salida_html, "<use xlink:href=\"#datapath\"\n");
    //	fprintf(f_salida_html, "transform=\"translate(0) scale(0.77)\"/>\n");
    fprintf(f_salida_html, "</svg>\n\n");


} /* end dibuja_datapath */
