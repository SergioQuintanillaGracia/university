 dword operacion_ALU ( 
	int codop,
	dword in1,
	dword in2
	 );
 dword operacion_COMP ( 
	uint8_t codop,
	dword in1,
	dword in2
	 );
 dword mux_ALUsup ( 
	dword pc,
	dword ra,
	dword mem,
	dword wb
	 );
 dword mux_ALUinf ( 
	dword rb,
	dword imm,
	dword mem,
	dword wb
	 );
 dword mux_COMPsup ( 
	dword ra,
	dword mem,
	dword wb
	 );
 dword mux_COMPinf ( 
	dword rb,
	dword mem,
	dword wb
	 );
 dword mux_EXmem ( 
	dword rb,
	dword wb
	 );
 dword mux_MEMmem ( 
	dword rb,
	dword wb
	 );
 dword mux_COMPidsup ( 
	dword ra,
	dword mem
	 );
 dword mux_COMPidinf ( 
	dword rb,
	dword mem
	 );
 void detectar_riesgos_control (void);
 void detectar_riesgos_datos (void);
 void detectar_riesgos_RAW_INT (void);
 void detectar_riesgos_WAW_INT (void);
 void detectar_riesgos_WB_INT (void);
