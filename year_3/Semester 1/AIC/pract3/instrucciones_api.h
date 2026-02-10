 int bloque_instruccion ( 
	dword PC
	 );
 void color_instruccion ( 
	char *color,
	dword PC
	 );
 void itobin4 ( 
	dword n,
	char *buffer,
	int digits
	 );
 void itobin ( 
	dword n,
	char *buffer,
	int digits
	 );
 codop_t fn_riscv_decode ( 
	dword PC,
	unsigned int instr,
	formato_t *fmt,
	int *opcode,
	int *rs1,
	int *rs2,
	int *rs3,
	int *rd,
	int *rm,
	int *imm,
	char *nemo,
	char *full
	 );
 void imprime_instruccion_color ( 
	char *instruc,
	dword PC,
	boolean encolor
	 );
 void extrae_campo ( 
	char *texto,
	uword valor,
	int b2,
	int b1
	 );
 void muestra_campo ( 
	g_string_t *binario,
	g_string_t *campos,
	uword dump,
	modo_formato_t format_mode,
	char *texto,
	char *color,
	int b2,
	int b1
	 );
 void muestra_formato ( 
	g_string_t *resultado,
	formato_t formato,
	uword dump,
	modo_formato_t format_mode
	 );
 void imprime_formato ( 
	g_string_t *formato,
	dword PC,
	modo_formato_t format_mode
	 );
 void genera_MInstruccion ( 
	programa_t *prog
	 );
