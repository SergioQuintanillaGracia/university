 size_t tam_dato ( 
	tipo_t tipo
	 );
 dword mem_direccion_alineada ( 
	dword direccion,
	int size
	 );
 void mem_etiqueta_datos ( 
	tipo_t *tipo_datos,
	boolean *dirty_datos,
	dword offset,
	tipo_t tipo,
	boolean activar_dirty
	 );
 void mem_etiqueta ( 
	mapa_memoria_t *mm,
	region_t r,
	dword offset,
	tipo_t tipo,
	boolean activar_dirty
	 );
 void inicializa_zona_datos ( 
	zona_datos_t *zona,
	size_t tam,
	tipo_region_t tipo
	 );
 void inicializa_memoria ( 
	mapa_memoria_t *mm
	 );
 void destruye_memoria ( 
	mapa_memoria_t *mm
	 );
 region_t dir_a_region ( 
	mapa_memoria_t *mm,
	dword dir
	 );
 int mem_chequea_direccion ( 
	mapa_memoria_t *memoria,
	dword addr,
	tipo_t tipo
	 );
 valor_t mem_lee_region_datos ( 
	mapa_memoria_t *mm,
	region_t r,
	dword offset,
	tipo_t tipo,
	boolean *dirty
	 );
 valor_t _lee_mem_datos ( 
	mapa_memoria_t *mm,
	dword direccion,
	tipo_t tipo_dato,
	boolean *dirty,
	char *file,
	int line
	 );
 boolean comprueba_direccion_datos ( 
	mapa_memoria_t *mm,
	dword direccion
	 );
 void mem_escribe_region_datos ( 
	mapa_memoria_t *mm,
	region_t r,
	dword offset,
	valor_t valor,
	tipo_t tipo_dato,
	boolean activar_dirty
	 );
 void escribe_mem_datos ( 
	mapa_memoria_t *mm,
	dword direccion,
	valor_t valor,
	tipo_t tipo_dato,
	boolean activar_dirty
	 );
 char *obtiene_ensamblador ( 
	mapa_memoria_t *mm,
	word indice
	 );
 instruccion_t obtiene_instruccion ( 
	mapa_memoria_t *mm,
	word indice
	 );
 word indice_instruccion ( 
	mapa_memoria_t *mm,
	dword direccion
	 );
 instruccion_t mem_lee_region_instruc ( 
	mapa_memoria_t *mm,
	region_t r,
	dword offset
	 );
 instruccion_t _lee_mem_instruc ( 
	mapa_memoria_t *mm,
	dword direccion,
	char *file,
	int line
	 );
 boolean comprueba_direccion_instruc ( 
	mapa_memoria_t *mm,
	dword direccion
	 );
 void mem_escribe_region_instruc ( 
	mapa_memoria_t *mm,
	region_t r,
	dword offset,
	instruccion_t instruccion
	 );
 void escribe_mem_instruc ( 
	mapa_memoria_t *mm,
	dword direccion,
	instruccion_t instruccion
	 );
