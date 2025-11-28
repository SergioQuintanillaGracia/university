/**
 * @file memoria-principal.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las funciones de acceso a la memoria principal.
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define memoria_principal_C

#include <memory.h>
#include <stdlib.h>

#include "main.h"
#include "memoria.h"
#include "jerarquia.h"
#include "memoria-principal.h"
#include "g_html.h"
#include "g_list.h"

/* -- Macros -- */

#define FOREACH_OPER(ITEM, HEAD) \
for (ITEM = (operacion_mem_principal_t *)(HEAD); \
     ITEM != NULL;               \
     ITEM = (operacion_mem_principal_t *)(ITEM)->info.next)

/* -- Functions -- */

/**
 * Comprueba la configuración de la memoria principal
 * @param m_config
 * @return 0 OK, 1 error
 */
int
comprueba_config_memoria_principal(
        config_memoria_principal_t m_config
) {
    /* Comprobaciones de error */
    return ((m_config.tam_memoria <= (m_config.num_bancos * m_config.tam_banco)) ||
            (m_config.tam_memoria % (m_config.num_bancos * m_config.tam_banco) != 0) ||
            (m_config.BW % BUS_SIZE != 0) ||
            (m_config.tam_banco % m_config.BW != 0));
}


/**
 * Inicializa un nivel de la jerarquía de memoria correspondiente a la memoria principal
 */
void
inicializa_memoria_principal(
        memoria_principal_t *memoria,
        config_memoria_principal_t m_config,    /**< Tamaño de la memoria en bytes */
        config_operaciones_t op_config          /**< Configuración del buffer de operaciones */
) {
    unsigned int i;

    /* Inicializa el nivel de la jerarquía de memoria */
    inicializa_nivel_jerarquia(&memoria->mem, op_config, sizeof(operacion_mem_principal_t));

    /* Inicializa los datos de la caché */

    memoria->config = m_config;

    memoria->bancos = calloc(m_config.num_bancos, sizeof(banco_memoria_t));
    foreach(i, m_config.num_bancos) {
        memoria->bancos[i].estado = EST_BANCO_LIBRE;
        memoria->bancos[i].fila_abierta = UNKNOWN_ROW; // Se supone que todas las filas ya han sido abiertas en algún momento
        memoria->bancos[i].signal = "&nbsp;";
    }

} /* end inicializa_memoria_principal */

static boolean
planifica_oper_mem_principal(
        operacion_mem_principal_t *oper
) {
    memoria_principal_t *mp = (memoria_principal_t *) oper->info.parent;
    banco_memoria_t *banco = &mp->bancos[oper->idx_banco];

    if (banco->estado == EST_BANCO_LIBRE) {
        // Tiempos de acceso
        oper->CL = mp->config.CL;
        oper->tRCD = mp->config.tRCD;
        oper->tRP = mp->config.tRP;
        oper->info.acierto = NO;

        if (banco->fila_abierta == oper->idx_fila) {
            // Está abierta la fila que buscamos
            oper->info.acierto = SI;
            oper->tRP = oper->tRCD = 0;

            // Actualizamos el estado del banco
            banco->estado = EST_BANCO_ACCEDIENDO;
            banco->signal = "CAS";
        } else if (banco->fila_abierta == CLOSED_ROW) {
            // No hay una fila abierta en ese banco
            oper->tRP = 0;
            // Actualizamos el estado del banco
            banco->estado = EST_BANCO_LEYENDO;
            banco->fila_abierta = oper->idx_fila;
            banco->direccion = DIREC_BASE_BANCO_MEMORIA(mp, oper->info.direccion);
            banco->signal = "RAS";
        } else {
            // Actualizamos el estado del banco
            banco->estado = EST_BANCO_CERRANDO;
            banco->signal = "WE";
        }

        // Tiempo de acceso = Latencia + tiempo de transferencia
        oper->t_acceso = oper->tRP + oper->tRCD + oper->CL + (oper->info.tam / mp->config.BW);

        // Actualizamos el estado de la operación
        oper->info.estado = EST_OP_ACCEDIENDO;

        return SI;
    }

    return NO;
}

/**
 * Lanza una operación de lectura en memoria principal
 * @param nivel
 * @param tipo
 * @param dir
 * @param tam
 * @param callback
 * @param oper_id
 * @return
 */
boolean
inicia_oper_mem_principal(
        NIVEL_MEM_PTR dst,    /**< Puntero al nivel destino */
        tipo_operacion_memoria_t tipo, /**< Tipo de operación solicitada */
        NIVEL_MEM_PTR src,    /**< Puntero al nivel solicitante */
        dword dir,              /**< Dirección de los datos solicitados */
        size_t tam,             /**< Tamaño de los datos solicitados */
        ciclo_t orden,          /**< Orden de la instrucción que ha encargado la operación */
        operacion_finalizada_fn callback, /**< Función que se debe invocar cuando la operación solicitada termine */
        void *oper_id          /**< Identificador de la operación en el nivel superior al invocar la callback */
) {
    operacion_mem_principal_t *oper;
    memoria_principal_t *mp = (memoria_principal_t *) dst;

    M_DEBUG(F_CICLO ": [%s] %s " F_DWORD " <%zu> ... ", Ciclo, dst->nombre,
            (tipo == OP_LECT) ? "Lectura" : "Escritura", dir, tam);

    // Mirar si la operación cabe en el buffer de operaciones
    oper = (operacion_mem_principal_t *) reserva_operacion(dst);
    if (oper == NULL) {
        M_DEBUG("\n");
        return NO;
    }

    M_DEBUG("OK\n");

    // Operación en un nivel de la jerarquía
    inicia_operacion_nivel(&oper->info, src, tipo, dir, tam, orden, callback, oper_id);

    oper->idx_fila = INDICE_FILA_MEMORIA(mp, dir);
    oper->idx_banco = INDICE_BANCO_MEMORIA(mp, dir);
    oper->t_acceso = 0; // Pendiente de calcular

    return SI;
}

/**
 * Termina una operación al final del tiempo de acceso
 * @param principal
 * @param oper
 */
void termina_operacion_mem_principal(
        memoria_principal_t *memoria,
        operacion_mem_principal_t *oper
) {
    void *p_datos;
    tipo_t *p_tipos;
    boolean *p_dirty;
    region_t r;
    dword offset;

    // Actualiza estadísticas
    actualiza_estadisticas(&oper->info);

    r = dir_a_region(mmem, oper->info.direccion);
    offset = dir_a_offset(mmem, oper->info.direccion, r);

    // Suponemos que los datos están ya alineados
    p_datos = &mmem->datos[r].mem.m_byte[indice_byte(offset)];
    p_tipos = &mmem->datos[r].tipos[offset];
    p_dirty = &mmem->datos[r].dirty[offset];

    // Invocar la callback del nivel superior
    oper->info.finaliza_operacion(&oper->info, p_datos, p_tipos, p_dirty);

    // Libera el banco
    memoria->bancos[oper->idx_banco].estado = EST_BANCO_LIBRE;
    memoria->bancos[oper->idx_banco].fila_abierta = oper->idx_fila;
    memoria->bancos[oper->idx_banco].direccion = DIREC_BASE_BANCO_MEMORIA(memoria, oper->info.direccion);

    // Finalizar la operación
    oper->info.tipo = OP_NULA;
    oper->info.estado = EST_OP_TERMINADA;
    libera_operacion(&memoria->mem, PTR_OPER(oper));

}

/**
 * Actualiza el estado de un nivel de principal
 * @param nivel
 */
void actualiza_estado_mem_principal(
        NIVEL_MEM_PTR nivel
) {
    memoria_principal_t *principal = (memoria_principal_t *) nivel;
    operacion_mem_principal_t *oper;
    banco_memoria_t *banco;
    uint8_t i_oper, n_terminando;
    operacion_mem_principal_t ** oper_terminando;

    I_INIT_PRINT();

    // Reinicia el número de operaciones lanzadas en este nivel
    nivel->oper_ciclo = 0;

    // Pide memoria para las posibles operaciones terminando
    oper_terminando = alloca(nivel->oper_config.max_oper * sizeof(operacion_mem_cache_t *));
    n_terminando = 0;

    // Comprueba si las operaciones que están a la espera pueden comenzar
    // Están almacenadas por orden de antigüedad
    FOREACH_OPER(oper, nivel->oper_pendientes) {
        if (oper->info.tipo != OP_NULA &&
            oper->info.estado == EST_OP_PENDIENTE &&
            principal->bancos[oper->idx_banco].estado == EST_BANCO_LIBRE) {
            // Esta operación podría comenzar
            planifica_oper_mem_principal(oper);
        }
    }

    // Comprobar el estado de las operaciones en curso
    FOREACH_OPER(oper, nivel->oper_pendientes) {
        if (oper->info.tipo != OP_NULA && oper->info.estado == EST_OP_ACCEDIENDO) {
            banco = &principal->bancos[oper->idx_banco];
            oper->info.ciclos++;

            if (dir_a_region(mmem, oper->info.direccion) != r_text) {
                muestra_fase(I_PRINTF("%s<small>%d</small>", nivel->nombre, oper->info.ciclos), oper->info.orden);
            } else {
                strcpy(fase_instruc_jerarquia, "MP");
            }

            banco->signal = (oper->info.ciclos == 1) ? "WE" : "&nbsp;";
            banco->signal = (oper->info.ciclos == oper->tRP + 1) ? "RAS" : banco->signal;
            banco->signal = (oper->info.ciclos == oper->tRP + oper->tRCD + 1) ? "CAS" : banco->signal;
            banco->signal = (oper->info.ciclos > oper->tRP + oper->tRCD + oper->CL) ? "DATA" : banco->signal;

            // La estructura de "ifs" secuenciales permitiría que algunos tiempos fueran 0
            if (banco->estado == EST_BANCO_CERRANDO) {

                if (oper->info.ciclos > oper->tRP) {
                    banco->estado = EST_BANCO_LEYENDO;
                    // Se comienza a traer la fila solicitada
                    banco->fila_abierta = oper->idx_fila;
                    banco->direccion = DIREC_BASE_BANCO_MEMORIA(principal, oper->info.direccion);
                }
            }
            if (banco->estado == EST_BANCO_LEYENDO) {
                if (oper->info.ciclos > oper->tRP + oper->tRCD) {
                    banco->estado = EST_BANCO_ACCEDIENDO;
                    // Ya está la fila en el buffer, falta recibir la columna
                    banco->fila_abierta = oper->idx_fila;
                    banco->direccion = DIREC_BASE_BANCO_MEMORIA(principal, oper->info.direccion);
                }
            }
            if (banco->estado == EST_BANCO_ACCEDIENDO) {
                if (oper->info.ciclos > oper->tRP + oper->tRCD + oper->CL) {
                    banco->estado = EST_BANCO_TRANSFIRIENDO;
                    // Ya está la fila en el buffer y queda transferir el contenido
                    banco->fila_abierta = oper->idx_fila;
                    banco->direccion = DIREC_BASE_BANCO_MEMORIA(principal, oper->info.direccion);
                }
            }

            if (oper->info.ciclos == oper->t_acceso) {
                banco->signal = "&nbsp;";
                // Si el bloque está en caché, apuntamos la operación para terminar
                oper_terminando[n_terminando++] = oper;
            }
        }

        // Ahora que ya no es peligroso borrarla de la lista de operaciones pendientes
        foreach(i_oper, n_terminando) {
            termina_operacion_mem_principal(principal, oper_terminando[i_oper]);
        }

    }


}

/**
 * Vuelca los datos en memoria principal. No hace nada
 * @param nivel
 */
void
vuelca_datos_mem_principal(
        NIVEL_MEM_PTR nivel
) {
    // Nada que hacer
}

/*---------------------------------------------------------------------------*/
/* PRESENTACIÓN                                                              */
/*---------------------------------------------------------------------------*/

/**
 * Escribe la configuración de la memoria principal en formato texto plano
 * @param nivel
 * @param info
 */
void
obtiene_config_memoria_principal(
        NIVEL_MEM_PTR nivel,
        formato_presentacion_t formato,
        g_string_t *info
) {
    switch (formato) {
        case formato_txt:
            obtiene_config_memoria_principal_txt(nivel, info);
            break;
        case ningun_formato:
        case formato_html:
        case formato_json:
            break;
    }
}

/**
 * Escribe la configuración de la memoria principal en formato texto plano
 * @param nivel
 * @param info
 */
void
obtiene_config_memoria_principal_txt(
        NIVEL_MEM_PTR nivel,
        g_string_t *info
) {
    config_memoria_principal_t *m_config = &((memoria_principal_t *) nivel)->config;

    g_str_copy(info, "  Memoria principal: ");
    if (nivel->activa) {
        g_str_copy(info, "activa\n");
        g_str_printf(info, "\tTamaño de la memoria: %d bytes\n", m_config->tam_memoria);
        g_str_printf(info, "\tNúmero de bancos: %hd\n", m_config->num_bancos);
        g_str_printf(info, "\tTamaño del buffer de fila: %hd bytes\n", m_config->tam_banco);

        g_str_printf(info, "\tTemporización: %hhd-%hhd-%hhd\n",
                     m_config->CL, m_config->tRCD, m_config->tRP);

        g_str_printf(info, "\tAncho de banda: %hhd bytes/ciclo\n", m_config->BW);
    } else {
        g_str_copy(info, "inactiva (nivel L1 perfecto)\n");
    }
}

/**
 * Muestra el estado actual de la memoria principal
 * @param nivel
 * @param output
 */
void
muestra_estado_memoria_principal(
        NIVEL_MEM_PTR nivel,
        formato_presentacion_t formato,
        g_string_t *output
) {
    switch (formato) {
        case formato_html:
            muestra_estado_memoria_principal_html(nivel, output);
            break;
        case ningun_formato:
        case formato_txt:
        case formato_json:
            break;
    }
}

/**
 * Muestra el estado actual de la memoria principal en formato HTML
 * @param nivel
 * @param output
 */
void
muestra_estado_memoria_principal_html(
        NIVEL_MEM_PTR nivel,
        g_string_t *output
) {
    memoria_principal_t *memoria = (memoria_principal_t *) nivel;

    region_t r;
    dword offset;
    size_t tam;

    unsigned int i, j, b;
    unsigned int n_rows, n_cells;
    char s_rows[SHORT_STR];
    char *data_class;

    T_COMPOSITE div;
    T_COMPOSITE table, tr;
    T_SINGLE td;

    I_INIT_PRINT();

    n_rows = memoria->config.tam_banco / ROW_SIZE;
    if (n_rows == 0) n_rows = 1;
    n_cells = MIN(memoria->config.tam_banco, ROW_SIZE);

    sprintf(s_rows, "rowspan=%d", n_rows);

    // Estado en formato HTML
    div = H_ATTR_COMPOSITE(div, "class='memoria'");

    // Cabecera
    H_ADD_SINGLE_TEXT(div, h2, I_PRINTF("Memoria principal %s", nivel->nombre));

    // Contenido
    table = H_ADD_COMPOSITE(div, table);
    {
        tr = H_ADD_COMPOSITE(table, tr);

        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='index'", "Banco");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='state'", "Estado");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='address'", "Dirección");
        if (n_rows > 1) {
            H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='data'", "&nbsp;");
        }

        foreach(i, n_cells) {
            H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='data'", I_PRINTF("+%d", i));
        }

        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='signal'", "Señal");

        foreach(b, memoria->config.num_bancos) {
            banco_memoria_t *banco = &memoria->bancos[b];

            tr = H_ADD_COMPOSITE(table, tr);

            td = H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%zu", b));
            if (n_rows > 1) {
                h_add_attr(PTR_BASE(td), s_rows);
            }

            switch (banco->estado) {
                case EST_BANCO_LIBRE:
                    td = H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='normal'", "Libre");
                    data_class = "data";
                    break;
                case EST_BANCO_CERRANDO:
                    td = H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='blocked'", "Cerrando");
                    data_class = "data old";
                    break;
                case EST_BANCO_LEYENDO:
                    td = H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='blocked'", "Leyendo");
                    data_class = "data old";
                    break;
                case EST_BANCO_ACCEDIENDO:
                    td = H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='blocked'", "Accediendo");
                    data_class = "data blocked";
                    break;
                case EST_BANCO_TRANSFIRIENDO:
                    td = H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='blocked'", "Transfiriendo");
                    data_class = "data blocked";
                    break;
            }

            if (n_rows > 1) {
                h_add_attr(PTR_BASE(td), s_rows);
            }

            if (banco->fila_abierta == CLOSED_ROW) {
                td = H_ADD_SINGLE_TEXT(tr, td, "cerrada");
            } else if (banco->fila_abierta == UNKNOWN_ROW) {
                td = H_ADD_SINGLE_TEXT(tr, td, "desconocida");
            } else {
                td = H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%d", banco->direccion));
            }

            if (n_rows > 1) {
                h_add_attr(PTR_BASE(td), s_rows);
            }

            if (banco->estado != EST_BANCO_LIBRE) {
                h_add_attr(PTR_BASE(td), "class='blocked'");
            }

            if (banco->fila_abierta == CLOSED_ROW || banco->fila_abierta == UNKNOWN_ROW ||
                banco->estado == EST_BANCO_LEYENDO) {
                if (n_rows > 1) {
                    H_ADD_SINGLE_TEXT(tr, td, "+0");
                }
                td = H_ADD_SINGLE_ATTR_TEXT(tr, td,
                                            I_PRINTF("class='data empty' colspan='%d'", n_cells),
                                            (banco->fila_abierta == UNKNOWN_ROW) ? "???" : "&nbsp;");
                if (n_rows > 1) {
                    h_add_attr(PTR_BASE(td), s_rows);
                }

                td = H_ADD_SINGLE_TEXT(tr, td, banco->signal);
                if (n_rows > 1) {
                    h_add_attr(PTR_BASE(td), s_rows);
                }

                if (n_rows > 1) {
                    for (j = 1; j < n_rows; j++) {
                        tr = H_ADD_COMPOSITE(table, tr);
                        H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("+%d", j * ROW_SIZE));
                    }
                }
            } else {
                foreach(j, n_rows) {
                    if (j > 0) {
                        tr = H_ADD_COMPOSITE(table, tr);
                    }

                    if (n_rows > 1) {
                        H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("+%d", j * ROW_SIZE));
                    }

                    i = 0;
                    while (i < n_cells) {
                        char valor[SHORT_STR];
                        unsigned int idx = j * ROW_SIZE + i;

                        I_CLEAR_PRINT();

                        r = dir_a_region(mmem, banco->direccion);
                        offset = dir_a_offset(mmem, banco->direccion, r);

                        tam = tam_dato(mmem->datos[r].tipos[offset + idx]);
                        if (es_region_datos(r)) {
                            imprime_dato(&mmem->datos[r], offset + idx, valor);
                            H_ADD_SINGLE_ATTR_TEXT(tr, td, I_PRINTF("class='%s' colspan='%d'", data_class, tam), valor);

                        } else {
                            word indice = indice_instruccion(mmem, banco->direccion + idx);

                            if (indice != INST_UNKNOWN) {
                                char *texto = obtiene_ensamblador(mmem, indice);
                                // sprintf(valor, "%d|%d : %s", idx, indice, texto);
                                strcpy(valor, texto);
                            } else {
                                strcpy(valor, "unknown");
                            }

                            H_ADD_SINGLE_ATTR_TEXT(tr, td, I_PRINTF("class='%s code' colspan='%d'", data_class, tam),
                                                   valor);
                        }

                        i += tam;
                    }

                    if (j == 0) {
                        td = H_ADD_SINGLE_TEXT(tr, td, banco->signal);
                        if (n_rows > 1) {
                            h_add_attr(PTR_BASE(td), s_rows);
                        }
                    }
                }
            }

        }
    }

    // Operaciones pendientes
    if (FLAG(show_ops)) {
        operacion_mem_principal_t *oper;

        table = H_ADD_COMPOSITE(div, table);
        h_add_attr(PTR_BASE(table), "class='oper'");
        tr = H_ADD_COMPOSITE(table, tr);

        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='index'", "Operación");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='state'", "Estado");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='oper'", "Tipo");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='address'", "Dirección");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='data'", "Tamaño");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='time'", "Comienzo");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='time'", "Ciclos");

        oper = (operacion_mem_principal_t *)nivel->oper_pendientes;
        foreach(i, nivel->oper_config.max_oper) {
            tr = H_ADD_COMPOSITE(table, tr);

            H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%zu", i));

            if (oper == NULL) {
                H_ADD_SINGLE_TEXT(tr, td, "&nbsp;");
                H_ADD_SINGLE_TEXT(tr, td, "&nbsp;");
                H_ADD_SINGLE_TEXT(tr, td, "&nbsp;");
                H_ADD_SINGLE_TEXT(tr, td, "&nbsp;");
                H_ADD_SINGLE_TEXT(tr, td, "&nbsp;");
                H_ADD_SINGLE_TEXT(tr, td, "&nbsp;");

            } else {
                switch (oper->info.estado) {
                    case EST_OP_PENDIENTE:
                        H_ADD_SINGLE_TEXT(tr, td, "Pendiente");
                        break;
                    case EST_OP_ACCEDIENDO:
                        H_ADD_SINGLE_TEXT(tr, td, "Accediendo");
                        break;
                    case EST_OP_TERMINADA:
                        H_ADD_SINGLE_TEXT(tr, td, "Terminada");
                        break;
                    default:
                        H_ADD_SINGLE_TEXT(tr, td, "&nbsp;");
                        break;
                }

                switch (oper->info.tipo) {
                    case OP_NULA:
                        H_ADD_SINGLE_TEXT(tr, td, "&nbsp;");
                        break;
                    case OP_LECT:
                        H_ADD_SINGLE_TEXT(tr, td, "Lectura");
                        break;
                    case OP_ESCR:
                        H_ADD_SINGLE_TEXT(tr, td, "Escritura");
                        break;
                }

                H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%ld", oper->info.direccion));
                H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%zu", oper->info.tam));
                H_ADD_SINGLE_TEXT(tr, td, I_PRINTF(F_CICLO, oper->info.inicio));
                H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%hu", oper->info.ciclos));

                oper = (operacion_mem_principal_t *)oper->info.next;
            }
        }
    }

    H_ADD_SINGLE(div, br);

    h_tag_flush(PTR_BASE(div), output);

    h_destroy_composite_tag(div);
}

