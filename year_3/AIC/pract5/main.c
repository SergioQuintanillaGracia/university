/**
 * @file main.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 * @author Salva Petit <spetit@disca.upv.es>
 *
 * @brief
 *    Módulo principal del simulador
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define main_C

/***** Includes ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>
#include <ctype.h>

#include "tipos.h"
#include "memoria.h"
#include "prediccion.h"
#include "presentacion.h"
#include "presentacion-html.h"
#include "presentacion-json.h"
#include "instrucciones.h"
#include "algoritmo.h"
#include "enlazador.h"
#include "memoria-cache.h"
#include "memoria-principal.h"

#include "g_options.h"
#include "g_color.h"

extern void ensamblador(fichero_objeto_t *obj);

/*** Definiciones **********************************************************/

#define SIMUL_NAME      "RiscV-ooo"
#define SIMUL_VERSION   "1.1"
#define SIMUL_DATE      "2023-10-06"

/*** Variables Locales *****************************************************/

/* Jerarquía de memoria */

memoria_cache_t L1I = INICIA_NIVEL_MEMORIA("L1I", nivel_tipo_cache);
memoria_cache_t L1D = INICIA_NIVEL_MEMORIA("L1D", nivel_tipo_cache);
memoria_cache_t L2 = INICIA_NIVEL_MEMORIA("L2", nivel_tipo_cache);
memoria_principal_t MP = INICIA_NIVEL_MEMORIA("MP", nivel_tipo_memoria_principal);

// Añadir aquí los nuevos elementos de la jerarquía de memoria en orden inverso para su actualización
// Los elementos no configurados en tiempo de ejecución estarán desactivados.
// OJO! Si se añaden niveles se debe modificar la función conecta_jerarquía() y la
//  función imprime_config() en presentacion.c
nivel_jerarquia_memoria_t *elementos_memoria[NUM_ELEM_MEMORIA] = {
        PTR_NIVEL(MP),
        PTR_NIVEL(L2),
        PTR_NIVEL(L1D),
        PTR_NIVEL(L1I)
};

/* Configuración por omisión de los operadores de la ruta de datos */

#define INI_OPERADOR(OPER)                     \
oper_info_t INI_ ## OPER = {                   \
        .num_inicio = INI_num_inicio_ ## OPER, \
        .num_oper = INI_num_oper_ ## OPER,     \
        .teval = INI_teval_ ## OPER            \
}

INI_OPERADOR(enteros);
INI_OPERADOR(sumrest);
INI_OPERADOR(multdiv);
INI_OPERADOR(direcciones);
INI_OPERADOR(memdatos);

#define INI_CACHE(NIVEL)                              \
config_cache_t INI_ ## NIVEL = {                      \
        .latencia = INI_ ## NIVEL ## _latencia,       \
        .num_bloques = INI_ ## NIVEL ## _num_bloques, \
        .tam_bloque = INI_ ## NIVEL ## _tam_bloque,   \
        .grado_asoc = INI_ ## NIVEL ## _grado_assoc   \
}

INI_CACHE(l1i);
INI_CACHE(l1d);
INI_CACHE(l2);

config_memoria_principal_t INI_mp = {
        .tam_memoria = INI_mp_tam_memoria,
        .num_bancos = INI_mp_num_bancos,
        .tam_banco = INI_mp_tam_banco,
        .CL = INI_mp_CL,
        .tRCD = INI_mp_tRCD,
        .tRP = INI_mp_tRP,
        .BW = INI_mp_BW
};

#define INI_OPER_NIVEL(NIVEL)                  \
boolean NIVEL ## _oper_config_set = NO;        \
boolean NIVEL ## _oper_config_used = NO;       \
config_operaciones_t NIVEL ## _oper_config = { \
        .max_oper = 1,                         \
        .max_oper_ciclo = 1,                   \
        .max_oper_acceso = 1                   \
}

INI_OPER_NIVEL(L1I);
INI_OPER_NIVEL(L1D);
INI_OPER_NIVEL(L2);
INI_OPER_NIVEL(MP);

num_vias_info_t INI_num_vias = {
        .issue = INI_num_vias_issue,
        .bus = INI_num_vias_bus,
        .commit = INI_num_vias_commit
};

tam_rs_info_t INI_tam_rs = {
        .enteros = INI_tam_rs_enteros,
        .sumrest = INI_tam_rs_sumrest,
        .multdiv = INI_tam_rs_multdiv,
        .carga = INI_tam_rs_carga,
        .almacen = INI_tam_rs_almacen
};

predic_config_t INI_config_predic = {
        .tam_buffer_predic = INI_tam_buffer_predic,
        .tipo_predictor = BTB_1_BIT,

        /* BTB */
        .n_bits_pred_local = 1,
        .subtipo_pred_local = 's',

        /* Hibrido */
        .n_bits_selector = 1,
        .subtipo_pred_hybrid = 's',

        /* 2 niveles */
        .tam_tabla_BHSR1 = INI_tam_tabla_BHSR1,
        .n_bits_BHSR1 = 0,

        .tam_tabla_BHSR2 = INI_tam_tabla_BHSR2,
        .n_bits_BHSR2 = 0
};

/*** Funciones Locales *****************************************************/

static const char *
simul_logo() {
    return
            "    ____  _          _    __                            \n"
            "   / __ \\(_)________| |  / /           ____  ____  ____ \n"
            "  / /_/ / / ___/ ___/ | / /  ______   / __ \\/ __ \\/ __ \\\n"
            " / _, _/ (__  ) /__ | |/ /  /_____/  / /_/ / /_/ / /_/ /\n"
            "/_/ |_/_/____/\\___/ |___/            \\____/\\____/\\____/ \n";
}

/**
 * Establece la información del programa
 * @param opts
 */
static void
simul_set_info(
        g_options_t *opts
) {
    g_str_printf(g_opt_header_str(opts), "%s%s%s\n",
                 FG_COLOR(BLUE), simul_logo(), GC_RESET);

    g_str_printf(g_opt_header_str(opts), "%s%s%s version %s%s%s %s\n",
                 FG_COLOR(GREEN), SIMUL_NAME, GC_RESET,
                 FG_COLOR(YELLOW), SIMUL_VERSION, GC_RESET,
                 SIMUL_DATE);

    g_str_printf(g_opt_usage_str(opts), "%s [options] {-f input.s}+", basename(opts->argv[0]));

    g_str_copy(g_opt_authors_str(opts),
               "Sergio Sáez <ssaez@disca.upv.es>\n"
               "Pedro López <plopez@disca.upv.es>\n"
               "Salvador Petit <spetit@disca.upv.es>\n"
               "\n"
               "Departamento de Informática de Sistemas y Computadores\n"
               "Universidad Politécnica de Valencia\n");

    g_str_copy(g_opt_brief_str(opts),
               "Simulador del procesador RiscV con ejecución fuera de orden\n");

    g_str_printf(g_opt_info_str(opts),
                 "%s es un simulador de un procesador RiscV superescalar con ejecución fuera de orden.\n"
                 "RiscV-ooo es un simulador de un procesador RiscV superescalar con ejecución fuera de orden.\n"
                 "Las principales características son:\n"
                 "    - Juego de instrucciones: RV64IMFD\n"
                 "    - Ejecución ciclo a ciclo. Para cada ciclo muestra:\n"
                 "        + el estado de las estructuras internas,\n"
                 "        + el estado de los registros y memoria de datos,\n"
                 "        + un cronograma con las fases que cada instrucción ha ejecutado en cada ciclo\n"
                 "    - Ejecución fuera de orden con especulación hardware\n"
                 "    - Procesador superescaler con N vías (configurable)\n"
                 "    - Predicción dinámica de saltos. Múltiples tipos de predictores.\n"
                 "    - Jerarquía de memoria configurable: L1I, L1D, L2 y MP.\n"
                 "        + Geometría y parámetros temporales configurables.\n"
                 "        + Caches con política Write-Back + Write-Allocate.\n",
                 SIMUL_NAME);
}

/**
 * Comprueba si un número entero positivo era potencia de 2
 * @param value
 * @return SI, si el número es positivo y potencia de 2, NO en otro caso
 */
static boolean is_pow_2(int value) {

    if (value <= 0) return NO;

    // Quitamos los 0s por la derecha
    while ((value & 1) == 0) {
        value >>= 1;
    }

    // Si al final sólo queda un 1, entonces era potencia de 2
    return (value == 1);
}

/**
 * Inicializa las estructuras de la unidad de gestión de
 *  instrucciones
 */
static void inicializa_estructuras() {

    int i, oper;

    /* Estadísticas */

    memset(&estat, 0, sizeof(estat));
    memset(&max_util, 0, sizeof(max_util));
    memset(&aver_util, 0, sizeof(max_util));

    /* Registros inter-etapas */

    memset(&IF_ISS_1, 0, sizeof(IF_ISS_1));
    //IF_ISS_1.IR.codop = OP_NOP;
    IF_ISS_1.instruccion = INSTRUC_NOP;
    IF_ISS_1.PC = -1;
    IF_ISS_1.orden = 0;
    IF_ISS_1.ignorar = SI;
    IF_ISS_1.excepcion = EXC_NONE;

    IF_ISS_2 = IF_ISS_1;

    Control_1.NPC = 0;
    Control_1.indice = INST_UNKNOWN;
    Control_1.orden = 1;
    Cancelar_Activa = NO;

    Control_2 = Control_1;

    SIG_SYSCALL = NO;

    for (i = 0; i < CONFIG(num_vias.issue); i++) {
        M_IF_ISS_1[i] = IF_ISS_1;
        M_IF_ISS_2[i] = IF_ISS_2;
    }

    /* Registros */

    for (i = 0; i < TAM_REGISTROS; i++) {
        Rfp[i].valor.fp_d = 0.0;
        Rfp[i].rob = MARCA_NULA;
        Rint[i].valor.int_d = 0;
        Rint[i].rob = MARCA_NULA;
        Rint_tipo[i] = t_ninguno;
        Rfp_tipo[i] = t_ninguno;
        Rint_dirty[i] = NO;
        Rfp_dirty[i] = NO;
    }

    /* Predictor */

    inicializa_prediccion();

    /* Estaciones de reserva */

    SET_CONFIG(tam_estaciones, CONFIG(tam_rs.enteros) + CONFIG(tam_rs.sumrest) + CONFIG(tam_rs.multdiv) +
                               CONFIG(tam_rs.carga) + CONFIG(tam_rs.almacen));

    for (i = 0; i < CONFIG(tam_estaciones); i++) {
        RS[i].OP = RV32I_ADDI;
        RS[i].ocupado = NO;
        RS[i].Q1 = MARCA_NULA;
        RS[i].V1.int_d = 0;
        RS[i].Q2 = MARCA_NULA;
        RS[i].V2.int_d = 0;
        RS[i].Q3 = MARCA_NULA;
        RS[i].V3.int_d = 0;
        RS[i].direccion = 0;
        RS[i].resultado.int_d = 0;
        RS[i].condicion = NO;
        RS[i].estado = PENDIENTE;
        RS[i].confirm = NO;
        RS[i].rob = MARCA_NULA;
        RS[i].orden = 0;
    } /* endfor */

    /* Reorder Buffer */

    RB_inicio = 0;
    RB_fin = 0;
    RB_long = 0;

    INICIO_REORDER = 0;
    for (i = 0; i < CONFIG(tam_reorder); i++) {
        RB[i].OP = RV32I_ADDI;
        RB[i].PC = -1;
        RB[i].ocupado = NO;
        RB[i].completado = NO;
        RB[i].dest = MARCA_NULA;
        RB[i].valor.int_d = 0;
        RB[i].condicion = NO;
        RB[i].prediccion = NO;
        RB[i].direccion = NO;
        RB[i].orden = 0;
    } /* endfor */
    FIN_REORDER = CONFIG(tam_reorder) - 1;

    /* Operadores */

    if (CONFIG(enteros.num_oper) + CONFIG(sumrest.num_oper) + CONFIG(multdiv.num_oper) +
        CONFIG(direcciones.num_oper) + CONFIG(memdatos.num_oper) > MAX_OPERADORES) {
        fprintf(stderr, "ERROR: Número de operadores excesiv. Actualice la constante MAX_OPERADORES (mínimo %d)\n",
                CONFIG(enteros.num_oper) + CONFIG(sumrest.num_oper) + CONFIG(multdiv.num_oper) +
                CONFIG(direcciones.num_oper) + CONFIG(memdatos.num_oper));
        exit(1);
    }

    if (CONFIG(tam_rs.enteros) + CONFIG(tam_rs.sumrest) + CONFIG(tam_rs.multdiv) +
        CONFIG(tam_rs.carga) + CONFIG(tam_rs.almacen) > MAX_ESTACIONES) {
        fprintf(stderr,
                "ERROR: Número de estaciones de reserva excesivo. Actualice la constante MAX_ESTACIONES (mínimo %d)\n",
                CONFIG(tam_rs.enteros) + CONFIG(tam_rs.sumrest) + CONFIG(tam_rs.multdiv) +
                CONFIG(tam_rs.carga) + CONFIG(tam_rs.almacen));
        exit(1);
    }

    oper = 0;
    INICIO_OP_ENTEROS = oper;
    for (i = 0; i < CONFIG(enteros.num_oper); i++, oper++) {
        Op[oper].tipo = OPER_ENTEROS;
        Op[oper].ocupado = NO;
        Op[oper].codigo = 0;
        Op[oper].ciclo = 0;
        Op[oper].Teval = CONFIG(enteros.teval);
    }
    FIN_OP_ENTEROS = oper - 1;

    INICIO_OP_SUMREST = oper;
    for (i = 0; i < CONFIG(sumrest.num_oper); i++, oper++) {
        Op[oper].tipo = OPER_SUMREST;
        Op[oper].ocupado = NO;
        Op[oper].codigo = 0;
        Op[oper].ciclo = 0;
        Op[oper].Teval = CONFIG(sumrest.teval);
    }
    FIN_OP_SUMREST = oper - 1;

    INICIO_OP_MULTDIV = oper;
    for (i = 0; i < CONFIG(multdiv.num_oper); i++, oper++) {
        Op[oper].tipo = OPER_MULTDIV;
        Op[oper].ocupado = NO;
        Op[oper].codigo = 0;
        Op[oper].ciclo = 0;
        Op[oper].Teval = CONFIG(multdiv.teval);
    }
    FIN_OP_MULTDIV = oper - 1;

    INICIO_OP_DIRECCIONES = oper;
    for (i = 0; i < CONFIG(direcciones.num_oper); i++, oper++) {
        Op[oper].tipo = OPER_DIRECCIONES;
        Op[oper].ocupado = NO;
        Op[oper].codigo = 0;
        Op[oper].ciclo = 0;
        Op[oper].Teval = CONFIG(direcciones.teval);
    }
    FIN_OP_DIRECCIONES = oper - 1;

    INICIO_OP_MEMDATOS = oper;
    for (i = 0; i < CONFIG(memdatos.num_oper); i++, oper++) {
        Op[oper].tipo = OPER_MEMDATOS;
        Op[oper].ocupado = NO;
        Op[oper].codigo = 0;
        Op[oper].ciclo = 0;
        Op[oper].Teval = CONFIG(memdatos.teval);
    }
    FIN_OP_MEMDATOS = oper - 1;

    NUM_OPERADORES = oper;

    /* Bus común */

    BUS.ocupado = NO;
    BUS.codigo = MARCA_NULA;
    BUS.valor.int_d = 0;

    estado_previo = ht_create(TAM_ESTADO);

} /* end inicializa_estructuras */

/**
 * Conecta los niveles de la jerarquía de memoria configurados
 */
static
void conecta_jerarquia(
        g_options_t *cmdline
) {
    unsigned int i;
    nivel_jerarquia_memoria_t **p_next = NULL;
    nivel_jerarquia_memoria_t *next = NULL;

#define conecta_nivel(N1, N2) do { (N1).mem.siguiente = &(N2).mem; } while(0)

    // Conexión inicial. Asume todos los niveles activos
    conecta_nivel(L2, MP);
    conecta_nivel(L1I, L2);
    conecta_nivel(L1D, L2);

    // Si alguna cache del primer nivel está activa ...
    if (L1I.mem.activa || L1D.mem.activa) {
        // Se activa la memoria principal por omisión
        if (!MP.mem.activa) {
            g_opt_error_option(cmdline, "--mp", "Opción obligatoria si se activan L1D y/o L1I");
        }

        // Elimina los niveles inactivos de la jerarquía
        foreach(i, NUM_ELEM_MEMORIA) {
            if (elementos_memoria[i]->activa) {
                p_next = &elementos_memoria[i]->siguiente;
                next = *p_next;
                // Busca el siguiente nivel activo
                while (next != NULL) {
                    if (next->activa) break;
                    next = next->siguiente;
                }
                // Apunta al siguiente nivel activo o a NULL
                *p_next = next;
            }
        }
    }

    // NOTE: Si L1I o L1D no están activas se suponen caches perfectas
    //  y se utiliza el comportamiento habitual de: latencia + acceso directo a MP
}

void vuelca_jerarquia_en_memoria_datos() {

    unsigned int i;

    foreach(i, NUM_ELEM_MEMORIA) {
        if (elementos_memoria[i]->activa) {
            INVOCA_INTERFAZ_MEMORIA(elementos_memoria[i], vuelca_datos);
        }
    }

}

/**
 * Mira si queda alguna operación pendiente de acabar
 * @return Cierto si han acabado todas las operaciones
 */
static boolean mira_si_todo_vacio(
) {
    int i;

    for (i = INICIO_BUFFER_ALMACEN; i <= FIN_BUFFER_ALMACEN; i++) {
        if (SB[i].ocupado & SB[i].confirm)
            return NO;
    }

    foreach(i, NUM_ELEM_MEMORIA) {
        if (elementos_memoria[i]->activa) {
            if (hay_operaciones_pendientes(elementos_memoria[i]))
                return NO;
        }
    }

    return SI;

} /* end mira_si_todo_vacio */

#define obtiene_util(estr, campo) \
do { \
    int i;\
    int ocup = 0; \
    for (i = INICIO_##campo; i <= FIN_##campo; i++) { \
        if (estr[i].ocupado == SI) \
            ocup++; \
    } \
    if (ocup > max_util.campo) \
        max_util.campo = ocup; \
    aver_util.campo += ocup; \
} while(0)

/**
 * Obtiene la utilización de las estructuras internas
 */
static void analiza_ocupacion() {
    /* Estaciones de reserva */

    obtiene_util(RS, RS_ENTEROS);
    obtiene_util(RS, RS_SUMREST);
    obtiene_util(RS, RS_MULTDIV);
    obtiene_util(RS, BUFFER_CARGA);
    obtiene_util(RS, BUFFER_ALMACEN);

    obtiene_util(Op, OP_ENTEROS);
    obtiene_util(Op, OP_SUMREST);
    obtiene_util(Op, OP_MULTDIV);
    obtiene_util(Op, OP_DIRECCIONES);
    obtiene_util(Op, OP_MEMDATOS);

    obtiene_util(RB, REORDER);

} /* end analiza_ocupacion */

/**
 * Construye un programa.
 * Imita el proceso de ensamblado, enlazado, etc.
 * @param prog
 */
static void construye_programa(
        programa_t *prog
) {

    /* Fichero objeto */
    for (int i = 0; i < prog->n_objs; ++i) {
        ensamblador(&prog->l_objs[i]);
    }

    /*** Enlazar ***/

    inicializa_memoria(&prog->memoria);

    // Variable global para simplificar la implementación
    mmem = &prog->memoria;

    /*** Enlazar ***/

    enlazador(prog);

    /*** Cargar ***/

    cargador(prog);

    /*** Generar hexadecimal */

    genera_MInstruccion();

}

/**
 * Ejecuta el bucle principal del computador
 */
static void ejecutar_codigo() {

    //boolean final;
    boolean OVF_HTML = NO;

    unsigned int m;
    dword validNPC;
    dword PC_bloque;
    boolean Cancelar;
    boolean Excepcion;
    boolean Procesada[MAX_VIAS_SUPERESCALAR];
    operacion_busqueda_t IF_buffer;

    reg_control_t Bloque;

    int i;

    /*** Inicialización del computador */

    Ciclo = 0;
    terminando = NO;
    final = NO;
    Excepcion_Activa = NO;

    IF_buffer.estado = EST_OP_PENDIENTE;
    inicializa_zona_datos(&IF_buffer.buffer, CONFIG(num_vias.issue) * sizeof(word), tr_code);

    /* Borra archivo de consola */
    unlink(console_name);

    /* Borra archivo de log */
    unlink(log_name);

    fprintf(stdout, "*** EJECUCIÓN ***\n\n");

    imprime_inicio();

    /* Directiva de ensamblador .pc */
    Control_2.NPC = PC_inicial;

    /*** Bucle principal */

    for (m = 0; m < CONFIG(num_vias.issue); m++) {
        Procesada[m] = NO;
    }

    IFhide = NO;
    while (!final) {
        Cancelar = NO;

        IFstall = NO;
        ISSstall = NO;
        Cancelar_Activa = NO;

        Ciclo++; /* Incrementa el número de ciclos ejecutados */

        inicio_ciclo(Ciclo);

        /*** Limpieza fuera de la vista ***/

        foreach(i, CONFIG(tam_estaciones)) {
            if (!RS[i].ocupado) {
                RS[i].excepcion = EXC_NONE;
            }
        }

        /*** Fase: EX (1) **********/

        fase_EX_1();

        /*** Fase: COMMIT ***********/

        foreach (m, CONFIG(num_vias.commit)) {
            fase_COM();
        }

        /*** Fase: WB **************/

        foreach (m, CONFIG(num_vias.bus)) {
            fase_WB();
        }

        /*** Fase: ISSUE ***********/

        Excepcion = Excepcion_Activa;
        foreach (m, CONFIG(num_vias.issue)) {
            IF_ISS_2 = M_IF_ISS_2[m];

            // La excepción se ha producido en este bloque
            // ... el resto de instrucciones se cancelan
            if (Excepcion != Excepcion_Activa) {
                IF_ISS_2.cancelar = SI;
                IF_ISS_2.ignorar = NO; // Se ignorarán en el siguiente ciclo
            }

            // Procesada == NO indica que hay que volverlo a intentar en el ciclo siguiente
            Procesada[m] = fase_ISS();
        }

        /** Estadísticas de ocupación */

        analiza_ocupacion();

        /*** Fase: IF (1) ***********/

        Bloque = Control_2;
        validNPC = Bloque.NPC; // Sólo para inicializarlo
        /* Calcula la dirección desde el comienzo del bloque,
         * por si Bloque.NPC era consecuencia de un salto en mitad del bloque */
        PC_bloque = Bloque.NPC - (Bloque.NPC % (CONFIG(num_vias.issue) * 4));

        // Solicita el bloque de instrucciones
        fase_IF_1(&IF_buffer, PC_bloque, Bloque.orden);

        /*** Actualiza el estado de la jerarquía de memoria, si está activa */

        if (JERARQUIA_ACTIVA()) {
            // Se actualiza el estado en orden inverso, para que los niveles inferiores no
            //   atiendan peticiones que se generan en este ciclo
            // Los niveles ya están en orden inverso en 'elementos_memoria[]'
            foreach(i, NUM_ELEM_MEMORIA) {
                if (elementos_memoria[i]->activa) {
                    INVOCA_INTERFAZ_MEMORIA(elementos_memoria[i], actualiza_estado);
                }
            }
        }

        /*** Fase: EX (2) **********/

        fase_EX_2();

        /*** Fase: IF (2) **********/

        if (Excepcion_Activa || ISSstall || IF_buffer.estado != EST_OP_TERMINADA) {
            // 1) Si se para Issue, hay que parar IF
            // 2) Las instrucciones no están disponibles
            IFstall = SI;
        }

        // Comprueba la fase IF
        Excepcion = NO;
        foreach (m, CONFIG(num_vias.issue)) {
            // Engañando a la fase de búsqueda
            Control_2.NPC = IF_buffer.PC[m];
            Control_2.indice = IF_buffer.buffer.mem.m_word[m];

            fase_IF_2();

            Control_2.orden = Control_1.orden;

            M_IF_ISS_1[m] = IF_ISS_1;

            if (!IFstall) {
                if (Control_2.NPC < Bloque.NPC || Cancelar) {
                    // Esta instrucción no la queremos ejecutar porque
                    //   1) es anterior a una dirección de salto en mitad de un bloque
                    //   2) es posterior a un salto que predice que sí que salta
                    M_IF_ISS_1[m].cancelar = SI;
                } else if (IF_ISS_1.prediccion) {
                    // Cancelar el resto de instrucciones si la predicción es saltar
                    Cancelar = SI;
                    validNPC = Control_1.NPC;
                } else if (IF_ISS_1.excepcion != EXC_NONE) {
                    // Cancelar el resto de instrucciones si hay una excepción
                    //   ya que es un salto incondicional
                    Excepcion = SI;
                    validNPC = Control_1.NPC;
                } else if (Excepcion) {
                    // Esta instrucción no la queremos ejecutar porque
                    //   1) es posterior a una excepción que seguro que salta
                    M_IF_ISS_1[m].cancelar = SI;
                    validNPC = Control_1.NPC;
                } else {
                    // Instrucción normal
                    validNPC = Control_1.NPC;
                }

            }
        }

        /*** Control del simulador ****************/

        IFhide = Excepcion_Activa;

        if (terminando) {
            if (mira_si_todo_vacio()) {
                final = SI; /* Termina */
            } else {
                Control_2 = Bloque; /* Repite bloque */
                // Control_2.Parar = SI;
            }
        } else {
            if (Cancelar_Activa) {
                Control_2 = Control_1;
            } else if (IFstall) {
                Control_2 = Bloque; /* Repite bloque */
            } else {
                // Utiliza el último NPC válido.
                Control_1.NPC = validNPC;
                Control_2 = Control_1;

                // Hay que buscar un nuevo conjunto de instrucciones
                IF_buffer.estado = EST_OP_PENDIENTE;
            }

            if (ISSstall) {
                // Se mantiene el contenido de IF_ISS_2, pero
                //   se deben ignorar las instrucciones que ya se han procesado (lanzado, cancelado, ...)
                foreach (m, CONFIG(num_vias.issue)) {
                    M_IF_ISS_2[m].ignorar = Procesada[m];
                }
            } else {
                // Se transfiere la salida de IF hacía ISS
                foreach (m, CONFIG(num_vias.issue)) {
                    M_IF_ISS_2[m] = M_IF_ISS_1[m];
                }
            }

        }

        if (Cancelar_Activa) {
            IF_buffer.estado = EST_OP_PENDIENTE;

            foreach(m, CONFIG(num_vias.issue)) {
                M_IF_ISS_2[m].orden = M_IF_ISS_1[m].orden;
                M_IF_ISS_2[m].PC = M_IF_ISS_1[m].PC;
                M_IF_ISS_2[m].ignorar = SI;

                IF_buffer.PC[m] = Control_2.NPC + (dword) (m * sizeof(word));
            }
        }

        /*** Estadísticas ***/

        estat.ciclos++;

        if (Ciclo >= MAX_CICLOS_TOTAL) {
            fprintf(stderr, "\nATENCIÓN: Número de ciclos (%d) excesivo"
                            "\n          Ejecución detenida\n\n", MAX_CICLOS_TOTAL);
            final = SI;
        }

        if (FLAG(depuracion) && (Ciclo >= MAX_CICLOS_HTML) && !OVF_HTML) {
            fprintf(stderr, "\nATENCIÓN: Número de ciclos (%d) excesivo"
                            "\n          No se generan más ciclos para visualizar\n\n", MAX_CICLOS_HTML);
            OVF_HTML = SI;
            ignora_firma();
        }

        /*** Visualización por consola */

        if (FLAG(depuracion) == NO) {
            if ((Ciclo > 0) && (Ciclo % 2000) == 0) {
                printf(".");
                fflush(stdout);
            }
            if ((Ciclo > 0) && (Ciclo % 10000) == 0) {
                printf("%5uK", Ciclo / 1000);
                fflush(stdout);
            }
            if ((Ciclo > 0) && (Ciclo % 50000) == 0) {
                printf("\n");
            }
        }

        /*** Imprime el estado del los operadores, etc.  */

        if (FLAG(depuracion) && !OVF_HTML) {
            imprime_crono();
            imprime_estado();
            imprime_predictor();
            imprime_memoria();
        } /* endif */

        fin_ciclo(Ciclo);

    } /* endwhile */

    vuelca_jerarquia_en_memoria_datos();

    imprime_final();

} /* end ejecutar_codigo */

/**
 * Comprueba si existe un fichero existe
 * @param filename
 * @return
 */
static int file_exists(char *filename) {
    struct stat buffer;

    return (stat(filename, &buffer) == 0);
}

/**
 * Establece la configuración por omisión
 */
static void set_default_config() {
    unsigned int i;

    // No se inicializa con la opción, porque se pasa memdatos y éste se rellena en el handler
    memcpy(PTR_CONFIG(direcciones), &INI_direcciones, sizeof(oper_info_t));

    /* Jerarquía de memoria */

    // Se activan por parámetros
    foreach(i, NUM_ELEM_MEMORIA) {
        elementos_memoria[i]->activa = NO;
    }
}

#define check_positive(CMDLINE, CONFIG, OPTION, MESSAGE) \
do {                                                     \
    if ((CONFIG) < 1) {                                  \
        g_opt_error_option(CMDLINE, OPTION, MESSAGE " (N > 0)"); \
    }                                                    \
} while (0)

#define check_positive_pow_2(CMDLINE, CONFIG, OPTION, MESSAGE) \
do {                                                           \
    if ((CONFIG) < 1 || !is_pow_2(CONFIG)) {                   \
        g_opt_error_option(CMDLINE, OPTION, MESSAGE " (N > 0 & N = 2^n)"); \
    }                                                          \
} while (0)

#define check_limit(CMDLINE, CONFIG, MIN, MAX, OPTION, MESSAGE) \
do {                                                            \
    if ((CONFIG) < (MIN) || (CONFIG) > (MAX)) {                 \
        g_opt_error_option(CMDLINE, OPTION, MESSAGE " ([%d..%d])", (MIN), (MAX)); \
    }                                                           \
} while (0)

#define check_limit_pow_2(CMDLINE, CONFIG, MIN, MAX, OPTION, MESSAGE) \
do {                                                                  \
    if ((CONFIG) < (MIN) || (CONFIG) > (MAX) || !is_pow_2(CONFIG)) {  \
        g_opt_error_option(CMDLINE, OPTION, MESSAGE " ([%d..%d] & N = 2^n)", (MIN), (MAX)); \
    }                                                                 \
} while (0)

static
void comprueba_param_cache(
        g_options_t *cmdline,
        config_cache_t config,
        const char *opcion,
        const char *nombre
) {
    if (config.num_bloques < 1 || !is_pow_2(config.num_bloques)) {
        g_opt_error_option(cmdline, opcion, "Número de bloques de %s incorrecto (N > 0 & N = 2^n)", nombre);
    }
    if (config.tam_bloque < 1 || !is_pow_2(config.tam_bloque)) {
        g_opt_error_option(cmdline, opcion, "Tamaño de bloque de %s incorrecto (N > 0 & N = 2^n)", nombre);
    }
    if (config.grado_asoc < 0 ||
        (config.grado_asoc > 0 && !is_pow_2(config.grado_asoc))) {
        g_opt_error_option(cmdline, opcion, "Grado asociatividad de %s incorrecto (N >= 0 & N = 2^n)", nombre);
    }
    if (config.latencia < 1) {
        g_opt_error_option(cmdline, opcion, "Latencia de %s incorrecta (N > 0)", nombre);
    }
    if (comprueba_config_memoria_cache(config)) {
        g_opt_error_option(cmdline, opcion, "Geometría de %s incorrecta", nombre);
    }
}

/*** Funciones Exportables *************************************************/

#ifndef LIBRISCV

/**
 * Función principal
 * @param argc
 * @param argv
 * @return
 */
int main(
        int argc,
        char *argv[]
) {

#else

/***************************************************************
*
* Func: libriscv_main
*
* Parametros:
*      ensamblador_s (char*): Cadena con programa ensamblador a simular. Al inicio del programa debe incluirse un comentario en ensamblador con los argumentos del simulador.
*      devuelve_json (int): Devolver el resultado de la simulacion representado en json.
*
* Retorna: resultado_htm (char*): Resultado de la simulacion (malloc). Es responsabilidad del que llama liberar con free.
*
* Descripcion:
*   Funcion principal de la libreria (reemplaza a main)
*
**************************************************************/

char* libriscv_main(char* ensamblador_s, int devuelve_json) {

    /*** Crea el fichero de entrada para el simulador a partir de ensamblador_s */

    {
        FILE* f_entrada;

        f_entrada = fopen("fichero.s", "w");
        fputs(ensamblador_s, f_entrada);
        fclose(f_entrada);
    }

    /*** Define los mínimos argumentos para que el parser no se queje */

    int argc = 3;
    char** argv = malloc(sizeof(char*) * (argc + 1));
    argv[0] = "riscv-m";
    argv[1] = "-f";
    argv[2] = "fichero.s";
    /* El último componente argv[argc] se pone a 0 más abajo cuando se termina de añadir argumentos */

    /*** Añade argumentos a partir del primer comentario de ensamblador_s */

    {
        char* i;

        /* Busca el primer carácter que no es un espacio. */
        for (i = ensamblador_s; isspace(*i); i++);

        if (*i == '#') { /* El primer carácter debe marcar un comentario */

            i++;

            char* j;

            /* Busca el final de los argumentos */
            for (j = i; *j != '\n' && *j != 0; j++);

            /* Aisla los argumentos */
            *j = 0;

            /* Separa y añade a argv los argumentos encontrados */
            char* p;

            for (p = strtok(i, " \t"); p != 0; p = strtok(0, " \t")) {
	        argv[argc] = p; /* Se asigna el argumento al último componente de argv */
                argc++; /* Se incrementa el número de componentes */
                argv = realloc(argv, sizeof(char*) * (argc + 1));
            }
        }
    }

    /*** Marca fin de argumentos */
    argv[argc] = 0;

#endif /* #ifndef LIBRISCV */

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    // Si se usa emscripten, se compila con clang.
    // OPTION_HANDLER se implementa con "nested functions" cuando se usa gcc.
    // OPTION_HANDLER se implementa con "blocks" cuando se usa clang.
    // Los blocks de clang necesitan sintaxis añadida:
    // Las variables locales externas al bloque que se modifican en el bloque necesitan definirse con el atributo "__block".
    // Si se usa gcc definimos "__block" como vacio para que la compilación en gcc no falle.
    
#ifndef LIBRISCV
#define __block
#else
#define __block static // En la versión actual de emscripten __block da fallos durante la ejecución. Sustituido por static por ahora. 
#endif
    
    int RC = 0; /* Valor de retorno */
    __block int np, lp;

    g_options_t cmdline;
    g_section_t *opt_main;
    g_section_t *opt_ejec;
    g_section_t *opt_config;

    boolean borrar_html = SI;
    boolean parametroM = NO;
    __block boolean parametroL1I = NO;
    G_STR_DECL(str_ficheros);

    char *fichero_firmas;

    __block char *ficheros_entrada[MAX_FICHEROS];
    __block int num_ficheros = 0;

    // Inicializa la estructura de las opciones de ejecución
    opt_main = g_opt_init(&cmdline, argc, argv);

    // Inicializa la información sobre el simulador
    simul_set_info(&cmdline);

    // --- Añade las opciones -------------------------------------------------

    /* --------------------------------------------------------------------- */
    g_flag_option_t *f_help =
            g_opt_add_optional_flag(opt_main, "h", "help",
                                    "Muestra esta ayuda",
                                    NULL, NO);
    OPTION_HANDLER(f_help) {
        g_opt_show_help(&cmdline);
        exit(1);
    };
    // Con clang OPTION_HANDLER no define una función sino que asigna una función a una variable.
    // Por tanto, hace falta el ; del final para que compile con clang. Con gcc el ; no afecta.

    /* --------------------------------------------------------------------- */
    g_flag_option_t *f_about =
            g_opt_add_optional_flag(opt_main, "", "about",
                                    "Muestra información sobre el simulador",
                                    NULL, NO);
    OPTION_HANDLER(f_about) {
        g_opt_show_about(&cmdline);
        exit(1);
    };

    /* --------------------------------------------------------------------- */
    g_string_option_t *s_file =
            g_opt_add_mandatory_string(opt_main, "f", "file",
                                       "Fichero de entrada en ensamblador",
                                       NULL);
    g_opt_set_parameter_name(s_file, "input.s");
    OPTION_HANDLER(s_file) {
        ficheros_entrada[num_ficheros] = opts->param;
        if (!file_exists(ficheros_entrada[num_ficheros])) {
            g_opt_error(&cmdline, "Fichero de entrada '%s' no encontrado.",
                        ficheros_entrada[num_ficheros]);
        } /* endif */

        g_str_printf(&str_ficheros, " %s", basename(ficheros_entrada[num_ficheros]));
        num_ficheros++;
    };

    /* --------------------------------------------------------------------- */
    // --- EJECUCIÓN ----------------------------------------------------------
    /* --------------------------------------------------------------------- */
    opt_ejec = g_opt_add_section(&cmdline, "Ejecución");

    /* --------------------------------------------------------------------- */
    g_string_option_t *s_test =
            g_opt_add_optional_string(opt_ejec, "t", "test",
                                      "Comprueba que la ejecución corresponde con la que hay en el fichero de firmas",
                                      &fichero_firmas, NULL);
    g_opt_set_parameter_name(s_test, "fichero.sign");
    OPTION_HANDLER(s_test) {
        abre_firma(fichero_firmas);
        SET_FLAG(firmas, SI);
    };

    /* --------------------------------------------------------------------- */
    g_string_option_t *s_gen =
            g_opt_add_optional_string(opt_ejec, "g", "gen",
                                      "Genera un fichero de firmas correspondiente a la ejecución actual",
                                      &fichero_firmas, NULL);
    g_opt_set_parameter_name(s_gen, "fichero.sign");
    OPTION_HANDLER(s_gen) {
        crea_firma(fichero_firmas);
        SET_FLAG(firmas, SI);
    };

    /* --------------------------------------------------------------------- */
    g_opt_add_optional_flag(opt_ejec, "j", "compact",
                            "Genera un sólo archivo .htm con todos los ficheros de estado y cronogramas",
                            &html_merge, NO);

    /* --------------------------------------------------------------------- */
    g_flag_option_t *f_json =
            g_opt_add_optional_flag(opt_ejec, "J", "json",
                                    "Genera un sólo archivo .js con todos los datos de la simulación",
                                    NULL, NO);
    OPTION_HANDLER(f_json) {
        presentacion = presentacion_json;
        SET_OPTION(formato_presentacion, formato_json);
    };

    /* --------------------------------------------------------------------- */
    g_opt_add_optional_flag(opt_ejec, "w", "symbols",
                            "Utiliza direcciones simbólicas para la representación",
                            PTR_FLAG(etiquetas), NO);

    /* --------------------------------------------------------------------- */
    g_opt_add_optional_flag(opt_ejec, "D", "macros",
                            "Se muestran las macros para el cálculo del valor inmediato a partir de las etiquetas.\n"
                            "Afecta solamente a las macros predeterminadas",
                            PTR_FLAG(macros), NO);

    /* --------------------------------------------------------------------- */
    g_opt_add_optional_flag(opt_ejec, "s", "short",
                            "Ejecuta sin imprimir los ficheros de estado y los cronogramas. \n"
                            "Se deshabilita la comprobación de firmas",
                            PTR_FLAG(depuracion), SI);

    /* --------------------------------------------------------------------- */
    g_opt_add_optional_flag(opt_ejec, "", "dump",
                            "Vuelca el contenido de la memoria en un fichero al final de la ejecución",
                            PTR_FLAG(dump), NO);

    /* --------------------------------------------------------------------- */
    g_opt_add_optional_flag(opt_ejec, "n", "no-clean",
                            "No borra los archivos html antes de la ejecución",
                            &borrar_html, SI);

    /* --------------------------------------------------------------------- */
    g_enum_option_t *e_reg =
            g_opt_add_optional_enum(opt_ejec, "", "reg",
                                    "Indica cómo se deben visualizar los registros",
                                    (int *) PTR_OPTION(show_abi_reg), reg_auto);
    g_opt_set_parameter_name(e_reg, "mode");

    g_opt_add_enum_case(e_reg, "auto",
                        "Se muestra lo que haya utilizado el programador", reg_auto);
    g_opt_add_enum_case(e_reg, "arch",
                        "Se muestran los registros en formato arquitectónico (xNN y fNN)", reg_ar);
    g_opt_add_enum_case(e_reg, "abi",
                        "Se muestran los registros con los nombre definidos en el ABI", reg_abi);

    /* --------------------------------------------------------------------- */
    g_enum_option_t *e_format =
            g_opt_add_optional_enum(opt_ejec, "", "format",
                                    "Indica cómo se deben visualizar el formato de instrucción",
                                    (int *) PTR_OPTION(show_format), format_full);
    g_opt_set_parameter_name(e_format, "mode");

    g_opt_add_enum_case(e_format, "full",
                        "Se muestra el formato en binario y la descripción de los campos", format_full);
    g_opt_add_enum_case(e_format, "compact",
                        "Se muestran los campos con el código binario dentro", format_compact);
    g_opt_add_enum_case(e_format, "none",
                        "No se muestra el formato de las instrucciones", format_none);

    /* --------------------------------------------------------------------- */
    g_flag_option_t *f_show_ops =
            g_opt_add_optional_flag(opt_ejec, "", "show-ops",
                                    "Muestra las operaciones oper_pendientes en cada nivel de la jerarquía de memoria\n",
                                    PTR_FLAG(show_ops), NO);
    OPTION_HANDLER(f_show_ops) {
        if (!L1I.mem.activa && !L1D.mem.activa) {
            g_opt_error_option(&cmdline, "--show-ops", "La jerarquía de memoria debe estar activa (L1I y/o L1D)");
        }
    };

    /* --------------------------------------------------------------------- */
    // --- CONFIGURACIÓN -----------------------------------------------------
    /* --------------------------------------------------------------------- */
    opt_config = g_opt_add_section(&cmdline, "Configuración");

    /* --------------------------------------------------------------------- */
    g_flag_option_t *f_config =
            g_opt_add_optional_flag(opt_config, "c", "config",
                                    "Muestra la configuración y termina",
                                    NULL, NO);
    OPTION_HANDLER(f_config) {
        conecta_jerarquia(&cmdline);
        imprime_config();
        exit(1);
    };

    /* --------------------------------------------------------------------- */
    g_integer_option_t *n_rob =
            g_opt_add_optional_integer(opt_config, "r", "rob",
                                       "Número de entradas del Reorder Buffer",
                                       PTR_CONFIG(tam_reorder), INI_tam_reorder);
    OPTION_HANDLER(n_rob) {
        check_limit(&cmdline, CONFIG(tam_reorder), 1, MAX_REORDER, "--rob", "Tamaño del reorder buffer inválido");
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_vias =
            g_opt_add_optional_custom(opt_config, "v", "vias",
                                      "Número de vías del procesador superescalar en ISSUE (vi), BUSES (vb) y COMMIT (vc)\n"
                                      "Un valor 0 en cualquier campo indica que se mantiene el valor inicial del mismo",
                                      PTR_CONFIG(num_vias), &INI_num_vias, sizeof(num_vias_info_t)
            );
    g_opt_set_parameter_name(c_vias, "vi:vb:vc");
    OPTION_HANDLER(c_vias) {
        if (parametroM) {
            g_opt_error_option(&cmdline, "--vias",
                               "La opción '-v|--vias' debe especificarse antes que la opción '-M|--max'");
        }
        if (parametroL1I) {
            g_opt_error_option(&cmdline, "--vias",
                               "La opción '-v|--vias' debe especificarse antes que la opción '--l1i'");
        }

        np = sscanf(opts->param, "%d:%d:%d%n",
                    PTR_CONFIG(num_vias.issue), PTR_CONFIG(num_vias.bus), PTR_CONFIG(num_vias.commit), &lp);
        if (np != 3 || lp != strlen(opts->param)) {
            g_opt_error_option(&cmdline, "--vias", "Formato erróneo en el parámetro 'v | vias'");
        }

        CHECK_DEFAULT_CONFIG(num_vias.issue);
        CHECK_DEFAULT_CONFIG(num_vias.bus);
        CHECK_DEFAULT_CONFIG(num_vias.commit);

        check_limit(&cmdline, CONFIG(num_vias.issue), 1, CONFIG(num_vias.issue), "--vias",
                    "Número de vías en ISSUE inválido");
        check_limit(&cmdline, CONFIG(num_vias.bus), 1, CONFIG(num_vias.bus), "--vias",
                    "Número de vías en BUS inválido");
        check_limit(&cmdline, CONFIG(num_vias.commit), 1, CONFIG(num_vias.commit), "--vias",
                    "Número de vías en COMMIT inválido");
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_est =
            g_opt_add_optional_custom(opt_config, "e", "est",
                                      "Número de estaciones de reserva\n"
                                      "  [es]: Estaciones reserva de enteros/saltos\n"
                                      "  [sr]: Estaciones reserva de suma/resta/comparación\n"
                                      "  [md]: Estaciones reserva de multiplicación/división\n"
                                      "  [bl]: Buffers de lectura\n"
                                      "  [be]: Buffers de escritura\n"
                                      "Un valor 0 en cualquier campo indica que se mantiene el valor inicial del mismo",
                                      PTR_CONFIG(tam_rs), &INI_tam_rs, sizeof(tam_rs_info_t)
            );
    g_opt_set_parameter_name(c_est, "es:sr:md:bl:be");
    OPTION_HANDLER(c_est) {
        np = sscanf(opts->param, "%d:%d:%d:%d:%d%n",
                    PTR_CONFIG(tam_rs.enteros),
                    PTR_CONFIG(tam_rs.sumrest),
                    PTR_CONFIG(tam_rs.multdiv),
                    PTR_CONFIG(tam_rs.carga),
                    PTR_CONFIG(tam_rs.almacen),
                    &lp);
        if (np != 5 || lp != strlen(opts->param)) {
            g_opt_error_option(&cmdline, "--est", "Formato erróneo en el parámetro 'e | est'");
        }

        CHECK_DEFAULT_CONFIG(tam_rs.enteros);
        CHECK_DEFAULT_CONFIG(tam_rs.sumrest);
        CHECK_DEFAULT_CONFIG(tam_rs.multdiv);
        CHECK_DEFAULT_CONFIG(tam_rs.carga);
        CHECK_DEFAULT_CONFIG(tam_rs.almacen);
    };
    
    /* --------------------------------------------------------------------- */
    // OPERADORES
#define CHECK_OPERADOR(OPT, PARAM) do { \
    if (np != 3 || lp != strlen(opts->param)) { \
        g_opt_error_option(&cmdline, PARAM, "Formato erróneo en el parámetro '" OPT "'"); \
    }                                   \
    if (tipo_oper != 's' && tipo_oper != 'c') { \
        g_opt_error_option(&cmdline, PARAM, "Formato erróneo en el tipo del operador del parámetro '" OPT "'"); \
    }                                   \
} while (0)

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_int =
            g_opt_add_optional_custom(opt_config, "i", "int",
                                      "Número, latencia y tipo  de los operadores de enteros/saltos\n"
                                      "  [tipo]: (s)egmentado / (c)onvencional",
                                      PTR_CONFIG(enteros), &INI_enteros, sizeof(oper_info_t)
            );
    g_opt_set_parameter_name(c_int, "num:lat:tipo");
    OPTION_HANDLER(c_int) {
        char tipo_oper;

        np = sscanf(opts->param, "%d:%d:%c%n",
                    PTR_CONFIG(enteros.num_inicio), PTR_CONFIG(enteros.teval), &tipo_oper, &lp);
        CHECK_OPERADOR("i | int", "--int");

        CHECK_DEFAULT_CONFIG(enteros.num_inicio);
        CHECK_DEFAULT_CONFIG(enteros.teval);

        CONFIG(enteros.num_oper) = (tipo_oper == 'c') ? CONFIG(enteros.num_inicio) :
                                   CONFIG(enteros.num_inicio) * CONFIG(enteros.teval);
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_add =
            g_opt_add_optional_custom(opt_config, "a", "add",
                                      "Número, latencia y tipo de los operadores de suma/resta/comparación\n"
                                      "  [tipo]: (s)egmentado / (c)onvencional",
                                      PTR_CONFIG(sumrest), &INI_sumrest, sizeof(oper_info_t)
            );
    g_opt_set_parameter_name(c_add, "num:lat:tipo");
    OPTION_HANDLER(c_add) {
        char tipo_oper;

        np = sscanf(opts->param, "%d:%d:%c%n",
                    PTR_CONFIG(sumrest.num_inicio), PTR_CONFIG(sumrest.teval), &tipo_oper, &lp);
        CHECK_OPERADOR("a | add", "--add");

        CHECK_DEFAULT_CONFIG(sumrest.num_inicio);
        CHECK_DEFAULT_CONFIG(sumrest.teval);

        CONFIG(sumrest.num_oper) = (tipo_oper == 'c') ? CONFIG(sumrest.num_inicio) :
                                   CONFIG(sumrest.num_inicio) * CONFIG(sumrest.teval);
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_mult =
            g_opt_add_optional_custom(opt_config, "m", "mult",
                                      "Número, latencia y tipo de los operadores de multiplicación/división\n"
                                      "  [tipo]: (s)egmentado / (c)onvencional",
                                      PTR_CONFIG(multdiv), &INI_multdiv, sizeof(oper_info_t)
            );
    g_opt_set_parameter_name(c_mult, "num:lat:tipo");
    OPTION_HANDLER(c_mult) {
        char tipo_oper;

        np = sscanf(opts->param, "%d:%d:%c%n",
                    PTR_CONFIG(multdiv.num_inicio), PTR_CONFIG(multdiv.teval), &tipo_oper, &lp);
        CHECK_OPERADOR("m | mult", "--mult");

        CHECK_DEFAULT_CONFIG(multdiv.num_inicio);
        CHECK_DEFAULT_CONFIG(multdiv.teval);

        CONFIG(multdiv.num_oper) = (tipo_oper == 'c') ? CONFIG(multdiv.num_inicio) :
                                   CONFIG(multdiv.num_inicio) * CONFIG(multdiv.teval);
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_mem =
            g_opt_add_optional_custom(opt_config, "l", "mem",
                                      "Número, latencia y tipo de los operadores de memoria\n"
                                      "Describe el comportamiento de una L1D perfecta\n"
                                      "  [tipo]: (s)egmentado / (c)onvencional",
                                      PTR_CONFIG(memdatos), &INI_memdatos, sizeof(oper_info_t)
            );
    g_opt_set_parameter_name(c_mem, "num:lat:tipo");
    OPTION_HANDLER(c_mem) {
        char tipo_oper;

        np = sscanf(opts->param, "%d:%d:%c%n",
                    PTR_CONFIG(memdatos.num_inicio), PTR_CONFIG(memdatos.teval), &tipo_oper, &lp);
        CHECK_OPERADOR("l | mem", "--mem");

        CHECK_DEFAULT_CONFIG(memdatos.num_inicio);
        CHECK_DEFAULT_CONFIG(memdatos.teval);

        CONFIG(direcciones.num_inicio) = CONFIG(memdatos.num_inicio);
        // El tiempo de evaluación del cálculo de direcciones no se puede cambiar

        CONFIG(memdatos.num_oper) = (tipo_oper == 'c')
                                    ? CONFIG(memdatos.num_inicio)
                                    : CONFIG(memdatos.num_inicio) * CONFIG(memdatos.teval);
        CONFIG(direcciones.num_oper) = (tipo_oper == 'c')
                                       ? CONFIG(direcciones.num_inicio)
                                       : CONFIG(direcciones.num_inicio) * CONFIG(direcciones.teval);
    };

    /* --------------------------------------------------------------------- */
    g_enum_option_t *e_depmem =
            g_opt_add_optional_enum(opt_config, "d", "depmem",
                                    "Determina el mecanismo de resolución de dependencias en memoria",
                                    (int *) PTR_CONFIG(dep_mem), l_bypassing);
    g_opt_set_parameter_name(e_depmem, "tipo");
    g_opt_add_enum_case(e_depmem, "b", "Load bypassing", l_bypassing);
    g_opt_add_enum_case(e_depmem, "f", "Load forwarding", l_forwarding);

    /* --------------------------------------------------------------------- */
    g_flag_option_t *f_max =
            g_opt_add_optional_flag(opt_config, "M", "max",
                                    "Número de operadores y estaciones de reserva en función del número de vías",
                                    &parametroM, NO);
    OPTION_HANDLER(f_max) {
        // Máximo de operadores y RS
        SET_CONFIG(enteros.num_inicio, CONFIG(num_vias.issue));
        SET_CONFIG(enteros.num_oper, CONFIG(enteros.num_inicio));
        SET_CONFIG(tam_rs.enteros, CONFIG(num_vias.issue) * 32);

        SET_CONFIG(sumrest.num_inicio, CONFIG(num_vias.issue));
        SET_CONFIG(sumrest.num_oper, CONFIG(sumrest.num_inicio));
        SET_CONFIG(tam_rs.sumrest, CONFIG(num_vias.issue) * 16);

        SET_CONFIG(multdiv.num_inicio, CONFIG(num_vias.issue));
        SET_CONFIG(multdiv.num_oper, CONFIG(multdiv.num_inicio));
        SET_CONFIG(tam_rs.multdiv, CONFIG(num_vias.issue) * 16);

        SET_CONFIG(memdatos.num_inicio, CONFIG(num_vias.issue));
        SET_CONFIG(direcciones.num_inicio, CONFIG(num_vias.issue));
        SET_CONFIG(memdatos.num_oper, CONFIG(memdatos.num_inicio));
        SET_CONFIG(direcciones.num_oper, CONFIG(direcciones.num_inicio));
        SET_CONFIG(tam_rs.carga, CONFIG(num_vias.issue) * 16);
        SET_CONFIG(tam_rs.almacen, CONFIG(num_vias.issue) * 16);
    };

    /* --------------------------------------------------------------------- */

    g_enum_option_t *e_predic =
            g_opt_add_optional_enum(opt_config, "p", "predic",
                                    "Tipo de predictor",
                                    (int *) PTR_CONFIG(predic.tipo_predictor), BTB_1_BIT);
    g_opt_set_parameter_name(e_predic, "tipo");
    g_opt_add_enum_case(e_predic, "1", "BTB 1 bit", BTB_1_BIT);
    g_opt_add_enum_case(e_predic, "2h", "BTB 2 bits con histéresis", BTB_2_BIT_H);
    g_opt_add_enum_case(e_predic, "2s", "BTB 2 bits con saturación", BTB_2_BIT_S);
    g_opt_add_enum_case(e_predic, "p", "Perfecto", PERFECTO);
    g_opt_add_enum_case(e_predic, "c", "BTB clarividente", CLARIVIDENTE);
    g_opt_add_enum_case(e_predic, "pnt", "Predict-not-taken", PREDICT_NOT_TAKEN);

    /* --------------------------------------------------------------------- */
    g_integer_option_t *n_btb =
            g_opt_add_optional_integer(opt_config, "b", "btb",
                                       "Número de entradas del BTB",
                                       PTR_CONFIG(predic.tam_buffer_predic), INI_tam_buffer_predic);
    OPTION_HANDLER(n_btb) {
        check_limit(&cmdline, CONFIG(predic.tam_buffer_predic), 1, MAX_BUFFER_PREDIC, "--rob",
                    "Tamaño del BTB inválido");
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_2level =
            g_opt_add_optional_custom(opt_config, "L", "2level",
                                      "Predictor de 2 niveles. Fmt: <bp:tipo:num:b-BHSR:n-BHSR>\n"
                                      "  [bp]:      Bits del predictor\n"
                                      "  [tipo]:    (h)isteresis / (s)aturación\n"
                                      "  [num]:     Número entradas del predictor\n"
                                      "  [b-BHSR]:  Bits del BHSR       [n-BHSR]:  Número de BHSRs\n",
                                      PTR_CONFIG(predic), &INI_config_predic, sizeof(predic_config_t));
    g_opt_set_parameter_name(c_2level, "...");
    OPTION_HANDLER(c_2level) {
        np = sscanf(opts->param, "%d:%c:%d:%d:%d%n",
                    PTR_CONFIG(predic.n_bits_pred_local), PTR_CONFIG(predic.subtipo_pred_local),
                    PTR_CONFIG(predic.tam_buffer_predic),
                    PTR_CONFIG(predic.n_bits_BHSR1), PTR_CONFIG(predic.tam_tabla_BHSR1),
                    &lp);
        if (np != 5 || lp != strlen(opts->param)) {
            g_opt_error_option(&cmdline, "--2level", "Formato erróneo en el parámetro 'L | 2level'");
        }

        check_limit(&cmdline, CONFIG(predic.n_bits_pred_local), 0, MAX_BITS_PRED_LOCAL, "--2level",
                    "Número de bits del predictor inválido");

        if (CONFIG(predic.subtipo_pred_local) == 's') {
            CONFIG(predic.tipo_predictor) = TWO_LEVEL_n_BIT_S;
        } else if (CONFIG(predic.subtipo_pred_local) == 'h') {
            CONFIG(predic.tipo_predictor) = TWO_LEVEL_n_BIT_H;
        } else {
            g_opt_error_option(&cmdline, "--2level", "Formato erróneo en el subtipo de predictor (h ó s)");
        }

        check_limit(&cmdline, CONFIG(predic.tam_buffer_predic), 0, MAX_BUFFER_PREDIC, "--2level",
                    "Tamaño del BTB inválido");
        check_limit_pow_2(&cmdline, CONFIG(predic.tam_tabla_BHSR1), 1, MAX_TABLA_BHSR, "--2level",
                          "Tamaño de la tabla BHSR inválido");
        check_limit(&cmdline, CONFIG(predic.n_bits_BHSR1), 0, MAX_BITS_BHSR, "--2level", "Num bits BHSR inválido");

    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_hybrid =
            g_opt_add_optional_custom(opt_config, "H", "hybrid",
                                      "Predictor híbrido. Fmt: <bs:bp:tipo:num:b-BHSR1:n-BHSR1:b-BHSR2:n-BHSR2>\n"
                                      "  [bs]:      Bits del selector   [bp]:      Bits del predictor\n"
                                      "  [tipo]:    (h)isteresis / (s)aturación\n"
                                      "  [num]:     Número entradas del predictor\n"
                                      "  [b-BHSR1]: Bits del BHSR 1     [n-BHSR1]: Número de BHSRs 1\n"
                                      "  [b-BHSR2]: Bits del BHSR 2     [n-BHSR2]: Número de BHSRs 2\n",
                                      PTR_CONFIG(predic), &INI_config_predic, sizeof(predic_config_t));
    g_opt_set_parameter_name(c_hybrid, "...");
    OPTION_HANDLER(c_hybrid) {
        np = sscanf(opts->param, "%d:%d:%c:%d:%d:%d:%d:%d%n",
                    PTR_CONFIG(predic.n_bits_selector), PTR_CONFIG(predic.n_bits_pred_local),
                    PTR_CONFIG(predic.subtipo_pred_local), PTR_CONFIG(predic.tam_buffer_predic),
                    PTR_CONFIG(predic.n_bits_BHSR1), PTR_CONFIG(predic.tam_tabla_BHSR1),
                    PTR_CONFIG(predic.n_bits_BHSR2), PTR_CONFIG(predic.tam_tabla_BHSR2),
                    &lp);
        if (np != 8 || lp != strlen(opts->param)) {
            g_opt_error_option(&cmdline, "--hybrid", "Formato erróneo en el parámetro 'H | hybrid'");
        }

        check_limit(&cmdline, CONFIG(predic.n_bits_selector), 1, MAX_BITS_PRED_LOCAL, "--hybrid",
                    "Número de bits del selector inválido");
        check_limit(&cmdline, CONFIG(predic.n_bits_pred_local), 0, MAX_BITS_PRED_LOCAL, "--hybrid",
                    "Número de bits del predictor inválido");

        if (CONFIG(predic.subtipo_pred_hybrid) == 's') {
            CONFIG(predic.tipo_predictor) = HYBRID_n_BIT_S;
        } else if (CONFIG(predic.subtipo_pred_hybrid) == 'h') {
            CONFIG(predic.tipo_predictor) = HYBRID_n_BIT_H;
        } else {
            g_opt_error_option(&cmdline, "--hybrid", "Formato erróneo en el subtipo de predictor (h ó s)");
        }

        check_limit(&cmdline, CONFIG(predic.tam_buffer_predic), 0, MAX_BUFFER_PREDIC, "--hybrid",
                    "Tamaño del BTB inválido");
        check_limit_pow_2(&cmdline, CONFIG(predic.tam_tabla_BHSR1), 1, MAX_TABLA_BHSR, "--hybrid",
                          "Tamaño de la tabla BHSR1 inválido");
        check_limit(&cmdline, CONFIG(predic.n_bits_BHSR1), 0, MAX_BITS_BHSR, "--hybrid", "Num bits BHSR1 inválido");
        check_limit_pow_2(&cmdline, CONFIG(predic.tam_tabla_BHSR2), 1, MAX_TABLA_BHSR, "--hybrid",
                          "Tamaño de la tabla BHSR2 inválido");
        check_limit(&cmdline, CONFIG(predic.n_bits_BHSR2), 0, MAX_BITS_BHSR, "--hybrid", "Num bits BHSR2 inválido");

        CONFIG(predic.tipo_predictor) = HYBRID_n_BIT_S;
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_mem_ops =
            g_opt_add_optional_custom(opt_config, "", "mem-ops",
                                      "Configuración del buffer de operaciones de un nivel\n"
                                      "  [lvl]: Nivel de la jerarquía: L1D, L2 o MP\n"
                                      "  [no]:  Número máximo de operaciones activas en este nivel\n"
                                      "  [ac]:  Número máximo de accesos activos en este nivel\n"
                                      "  [in]:  Número máximo de operaciones aceptadas por ciclo\n"
                                      "La configuración de L1I es automática en función del número de vías\n",
                                      NULL, NULL, 0
            );
    g_opt_set_parameter_name(c_mem_ops, "lvl:no:ac:in");
    OPTION_HANDLER(c_mem_ops) {
        char nivel[8];
        config_operaciones_t value;
        np = sscanf(opts->param, "%[L1D2MP]:%hhd:%hhd:%hhd%n",
                    nivel, &value.max_oper, &value.max_oper_acceso, &value.max_oper_ciclo,
                    &lp);
        if (np != 4 || lp != strlen(opts->param)) {
            g_opt_error_option(&cmdline, "--mem-ops", "Formato erróneo en el parámetro 'mem-ops' (%d)", np);
        }

#define SET_OPER_CONFIG(NIVEL, OPT, VALUE) do { \
    if (NIVEL ## _oper_config_used) {           \
        g_opt_error_option(&cmdline, "--mem-ops",\
                           "La opción '--mem-ops' debe especificarse antes de la opción '--" # OPT "'"); \
    } else if (NIVEL ## _oper_config_set) {     \
        g_opt_error_option(&cmdline, "--mem-ops",\
                           "La opción '--mem-ops' para '" # NIVEL "' sólo puede especificarse una vez"); \
    } else {                                    \
        NIVEL ## _oper_config = (VALUE);        \
        NIVEL ## _oper_config_set = SI;         \
    }                                           \
} while (0)

        if (strcmp(nivel, "L1D") == 0) {
            SET_OPER_CONFIG(L1D, l1d, value);
        } else if (strcmp(nivel, "L2") == 0) {
            SET_OPER_CONFIG(L2, l2, value);
        } else if (strcmp(nivel, "MP") == 0) {
            SET_OPER_CONFIG(MP, mp, value);
        } else {
            g_opt_error_option(&cmdline, "--mem-ops", "Nivel desconocido en el parámetro 'mem-ops'");
        }
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_l1i =
            g_opt_add_optional_custom(opt_config, "", "l1i",
                                      "Configuración del nivel de cache L1 instrucciones\n"
                                      "  [nb]:  Número de bloques       [tb]: Tamaño de bloque\n"
                                      "  [ga]:  Grado asociatividad. 0 completamente asociativa\n",
                                      PTR_CONFIG(l1i_config), &INI_l1i, sizeof(config_cache_t)
            );
    g_opt_set_parameter_name(c_l1i, "nb:tb:ga");
    OPTION_HANDLER(c_l1i) {
        SET_CONFIG(l1i_config.latencia, 1); // La latencia de L1I sólo puede ser 1
        np = sscanf(opts->param, "%d:%hd:%d%n",
                    PTR_CONFIG(l1i_config.num_bloques), PTR_CONFIG(l1i_config.tam_bloque),
                    PTR_CONFIG(l1i_config.grado_asoc),
                    &lp);
        if (np != 3 || lp != strlen(opts->param)) {
            g_opt_error_option(&cmdline, "--l1i", "Formato erróneo en el parámetro 'l1i'");
        }
        comprueba_param_cache(&cmdline, CONFIG(l1i_config), "--l1i", "L1I");

        if (CONFIG(l1i_config.tam_bloque) % (CONFIG(num_vias.issue) * sizeof(word)) != 0) {
            g_opt_error_option(&cmdline, "--l1i", "Geometría de L1I incompatible con el número de vías");
        }

        inicializa_memoria_cache(&L1I, CONFIG(l1i_config), L1I_oper_config);
        L1I_oper_config_used = SI;
        parametroL1I = SI;
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_l1d =
            g_opt_add_optional_custom(opt_config, "", "l1d",
                                      "Configuración del nivel de cache L1 datos\n"
                                      "  [nb]:  Número de bloques       [tb]: Tamaño de bloque\n"
                                      "  [ga]:  Grado asociatividad. 0 completamente asociativa\n"
                                      "  [lat]: Latencia en ciclos\n",
                                      PTR_CONFIG(l1d_config), &INI_l1d, sizeof(config_cache_t)
            );
    g_opt_set_parameter_name(c_l1d, "nb:tb:ga:lat");
    OPTION_HANDLER(c_l1d) {
        np = sscanf(opts->param, "%d:%hd:%d:%hhd%n",
                    PTR_CONFIG(l1d_config.num_bloques), PTR_CONFIG(l1d_config.tam_bloque),
                    PTR_CONFIG(l1d_config.grado_asoc),
                    PTR_CONFIG(l1d_config.latencia),
                    &lp);
        if (np != 4 || lp != strlen(opts->param)) {
            g_opt_error_option(&cmdline, "--l1d", "Formato erróneo en el parámetro 'l1d'");
        }
        comprueba_param_cache(&cmdline, CONFIG(l1d_config), "--l1d", "L1D");

        inicializa_memoria_cache(&L1D, CONFIG(l1d_config), L1D_oper_config);
        L1D_oper_config_used = SI;
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_l2 =
            g_opt_add_optional_custom(opt_config, "", "l2",
                                      "Configuración del nivel de cache L2 unificada\n"
                                      "  [nb]:  Número de bloques       [tb]: Tamaño de bloque\n"
                                      "  [ga]:  Grado asociatividad. 0 completamente asociativa\n"
                                      "  [lat]: Latencia en ciclos\n",
                                      PTR_CONFIG(l2_config), &INI_l2, sizeof(config_cache_t)
            );
    g_opt_set_parameter_name(c_l2, "nb:tb:ga:lat");
    OPTION_HANDLER(c_l2) {
        np = sscanf(opts->param, "%d:%hd:%d:%hhd%n",
                    PTR_CONFIG(l2_config.num_bloques), PTR_CONFIG(l2_config.tam_bloque),
                    PTR_CONFIG(l2_config.grado_asoc),
                    PTR_CONFIG(l2_config.latencia),
                    &lp);
        if (np != 4 || lp != strlen(opts->param)) {
            g_opt_error_option(&cmdline, "--l2", "Formato erróneo en el parámetro 'l2'");
        }
        if (!c_l1i->common.provided && !c_l1d->common.provided) {
            g_opt_error_option(&cmdline, "--l2",
                               "Antes de la opción '--l2' debe especificar las opciones '--l1i' o '--l1d'");
        }

        comprueba_param_cache(&cmdline, CONFIG(l2_config), "--l2", "L2");

        if (CONFIG(l1i_config.tam_bloque) > CONFIG(l2_config.tam_bloque)) {
            g_opt_error_option(&cmdline, "--l2", "Tamaño de bloque de L2 incompatible con L1I");
        }

        if (CONFIG(l1d_config.tam_bloque) > CONFIG(l2_config.tam_bloque)) {
            g_opt_error_option(&cmdline, "--l2", "Tamaño de bloque de L2 incompatible con L1D");
        }

        inicializa_memoria_cache(&L2, CONFIG(l2_config), L2_oper_config);
        L2_oper_config_used = SI;
    };

    /* --------------------------------------------------------------------- */
    g_custom_option_t *c_mp =
            g_opt_add_optional_custom(opt_config, "", "mp",
                                      "Configuración de la memoria principal. Fmt: <tam:nb:tb:CL:tRCD:tRP:BW>\n"
                                      "  [tam]: Tamaño de la memoria principal\n"
                                      "  [nb]:  Número de bancos [tb]:   Tamaño del buffer de fila\n"
                                      "  [CL]:  CAS Latency      [tRCD]: RAS to CAS delay   [tRP]: RAS Precharge\n"
                                      "  [BW]:  Ancho de banda en bytes/ciclo\n",
                                      PTR_CONFIG(mp_config), &INI_mp, sizeof(config_memoria_principal_t)
            );
    g_opt_set_parameter_name(c_mp, "...");
    OPTION_HANDLER(c_mp) {
        np = sscanf(opts->param, "%d:%hd:%hd:%hhd:%hhd:%hhd:%hhd%n",
                    PTR_CONFIG(mp_config.tam_memoria),
                    PTR_CONFIG(mp_config.num_bancos), PTR_CONFIG(mp_config.tam_banco),
                    PTR_CONFIG(mp_config.CL), PTR_CONFIG(mp_config.tRCD), PTR_CONFIG(mp_config.tRP),
                    PTR_CONFIG(mp_config.BW),
                    &lp);
        if (np != 7 || lp != strlen(opts->param)) {
            g_opt_error_option(&cmdline, "--mp", "Formato erróneo en el parámetro 'mp'");
        }
        if (!c_l1i->common.provided && !c_l1d->common.provided) {
            g_opt_error_option(&cmdline, "--mp",
                               "Antes de la opción '--mp' debe especificar las opciones '--l1i' o '--l1d'");
        }
        if (CONFIG(l1i_config.tam_bloque) > CONFIG(mp_config.tam_banco)) {
            g_opt_error_option(&cmdline, "--mp", "Tamaño del buffer de MP incompatible con L1I");
        }
        if (CONFIG(l1d_config.tam_bloque) > CONFIG(mp_config.tam_banco)) {
            g_opt_error_option(&cmdline, "--mp", "Tamaño del buffer de MP incompatible con L1D");
        }
        if (c_l2->common.provided && CONFIG(l2_config.tam_bloque) > CONFIG(mp_config.tam_banco)) {
            g_opt_error_option(&cmdline, "--mp", "Tamaño del buffer de MP incompatible con L2");
        }

        check_positive_pow_2(&cmdline, CONFIG(mp_config.tam_memoria), "--mp", "Tamaño de la memoria inválido");
        check_positive_pow_2(&cmdline, CONFIG(mp_config.num_bancos), "--mp", "Número de bancos inválido");
        check_positive_pow_2(&cmdline, CONFIG(mp_config.tam_banco), "--mp", "Tamaño del buffer de fila inválido");
        check_positive(&cmdline, CONFIG(mp_config.CL), "--mp", "CAS Latency inválido");
        check_positive(&cmdline, CONFIG(mp_config.tRCD), "--mp", "RAS to CAS delay inválido");
        check_positive(&cmdline, CONFIG(mp_config.tRP), "--mp", "RAS Precharge inválido");
        check_positive_pow_2(&cmdline, CONFIG(mp_config.BW), "--mp", "Ancho de banda inválido");

        if (comprueba_config_memoria_principal(CONFIG(mp_config))) {
            g_opt_error_option(&cmdline, "--mp", "Geometría de memoria principal incorrecta");
        }

        inicializa_memoria_principal(&MP, CONFIG(mp_config), MP_oper_config);
        MP_oper_config_used = SI;
    };

    /* --------------------------------------------------------------------- */
    // Configuración por omisión
    set_default_config();
    presentacion = presentacion_html;
    SET_OPTION(formato_presentacion, formato_html);
    h_set_compact_mode(NO); // Un valor SI para compactar y acelerar la escritura de HTML en el estado de la memoria
    nombre_fich[0] = '\0';
    
    /*** Lectura de parámetros */

    // Analiza las opciones suministradas
    g_opt_parse(&cmdline, 1);

    // Comprobaciones adicionales
    if (L1D_oper_config_set && !L1D_oper_config_used) {
      g_opt_error_option(&cmdline, "--l1d", "Se han configurado las operaciones del nivel 'L1D' con la opción '--mem-ops', "
			 "pero el nivel 'L1D' no está activo");
    }
    
    if (L2_oper_config_set && !L2_oper_config_used) {
      g_opt_error_option(&cmdline, "--l2", "Se han configurado las operaciones del nivel 'L2' con la opción '--mem-ops', "
			 "pero el nivel 'L2' no está activo");
    }

    if (L1D_oper_config_set && !L1D_oper_config_used) {
      g_opt_error_option(&cmdline, "--mp", "Se han configurado las operaciones del nivel 'MP' con la opción '--mem-ops', "
			 "pero el nivel 'MP' no está activo");
    }

    strcpy(nombre_fich, g_str_string(&str_ficheros));

#ifdef LIBRISCV
    /*** Corrige configuracion si se usa libriscv */

    FLAG(depuracion) = SI;
    //FLAG(etiquetas) = SI;
    borrar_html = NO;
    html_merge = SI;
    if (devuelve_json) {
      presentacion = presentacion_json;
      SET_OPTION(formato_presentacion, formato_json);
    }
#endif

    /*** Conecta los elementos de la jerarquía de memoria activos */

    conecta_jerarquia(&cmdline);

    /*** Borrar html */

    if (borrar_html) {
        system("rm *.html 2>/dev/null");
    }

    /* Si el predictor es perfecto, tenemos que obtener la traza */
    if ((CONFIG(predic.tipo_predictor) == PERFECTO) || (CONFIG(predic.tipo_predictor) == CLARIVIDENTE)) {
        fprintf(stdout, "*** Generando traza para predictor perfecto/clarividente ... ***\n");

        fase_ejecucion_pred = TRAZA;
        f_trace = fopen(trace_filename, "w");

        /*** Inicialización del programa */

        inicializa_programa(&programa, num_ficheros, ficheros_entrada);

        /*** Inicialización de estructuras */

        inicializa_estructuras();

        /*** Carga el código en lenguaje ensamblador */

        construye_programa(&programa);

        /*** Llama al bucle principal */

        ejecutar_codigo();

        /* Esta línea la leerá */
        fprintf(f_trace, "%d %d %d %d\n", -1, -1, -1, -1);
        /* Esta ya no debe leerse */
        fprintf(f_trace, "%d %d %d %d\n", -2, -2, -2, -2);
        fclose(f_trace);
        fase_ejecucion_pred = FINAL;

        fprintf(stdout, "*** Traza generada... ***\n");

        system("rm *.html 2>/dev/null");
    }

    if ((CONFIG(predic.tipo_predictor) == PERFECTO) || (CONFIG(predic.tipo_predictor) == CLARIVIDENTE)) {
        f_trace = fopen(trace_filename, "r");
    }

    /*** Inicialización del programa */

    inicializa_programa(&programa, num_ficheros, ficheros_entrada);

    /*** Inicialización de estructuras */

    inicializa_estructuras();

    /*** Imprime configuración */

    imprime_config();

    /*** Carga el código en lenguaje ensamblador */

    construye_programa(&programa);

    /*** Imprime mapa de memoria */

    imprime_mapa_memoria();

    /*** Llama al bucle principal */

    ejecutar_codigo();

    if (FLAG(firmas)) {
        cierra_firma();
    }

    /*** Vuelca el contenido de la memoria ***/
    if (FLAG(dump)) {
        vuelca_memoria(&programa);
    }

    /*** Destruye los objetos que conformaban el programa */

    destruye_objetos_programa(&programa);

    /*** Imprime estadísticas */

    imprime_estadisticas();

    if ((CONFIG(predic.tipo_predictor) == PERFECTO) || (CONFIG(predic.tipo_predictor) == CLARIVIDENTE)) {
        fclose(f_trace);
    }

#ifndef LIBRISCV

    return RC;

#else

    /*** libriscv_main devuelve una cadena con el contenido del fichero .htm o .json de salida */

    FILE* f_salida = fopen(devuelve_json ? "data.js" : "index.htm", "r");
    long file_size;
    char* result;

    fseek(f_salida, 0, SEEK_END);
    file_size = ftell(f_salida);
    rewind(f_salida);

    result = malloc(file_size + 1);
    fread(result, file_size, 1, f_salida);
    result[file_size] = 0;

    fclose(f_salida);

    /*** Libera argv (se construyó al inicio) */
    free(argv);

    return result;

#endif /* LIBRISCV */

}

