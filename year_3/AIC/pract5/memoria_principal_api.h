 int comprueba_config_memoria_principal ( 
	config_memoria_principal_t m_config
	 );
 void inicializa_memoria_principal ( 
	memoria_principal_t *memoria,
	config_memoria_principal_t m_config,
	config_operaciones_t op_config
	 );
 boolean inicia_oper_mem_principal ( 
	struct nivel_jerarquia_memoria_struct *dst,
	tipo_operacion_memoria_t tipo,
	struct nivel_jerarquia_memoria_struct *src,
	dword dir,
	size_t tam,
	ciclo_t orden,
	operacion_finalizada_fn callback,
	void *oper_id
	 );
 void termina_operacion_mem_principal ( 
	memoria_principal_t *memoria,
	operacion_mem_principal_t *oper
	 );
 void actualiza_estado_mem_principal ( 
	struct nivel_jerarquia_memoria_struct *nivel
	 );
 void vuelca_datos_mem_principal ( 
	struct nivel_jerarquia_memoria_struct *nivel
	 );
 void obtiene_config_memoria_principal ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	formato_presentacion_t formato,
	g_string_t *info
	 );
 void obtiene_config_memoria_principal_txt ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	g_string_t *info
	 );
 void muestra_estado_memoria_principal ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	formato_presentacion_t formato,
	g_string_t *output
	 );
 void muestra_estado_memoria_principal_html ( 
	struct nivel_jerarquia_memoria_struct *nivel,
	g_string_t *output
	 );
