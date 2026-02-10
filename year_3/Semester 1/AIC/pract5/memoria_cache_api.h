 int comprueba_config_memoria_cache ( 
	config_cache_t c_config
	 );
 void inicializa_memoria_cache ( 
	memoria_cache_t *cache,
	config_cache_t c_config,
	config_operaciones_t op_config
	 );
 boolean inicia_oper_mem_cache ( 
	struct nivel_jerarquia_memoria_struct *dst,
	tipo_operacion_memoria_t tipo,
	struct nivel_jerarquia_memoria_struct *src,
	dword dir,
	size_t tam,
	ciclo_t orden,
	operacion_finalizada_fn callback,
	void *oper_id
	 );
 void termina_oper_mem_cache ( 
	memoria_cache_t *cache,
	operacion_mem_cache_t *oper
	 );
 void actualiza_estado_mem_cache ( 
	struct nivel_jerarquia_memoria_struct *nivel
	 );
 void finaliza_oper_mem_cache ( 
	operacion_nivel_t *oper_sig,
	void *datos,
	tipo_t *tipos,
	boolean *dirty
	 );
 void cancela_operaciones_mem_cache ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	tipo_operacion_memoria_t tipo
	 );
 void vuelca_datos_cache ( 
	struct nivel_jerarquia_memoria_struct *nivel
	 );
 void obtiene_config_memoria_cache ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	formato_presentacion_t formato,
	g_string_t *info
	 );
 void obtiene_config_memoria_cache_txt ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	g_string_t *info
	 );
 void obtiene_config_memoria_cache_html ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	g_string_t *info
	 );
 void muestra_estado_memoria_cache ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	formato_presentacion_t formato,
	g_string_t *output
	 );
 void muestra_estado_memoria_cache_html ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	g_string_t *output
	 );
