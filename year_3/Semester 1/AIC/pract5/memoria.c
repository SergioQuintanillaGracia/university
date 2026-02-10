/**
 * @file memoria.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las funciones de acceso a la memoria física de datos e instrucciones.
 *    El acceso a la memoria principal se implementará en la jerarquía de memoria.
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define memoria_C

/* --- Includes --- */

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

//#include "presentacion.h"
#include "depurar.h"
#include "instrucciones.h"
#include "main.h"
#include "memoria.h"

/**
 * Devuelve el tamaño del tipo de dato
 *
 * @param tipo Tipo de dato
 * @return Tamaño en bytes del tipo de dato
 */
size_t tam_dato(tipo_t tipo) {

    size_t len;

    switch (tipo) {
        case t_byte:
            len = sizeof(byte);
            break;
        case t_half:
            len = sizeof(half);
            break;
        case t_word:
            len = sizeof(word);
            break;
        case t_dword:
            len = sizeof(dword);
            break;
        case t_float:
            len = sizeof(float);
            break;
        case t_float_ps:
            len = 2 * sizeof(float);
            break;
        case t_double:
            len = sizeof(double);
            break;
        case t_code:
            len = sizeof(word);
            break;
        default:
            len = 1;
    }

    return len;

}

/**
 * Devuelve una dirección re-alineada con un tamaño de dato (por arriba)
 *
 * @param direccion Dirección actual
 * @param size Tamaño al que se debe realinear la dirección
 * @return Dirección alineada (mayor o igual que la actual)
 */
dword mem_direccion_alineada(dword direccion, int size) {

    if (esta_alineada_dir(direccion, size)) {
        return direccion;
    } else {
        return direccion + size - (direccion % size);
    }
}

/**
 * Rango de memoria que se ve afectado por la escritura de un nuevo dato
 *
 * @param offset Desplazamiento dentro de la región de memoria
 * @param tipo Tipo de dato
 * @param m_from (out) Inicio de la zona de memoria afectada
 * @param m_to (out) Fin de la zona de memoria afectada
 */
static
void mem_rango(dword offset, tipo_t tipo, dword *m_from, dword *m_to) {

    size_t len = tam_dato(tipo);

    *m_from = indice_dir(offset, len) * len;
    *m_to = (*m_from) + len;

} /* end mem_rango */

void mem_etiqueta_datos(
        tipo_t *tipo_datos,
        boolean *dirty_datos,
        dword offset,
        tipo_t tipo,
        boolean activar_dirty
) {

    dword i;
    dword old_from, old_to;
    dword new_from, new_to;

    mem_rango(offset, tipo_datos[offset], &old_from, &old_to);
    for (i = old_from; i < old_to; i++)
        tipo_datos[i] = t_ninguno;

    // Los float_ps se etiquetan como floats
    tipo = (tipo != t_float_ps) ? tipo : t_float;

    mem_rango(offset, tipo, &new_from, &new_to);
    for (i = new_from; i < new_to; i++) {
        tipo_datos[i] = tipo;
        if (activar_dirty) {
            dirty_datos[i] = SI;
        }
    }

} /* end mem_etiqueta_datos */

/**
 * Re-etiqueta los tipos de datos de una zona de memoria para un tipo de dato
 *
 * @param mm Mapa de memoria
 * @param r Región donde se va a escribir el dato
 * @param offset Desplazamiento dentro de la región donde se va a escribir el dato
 * @param tipo Tipo de dato que se va a escribir en memoria
 */
void mem_etiqueta(
        mapa_memoria_t *mm,
        region_t r,
        dword offset,
        tipo_t tipo,
        boolean activar_dirty) {

    mem_etiqueta_datos(mm->datos[r].tipos, mm->datos[r].dirty, offset, tipo, activar_dirty);
}


static
void muestra_estado_mem_datos(
        mapa_memoria_t *mm,
        region_t r,
        dword offset,
        tipo_t tipo,
        mem_status_t status
) {

    dword old_from, old_to;
    dword new_from, new_to;
    dword min_dir, max_dir;

    mem_rango(offset, mm->datos[r].tipos[offset], &old_from, &old_to);
    mem_rango(offset, tipo, &new_from, &new_to);

    min_dir = MIN(old_from, new_from);
    max_dir = MAX(old_to, new_to);

    //muestra_datos(r, offset_a_dir(mm, min_dir, r), offset_a_dir(mm, max_dir, r), status);
}

void inicializa_zona_datos(
        zona_datos_t *zona,
        size_t tam,
        tipo_region_t tipo
) {
    dword d;

    zona->mem.m_byte = (byte *) malloc(tam);
    zona->tipos = (tipo_t *) calloc(tam, sizeof(tipo_t));
    zona->dirty = (boolean *) calloc(tam, sizeof(boolean));

    switch (tipo) {
        case tr_code:
            for (d = 0; d < tam / sizeof(word); d++) {
                // Cada posición de la región de instrucciones contendrá un índice sobre
                // el vector de instrucciones de la memoria principal
                // De momento no apuntan a ningún sitio
                zona->mem.m_word[d] = INST_UNKNOWN;
            }

            for (d = 0; d < tam; d++) {
                zona->tipos[d] = t_code;
                zona->dirty[d] = NO;
            }
            break;
        case tr_data:
        case tr_other:
            for (d = 0; d < tam; d++) {
                zona->mem.m_byte[d] = 0;
                zona->tipos[d] = t_ninguno;
                zona->dirty[d] = NO;
            }
            break;
    }

}

/**
 * Libera la memoria asociada a una región
 *
 * @param mm Mapa de memoria
 * @param r Región que se desea liberar
 */
static
void destruye_region(
        mapa_memoria_t *mm,
        region_t r
) {
    if (mm->datos[r].mem.m_byte != NULL) {
        free(mm->datos[r].mem.m_byte);
        mm->datos[r].mem.m_byte = NULL;
    }
    if (mm->datos[r].tipos != NULL) {
        free(mm->datos[r].tipos);
        mm->datos[r].tipos = NULL;
    }
    if (mm->datos[r].dirty != NULL) {
        free(mm->datos[r].dirty);
        mm->datos[r].dirty = NULL;
    }
}

/**
 * Inicializa un mapa de memoria
 *
 * @param mm Mapa de memoria
 */
void inicializa_memoria(
        mapa_memoria_t *mm
) {
    // Nombre de las regiones hasta 'num_regiones', después el nombre es inválido
    static char *nombre_region[num_regiones] = {
            "reserved", "text", "data", "stack"
    };

    region_t r;

    /* Regiones */

    for (r = (region_t) 0; r < num_regiones; r++) {
        mm->regiones[r].inicio = 0;
        mm->regiones[r].final = 0;

        // Alineación por defecto de las regiones
        if (es_region_datos(r)) {
            mm->regiones[r].align = (r == r_stack) ? 1 : ALIGN_DATA;
            mm->regiones[r].tipo = tr_data;
        } else if (es_region_codigo(r)) {
            mm->regiones[r].align = ALIGN_CODE;
            mm->regiones[r].tipo = tr_code;
        } else {
            mm->regiones[r].align = 1;
            mm->regiones[r].tipo = tr_other;
        }

        mm->regiones[r].nombre = nombre_region[r];

        mm->datos[r].mem.m_byte = NULL;
        mm->datos[r].tipos = NULL;
        mm->datos[r].dirty = NULL;
    }

    /* Memorias principal */

    for (r = (region_t) 0; r < num_regiones; r++) {
        if (es_region_datos(r)) {
            inicializa_zona_datos(&mm->datos[r], tam_regiones[r], tr_data);
        } else if (es_region_codigo(r)) {
            inicializa_zona_datos(&mm->datos[r], tam_regiones[r], tr_code);
        }
    }

    /* Vector de instrucciones */

    size_t max_instrucciones = TAM_MEMO_INSTRUC / sizeof(word);
    mm->instruc = (instruccion_t *) calloc(max_instrucciones, sizeof(instruccion_t));
    /* Código en ensamblador */
    mm->ensamblador = (texto_instruccion_t *) calloc(max_instrucciones, sizeof(texto_instruccion_t));

    for (dword i = 0; i < max_instrucciones; i++) {
        mm->instruc[i].codop = PSEUDO_NOP;
        mm->instruc[i].tipo = FormatoI;
        mm->instruc[i].rd = 0;
        mm->instruc[i].rs1 = 0;
        mm->instruc[i].rs2 = 0;
        mm->instruc[i].imm = 0;
        mm->instruc[i].valor = INSTRUC_NOP;

        mm->ensamblador[i].texto[0] = '\0';
    }

}

/**
 * Destruye un mapa de memoria
 *
 * @param mm Mapa de memoria
 */
void destruye_memoria(
        mapa_memoria_t *mm
) {
    region_t r;

    /* Regiones */
    for (r = (region_t) 0; r < num_regiones; r++) {
        if (es_region_datos(r) || es_region_codigo(r)) {
            destruye_region(mm, r);
        }
    }

    // Libera vector de instrucciones
    if (mm->instruc != NULL) {
        free(mm->instruc);
        mm->instruc = NULL;
    }
}

/**
 * Indica a que región pertenece una dirección de memoria.
 *
 * @param mm Mapa de memoria
 * @param dir Dirección absoluta
 * @return Región a la que pertenece la dirección. r_invalid si no pertenece a una región.
 */
region_t dir_a_region(
        mapa_memoria_t *mm,
        dword dir
) {
    region_t r;

    for (r = (region_t) 0; r < num_regiones; r++) {
        if (en_region(mm, dir, r)) {
            return r;
        }
    }

    return r_invalid;
}

/**
 * Comprueba el acceso a memoria. Hace las veces de MMU.
 *
 * @param memoria Mapa de memoria
 * @param addr Dirección de acceso
 * @param tipo Tipo del acceso
 * @return int Excepción en caso de error, EXC_NONE en otro caso
 */
int mem_chequea_direccion(mapa_memoria_t *memoria, dword addr, tipo_t tipo) {
    int excepcion = EXC_NONE;

    if (tipo == t_code) {
        if (en_region(memoria, addr, r_text)) {
            if (!esta_alineada_dir(addr, tam_dato(tipo))) {
                excepcion = EXC_UNALIGN_MEM_INST;
            }
        } else {
            excepcion = EXC_OUT_MEM_INST;
        }
    } else {
        if (en_region(memoria, addr, r_data) || en_region(memoria, addr, r_stack)) {
            if (!esta_alineada_dir(addr, tam_dato(tipo))) {
                excepcion = EXC_UNALIGN_MEM_DAT;
            }
        } else {
            excepcion = EXC_OUT_MEM_DAT;
        }
    }

    return excepcion;
}

/**
 * Lee un dato de una region de datos.
 *
 * @param mm Mapa de memoria
 * @param r Región de memoria de datos
 * @param offset Desplazamiento dentro de la region
 * @param tipo Tipo de dato al que se desea acceder
 * @return Valor del dato accedido
 */
valor_t mem_lee_region_datos(
        mapa_memoria_t *mm,
        region_t r,
        dword offset,
        tipo_t tipo,
        boolean *dirty
) {
    valor_t resultado;

    if (r >= num_regiones) {
        fnError("Región invalida");
    }

    switch (tipo) {
        case t_ninguno:
            memset(&resultado, 0, sizeof(valor_t));
            break;
        case t_byte:
        case t_char:
            resultado.int_d = mm->datos[r].mem.m_byte[indice_byte(offset)];
            break;
        case t_half:
            resultado.int_d = mm->datos[r].mem.m_half[indice_half(offset)];
            break;
        case t_word:
            resultado.int_d = mm->datos[r].mem.m_word[indice_word(offset)];
            break;
        case t_dword:
            resultado.int_d = mm->datos[r].mem.m_dword[indice_dword(offset)];
            break;
        case t_float:
            resultado.fp_s = mm->datos[r].mem.m_float[indice_float(offset)];
            break;
        case t_double:
            resultado.fp_d = mm->datos[r].mem.m_double[indice_double(offset)];
            break;
        case t_float_ps:
            resultado.fp_ps[Lo] = mm->datos[r].mem.m_float[indice_float(offset)];
            resultado.fp_ps[Hi] = mm->datos[r].mem.m_float[indice_float(offset + sizeof(float))];
            break;
        default: fnError("Lectura de datos no implementada");
    }
    *dirty = mm->datos[r].dirty[offset];
    return resultado;
}

/**
 * Lee un dato de la memoria de datos. Si la dirección no se corresponde con una región de datos da un error.
 *   No comprueba si el dato accedido tiene el mismo tipo que el especificado como parámetro.
 *
 * @param mm Mapa de memoria.
 * @param direccion Dirección absoluta del dato
 * @param tipo_dato Tipo del dato alq ue se desea acceder
 * @param file Fichero desde el que se realiza al lectura
 * @param line Línea desde que se realiza la lectura
 * @return Valor del dato accedido
 */
valor_t _lee_mem_datos(
        mapa_memoria_t *mm,
        dword direccion,
        tipo_t tipo_dato,
        boolean *dirty,
        char *file,
        int line
) {
    valor_t resultado;
    dword offset;
    region_t r;

    r = dir_a_region(mm, direccion);

    if (es_region_datos(r)) {
        offset = dir_a_offset(mm, direccion, r);

        resultado = mem_lee_region_datos(mm, r, offset, tipo_dato, dirty);
    } else {
        fprintf(stderr, "ERROR: Lectura fuera del rango de memoria de datos (dir: " F_DWORD ")\n\ten %s:%d\n ",
                direccion, file, line);
        exit(1);
    }
    return resultado;
}

/**
 * Comprueba si la dirección de memoria es un dirección válida de la memoria de datos
 *
 * @param mm Mapa de memoria.
 * @param direccion Dirección absoluta del dato
 * @return boolean
 */
boolean comprueba_direccion_datos(
        mapa_memoria_t *mm,
        dword direccion
) {
    region_t r;

    r = dir_a_region(mm, direccion);

    return es_region_datos(r);
}


/**
 * Escribe un dato en un región de memoria de datos
 *
 * @param mm Mapa de memoria
 * @param r Región en la que se desea escribir (de datos)
 * @param offset Desplazamiento dentro de la región
 * @param valor Valor que se desea escribir
 * @param tipo_dato Tipo del dato que se desea escribir
 */
void mem_escribe_region_datos(
        mapa_memoria_t *mm,
        region_t r,
        dword offset,
        valor_t valor,
        tipo_t tipo_dato,
        boolean activar_dirty
) {
    if (r >= num_regiones) {
        fnError("Región invalida");
    }

    switch (tipo_dato) {
        case t_char:
        case t_byte:
            mm->datos[r].mem.m_byte[indice_byte(offset)] = (byte) valor.int_d;
            break;
        case t_half:
            mm->datos[r].mem.m_half[indice_half(offset)] = (half) valor.int_d;
            break;
        case t_word:
            mm->datos[r].mem.m_word[indice_word(offset)] = (word) valor.int_d;
            break;
        case t_dword:
            mm->datos[r].mem.m_dword[indice_dword(offset)] = valor.int_d;
            break;
        case t_float:
            mm->datos[r].mem.m_float[indice_float(offset)] = valor.fp_s;
            break;
        case t_double:
            mm->datos[r].mem.m_double[indice_double(offset)] = valor.fp_d;
            break;
        case t_float_ps:
            mm->datos[r].mem.m_float[indice_float(offset)] = valor.fp_ps[Lo];
            mm->datos[r].mem.m_float[indice_float(offset) + 1] = valor.fp_ps[Hi];
            break;
        default:
            fnError("Escritura de datos no implementada");
    }

    mem_etiqueta(mm, r, offset, tipo_dato, activar_dirty);

}

/**
 * Escribe un dato en la memoria de datos.
 *   Si la dirección no se corresponde con una región de datos da un error.
 *
 * @param mm Mapa de memoria
 * @param direccion Dirección absoluta donde se desea escribir
 * @param valor Valor que se dessea escribir
 * @param tipo_dato Tipo del dato que se desea escribir
 */
void escribe_mem_datos(
        mapa_memoria_t *mm,
        dword direccion,
        valor_t valor,
        tipo_t tipo_dato,
        boolean activar_dirty
) {
    dword offset;
    region_t r;

    r = dir_a_region(mm, direccion);

    if (es_region_datos(r)) {
        offset = dir_a_offset(mm, direccion, r);

        muestra_estado_mem_datos(mm, r, offset, tipo_dato, OLD_MEM);

        mem_escribe_region_datos(mm, r, offset, valor, tipo_dato, activar_dirty);

        muestra_estado_mem_datos(mm, r, offset, tipo_dato, NEW_MEM);
    } else {
        fprintf(stderr, "ERROR: Escritura fuera del rango de memoria de datos (" F_DWORD ")\n", direccion);
        exit(1);
    }
}

/**
 * Obtiene el puntero al texto en ensamblador de una instrucción a partir del índice.
 *   Sera una cadena vacía si todavía no se ha decodificado.
 * @param mm
 * @param indice
 * @return
 */
char *
obtiene_ensamblador(
        mapa_memoria_t *mm,
        word indice
) {
    return mm->ensamblador[indice].texto;
}

/**
 * Obtiene una instrucción a partir de su índice
 * @param mm
 * @param indice
 * @return
 */
instruccion_t obtiene_instruccion(
        mapa_memoria_t *mm,
        word indice
) {
    return mm->instruc[indice];
}

/**
 * Obtiene el indice de una instrucción en le vector de instrucciones
 * @param mm
 * @param direccion
 * @return
 */
word indice_instruccion(
        mapa_memoria_t *mm,
        dword direccion
) {
    dword offset;
    region_t r;

    r = dir_a_region(mm, direccion);

    if (es_region_codigo(r)) {
        offset = dir_a_offset(mm, direccion, r);

        return indice_code(offset);
    } else {
        return -1;
    }
}

/**
 * Lee una instrucción de una región de código
 *
 * @param mm Mapa de memoria
 * @param r Región de código
 * @param offset Desplazamiento dentro de la región de código
 * @return Instrucción leida
 */
instruccion_t mem_lee_region_instruc(
        mapa_memoria_t *mm,
        region_t r,
        dword offset
) {
    if (r != r_text) {
        fnError("Región invalida");
    }

    if (!esta_alineada_dir_code(offset)) {
        fprintf(stderr, "ERROR: Lectura desalineada en memoria de instrucciones (offset: " F_DWORD ")\n", offset);
        exit(1);
    }

    /*
     * De momento sólo hay una región de código (r_text), pero añadimos la región para que la interfaz sea ortogonal
     * con la memoria de datos
     */

    return obtiene_instruccion(mm, indice_code(offset));
}

/**
 * Lee una instrucción de la memoria de instrucciones. Si la dirección no se corresponde con una región de código da un error.
 *
 * @param mm Mapa de memoria.
 * @param direccion Dirección absoluta del dato
 * @param file Fichero desde el que se realzia al lectura
 * @param line Línea desde que se realzia la lectura
 * @return Instrucción accedida
 */
instruccion_t _lee_mem_instruc(
        mapa_memoria_t *mm,
        dword direccion,
        char *file,
        int line
) {
    dword offset;
    region_t r;

    r = dir_a_region(mm, direccion);

    if (es_region_codigo(r)) {
        offset = dir_a_offset(mm, direccion, r);

        return mem_lee_region_instruc(mm, r, offset);
    } else {
        fprintf(stderr, "ERROR: Lectura fuera del rango de memoria de instrucciones (dir: " F_DWORD ")\n\ten %s:%d\n ",
                direccion, file, line);
        exit(1);
    }
}

/**
 * Comprueba si la dirección de memoria es un dirección valida de la memoria de instrucciones
 *
 * @param mm Mapa de memoria.
 * @param direccion Dirección absoluta de la instrucción
 * @return boolean
 */
boolean comprueba_direccion_instruc(
        mapa_memoria_t *mm,
        dword direccion
) {
    region_t r;

    r = dir_a_region(mm, direccion);

    return es_region_codigo(r);
}

/**
 * Escribe una instrucción en la región de código
 *
 * @param mm Mapa de memoria
 * @param r Región en la que se desea escribir (de código)
 * @param offset Desplazamiento dentro de la región
 * @param instruccion Instrucción que se desea escribir
 */
void mem_escribe_region_instruc(
        mapa_memoria_t *mm,
        region_t r,
        dword offset,
        instruccion_t instruccion
) {
    if (r != r_text) {
        fnError("Región invalida");
    }

    if (!esta_alineada_dir_code(offset)) {
        fprintf(stderr, "ERROR: Escritura desalineada en memoria de instrucciones (offset: " F_DWORD ")\n", offset);
        exit(1);
    }

    // En la memoria de datos se almacena el índice sobre el vector de instrucciones
    // Hace el papel del código binario de la instrucción
    // TODO: Podríamos usar el código binario, que sí existe en RiscV

    mm->datos[r].mem.m_word[indice_code(offset)] = indice_code(offset);
    mm->instruc[indice_code(offset)] = instruccion;
}

/**
 * Escribe una instrucción en la memoria de instrucciones
 * @param mm
 * @param direccion
 * @param instruccion
 */
void escribe_mem_instruc(
        mapa_memoria_t *mm,
        dword direccion,
        instruccion_t instruccion
) {
    dword offset;

    region_t r;

    r = dir_a_region(mm, direccion);

    if (es_region_codigo(r)) {
        offset = dir_a_offset(mm, direccion, r_text);

        mem_escribe_region_instruc(mm, r, offset, instruccion);
    } else {
        fprintf(stderr, "ERROR: Escritura fuera del rango de memoria de instrucciones (dir:" F_DWORD ")\n", direccion);
        exit(1);
    }
}

