/**
 * @file memoria-cache.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las funciones de acceso a la memoria cache.
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define memoria_cache_C

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "memoria.h"
#include "jerarquia.h"
#include "memoria-cache.h"

#include "g_html.h"
#include "g_list.h"

/* -- Macros -- */

#define FOREACH_OPER(ITEM, HEAD) \
for (ITEM = (operacion_mem_cache_t *)(HEAD); \
     ITEM != NULL;               \
     ITEM = (operacion_mem_cache_t *)(ITEM)->info.next)

/* -- Functions -- */

/**
 * Comprueba la configuración de la memoria cache
 * @param c_config
 * @return 0 OK, 1 error
 */
int
comprueba_config_memoria_cache(
        config_cache_t c_config
) {
    /* Comprobaciones de error */
    return ((c_config.tam_bloque % sizeof(valor_t) != 0) ||
            (c_config.tam_bloque % BUS_SIZE != 0) ||
            (c_config.grado_asoc > 0 && (c_config.num_bloques % c_config.grado_asoc != 0)));
}

/**
 * Inicializa un nivel de la jerarquía de cache correspondiente a una cache de datos
 */
void
inicializa_memoria_cache(
        memoria_cache_t *cache,
        config_cache_t c_config,        /**< Geometría de la cache */
        config_operaciones_t op_config  /**< Configuración del buffer de operaciones */
) {
    unsigned int i_bloque;

    /* Comprobaciones de error */
    if (comprueba_config_memoria_cache(c_config)) {
        // No debería pasar porque se ha comprobado en el programa principal
        fprintf(stderr, "ERROR: Errores en la geometría de la cache '%s'", cache->mem.nombre);
        exit(1);
    } /* endif */

    /* Inicializa el nivel de la jerarquía de memoria */
    inicializa_nivel_jerarquia(&cache->mem, op_config, sizeof(operacion_mem_cache_t));

    // De momento sólo soportamos WB-WA
    // En caso de añadir WT-NWA, debería pasarse como parámetro la política
    cache->politica = WRITE_BACK_WRITE_ALLOCATE;

    // Configuración de la cache
    cache->config = c_config;
    /* Grado 0 = completamente asociativa */
    cache->config.grado_asoc = (c_config.grado_asoc == 0) ? cache->config.num_bloques : c_config.grado_asoc;

    /* Inicializa los bloques de la cache */
    cache->bloques = calloc(cache->config.num_bloques, sizeof(bloque_cache_t));

    foreach(i_bloque, cache->config.num_bloques) {
        cache->bloques[i_bloque].estado = BLOQ_INVALIDO;
        cache->bloques[i_bloque].oper = OP_NULA;
        inicializa_zona_datos(&cache->bloques[i_bloque].datos, c_config.tam_bloque, tr_data);
    }
}

/**
 * Busca en la caché para ver si el bloque solicitado está presente
 * @param cache
 * @param dir
 * @return
 */
static bloque_cache_t *
busca_bloque_cache(
        memoria_cache_t *cache,    /**< Estructura de la memoria caché */
        dword dir                  /**< Dirección de los datos solicitados */
) {
    uint32_t i_bloque;
    bloque_cache_t *result = NULL;
    bloque_cache_t *bloque_inicio;
    dword etiqueta = ETIQUETA_BLOQUE(cache, dir);

    // Primer bloque del conjunto
    bloque_inicio = &cache->bloques[INDICE_CONJUNTO(cache, dir) * cache->config.grado_asoc];
    foreach(i_bloque, cache->config.grado_asoc) {
        bloque_cache_t *bloque = &bloque_inicio[i_bloque];
        if (bloque->estado != BLOQ_INVALIDO && bloque->etiqueta == etiqueta) {
            // El bloque tiene contenidos válidos y la etiqueta coincide
            result = bloque;
            break;
        }
    }

    return result;
}

/**
 * Busca un bloque candidato para reemplazo
 * @param cache
 * @param dir
 * @return
 */
static bloque_cache_t *
busca_reemplazo_cache(
        memoria_cache_t *cache,    /**< Estructura de la memoria caché */
        dword dir                  /**< Dirección de los datos solicitados */
) {
    uint32_t i_bloque;
    bloque_cache_t *result = NULL;
    bloque_cache_t *bloque_inicio;
    ciclo_t ultimo_acceso = MAX_WORD;

    // Primer bloque del conjunto
    bloque_inicio = &cache->bloques[INDICE_CONJUNTO(cache, dir) * cache->config.grado_asoc];
    foreach(i_bloque, cache->config.grado_asoc) {
        bloque_cache_t *bloque = &bloque_inicio[i_bloque];
        if (bloque->oper == OP_NULA) {
            // ese bloque no está reservado por ninguna operación en curso
            if (bloque->estado == BLOQ_INVALIDO) {
                // Un bloque libre es la mejor opción
                result = bloque;
                break;
            } else if (ultimo_acceso > bloque->ciclo) {
                // LRU: Busca el bloque que hace más que no se ha accedido
                result = bloque;
                ultimo_acceso = bloque->ciclo;
            }
        }
    }

    return result;
}

static void
planifica_mem_cache(
        NIVEL_MEM_PTR nivel
) {
    uint8_t i_oper, idx, n_accediendo;
    operacion_mem_cache_t *oper;
    operacion_bloque_t *operaciones;

    operaciones = alloca(nivel->oper_config.max_oper * sizeof(operacion_bloque_t));
    foreach(i_oper, nivel->oper_config.max_oper) {
        operaciones[i_oper].etiqueta = SIN_ETIQUETA;
    }

    i_oper = 0;
    n_accediendo = 0;
    FOREACH_OPER(oper, nivel->oper_pendientes) {
        if (oper->info.tipo != OP_NULA) {
            if (oper->info.estado != EST_OP_PENDIENTE)
                operaciones[i_oper].etiqueta = oper->etiqueta;
            if (oper->info.estado == EST_OP_ACCEDIENDO)
                n_accediendo++;
        }
        i_oper++;
    }

    // Comprueba si las operaciones que están a la espera pueden comenzar
    FOREACH_OPER(oper, nivel->oper_pendientes) {
        if (n_accediendo == nivel->oper_config.max_oper_acceso) {
            // No puede haber más operaciones accediendo simultáneamente
            break;
        }

        if (oper->info.tipo != OP_NULA &&
            oper->info.estado == EST_OP_PENDIENTE) {

            boolean bloque_reservado = NO;

            // Busca si hay otra operación pendiente o accediendo sobre el mismo bloque
            foreach(idx, i_oper) {
                if (operaciones[idx].etiqueta == oper->etiqueta) {
                    bloque_reservado = SI;
                    break;
                }
            }

            // Si ya había otra operación pendiente sobre el mismo bloque, ésta no la puedes lanzar
            if (bloque_reservado) continue;

            // Esta operación podría comenzar
            oper->info.estado = EST_OP_ACCEDIENDO;
            oper->info.acierto = NO;
            oper->cancelada = NO;
            oper->bloque = NULL; // No hay un bloque asignado todavía

            // No puede comenzar ninguna otra sobre el mismo bloque
            operaciones[i_oper].etiqueta = oper->etiqueta;
            i_oper++;
            n_accediendo++;
        }
    }

}

/**
 * Lanza una operación en la memoria cache
 * @param nivel
 * @param dir
 * @param tam
 * @param callback
 * @param oper_id
 * @return SI en caso de que la operación se pueda lanzar correctamente, NO en caso contrario
 */
boolean
inicia_oper_mem_cache(
        NIVEL_MEM_PTR dst,      /**< Puntero al nivel destino */
        tipo_operacion_memoria_t tipo,
        NIVEL_MEM_PTR src,      /**< Puntero al nivel solicitante */
        dword dir,              /**< Dirección de los datos solicitados */
        size_t tam,             /**< Tamaño de los datos solicitados */
        ciclo_t orden,          /**< Orden de la instrucción que ha encargado la operación */
        operacion_finalizada_fn callback, /**< Función que se debe invocar cuando la operación solicitada termine */
        void *oper_id           /**< Identificador de la operación en el nivel superior al invocar la callback */
) {
    memoria_cache_t *cache = (memoria_cache_t *) dst;
    operacion_mem_cache_t *oper;

    M_DEBUG(F_CICLO ": [%s] %s " F_DWORD " <%zu>  ... ", Ciclo, dst->nombre,
            (tipo == OP_LECT) ? "Lectura" : "Escritura", dir, tam);

    // Mirar si la operación cabe en el buffer de operaciones
    oper = (operacion_mem_cache_t *) reserva_operacion(dst);
    if (oper == NULL) {
        M_DEBUG("\n");
        return NO;
    }

    M_DEBUG(" (%d) OK\n", oper->info.indice);

    // Operación en un nivel de la jerarquía
    inicia_operacion_nivel(&oper->info, src, tipo, dir, tam, orden, callback, oper_id);

    oper->etiqueta = ETIQUETA_BLOQUE(cache, oper->info.direccion);

    return SI;
}

/**
 * Termina una operación al final de la latencia (acierto) o cuando termina en el nivel inferior (fallo)
 * @param cache
 * @param oper
 */
void termina_oper_mem_cache(
        memoria_cache_t *cache,
        operacion_mem_cache_t *oper
) {
    void *p_datos;
    tipo_t *p_tipos;
    boolean *p_dirty;
    int offset;

    offset = DESPLAZ_BLOQUE_CACHE(cache, oper->info.direccion);
    p_datos = &oper->bloque->datos.mem.m_byte[offset];
    p_tipos = &oper->bloque->datos.tipos[offset];
    p_dirty = &oper->bloque->datos.dirty[offset];

    // Invocar la callback del nivel superior, si la operación no se ha cancelado
    // Es el nivel superior el que lee o escribe los datos en su sitio.
    if (!oper->cancelada) {
        // Actualiza estadísticas
        actualiza_estadisticas(&oper->info);

        oper->info.finaliza_operacion(&oper->info, p_datos, p_tipos, p_dirty);
    }

    // Actualiza el estado del bloque
    switch (oper->info.tipo) {
        case OP_LECT:
            // Si estaba previamente sucio, se deja sucio
            oper->bloque->estado = (oper->bloque->estado != BLOQ_SUCIO) ? BLOQ_VALIDO : BLOQ_SUCIO;
            break;
        case OP_ESCR:
            // Las escrituras siempre dejan el bloque en estado sucio
            oper->bloque->estado = BLOQ_SUCIO;
            break;
        default:
            fprintf(stderr, "%s:%d. La operación no puede ser nula", __FILE__, __LINE__);
            exit(1);
    }

    oper->bloque->direccion = DIREC_BASE_BLOQUE_CACHE(cache, oper->info.direccion);
    oper->bloque->etiqueta = ETIQUETA_BLOQUE(cache, oper->info.direccion);

    // Libera el bloque
    oper->bloque->oper = OP_NULA;
    if (!oper->cancelada) {
        // Si la operación se ha cancelado, no se considera que el bloque se ha accedido
        oper->bloque->ciclo = Ciclo;
    }

    // Finalizar la operación
    oper->info.tipo = OP_NULA;
    oper->info.estado = EST_OP_TERMINADA;
    oper->bloque = NULL;
    libera_operacion(&cache->mem, PTR_OPER(oper));

}

/**
 * Actualiza el estado de un nivel de cache
 * @param nivel
 */
void actualiza_estado_mem_cache(
        NIVEL_MEM_PTR nivel
) {
    memoria_cache_t *cache = (memoria_cache_t *) nivel;
    operacion_mem_cache_t *oper;
    boolean result;
    uint8_t i_oper, n_terminando;
    operacion_mem_cache_t ** oper_terminando;

    I_INIT_PRINT();

    // Reinicia el número de operaciones lanzadas en este nivel
    nivel->oper_ciclo = 0;

    // Pide memoria para las posibles operaciones terminando
    oper_terminando = alloca(nivel->oper_config.max_oper * sizeof(operacion_mem_cache_t *));
    n_terminando = 0;

    // Planifica las operaciones pendientes si las hay
    planifica_mem_cache(nivel);

    // Comprobar el estado de las operaciones en curso
    FOREACH_OPER(oper, nivel->oper_pendientes) {
        if (oper->info.tipo != OP_NULA) {
            estado_operacion_memoria_t estado_inicial = oper->info.estado;

            limpia_estado_operacion(oper);

            switch (oper->info.estado) {
                case EST_OP_PENDIENTE:
                    estado_operacion(oper, "Pendiente");
                    if (dir_a_region(mmem, oper->info.direccion) != r_text) {
                        muestra_fase(I_PRINTF("<i>L%c</i>", nivel->nombre[1]),
                                     oper->info.orden);
                    } else {
                        sprintf(fase_instruc_jerarquia, "<i>L%c</i>", nivel->nombre[1]);
                    }
                    break;
                case EST_OP_ACCEDIENDO:
                    oper->info.ciclos++;

                    if (dir_a_region(mmem, oper->info.direccion) != r_text) {
                        muestra_fase(I_PRINTF("L%c<small>%d</small>", nivel->nombre[1], oper->info.ciclos),
                                     oper->info.orden);
                    } else {
                        sprintf(fase_instruc_jerarquia, "L%c", nivel->nombre[1]);
                    }

                    if (oper->info.ciclos >= cache->config.latencia) {
                        // Si acaba de terminar el acceso ...
                        if (oper->info.ciclos == cache->config.latencia) {
                            // ... buscamos a ver si el bloque está en cache
                            oper->bloque = busca_bloque_cache(cache, oper->info.direccion);
                            oper->info.acierto = (oper->bloque != NULL);

                            estado_operacion(oper, "Accediendo (%c)", (oper->info.acierto) ? 'A' : 'F');
                        }

                        // Acceso completado a este nivel
                        if (oper->info.acierto) {
                            // ACIERTO
                            M_DEBUG(F_CICLO ": [%s] (%d) Acierto => Bloque %ld\n",
                                    Ciclo, nivel->nombre, oper->info.indice, oper->bloque->direccion);
                            // Si el bloque está en caché, apuntamos la operación para terminar
                            oper_terminando[n_terminando++] = oper;
                            break;
                        } else {
                            // FALLO
                            M_DEBUG(F_CICLO ": [%s] (%d) Fallo\n", Ciclo, nivel->nombre, oper->info.indice);
                            // El bloque no está en caché, hay que pedírselo al siguiente nivel
                            oper->bloque = busca_reemplazo_cache(cache, oper->info.direccion);

                            if (oper->bloque == NULL) {
                                // No hay un bloque disponible para reemplazo porque
                                //  todos los bloques de destino posibles están en uso.
                                //  Solo sería posible si todos los bloques del conjunto se están reemplazando
                                //  → Se mantiene la operación en la cola sin modificar
                                estado_operacion(oper, "Accediendo (F!!)");
                                break;
                            } else {
                                oper->info.estado = EST_OP_BLOQUEADA;
                                // No hay break e intenta lanzarla en el siguiente nivel
                            }
                        }
                    } else {
                        // Está accediendo pero todavía no se ha cumplido la latencia
                        estado_operacion(oper, "Accediendo");
                        break;
                    }
                case EST_OP_BLOQUEADA:
                    // Hay que lanzar la operación sobre el nivel inferior
                    if (oper->bloque != NULL) {
                        tipo_operacion_memoria_t tipo_oper;

                        if (estado_inicial == oper->info.estado)
                            estado_operacion(oper, "Bloqueada %s", nivel->siguiente->nombre);

                        // Si está sucio primero hay que actualizarlo en el siguiente nivel
                        if (oper->bloque->estado == BLOQ_SUCIO) {
                            tipo_oper = OP_ESCR;
                            // Vamos a reemplazar el bloque, lanzando una operación sobre el siguiente nivel
                            result = INICIA_OPERACION_MEMORIA(nivel->siguiente, tipo_oper, nivel,
                                                              oper->bloque->direccion,
                                                              cache->config.tam_bloque,
                                                              oper->info.orden,
                                                              finaliza_oper_mem_cache,
                                                              oper);
                        } else {
                            tipo_oper = OP_LECT;
                            // Vamos a reemplazar el bloque, lanzando una operación sobre el siguiente nivel
                            result = INICIA_OPERACION_MEMORIA(nivel->siguiente, tipo_oper, nivel,
                                                              DIREC_BASE_BLOQUE_CACHE(cache,
                                                                                      oper->info.direccion),
                                                              cache->config.tam_bloque,
                                                              oper->info.orden,
                                                              finaliza_oper_mem_cache,
                                                              oper);
                        }

                        oper->bloque->oper = tipo_oper; // Indicamos la operación que se intenta lanzar al nivel inferior

                        if (result) {
                            M_DEBUG(F_CICLO ": [%s] -> %s\n", Ciclo, nivel->nombre, nivel->siguiente->nombre);
                            // El bloque está involucrado en una operación con el nivel inferior,
                            //  lo pasamos a estado 'esperando'. El estado se cambiará desde la callback
                            //  cuando el nivel inferior termine
                            oper->info.estado = EST_OP_ESPERANDO;

                            if (dir_a_region(mmem, oper->info.direccion) != r_text) {
                                muestra_fase(I_PRINTF("L%c<small>%d</small>", nivel->nombre[1], oper->info.ciclos),
                                             oper->info.orden);
                            }
                        } else {
                            M_DEBUG(F_CICLO ": [%s] %s stall\n", Ciclo, nivel->nombre,
                                    nivel->siguiente->nombre);

                            if (dir_a_region(mmem, oper->info.direccion) != r_text) {
                                muestra_fase(I_PRINTF("<i>L%c<small>%d</small></i>", nivel->nombre[1],
                                                      oper->info.ciclos),
                                             oper->info.orden);
                            } else {
                                if (estado_inicial == oper->info.estado)
                                    sprintf(fase_instruc_jerarquia, "<i>L%c</i>", nivel->nombre[1]);
                            }
                            // El siguiente nivel no acepta la petición
                            // → Se mantiene la operación en estado 'bloqueada' sin modificar
                        }
                    } else {
                        fprintf(stderr, "%s:%d. Operación %d bloqueada sin bloque de reemplazo",
                                __FILE__, __LINE__, oper->info.indice);
                        exit(1);
                    }

                    break;
                case EST_OP_ESPERANDO:
                    // La operación está esperando que termine el nivel inferior
                    estado_operacion(oper, "Esperando %s", nivel->siguiente->nombre);

                    // El nivel inferior pone el estado en el cronograma
                    break;
                case EST_OP_TERMINADA:
                default:
                    fprintf(stderr, "%s:%d. Bloque en un estado incoherente (%d)",
                            __FILE__, __LINE__, oper->info.estado);
                    exit(1);
            }
        }
    }

    // Ahora que ya no es peligroso borrarla de la lista de operaciones pendientes
    foreach(i_oper, n_terminando) {
        termina_oper_mem_cache(cache, oper_terminando[i_oper]);
    }
}

/**
 * Actualiza el estado de una operación cuando la petición en el nivel inferior ha concluido
 * @param oper_sig
 * @param datos
 * @param tipos
 * @param dirty
 */
void finaliza_oper_mem_cache(
        operacion_nivel_t *oper_sig, /**< Operación del nivel inferior que ha finalizado */
        void *datos,
        tipo_t *tipos,
        boolean *dirty
) {
    unsigned int i;
    memoria_cache_t *cache = (memoria_cache_t *) oper_sig->src;
    // Operación de este nivel que solicito la del siguiente nivel
    operacion_mem_cache_t *oper_cache = (operacion_mem_cache_t *) oper_sig->oper_data;

    switch (oper_sig->tipo) {
        case OP_LECT:
            // Se estaba buscando un bloque que no se encontraba en este nivel

            // Copia los datos desde el nivel inferior
            // No hay problema con los tipos porque un bloque siempre es mayor o igual que valor_t
            memcpy(oper_cache->bloque->datos.mem.m_byte, datos, oper_sig->tam);
            foreach(i, oper_sig->tam) {
                oper_cache->bloque->datos.tipos[i] = tipos[i];
                oper_cache->bloque->datos.dirty[i] = dirty[i];
            }

            // Procede a cerrar la operación, informando al nivel superior
            termina_oper_mem_cache(cache, oper_cache);
            break;
        case OP_ESCR:
            // Se había solicitado la escritura de un bloque sucio en el siguiente nivel

            // Copia los datos al nivel inferior
            // No hay problema con los tipos porque un bloque siempre es mayor o igual que valor_t
            memcpy(datos, oper_cache->bloque->datos.mem.m_byte, oper_sig->tam);
            foreach(i, oper_sig->tam) {
                tipos[i] = oper_cache->bloque->datos.tipos[i];
                dirty[i] = oper_cache->bloque->datos.dirty[i];
            }

            // Ahora falta leer el bloque que estábamos buscando
            // Como la solicitud podría fallar, se deberá lanzar desde la función de actualizar
            oper_cache->info.estado = EST_OP_BLOQUEADA;
            oper_cache->bloque->estado = BLOQ_INVALIDO;

            // La operación todavía no ha terminado
            break;
        default:
            fprintf(stderr, "%s:%d. La operación no puede ser nula", __FILE__, __LINE__);
            exit(1);
    }
}

/**
 * Marca como canceladas las operaciones en curso en este nivel de cache
 *  correspondientes con un determinado tipo.
 *  Sólo se debería usar con operaciones de lectura.
 * @param nivel
 * @param tipo
 */
void
cancela_operaciones_mem_cache(
        NIVEL_MEM_PTR nivel,
        tipo_operacion_memoria_t tipo
) {
    operacion_mem_cache_t *oper;

    FOREACH_OPER(oper, nivel->oper_pendientes) {
        if (oper->info.tipo == tipo) {
            oper->cancelada = SI;
        }
    }
}

/**
 * Vuelca los datos en memoria principal. No hace nada
 * @param nivel
 */
void
vuelca_datos_cache(
        NIVEL_MEM_PTR nivel
) {
    memoria_cache_t *cache = (memoria_cache_t *) nivel;

    unsigned int i, b, d;
    size_t tam;

    foreach(b, cache->config.num_bloques) {
        bloque_cache_t *bloque = &cache->bloques[b];

        if (bloque->estado == BLOQ_SUCIO) {
            d = bloque->direccion;
            i = 0;
            while (i < cache->config.tam_bloque) {
                tipo_t tipo = bloque->datos.tipos[i];
                tam = tam_dato(tipo);
                if (tipo != t_ninguno && mem_chequea_direccion(mmem, d, tipo) == EXC_NONE) {
                    escribe_mem_datos(mmem, d, *(valor_t *) &bloque->datos.mem.m_byte[i], tipo, bloque->datos.tipos[i]);
                }

                d += tam;
                i += tam;
            }
        }
    }
}

/*---------------------------------------------------------------------------*/
/* PRESENTACIÓN                                                              */
/*---------------------------------------------------------------------------*/

/**
 * Escribe la configuración de la memoria cache en formato texto plano
 * @param nivel
 * @param info
 */
void
obtiene_config_memoria_cache(
        NIVEL_MEM_PTR nivel,
        formato_presentacion_t formato,
        g_string_t *info
) {
    switch (formato) {
        case ningun_formato:
            break;
        case formato_txt:
            obtiene_config_memoria_cache_txt(nivel, info);
            break;
        case formato_html:
            obtiene_config_memoria_cache_html(nivel, info);
            break;
        case formato_json:
            break;
    }
}

/**
 * Escribe la configuración de la memoria cache en formato texto plano
 * @param nivel
 * @param info
 */
void
obtiene_config_memoria_cache_txt(
        NIVEL_MEM_PTR nivel,
        g_string_t *info
) {
    config_cache_t *c_config = &((memoria_cache_t *) nivel)->config;

    g_str_printf(info, "  Cache %s: ", nivel->nombre);
    if (nivel->activa) {
        if (c_config->grado_asoc == 1) {
            g_str_copy(info, "Correspondencia directa\n");
            g_str_printf(info, "\tNúmero de bloques: %d\n", c_config->num_bloques);
        } else if (c_config->grado_asoc == c_config->num_bloques) {
            g_str_copy(info, "Completamente asociativa\n");
            g_str_printf(info, "\tNúmero de bloques: %d\n", c_config->num_bloques);
        } else {
            g_str_printf(info, "Asociativa por conjuntos de %d vías\n", c_config->grado_asoc);
            g_str_printf(info, "\tNúmero de conjuntos: %d\n", c_config->num_bloques / c_config->grado_asoc);
        }

        g_str_printf(info, "\tTamaño de bloque: %d bytes\n", c_config->tam_bloque);
        g_str_printf(info, "\tLatencia: %d ciclos\n", c_config->latencia);
    } else {
        if (nivel->nombre[1] == '1') {
            // Cache nivel 1
            g_str_copy(info, "perfecta\n");
        } else {
            g_str_copy(info, "inactiva\n");
        }
    }
}

/**
 * Escribe la configuración de la memoria cache en formato HTML
 * @param nivel
 * @param info
 */
void
obtiene_config_memoria_cache_html(
        NIVEL_MEM_PTR nivel,
        g_string_t *info
) {
    config_cache_t *c_config = &((memoria_cache_t *) nivel)->config;

    T_COMPOSITE table, tr;

    I_INIT_PRINT();

    // Configuración en formato HTML
    table = H_COMPOSITE(table);
    h_add_attr(PTR_BASE(table), "class='config'");

    tr = H_ADD_COMPOSITE(table, tr);

    H_ADD_SINGLE_TEXT(tr, th, I_PRINTF("Cache %s", nivel->nombre));

    if (nivel->activa) {
        if (c_config->grado_asoc == 1) {
            H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='text'", "Correspondencia directa");

            tr = H_ADD_COMPOSITE(table, tr);
            H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='header'", "Número de bloques");
            H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%d", c_config->num_bloques));
        } else if (c_config->grado_asoc == c_config->num_bloques) {
            H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='text'", "Completamente asociativa");

            tr = H_ADD_COMPOSITE(table, tr);
            H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='header'", "Número de bloques");
            H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%d", c_config->num_bloques));
        } else {
            H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='text'", I_PRINTF("Asociativa de %d vías", c_config->grado_asoc));

            tr = H_ADD_COMPOSITE(table, tr);
            H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='header'", "Número de conjuntos");
            H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%d", c_config->num_bloques / c_config->grado_asoc));
        }

        tr = H_ADD_COMPOSITE(table, tr);
        H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='header'", "Tamaño de bloque");
        H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%d bytes", c_config->tam_bloque));

        tr = H_ADD_COMPOSITE(table, tr);
        H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='header'", "Latencia");
        H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%d ciclos", c_config->latencia));
    } else {
        if (nivel->nombre[1] == '1') {
            // Cache nivel 1
            H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='text'", "perfecta");
        } else {
            H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='text'", "inactiva");
        }
    }

    h_tag_flush(PTR_BASE(table), info);

    h_destroy_composite_tag(table);
}

/**
 * Muestra el estado actual de este nivel de memoria cache
 * @param nivel
 * @param formato
 * @param output
 */
void
muestra_estado_memoria_cache(
        NIVEL_MEM_PTR nivel,
        formato_presentacion_t formato,
        g_string_t *output
) {
    switch (formato) {
        case formato_html:
            muestra_estado_memoria_cache_html(nivel, output);
            break;
        case ningun_formato:
        case formato_txt:
        case formato_json:
            break;
    }
}

/**
 * Muestra el estado actual de este nivel de memoria cache en formato HTML
 * @param nivel
 * @param output
 */
void
muestra_estado_memoria_cache_html(
        NIVEL_MEM_PTR nivel,
        g_string_t *output
) {
    memoria_cache_t *cache = (memoria_cache_t *) nivel;

    unsigned int i, j, b;
    unsigned int n_rows, n_cells;
    char s_rows[SHORT_STR];

    size_t tam;

    T_COMPOSITE div;
    T_COMPOSITE table, tr;
    T_SINGLE td;

    I_INIT_PRINT();

    n_rows = cache->config.tam_bloque / ROW_SIZE;
    if (n_rows == 0) n_rows = 1;
    n_cells = MIN(cache->config.tam_bloque, ROW_SIZE);

    sprintf(s_rows, "rowspan=%d", n_rows);

    // Estado en formato HTML
    div = H_ATTR_COMPOSITE(div, "class='cache'");

    // Cabecera
    H_ADD_SINGLE_TEXT(div, h2, I_PRINTF("Memoria cache %s", nivel->nombre));

    // Contenido
    table = H_ADD_COMPOSITE(div, table);
    {
        tr = H_ADD_COMPOSITE(table, tr);

        if (cache->config.grado_asoc > 1 && cache->config.grado_asoc < cache->config.num_bloques) {
            H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='index'", "Conjunto");
        } else {
            H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='index'", "Bloque");
        }

        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='state'", "Estado");
        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='address'", "Dirección");
        if (n_rows > 1) {
            H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='data'", "&nbsp;");
        }

        foreach(i, n_cells) {
            g_str_clear(&g_inline);
            H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='data'", I_PRINTF("+%d", i));
        }

        H_ADD_SINGLE_ATTR_TEXT(tr, th, "class='time'", "LRU");

        foreach(b, cache->config.num_bloques) {
            bloque_cache_t *bloque = &cache->bloques[b];

            tr = H_ADD_COMPOSITE(table, tr);

            if (cache->config.grado_asoc > 1 && cache->config.grado_asoc < cache->config.num_bloques) {
                if (b % cache->config.grado_asoc == 0) {
                    H_ADD_SINGLE_ATTR_TEXT(tr, td, I_PRINTF("rowspan='%zu'", cache->config.grado_asoc * n_rows),
                                           I_PRINTF("%zu", b / cache->config.grado_asoc));
                }
            } else {
                td = H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%zu", b));
                if (n_rows > 1) {
                    h_add_attr(PTR_BASE(td), s_rows);
                }
            }

            if (bloque->oper == OP_NULA) {
                switch (bloque->estado) {
                    case BLOQ_INVALIDO:
                        td = H_ADD_SINGLE_TEXT(tr, td, "Inválido");
                        break;
                    case BLOQ_VALIDO:
                        td = H_ADD_SINGLE_TEXT(tr, td, "Válido");
                        break;
                    case BLOQ_SUCIO:
                        td = H_ADD_SINGLE_TEXT(tr, td, "Sucio");
                        break;
                }
                h_add_attr(PTR_BASE(td), "class='normal'");
            } else {
                td = H_ADD_SINGLE_ATTR_TEXT(tr, td, "class='blocked'", "Accediendo");
            }

            if (n_rows > 1) {
                h_add_attr(PTR_BASE(td), s_rows);
            }

            if (bloque->estado == BLOQ_INVALIDO) {
                // Dirección
                td = H_ADD_SINGLE_TEXT(tr, td, "&nbsp;");
                if (n_rows > 1) {
                    h_add_attr(PTR_BASE(td), s_rows);
                }

                // Offset vertical
                if (n_rows > 1) {
                    H_ADD_SINGLE_TEXT(tr, td, "+0");
                }
                // Datos
                td = H_ADD_SINGLE_ATTR_TEXT(tr, td, I_PRINTF("class='data empty' colspan='%d'", n_cells),
                                            "&nbsp;");
                if (n_rows > 1) {
                    h_add_attr(PTR_BASE(td), s_rows);
                }
                // LRU
                td = H_ADD_SINGLE_TEXT(tr, td, "&nbsp;");
                if (n_rows > 1) {
                    h_add_attr(PTR_BASE(td), s_rows);
                }

                // Offset vertical resto de filas
                if (n_rows > 1) {
                    for (j = 1; j < n_rows; j++) {
                        tr = H_ADD_COMPOSITE(table, tr);
                        H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("+%d", j * ROW_SIZE));
                    }
                }
            } else {
                // Dirección
                td = H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%d", bloque->direccion));
                if (bloque->oper != OP_NULA) {
                    h_add_attr(PTR_BASE(td), "class='old'");
                }
                if (n_rows > 1) {
                    h_add_attr(PTR_BASE(td), s_rows);
                }

                foreach(j, n_rows) {
                    if (j > 0) {
                        tr = H_ADD_COMPOSITE(table, tr);
                    }

                    // Offset vertical
                    if (n_rows > 1) {
                        H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("+%d", j * ROW_SIZE));
                    }

                    // Datos
                    i = 0;
                    while (i < n_cells) {
                        char valor[SHORT_STR];
                        char *v_class = "data";

                        unsigned int idx = j * ROW_SIZE + i;
                        tipo_t tipo = bloque->datos.tipos[idx];

                        tam = tam_dato(tipo);

                        if (tipo == t_code) {
                            word indice = bloque->datos.mem.m_word[indice_code(idx)];

                            if (indice != INST_UNKNOWN) {
                                char *texto = obtiene_ensamblador(mmem, indice);
                                // sprintf(valor, "%d|%d : %s", idx, indice, texto);
                                strcpy(valor, texto);
                            } else {
                                strcpy(valor, "unknown");
                            }

                            v_class = "code";
                        } else {
                            imprime_dato(&bloque->datos, idx, valor);
                        }

                        g_str_clear(&g_inline);
                        if (bloque->oper == OP_NULA) {
                            H_ADD_SINGLE_ATTR_TEXT(tr, td, I_PRINTF("class='%s' colspan='%d'", v_class, tam), valor);
                        } else {
                            H_ADD_SINGLE_ATTR_TEXT(tr, td, I_PRINTF("class='%s old' colspan='%d'", v_class, tam),
                                                   valor);
                        }

                        i += tam;
                    }

                    // LRU
                    if (j == 0) {
                        td = H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%d", Ciclo - bloque->ciclo));
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
        operacion_mem_cache_t *oper;

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

        oper = (operacion_mem_cache_t *) nivel->oper_pendientes;
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
                char *f_cancelada = (oper->cancelada) ? "class='cancelled'" : "";

                // Estado
                H_ADD_SINGLE_TEXT(tr, td, oper->info.texto);

                switch (oper->info.tipo) {
                    case OP_LECT:
                        H_ADD_SINGLE_ATTR_TEXT(tr, td, f_cancelada, "Lectura");
                        break;
                    case OP_ESCR:
                        H_ADD_SINGLE_ATTR_TEXT(tr, td, f_cancelada, "Escritura");
                        break;
                    default:
                        break;
                }

                H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%ld", oper->info.direccion));
                H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%zu", oper->info.tam));
                H_ADD_SINGLE_TEXT(tr, td, I_PRINTF(F_CICLO, oper->info.inicio));
                H_ADD_SINGLE_TEXT(tr, td, I_PRINTF("%hu", oper->info.ciclos));

                oper = (operacion_mem_cache_t *) oper->info.next;
            }

        }
    }

    H_ADD_SINGLE(div, br);

    h_tag_flush(PTR_BASE(div), output);

    h_destroy_composite_tag(div);
}
