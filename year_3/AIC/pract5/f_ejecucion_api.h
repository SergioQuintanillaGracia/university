 void fase_EX_1 (void);
 boolean fase_EX_1_enteros ( 
	unsigned int oper
	 );
 boolean fase_EX_1_sumrest ( 
	unsigned int oper
	 );
 boolean fase_EX_1_multdiv ( 
	unsigned int oper
	 );
 boolean fase_EX_1_direcciones ( 
	unsigned int oper
	 );
 boolean fase_EX_1_memdatos ( 
	unsigned int oper
	 );
 void fase_EX_2 (void);
 void fase_EX_2_enteros ( 
	unsigned int oper
	 );
 int fp_classify ( 
	float num
	 );
 void config_Redondeo ( 
	int RM
	 );
 void fase_EX_2_sumrest ( 
	unsigned int oper
	 );
 void fase_EX_2_multdiv ( 
	unsigned int oper
	 );
 void fase_EX_2_direcciones ( 
	unsigned int oper
	 );
 void fase_EX_2_memdatos ( 
	unsigned int oper
	 );
 void operacion_finalizada_L1D ( 
	operacion_nivel_t *oper_sig,
	void *datos,
	tipo_t *tipo_datos,
	boolean *dirty_datos
	 );
