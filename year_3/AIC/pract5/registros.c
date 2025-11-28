/**
 * @file registros.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las funciones para acceso al banco de registros
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define registros_C

#include "registros.h"
#include "tipos.h"
#include "main.h"

char *nombre_int_reg[] = {
    "zero", "ra",
    "sp", "gp", "tp",
    "t0", "t1", "t2",
    "s0", "s1",
    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
    "t3", "t4", "t5", "t6"
};

char *nombre_fp_reg[] = {
    "ft0", "ft1", "ft2", "ft3", "ft4", "ft5", "ft6", "ft7",
    "fs0", "fs1",
    "fa0", "fa1",
    "fa2", "fa3", "fa4", "fa5", "fa6", "fa7",
    "fs2", "fs3", "fs4", "fs5", "fs6", "fs7", "fs8", "fs9", "fs10", "fs11",
    "ft8", "ft9", "ft10", "ft11"
};

valor_t lee_int_reg(
        dword n,
        tipo_t *tipo,
        boolean *dirty
        ) {
    *tipo = Rint_tipo[n];
    *dirty = Rint_dirty[n];
    return (Rint[n].valor);

}

void marca_int_reg(
        dword n,
        tipo_t tipo_dato,
        boolean activar_dirty
        ) {
    if (n != 0) {
        Rint_tipo[n] = tipo_dato;
        if (activar_dirty)
            Rint_dirty[n] = SI;
    }
}

void escribe_int_reg(
        dword n,
        xword dato,
        tipo_t tipo_dato,
        boolean activar_dirty
        ) {
    if (n != 0) {
        Rint[n].valor.int_d = dato;
    }
    marca_int_reg(n, tipo_dato, activar_dirty);
}

valor_t lee_fp_reg(
        dword n,
        tipo_t *tipo,
        boolean *dirty
        ) {
    *tipo = Rfp_tipo[n];
    *dirty = Rfp_dirty[n];
    return (Rfp[n].valor);
}


void marca_fp_reg(
        dword n,
        tipo_t tipo_dato,
        boolean activar_dirty
        ) {
    Rfp_tipo[n] = tipo_dato;
    if (activar_dirty)
        Rfp_dirty[n] = SI;
}

void escribe_fp_reg(
        dword n,
        valor_t dato,
        tipo_t tipo_dato,
        boolean activar_dirty
        ) {
    Rfp[n].valor = dato;
    marca_fp_reg(n, tipo_dato, activar_dirty);
}
