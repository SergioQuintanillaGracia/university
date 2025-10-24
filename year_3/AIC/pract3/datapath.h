/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   datapath-svg.h
 * Author: plopez
 *
 * Created on 3 de noviembre de 2017, 13:30
 */

#ifndef DATAPATH_SVG_H
#define DATAPATH_SVG_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SVG_WIDTH
#define SVG_WIDTH "800px"
#endif

    /* Para dibujar el datapath */

#define trazo_grueso 1.5
#define trazo_fino 1.5
#define trazo_resaltado 3.0

#define PC_ancho 30
#define PC_alto 100
#define reg_ancho 30
#define reg_alto 400
#define mem_ancho 100
#define mem_alto 100
#define sep1x 10
#define sep1y 10
#define aluPC_ancho 50
#define aluPC_alto 100
#define alu_ancho 50
#define alu_alto 120
#define mux2_ancho 20
#define mux2_alto 20+1*10
#define mux3_ancho 20
#define mux3_alto 20+2*10
#define mux4_ancho 20
#define mux4_alto 20+3*10
#define regs_ancho 100
#define regs_alto 140
#define elipse_ancho 70
#define elipse_alto 80
#define zero_ancho 50
#define zero_alto 100

#define eje_x_IF 20
#define eje_x_ID eje_x_IF+PC_ancho+mem_ancho+4*sep1x
#define eje_x_EX eje_x_ID+reg_ancho+2*regs_ancho+5*sep1x // experimental
#define eje_x_M eje_x_EX+mux4_ancho+alu_ancho+11*sep1x+reg_ancho
#define eje_x_WB eje_x_M+mux2_ancho+mem_ancho+7*sep1x+reg_ancho

#define eje_y_texto 15

#define eje_y1_IF 100 // Registro IF/ID
//#define eje_y2_IF eje_y1_IF-20 // ALU PC
//#define eje_y3_IF eje_y2_IF+aluPC_alto+30 // PC, meminstr
//#define eje_y4_IF eje_y2_IF+aluPC_alto // npc

//#define eje_y1_ID eje_y1_IF // Registro ID/EX
#define eje_y2_ID eje_y1_ID-20 // control
//#define eje_y3_ID eje_y3_IF // regs
//#define eje_y4_ID eje_y3_ID+regs_alto+20 // ext
#define eje_y5_ID eje_y4_ID+elipse_alto+30 // mux
#define eje_y6_ID eje_y4_ID+30 // IR
//#define eje_y7_ID eje_y1_ID-10 // Zero


//#define eje_y1_EX eje_y1_ID // Registro EX/M
//#define eje_y2_EX eje_y7_ID // Zero
//#define eje_y3_EX eje_y3_ID+(regs_alto-alu_alto)/2+20 // ALU
//#define eje_y4_EX eje_y3_EX+30 // mux

#define eje_y1_M eje_y1_ID // Registro M/WB
    //#define eje_y2_M eje_y3_ID+(regs_alto-mem_alto)/2 // memdatos
    //#define eje_y2_M eje_y3_ID+(alu_alto-mem_alto)/2 // memdatos
//#define eje_y2_M eje_y3_EX // memdatos
#define eje_y3_M eje_y4_EX // mux

#define IFID_x eje_x_ID
#define IFID_y 100
#define alu_IF_PC_x mem_instr_x
#define alu_IF_PC_y IFID_y-20
#define PC_x eje_x_IF
#define PC_y alu_IF_PC_y+aluPC_alto+6*sep1y
#define mem_instr_x PC_x+PC_ancho+2*sep1x
#define mem_instr_y PC_y
#define muxpc_x alu_IF_PC_x+aluPC_ancho+2*sep1x
#define muxpc_y alu_IF_PC_y+aluPC_alto/2-5
#define IFID_y_npc alu_IF_PC_y+aluPC_ancho+7*sep1y
#define IFID_y_pc IFID_y_npc+sep1y


#define regs_x IFID_x+reg_ancho+6*sep1x
#define regs_y mem_instr_y
    //#define control_x IFID_x+reg_ancho+2*sep1x
    //#define control_y eje_y2_ID
#define ext_x regs_x
#define ext_y regs_y+regs_alto+2*sep1y
#define alu_ID_x IFID_x+reg_ancho+6*sep1x
#define alu_ID_y alu_IF_PC_y+sep1y
#define mux3_ID_addr_s_x alu_ID_x-3*sep1x
#define muxZeroID_x alu_ID_x+aluPC_ancho+4*sep1x
#define zeroid_x muxZeroID_x+mux2_ancho+sep1x
#define zeroid_y alu_ID_y


#define IDEX_x eje_x_EX
#define IDEX_y IFID_y
#define alu_x IDEX_x+13*sep1x
#define alu_y regs_y+(regs_alto-alu_alto)/2+20
// Multiplexores ALU sin cortocircuitos
#define mux2s_x IDEX_x + 7*sep1x + mux2_ancho
#define mux2i_x mux2s_x
    // Multiplexores ALU con cortocircuitos
#define mux4s_x IDEX_x + 7*sep1x + mux4_ancho
#define mux4i_x mux4s_x
#define muxW_x mux4s_x
#define muxZeroEX_x mux4s_x
#define ZeroEX_x alu_x-sep1x
#define ZeroEX_y zeroid_y
#define RegA_x sep1x
#define RegB_x 2*sep1x
#define Imm_x 3*sep1x
#define NPC_x 3*sep1x

#define EXM_x eje_x_M
#define EXM_y IDEX_y

#define muxmem_x EXM_x+6*sep1x
#define mem_datos_x muxmem_x+mux2_ancho+2*sep1x
#define mem_datos_y alu_y

#define MWB_x eje_x_WB
#define MWB_y EXM_y
#define muxwb_x MWB_x+reg_ancho+4*sep1x



#ifndef CPROTO
#include "datapath_api.h"
#endif



#endif /* DATAPATH_SVG_H */

