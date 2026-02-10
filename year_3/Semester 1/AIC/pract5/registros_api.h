 valor_t lee_int_reg ( 
	dword n,
	tipo_t *tipo,
	boolean *dirty
	 );
 void marca_int_reg ( 
	dword n,
	tipo_t tipo_dato,
	boolean activar_dirty
	 );
 void escribe_int_reg ( 
	dword n,
	xword dato,
	tipo_t tipo_dato,
	boolean activar_dirty
	 );
 valor_t lee_fp_reg ( 
	dword n,
	tipo_t *tipo,
	boolean *dirty
	 );
 void marca_fp_reg ( 
	dword n,
	tipo_t tipo_dato,
	boolean activar_dirty
	 );
 void escribe_fp_reg ( 
	dword n,
	valor_t dato,
	tipo_t tipo_dato,
	boolean activar_dirty
	 );
