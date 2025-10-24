 int fp_classify ( 
	float num
	 );
 void config_Redondeo ( 
	int RM
	 );
 dword operacion_ALU_INT_MUL ( 
	int codop,
	dword in1,
	dword in2
	 );
 valor_t operacion_ALU_FP_ADD ( 
	int codop,
	valor_t in1,
	valor_t in2,
	int redondeo
	 );
 valor_t operacion_ALU_FP_MUL ( 
	int codop,
	valor_t in1,
	valor_t in2,
	valor_t in3,
	int redondeo
	 );
 valor_t operacion_ALU_FP_CMP ( 
	int codop,
	valor_t in1,
	valor_t in2,
	int redondeo
	 );
 valor_t operacion_ALU_FP_MISC ( 
	int codop,
	valor_t in1,
	valor_t in2,
	int redondeo
	 );
 valor_t operacion_ALU_FP_CVT ( 
	int codop,
	valor_t in1,
	int redondeo
	 );
 dword mux_FP_LS_sup ( 
	dword ra,
	dword mem,
	dword wb
	 );
 valor_t mux_FP_LS_inf ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_LS2_inf ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_ADD_sup ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_ADD_inf ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_CMP_sup ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_CMP_inf ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_MUL_sup ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_MUL_inf ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_MUL_3 ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_MISC_sup ( 
	valor_t reg_val,
	valor_t wb
	 );
 valor_t mux_FP_MISC_inf ( 
	valor_t reg_val,
	valor_t wb
	 );
 dword mux_INT_MUL_sup ( 
	dword reg_val,
	dword mem,
	dword wb
	 );
 dword mux_INT_MUL_inf ( 
	dword reg_val,
	dword mem,
	dword wb
	 );
 dword mux_FP_CVT_sup ( 
	dword reg_val,
	dword mem,
	dword wb
	 );
 MULTI_WB_t mux_FP_WB ( 
	MULTI_WB_t fp_ls,
	MULTI_WB_t fp_add,
	MULTI_WB_t fp_mul,
	MULTI_WB_t fp_cvt
	 );
 MEM_WB_t mux_INT_WB ( 
	MEM_WB_t wb,
	MULTI_WB_t fp_cmp,
	MULTI_WB_t int_mul,
	MULTI_WB_t to_int
	 );
 void fase_FP_LS ( 
	short i
	 );
 void fase_FP_ADD ( 
	short i
	 );
 void fase_FP_MUL ( 
	short i
	 );
 void fase_INT_MUL ( 
	short i
	 );
 void fase_FP_CMP ( 
	short i
	 );
 void fase_FP_CVT ( 
	short i
	 );
 void fase_FP_MISC ( 
	short i
	 );
 void detectar_riesgos_RAW_FP (void);
 void detectar_riesgos_WAW_FP (void);
 void detectar_riesgos_WB_FP (void);
