 void pasar_a_ID_NOP (void);
 void pasar_a_EX_NOP (void);
 void pasar_a_MEM_NOP (void);
 void pasar_a_WB_NOP (void);
 void pasar_a_FP_LS1_NOP (void);
 void pasar_a_FP_A1_NOP (void);
 void pasar_a_FP_M1_NOP (void);
 void pasar_a_iM1_NOP (void);
 void pasar_a_FP_MISC1_NOP (void);
 void pasar_a_FP_C1_NOP (void);
 void pasar_a_FP_CVT1_NOP (void);
 void pasar_a_FP_LS2_NOP (void);
 void pasar_a_FP_A2_NOP (void);
 void pasar_a_FP_M2_NOP (void);
 void pasar_a_FP_C2_NOP (void);
 void pasar_a_iM2_NOP (void);
 void pasar_a_FP_MISC2_NOP (void);
 void pasar_a_FP_CVT2_NOP (void);
 void pasar_a_FP_LS_WB_NOP (void);
 void cancelar_en_ID (void);
 void cancelar_en_EX (void);
 boolean rs1_valido ( 
	instruccion_t inst
	 );
 boolean rs2_valido ( 
	instruccion_t inst
	 );
 boolean rd_valido ( 
	instruccion_t inst
	 );
 boolean hay_fuente1_ID (void);
 boolean hay_fuente2_ID (void);
 boolean hay_fuente1_EX (void);
 boolean hay_fuente2_EX (void);
 boolean hay_destino_EX (void);
 boolean hay_destino_MEM (void);
 boolean hay_destino_WB (void);
 boolean rfp1_valido ( 
	instruccion_t inst
	 );
 boolean rfp2_valido ( 
	instruccion_t inst
	 );
 boolean rfp3_valido ( 
	instruccion_t inst
	 );
 boolean rfpd_valido ( 
	instruccion_t inst
	 );
 boolean hay_fuente1_FP_ID (void);
 boolean hay_fuente2_FP_ID (void);
 boolean hay_fuente3_FP_ID (void);
 boolean hay_destino_FP_WB (void);
 boolean hay_fuente1_FP_ADD1 (void);
 boolean hay_fuente2_FP_ADD1 (void);
 boolean hay_fuente1_FP_MUL1 (void);
 boolean hay_fuente2_FP_MUL1 (void);
 boolean hay_fuente3_FP_MUL1 (void);
 boolean hay_fuente1_FP_CMP1 (void);
 boolean hay_fuente2_FP_CMP1 (void);
 boolean hay_fuente1_iM1 (void);
 boolean hay_fuente2_iM1 (void);
 boolean hay_fuente1_FP_MISC1 (void);
 boolean hay_fuente2_FP_MISC1 (void);
 boolean hay_fuente1_LS1 (void);
 boolean hay_fuente2_LS1 (void);
 boolean hay_fuente2_LS2 (void);
