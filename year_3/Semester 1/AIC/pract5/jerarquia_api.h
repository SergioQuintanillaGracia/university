 void inicializa_nivel_jerarquia ( 
	nivel_jerarquia_memoria_t *nivel,
	config_operaciones_t op_config,
	size_t size_oper
	 );
 boolean hay_operaciones_pendientes ( 
	struct nivel_jerarquia_memoria_struct *nivel
	 );
 boolean hay_operaciones_libres ( 
	struct nivel_jerarquia_memoria_struct *nivel
	 );
 operacion_nivel_t *reserva_operacion ( 
	struct nivel_jerarquia_memoria_struct *nivel
	 );
 void libera_operacion ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	operacion_nivel_t *oper
	 );
 void inicia_operacion_nivel ( 
	operacion_nivel_t *oper,
	nivel_jerarquia_memoria_t *src,
	tipo_operacion_memoria_t tipo,
	dword dir,
	size_t tam,
	ciclo_t orden,
	operacion_finalizada_fn callback,
	void *oper_data
	 );
 void suma_estadisticas ( 
	estadisticas_accesos_t *seccion,
	operacion_nivel_t *oper
	 );
 void actualiza_estadisticas ( 
	operacion_nivel_t *oper
	 );
 void muestra_estadisticas_nivel_txt ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	g_string_t *output
	 );
 void muestra_estadistica_tipo_txt ( 
	estadisticas_accesos_t *stat,
	g_string_t *output
	 );
 void muestra_estadisticas_nivel_html ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	T_COMPOSITE table
	 );
 void muestra_estadistica_tipo_html ( 
	estadisticas_accesos_t *stat,
	T_COMPOSITE tr
	 );
 void imprime_memoria_html (void);
