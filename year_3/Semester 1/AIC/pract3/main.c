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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>
#include <sys/stat.h>



/*** Definiciones Externas *************************************************/

#include "main.h"
#include "tipos.h"
#include "instrucciones.h"
#include "presentacion-txt.h"
#include "presentacion-html.h"
#include "riscv.h"
#include "riscv_fp.h"
#include "memoria.h"
#include "log.h"

/*** Definiciones **********************************************************/

#define SIMUL_NAME      "RiscV-m"
#define SIMUL_VERSION   "1.1"
#define SIMUL_DATE      "2025-10-01"

extern void ensamblador(fichero_objeto_t *obj);


extern int pos_datos;

#define init_ID_x(operador) \
    ID##_##operador.IR = inst_vacia; \
    ID##_##operador.iPC = -1; \
    ID##_##operador.orden = -1; \
    ID##_##operador##n = ID_##operador \

#define init_OP_x(operador) \
   for (i = 1; i <= operador##_LAT; i++) { \
        operador[i].IR = inst_vacia; \
        operador[i].iPC = -1; \
        operador[i].orden = -1; \
        operador##n[i] = operador [i]; \
    }

#define init_x_WB(operador) \
    operador##_WB.IR = inst_vacia; \
    operador##_WB.iPC = -1; \
    operador##_WB.orden = -1; \
    operador##_WB##n = operador##_WB; \

#define init_RegXY(reg) \
    reg.IR = inst_vacia; \
    reg.iPC = -1; \
    reg.orden = -1;

#define do_NO_SEGM_OP(tipo, operador) \
    ID_##tipo##_##operador = ID_##tipo##_##operador##n; \
    for (i = 1; i <= tipo##_##operador##_LAT; i++) { \
         fase_##tipo##_##operador(i);                    \
         if (tipo_procesador==no_segm_multiciclo)  { \
            actualiza_crono_estado();    \
            Ciclo++; \
         } \
         if (i<= tipo##_##operador##_LAT) \
             tipo##_##operador[i+1] = tipo##_##operador##n[i+1]; \
    }   \
    tipo##_##operador##_WB = tipo##_##operador##_WBn;   \

#define do_SEGM_OP(tipo, operador) \
    i = tipo##_##operador##_LAT; \
    while (i > 0) { \
        fase_##tipo##_##operador(i); \
        i = i - 1; \
    } \

/*** Funciones Locales *****************************************************/

/**
 * Inicializa registros intermedios del procesador
 *
 */
static void inicializa_registros_intermedios(
) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int i;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /* Instruccion NOP*/
    //inst_vacia.codop = PSEUDO_NOP;
    inst_vacia.codop = RV32I_ADDI;
    inst_vacia.tipo = FormatoI;

    inst_vacia.rs1 = 0; /* Índice del registro fuente 1 */
    inst_vacia.rs2 = 0; /* Índice del registro fuente 2 */
    inst_vacia.rs3 = 0; /* Índice del registro fuente 3 */
    inst_vacia.rm = 0; /* Índice del modo de rendondeo */
    inst_vacia.rd = 0; /* Índice del registro destino */
    inst_vacia.imm = 0; /* Valor Inmediato. RiscV R32I usa hasta 20 bits */

    //inst_vacia.PC = -1; /* Inst. inválida es -1 */
    inst_vacia.valor = 0; /* Codificacion */

    sprintf(inst_vacia.etiqueta, "%s", ""); /* Posible etiqueta */

    /* Registros intermedios ud 5 etapas */
    IF_ID.IR = inst_vacia;
    IF_ID.iPC = -1;
    IF_ID.orden = -1;
    IF_ID.NPC = 0;
    IF_IDn = IF_ID;

    ID_EX.IR = inst_vacia;
    ID_EX.iPC = -1;
    ID_EX.orden = -1;
    ID_EX.NPC = 0;
    ID_EX.Ra = 0;
    ID_EX.Rb = 0;
    ID_EX.Imm = 0;
    ID_EXn = ID_EX;

    EX_MEM.IR = inst_vacia;
    EX_MEM.iPC = -1;
    EX_MEM.orden = -1;
    EX_MEM.ALUout = 0;
    EX_MEM.data = 0;
    EX_MEM.cond = NO;
    EX_MEMn = EX_MEM;

    MEM_WB.IR = inst_vacia;
    MEM_WB.iPC = -1;
    MEM_WB.orden = -1;
    MEM_WB.ALUout = 0;
    MEM_WB.MEMout = 0;
    MEM_WBn = MEM_WB;

    /* Señales */
    IFstall = NO;
    IDstall = NO;
    EXstall = NO;
    IFnop = NO;
    IDnop = NO;
    EXnop = NO;

    /* Registros intermedios ID/operadores multiciclo */
    init_ID_x(INT_MUL);
    init_ID_x(FP_LS);
    init_ID_x(FP_ADD);
    init_ID_x(FP_MUL);
    init_ID_x(FP_CMP);
    init_ID_x(FP_MISC);
    init_ID_x(FP_CVT);


    /* Registros intermedios operadores segmentados ud multiciclo */
    init_OP_x(FP_LS);
    init_OP_x(FP_ADD);
    init_OP_x(FP_MUL);
    init_OP_x(FP_CMP);
    init_OP_x(FP_MISC);
    init_OP_x(FP_CVT);
    init_OP_x(INT_MUL);


    /* Registros intermedios operadores multiciclo/WB */
    init_x_WB(FP_LS);
    init_x_WB(FP_ADD);
    init_x_WB(FP_MUL);
    init_x_WB(FP_CMP);
    init_x_WB(FP_MISC);
    init_x_WB(FP_CVT);
    init_x_WB(INT_MUL);

    /* mux a la entrada FP regs */
    init_RegXY(FP_WB);

    /* mux a la entrada INT regs */
    init_RegXY(iWB);

} /* end inicializa_registros_intermedios */

/**
 * Inicializa registros de uso general
 *
 */
static void inicializa_bancos_registros(
) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int i;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/


    PC = 0;

    /* Registros de uso general */
    for (i = 0; i < TAM_REGISTROS; i++) // +1 por el FPSR
    {
        Rint[i].valor.int_d = 0;
        Rint[i].busy = 0;
        Rint[i].dirty = NO;
        Rint[i].tipo = t_ninguno;
    } /* endfor */

    for (i = 0; i < TAM_REGISTROS; i++) // +1 por el FPSR
    {
        Rfp[i].valor.fp_d = 0;
        Rfp[i].busy = 0;
        Rfp[i].dirty = NO;
        Rfp[i].tipo = t_ninguno;
    } /* endfor */
}

/**
 * Inicializa registros de uso general y registros intermedios
 *
 */
static void inicializa_estructuras(
) {
    inicializa_bancos_registros();
    inicializa_registros_intermedios();
}

static boolean OVF_HTML = NO;

/**
 * Actualiza crono y estado
 *
 */
static void actualiza_crono_estado(
) {
        if ((salida == html) && (!OVF_HTML)) {
            actualiza_crono();
            imprime_css();
        }
}



static const char *
simul_logo() {
    return
            "    ____  _          _    __\n"
            "   / __ \\(_)________| |  / /           ____ ___\n"
            "  / /_/ / / ___/ ___/ | / /  ______   / __ `__ \\\n"
            " / _, _/ (__  ) /__ | |/ /  /_____/  / / / / / /\n"
            "/_/ |_/_/____/\\___/ |___/           /_/ /_/ /_/\n";
}


static const char *show_usage() {
    return
            YELLOW_TEXT
            "Uso:\n"
            COLOR_RESET
            "\triscv-m [options] {-f input.s}+\n"
            YELLOW_TEXT
            "\nOpciones:\n"
            COLOR_RESET
            GREEN_TEXT
            "\t-s resultado <modo>\n"
            COLOR_RESET
            "\t\tEspecifíca cómo se deben visualizar el resultado (def. html).\n"
            "\t\t   tiempo: Se muestra sólo el tiempo de ejecución en la consola.\n"
            "\t\t   final: Se muestra el tiempo de ejecución y estado en la consola.\n"
            "\t\t   html(*): Se genera un archivo html por ciclo.\n "
            "\t\t   html-final: Se genera un único archivo html con el resultado.\n\n"
            GREEN_TEXT
            "\t-p tipo procesador <tipo>\n"
            COLOR_RESET
            "\t\tEspecifíca el tipo de procesador (def. segm).\n"
            "\t\t   segm(*): Segmentado.\n"
            "\t\t   no-segm-mono: No segmentado. Ud. de control monociclo.\n"
            "\t\t   no-segm-multi: No segmentado. Ud. de control multiciclo.\n "
            GREEN_TEXT
            "\t-d r.datos <modo>\n"
            COLOR_RESET
            "\t\tEspecifíca cómo se resuelven los riesgos de datos (def. c).\n"
            "\t\t   n: No se resuelven\n"
            "\t\t   p: Se insertan ciclos parada\n"
            "\t\t   c(*): Se utilizan cortocircuitos\n"
            GREEN_TEXT
            "\t-c r. control <modo>\n"
            COLOR_RESET
            "\t\tEspecifíca cómo se resuelven los riesgos de control (def. pnt3).\n"
            "\t\t   n3: No se resuelven (latencia salto=3)\n"
            "\t\t   n2: No se resuelven (latencia salto=2)\n"
            "\t\t   n1: No se resuelven (latencia salto=1)\n"
            "\t\t   s3: Se insertan ciclos de parada (latencia salto=3)\n"
            "\t\t   s2: Se insertan ciclos de parada (latencia salto=2)\n"
            "\t\t   s1: Se insertan ciclos de parada (latencia salto=1)\n"
            "\t\t   pnt3(*): Se utiliza predict-not-taken (latencia salto=3)\n"
            "\t\t   pnt2: Se utiliza predict-not-taken (latencia salto=2)\n"
            "\t\t   pnt1: Se utiliza predict-not-taken (latencia salto=1)\n"
            GREEN_TEXT
            "\t-l <lat>\n"
            COLOR_RESET
            "\t\tEspecifica la latencia del operador de LS FP(def. 2)\n"
            GREEN_TEXT
            "\t-a <lat>\n"
            COLOR_RESET
            "\t\tEspecifica la latencia del operador ADD FP (def. 4)\n"
            GREEN_TEXT
            "\t-m <lat>\n"
            COLOR_RESET
            "\t\tEspecifica la latencia del operador MUL INT/FP(def. 7)\n"
            GREEN_TEXT
            "\t--cmp <lat>\n"
            COLOR_RESET
            "\t\tEspecifica la latencia del operador CMP FP(def. 4)\n"
            GREEN_TEXT
            "\t--cvt <lat>\n"
            COLOR_RESET
            "\t\tEspecifica la latencia del operador CVT (INT->FP) (def. 4)\n"
            GREEN_TEXT
            "\t--misc <lat>\n"
            COLOR_RESET
            "\t\tEspecifica la latencia del operador MISC (FP->INT) (def. 4)\n"
            GREEN_TEXT
            "\t-j\n"
            COLOR_RESET
            "\t\tGenera un único archivo htm con javascript\n"
            GREEN_TEXT
            "\t--opt-int\n"
            COLOR_RESET
            "\t\tElimina WB en STORE INT\n"
            GREEN_TEXT
            "\t--reg <modo>\n"
            COLOR_RESET
            "\t\tEspecifíca cómo se deben visualizar los registros (def. auto).\n"
            "\t\t   auto(*): Se muestra lo que haya utilizado el programador.\n"
            "\t\t   ar: Se muestran los registros en formato arquitectónico (xNN y fNN).\n"
            "\t\t   abi: Se muestran los registros con los nombre definidos en el ABI.\n"
            GREEN_TEXT
            "\t--format <modo>\n"
            COLOR_RESET
            "\t\tIndica cómo se deben visualizar el formato de instrucción (def. full).\n"
            "\t\t   full(*): Se muestra el formato en binario y la descripción de los campos.\n"
            "\t\t   compact: Se muestran los campos con el código binario dentro.\n"
            "\t\t   none: No se muestra el formato de las instrucciones.\n"
            GREEN_TEXT
            "\t--no-show-IT\n"
            COLOR_RESET
            "\t\tNo muestra cronograma.\n"
            GREEN_TEXT
            "\t--no-show-DP\n"
            COLOR_RESET
            "\t\tNo muestra ruta de datos.\n"
            GREEN_TEXT
            "\t--no-show-R\n"
            COLOR_RESET
            "\t\tNo muestra registros.\n"
            GREEN_TEXT
            "\t--no-show-M\n"
            COLOR_RESET
            "\t\tNo muestra memoria.\n"
            GREEN_TEXT
            "\t--no-show-LOG\n"
            COLOR_RESET
            "\t\tNo muestra LOG.\n"
            GREEN_TEXT
            "\t--no-show-C\n"
            COLOR_RESET
            "\t\tNo muestra CONSOLA.\n"
            GREEN_TEXT
            "\t-n\n"
            COLOR_RESET
            "\t\tNo borra archivos html antes de la ejecución.\n"
            GREEN_TEXT
            "\t-f <fichero>\n"
            COLOR_RESET
            "\t\tEspecifica el nombre del fichero en ensamblador.\n";
}

static const char *show_about() {
    return
            YELLOW_TEXT
            "Autores:\n"
            COLOR_RESET
            "\tPedro López <plopez@disca.upv.es>\n"
            "\tSergio Sáez <ssaez@disca.upv.es>\n"
            "\tSalva Petit <spetit@disca.upv.es>\n"
            "\n"
            "\tDepartamento de Informática de Sistemas y Computadores (DISCA)\n"
            "\tUniversidad Politécnica de Valencia\n"
            "\n"
            YELLOW_TEXT
            "\nDescripción:\n"
            COLOR_RESET
            "\triscv-m es un simulador de un procesador RISC-V con ejecución en orden\n"
            "\tLas principales características son:\n"
            "\t\t-Juego de instrucciones: RV64IMFD\n"
            "\t\t  +visualización del formato y sus campos\n"
            "\t\t-Ruta de datos convencional o segmentada, configurable\n"
            "\t\t  +diferentes configuraciones\n"
            "\t\t-Ejecución ciclo a ciclo. Para cada ciclo muestra:\n "
            "\t\t   +un cronograma con las etapas ejecutadas por cada instrucción\n"
            "\t\t   +el estado de los registros y memoria de datos\n"
            "\t\t   +el estado de la ruta de datos\n";
}


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

    genera_MInstruccion(prog);


}

/***************************************************************
 *
 * Func: ejecutar_codigo
 *
 *
 * Retorna:  RC  (int)
 *
 * Descripción:
 *   Ejecuta el bucle principal del computador
 *
 **************************************************************/

static int ejecutar_codigo(
) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int RC; /* Valor de retorno */
    int i;


    //    boolean OVF_TXT = NO;

    /*  boolean	html = NO; */

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    RC = 0;

    /*** Inicializacion del computador */

    Ciclo = 1;
    //Instrucciones = 0;
    estat.instrucciones = 0;
    estat.ciclos = 0;
    estat.enteras = 0;
    estat.multiciclo = 0;
    estat.flops = 0;


    orden = 1;

    if ((!hay_float) && (!hay_double))
        small_screen_flag = 1;


    init_presentacion();

    if (salida == html) {
        imprime_inicio_css();
    }

    if (salida == html_final) {
        imprime_inicio_css();
    }

    /* Directiva de ensamblador .pc */
    PC = PC_inicial;


    /*** Bucle principal */

    while (!final) {

        if ((salida == res_final) || (salida == res_tiempo) || (salida == html_final)) {
            if ((Ciclo > 0) && (Ciclo % 2000) == 0) {
                printf(".");
                fflush(stdout);
            };
            if ((Ciclo > 0) && (Ciclo % 10000) == 0) {
                printf("%5uK", Ciclo / 1000);
                fflush(stdout);
            };
            if ((Ciclo > 0) && (Ciclo % 50000) == 0) {
                printf("\n");
            };
        }

        if (tipo_procesador == segm) {
            /*** Enteros ******************************/

            /*** Etapa: WB **************/
            final = fase_escritura();

            /*** Etapa: MEM **************/
            fase_memoria();

            /*** Etapa: EX *************/
            fase_ejecucion();

            /*** Etapa: WB **************/
            fase_escritura_FP();

            /*** Etapa: Ejecución multiciclo **************/
            do_SEGM_OP(FP, LS);
            do_SEGM_OP(FP, ADD);
            do_SEGM_OP(FP, MUL);
            do_SEGM_OP(FP, CMP);
            do_SEGM_OP(FP, MISC);
            do_SEGM_OP(FP, CVT);
            do_SEGM_OP(INT, MUL);
            /*
            i = FP_LS_LAT;
            while (i > 0) {
                fase_FP_LS(i);
                i = i - 1;
            }

            i = FP_ADD_LAT;
            while (i > 0) {
                fase_FP_ADD(i);
                i = i - 1;
            }

            i = FP_MUL_LAT;
            while (i > 0) {
                fase_FP_MUL(i);
                i = i - 1;
            }

            i = FP_CMP_LAT;
            while (i > 0) {
                fase_FP_CMP(i);
                i = i - 1;
            }

            i = INT_MUL_LAT;
            while (i > 0) {
                fase_INT_MUL(i);
                i = i - 1;
            }

            i = FP_MISC_LAT;
            while (i > 0) {
                fase_FP_MISC(i);
                i = i - 1;
            }
            */

            /*** Etapa: ID **************/
            fase_decodificacion();

            /*** Etapa: IF ***********/
            fase_busqueda();


            // Muestra busy de los registros
            /*
            fprintf(stdout, "Ciclo %d\n", Ciclo);
            fprintf(stdout, "DEBUG FP  Regs: busy");
            for (int i = 0; i < TAM_REGISTROS; i++) {
                fprintf(stdout, "%3d ", Rfp[i].busy);
            }
            fprintf(stdout, "\n");
            fprintf(stdout, "DEBUG INT Regs: busy");
            for (int i = 0; i < TAM_REGISTROS; i++) {
                fprintf(stdout, "%3d ", Rint[i].busy);
            }
            fprintf(stdout, "\n");
             */

            actualiza_crono_estado();
            impulso_reloj();
            /* Incrementa el número de ciclos ejecutados */
            Ciclo++;

        } else if (tipo_procesador == no_segm_multiciclo) {
            /*** Borrar valores previos ***/
            inicializa_registros_intermedios();

            /*** Enteros ******************************/

            /*** Etapa: IF ***********/
            fase_busqueda();
            actualiza_crono_estado();
            IF_ID = IF_IDn;
            Ciclo++;

            /*** Etapa: ID **************/
            fase_decodificacion();
            actualiza_crono_estado();
            ID_EX = ID_EXn;
            Ciclo++;

            /*** Etapa: EX *************/
            if (ID_EXn.iPC != -1) {
                fase_ejecucion();
                actualiza_crono_estado();

                EX_MEM = EX_MEMn;
                if (EX_MEM.cond) {
                    SaltoEfectivo = SI;
                    PCn = EX_MEM.ALUout;
                    write_log("Salto Efectivo", EX_MEM.iPC);
                }
                Ciclo++;
            }

            /*** Etapa: MEM **************/
            if (EX_MEMn.iPC!=-1) {
                fase_memoria();
                actualiza_crono_estado();
                MEM_WB = MEM_WBn;
                Ciclo++;
            }

            /*** Etapa: Ejecución multiciclo **************/

            if (ID_FP_LSn.iPC != -1) {
                do_NO_SEGM_OP(FP, LS);
            }
            if (ID_FP_ADDn.iPC != -1) {
                do_NO_SEGM_OP(FP, ADD);
            }
            if (ID_FP_MULn.iPC != -1) {
                do_NO_SEGM_OP(FP, MUL);
            }
            if (ID_FP_CMPn.iPC != -1) {
                do_NO_SEGM_OP(FP, CMP);
            }
            if (ID_FP_CVTn.iPC != -1) {
                do_NO_SEGM_OP(FP, CVT);
            }
            if (ID_FP_MISCn.iPC != -1) {
                do_NO_SEGM_OP(FP, MISC);
            }
            if (ID_INT_MULn.iPC != -1) {
                do_NO_SEGM_OP(INT, MUL);
            }

            /*** Etapa: WB **************/
            fase_escritura_FP();
            Rfp[FP_WB.IR.rd].busy = 0;

            /*** Etapa: WB **************/
            final = fase_escritura();
            Rint[iWB.IR.rd].busy = 0;

            actualiza_crono_estado();
            Ciclo++;

            Rfp_writings = Rfp_writings >> 1;
            Rint_writings = Rint_writings >> 1;
            orden = orden + 1; /* VISUALIZACIÓN */
            PC = PCn;

            
        } else if (tipo_procesador == no_segm_monociclo) {
            /*** Borrar valores previos ***/
            inicializa_registros_intermedios();

            /*** Enteros ******************************/

            /*** Etapa: IF ***********/
            fase_busqueda();
            IF_ID = IF_IDn;

            /*** Etapa: ID **************/
            fase_decodificacion();
            ID_EX = ID_EXn;

            /*** Etapa: EX *************/
            if (ID_EXn.iPC != -1) {
                fase_ejecucion();
                EX_MEM = EX_MEMn;
                if (EX_MEM.cond) {
                    SaltoEfectivo = SI;
                    PCn = EX_MEM.ALUout;
                    write_log("Salto Efectivo", EX_MEM.iPC);
                }
            }

            /*** Etapa: MEM **************/
            if (EX_MEMn.iPC!=-1) {
                fase_memoria();
                MEM_WB = MEM_WBn;
            }

            /*** Etapa: Ejecución multiciclo **************/

            if (ID_FP_LSn.iPC != -1) {
                do_NO_SEGM_OP(FP, LS);
            }
            if (ID_FP_ADDn.iPC != -1) {
                do_NO_SEGM_OP(FP, ADD);
            }
            if (ID_FP_MULn.iPC != -1) {
                do_NO_SEGM_OP(FP, MUL);
            }
            if (ID_FP_CMPn.iPC != -1) {
                do_NO_SEGM_OP(FP, CMP);
            }
            if (ID_FP_MISCn.iPC != -1) {
                do_NO_SEGM_OP(FP, MISC);
            }
            if (ID_FP_CVTn.iPC != -1) {
                do_NO_SEGM_OP(FP, CVT);
            }
            if (ID_INT_MULn.iPC != -1) {
                do_NO_SEGM_OP(INT, MUL);
            }

            /*** Etapa: WB **************/
            fase_escritura_FP();
            Rfp[FP_WB.IR.rd].busy = 0;

            /*** Etapa: WB **************/
            final = fase_escritura();
            Rint[iWB.IR.rd].busy = 0;

            actualiza_crono_estado();
            Ciclo++;
            
            Rfp_writings = Rfp_writings >> 1;
            Rint_writings = Rint_writings >> 1;
            orden = orden + 1; /* VISUALIZACIÓN */
            PC = PCn;
            
        }
        estat.ciclos = Ciclo-1;


        if (Ciclo >= MAX_CICLOS_TOTAL) {
            fprintf(stderr, "\nATENCION: Numero de ciclos (%d) excesivo\n Ejecución detenida\n\n", MAX_CICLOS_TOTAL);
            final = SI;
        }

        if ((salida == html) && (Ciclo >= MAX_CICLOS_HTML) && !OVF_HTML) {
            fprintf(stderr, "\nATENCION: Numero de ciclos (%d) excesivo\n No se generan mas archivos html\n\n",
                    MAX_CICLOS_HTML);
            OVF_HTML = SI;
        };

        if (OVF_HTML) {
            if ((Ciclo > 0) && (Ciclo % 2000) == 0) {
                printf(".");
                fflush(stdout);
            };
            if ((Ciclo > 0) && (Ciclo % 10000) == 0) {
                printf("%5uK", Ciclo / 1000);
                fflush(stdout);
            };
            if ((Ciclo > 0) && (Ciclo % 50000) == 0) {
                printf("\n");
            };
        }

        /*
        if ((salida != html) && (Ciclo >= MAX_CICLOS_TXT) && !OVF_TXT)
        {
            fprintf(stderr, "\nATENCION: Numero de ciclos excesivo\n No se muestran mas ciclos\n\n");
                        OVF_TXT = SI;
        } ;
         */


        /*** Imprime el estado -- interfaz de texto  */
        /*
        if ((salida == res_ciclo)  && (!OVF_TXT)) {
            imprime_etapas_txt ();
            imprime_reg_txt ();
            imprime_memdatos_txt ();
        }
         */
        /*** Imprime el estado del los operadores, etc.  */

        


    } /* endwhile */


    if (salida == res_tiempo) {
        //imprime_inicio_txt();
        imprime_final_txt();
        //        imprime_reg_txt ();
        //        imprime_memdatos_txt ();
    }

    if (salida == res_final) {
        printf("Archivo: %s\n\n", nombre_fich);
        imprime_inicio_txt();
        imprime_final_txt();
        imprime_reg_txt();
        imprime_memdatos_txt();
    }

    if (salida == html) {
        imprime_final_txt();
        imprime_final_css();

    }

    if (salida == html_final) {
        imprime_final_txt();
        imprime_final_css();
    }


    return (RC);

} /* end ejecutar_codigo */

/**
 * Gestiona un error fatal
 *
 * @param msg_text Mensaje a mostrar
 * @param msg_arg1 Mensaje a mostrar
 * @param msg_arg2 Mensaje a mostrar
 *
 */
static void error_fatal
        (
                char *msg_text,
                char *msg_arg1,
                char *msg_arg2
        ) {
    fprintf(stderr, RED_TEXT "%s %s %s\n\n" COLOR_RESET, msg_text, msg_arg1, msg_arg2);
    fprintf(stderr, "%s\n", show_usage());
    exit(1);
} /* end error_fatal */

/**
 * Comprueba si existe el archivo
 *
 * @param filename Nombre del archivo
 *
 */
static int file_exists(char *filename) {
    struct stat buffer;

    return (stat(filename, &buffer) == 0);
}

/*** Funciones Exportables *************************************************/

#ifndef LIBRISCV

/***************************************************************
 *
 * Func: main
 *
 * Parametros:
 *	argc  (int) ; N˙mero de parametros en linea
 *	argv[]  (char*) ; Texto de los parametros en linea
 *
 * Retorna:  RC  (int)
 *
 * Descripción:
 *   Función principal
 *
 **************************************************************/

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
*
* Retorna: resultado_htm (char*): Resultado de la simulacion (malloc). Es responsabilidad del que llama liberar con free.
*
* Descripcion:
*   Funcion principal de la libreria (reemplaza a main)
*
**************************************************************/

char* libriscv_main(char* ensamblador_s) {

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

    /*** Añade argumentos a partir de la primera línea de ensamblador_s */

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

    /*** Marca fin de argumentos. */
    argv[argc] = 0;

#endif /* #ifndef LIBRISCV */

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int RC; /* Valor de retorno */
    int np;
    int param;


    int argn;


    boolean borrar_html = 1;
    boolean f_dump = NO;

    FILE *f_aux;

    static FILE *f_consola;
    static FILE *f_log;

    //char aux[128];

    char *ficheros_entrada[MAX_FICHEROS];
    int num_ficheros = 0;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    nombre_fich[0] = '\0';

    /*** Lectura de parametros */

        tipo_procesador = segm;
        solucion_riesgos_datos = cortocircuito;
        solucion_riesgos_control = pnt3;
        salida = html;


        argn = 1;
        if (argc < 2) {
            error_fatal("Error: ", "Faltan argumentos", "");
        } /* endif */

        fprintf(stdout, "%s%s%s\n", BLUE_TEXT, simul_logo(), COLOR_RESET);
        fprintf(stdout, "%s%s%s", GREEN_TEXT, SIMUL_NAME, COLOR_RESET);
        fprintf(stdout, " version %s%s%s", YELLOW_TEXT, SIMUL_VERSION, COLOR_RESET);
        fprintf(stdout, " %s\n\n", SIMUL_DATE);
        fprintf(stdout, "Simulador del procesador RISC-V multiciclo\n\n");
        while (argn < argc) {
            if (strcmp(argv[argn], "-f") == 0) {
                char str[64 * MAX_FICHEROS];
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                ficheros_entrada[num_ficheros] = argv[argn];
                if (!file_exists(ficheros_entrada[num_ficheros])) {
                    fprintf(stderr, "Fichero de entrada '%s' no encontrado.\n",
                            ficheros_entrada[num_ficheros]);
                    error_fatal("Error en opción", argv[argn - 1], "falta nombre del fichero");
                } /* endif */

                sprintf(str, "%s %s", nombre_fich, basename(argv[argn]));
                strcpy(nombre_fich, str);
                num_ficheros++;
            } else if (strcmp(argv[argn], "-s") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                if (strcmp(argv[argn], "tiempo") == 0) {
                    salida = res_tiempo;
                } else if (strcmp(argv[argn], "final") == 0) {
                    salida = res_final;
                } else if (strcmp(argv[argn], "html") == 0) {
                    salida = html;
                } else if (strcmp(argv[argn], "html-final") == 0) {
                    salida = html_final;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "-p") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                if (strcmp(argv[argn], "segm") == 0) {
                    tipo_procesador = segm;
                } else if (strcmp(argv[argn], "no-segm-mono") == 0) {
                    tipo_procesador = no_segm_monociclo;
                    solucion_riesgos_datos = ninguno;
                    solucion_riesgos_control = ds3;
                } else if (strcmp(argv[argn], "no-segm-multi") == 0) {
                    tipo_procesador = no_segm_multiciclo;
                    solucion_riesgos_datos = ninguno;
                    solucion_riesgos_control = ds3;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "-d") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                if (strcmp(argv[argn], "n") == 0) {
                    solucion_riesgos_datos = ninguno;
                } else if (strcmp(argv[argn], "p") == 0) {
                    solucion_riesgos_datos = parada;
                } else if (strcmp(argv[argn], "c") == 0) {
                    solucion_riesgos_datos = cortocircuito;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "-c") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                if (strcmp(argv[argn], "s3") == 0) {
                    solucion_riesgos_control = stall3;
                } else if (strcmp(argv[argn], "s2") == 0) {
                    solucion_riesgos_control = stall2;
                } else if (strcmp(argv[argn], "s1") == 0) {
                    solucion_riesgos_control = stall1;
                } else if (strcmp(argv[argn], "pnt3") == 0) {
                    solucion_riesgos_control = pnt3;
                } else if (strcmp(argv[argn], "pnt2") == 0) {
                    solucion_riesgos_control = pnt2;
                } else if (strcmp(argv[argn], "pnt1") == 0) {
                    solucion_riesgos_control = pnt1;
                } else if (strcmp(argv[argn], "n3") == 0) {
                    solucion_riesgos_control = ds3;
                } else if (strcmp(argv[argn], "n2") == 0) {
                    solucion_riesgos_control = ds2;
                } else if (strcmp(argv[argn], "n1") == 0) {
                    solucion_riesgos_control = ds1;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "-a") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                np = sscanf(argv[argn], "%d", &param);
                if (np != 1) {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }

                if (param > 1) {
                    if (param >= MAX_FP_LAT) {
                        error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                    }
                    FP_ADD_LAT = param;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "--cmp") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                np = sscanf(argv[argn], "%d", &param);
                if (np != 1) {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }

                if (param > 1) {
                    if (param >= MAX_FP_LAT) {
                        error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                    }
                    FP_CMP_LAT = param;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "--misc") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                np = sscanf(argv[argn], "%d", &param);
                if (np != 1) {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }

                if (param > 1) {
                    if (param >= MAX_FP_LAT) {
                        error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                    }
                    FP_MISC_LAT = param;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "--cvt") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                np = sscanf(argv[argn], "%d", &param);
                if (np != 1) {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }

                if (param > 1) {
                    if (param >= MAX_FP_LAT) {
                        error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                    }
                    FP_CVT_LAT = param;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "-l") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                np = sscanf(argv[argn], "%d", &param);
                if (np != 1) {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }

                if (param > 1) {
                    if (param >= MAX_FP_LAT) {
                        error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                    }
                    FP_LS_LAT = param;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "-m") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                np = sscanf(argv[argn], "%d", &param);
                if (np != 1) {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }

                if (param > 1) {
                    if (param >= MAX_FP_LAT) {
                        error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
                FP_MUL_LAT = param;
                INT_MUL_LAT = FP_MUL_LAT;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "--reg") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                if (strcmp(argv[argn], "auto") == 0) {
                    SHOW_ABI_REG = reg_auto;
                    printf("reg argn %d argv[argn]: %s\n", argn, argv[argn]);
                } else if (strcmp(argv[argn], "ar") == 0) {
                    SHOW_ABI_REG = reg_ar;
                    printf("reg argn %d argv[argn]: %s\n", argn, argv[argn]);
                } else if (strcmp(argv[argn], "abi") == 0) {
                    SHOW_ABI_REG = reg_abi;
                    printf("reg argn %d argv[argn]: %s\n", argn, argv[argn]);
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
            } else if (strcmp(argv[argn], "--format") == 0) {
                argn++;
                if (argn == argc)
                    error_fatal("Error en opción", argv[argn - 1], "");
                if (!strcmp(argv[argn], "full")) {
                    show_format = format_full;
                } else if (!strcmp(argv[argn], "compact")) {
                    show_format = format_compact;
                } else if (!strcmp(argv[argn], "none")) {
                    show_format = format_none;
                } else {
                    error_fatal("Error en opción", argv[argn - 1], argv[argn]);
                }
                printf("HEY %s\n", argv[argn]);
            } else if (strcmp(argv[argn], "-n") == 0) {
                borrar_html = 0;
            } else if (strcmp(argv[argn], "-j") == 0) {
                html_merge = SI;
            } else if (strcmp(argv[argn], "--dump") == 0) {
                f_dump = SI;
            } else if (strcmp(argv[argn], "--opt-int") == 0) {
                AJUSTAR_INT = SI;
            } else if (strcmp(argv[argn], "--small-screen") == 0) {
                small_screen_flag = 1;
            } else if (strcmp(argv[argn], "--no-show-IT") == 0) {
                SHOW_DIT = NO;
            } else if (strcmp(argv[argn], "--no-show-DP") == 0) {
                SHOW_DATAPATH = NO;
            } else if (strcmp(argv[argn], "--no-show-R") == 0) {
                SHOW_REG = NO;
            } else if (strcmp(argv[argn], "--no-show-M") == 0) {
                SHOW_MEM = NO;
            } else if (strcmp(argv[argn], "--no-show-LOG") == 0) {
                SHOW_LOG = NO;
            } else if (strcmp(argv[argn], "--no-show-CONSOLE") == 0) {
                SHOW_CONSOLE = NO;
            } else if (strcmp(argv[argn], "-?") == 0) {
                fprintf(stderr, "%s\n", show_usage());
                exit(1);
            } else if (strcmp(argv[argn], "--help") == 0) {
                fprintf(stderr, "%s\n", show_usage());
                exit(1);
            } else if (strcmp(argv[argn], "--about") == 0) {
                fprintf(stderr, "%s\n", show_about());
                exit(1);
            } else {
                error_fatal("Error en opción", argv[argn], "");
            }
            argn++;
        }

        /*
        while (1) {
            break;
            static struct option long_options[] = {


                    //{"no-w3css", no_argument, &w3css_flag, 0},
                    {"help",           no_argument,       0,                  '?'},
                    {"no-borrar-html", no_argument,       0,                  'n'},
                    {"opt-int",        no_argument,       &AJUSTAR_INT,       SI},
                    {"javascript",     no_argument,       0,                  'j'},
                    {"small-screen",   no_argument,       &small_screen_flag, 1},

                    {"no-show-IT",     no_argument,       &SHOW_DIT,          0},
                    {"no-show-DP",     no_argument,       &SHOW_DATAPATH,     0},
                    {"no-show-R",      no_argument,       &SHOW_REG,          0},
                    {"no-show-M",      no_argument,       &SHOW_MEM,          0},
                    {"no-show-LOG",    no_argument,       &SHOW_LOG,          0},
                    {"no-show-C",      no_argument,       &SHOW_CONSOLE,      0},

                    {"salida",         required_argument, 0,                  's'},
                    {"rdatos",         required_argument, 0,                  'd'},
                    {"rcontrol",       required_argument, 0,                  'c'},
                    {"FPadd",          required_argument, 0,                  'a'},
                    {"FPmul",          required_argument, 0,                  'm'},
                    {"FPls",           required_argument, 0,                  'l'},
                    {"FPcmp",          required_argument, 0,                  'k'},
                    {"programa",       required_argument, 0,                  'f'},
                    {"reg",            required_argument, 0,                  'r'},
                    {0, 0,                                0,                  0}
            };

            int option_index = 0;

            c = getopt_long(argc, argv, "?a:c:d:f:k:l:m:s:nj",
                            long_options, &option_index);

            if (c == -1)
                break;

            switch (c) {
                case '?':
                    fprintf(stderr, uso, argv[0]);
                    exit(1);
                    break;

                case 0:

                    if (long_options[option_index].flag != 0)
                        break;
                    printf("option %s", long_options[option_index].name);
                    if (optarg)
                        printf(" with arg %s", optarg);
                    printf("\n");
                    break;

                case 'c':
                    if (strncmp(optarg, "p", 4) == 0) { // Por compatibilidad previa
                        solucion_riesgos_control = stall3;
                    } else if (strncmp(optarg, "s3", 4) == 0) {
                        solucion_riesgos_control = stall3;
                    } else if (strncmp(optarg, "s2", 4) == 0) {
                        solucion_riesgos_control = stall2;
                    } else if (strncmp(optarg, "s1", 4) == 0) {
                        solucion_riesgos_control = stall1;
                    } else if (strncmp(optarg, "t", 4) == 0) { // Por compatibilidad previa
                        solucion_riesgos_control = pnt3;
                    } else if (strncmp(optarg, "pnt3", 4) == 0) {
                        solucion_riesgos_control = pnt3;
                    } else if (strncmp(optarg, "pnt2", 4) == 0) {
                        solucion_riesgos_control = pnt2;
                    } else if (strncmp(optarg, "pnt1", 4) == 0) {
                        solucion_riesgos_control = pnt1;
                    } else if (strncmp(optarg, "ds3", 4) == 0) {
                        solucion_riesgos_control = ds3;
                    } else if (strncmp(optarg, "ds2", 4) == 0) {
                        solucion_riesgos_control = ds2;
                    } else if (strncmp(optarg, "ds1", 4) == 0) {
                        solucion_riesgos_control = ds1;
                    } else {
                        fprintf(stderr, "Error en argumento %s %s\n", "-c", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                case 'd':
                    if (strncmp(optarg, "n", 1) == 0) {
                        solucion_riesgos_datos = ninguno;
                    } else if (strncmp(optarg, "p", 1) == 0) {
                        solucion_riesgos_datos = parada;
                    } else if (strncmp(optarg, "c", 1) == 0) {
                        solucion_riesgos_datos = cortocircuito;
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-d", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                case 'f':
                    ficheros_entrada[num_ficheros] = optarg;
                    if (!file_exists(ficheros_entrada[num_ficheros])) {
                        fprintf(stderr, "%s: Fichero de entrada '%s' no encontrado.\n", argv[0],
                                ficheros_entrada[num_ficheros]);
                        exit(1);
                    }

                    sprintf(str, "%s %s", nombre_fich, basename(optarg));
                    strcpy(nombre_fich, str);
                    num_ficheros++;

                    break;

                case 'a':
                    if (isdigit(*optarg) && (atoi(optarg) > 1)) {
                        if (atoi(optarg) >= MAX_FP_LAT) {
                            fprintf(stderr, "\nError. La latencia máxima del operador es %d\n", MAX_FP_LAT - 1);
                            exit(1);
                        }
                        FP_ADD_LAT = atoi(optarg);
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-a", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                case 'k':
                    if (isdigit(*optarg) && (atoi(optarg) > 1)) {
                        if (atoi(optarg) >= MAX_FP_LAT) {
                            fprintf(stderr, "\nError. La latencia máxima del operador es %d\n", MAX_FP_LAT - 1);
                            exit(1);
                        }
                        FP_CMP_LAT = atoi(optarg);
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-k", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                case 'l':
                    if (isdigit(*optarg) && (atoi(optarg) > 1)) {
                        if (atoi(optarg) >= MAX_FP_LAT) {
                            fprintf(stderr, "\nError. La latencia máxima del operador es %d\n", MAX_FP_LAT - 1);
                            exit(1);
                        }
                        FP_LS_LAT = atoi(optarg);
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-l", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;


                case 'm':
                    if (isdigit(*optarg) && (atoi(optarg) > 1)) {
                        if (atoi(optarg) >= MAX_FP_LAT) {
                            fprintf(stderr, "\nError. La latencia máxima del operador es %d\n", MAX_FP_LAT - 1);
                            exit(1);
                        }
                        FP_MUL_LAT = atoi(optarg);
                        INT_MUL_LAT = FP_MUL_LAT;
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-m", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;


                case 's':
                    if (strncmp(optarg, "tiempo", 6) == 0) {
                        salida = res_tiempo;
                    } else if (strncmp(optarg, "final", 5) == 0) {
                        salida = res_final;
                    } else if (strncmp(optarg, "html", 10) == 0) {
                        salida = html;
                    } else if (strncmp(optarg, "html-final", 10) == 0) {
                        salida = html_final;
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-s", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                case 'n':
                    borrar_html = 0;
                    break;

                case 'j':
                    html_merge = SI;
                    break;

                case 'r':
                    if (strncmp(optarg, "auto", 4) == 0) {
                        SHOW_ABI_REG = reg_auto;
                    } else if (strncmp(optarg, "ar", 2) == 0) {
                        SHOW_ABI_REG = reg_ar;
                    } else if (strncmp(optarg, "abi", 3) == 0) {
                        SHOW_ABI_REG = reg_abi;
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-s", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                default:
                    abort();
            }
        }*/

#ifdef LIBRISCV
    /*** Corrige configuracion si se usa libriscv */

    //strcpy(ficheros_entrada[num_ficheros],
    //       "fichero.s");
    //num_ficheros = 1;
    //f_etiquetas = NO;
    borrar_html = NO;
    html_merge = SI;
    salida = html;
#endif

    if (num_ficheros == 0) {
        error_fatal("Error en opción", "-f: ", "faltan nombres de archivos .s");
    }


    fprintf(stdout,
            "Archivos: %s\n\n", nombre_fich);

    if (borrar_html) {
        system("rm *.html 2>/dev/null");
    }

    inicializa_programa(&programa, num_ficheros, ficheros_entrada);


/* Si existe el archivo en el /tmp, se utilizará su contenido para poner
 un enlace HOME en la salida html */
    if (file_exists("/tmp/_url.txt")) {
        f_aux = fopen("/tmp/_url.txt", "r");
        fgets(HOME_LINK,
              128, f_aux);
        home_link_flag = 1;
        fclose(f_aux);
    }

    /* Borra archivo de consola */
    f_consola = fopen(console_name, "w");
    fclose(f_consola);

    /* Borra archivo de log */
    f_log = fopen(log_name, "w");
    fclose(f_log);

/*** Inicializacion de estructuras */

    inicializa_estructuras();


/*** Carga el codigo en lenguaje ensamblador */

    construye_programa(&programa);

/*** Imprime mapa de memoria */

    imprime_mapa_memoria();


/*** Llama al bucle principal */

    RC = ejecutar_codigo();

    /*** Vuelca el contenido de la memoria ***/
    if (f_dump) {
        vuelca_memoria(&programa);
    }

#ifndef LIBRISCV

    return (RC);

#else

    /*** libriscv devuelve una cadena con el contenido del fichero .htm de salida */

    FILE* f_salida = fopen("index.htm", "r");
    long htm_size;
    char* result;

    fseek(f_salida, 0, SEEK_END);
    htm_size = ftell(f_salida);
    rewind(f_salida);

    result = malloc(htm_size + 1);
    fread(result, htm_size, 1, f_salida);
    result[htm_size] = 0;

    fclose(f_salida);

    /*** Libera argv (se construyó al inicio) */
    free(argv);

    return result;

#endif /* LIBRISCV */

} /* end main */

