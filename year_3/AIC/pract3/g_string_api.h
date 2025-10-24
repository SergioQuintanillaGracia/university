 void g_str_init ( 
	g_string_t *g_str
	 );
 void g_str_destroy ( 
	g_string_t *g_str
	 );
 size_t g_str_printf ( 
	g_string_t *g_str,
	g_mode_t g_mode,
	const char *format,
	...
	 );
 size_t g_str_copy ( 
	g_string_t *g_str,
	g_mode_t g_mode,
	const char *src
	 );
 size_t g_str_write ( 
	FILE *file,
	g_string_t *g_str
	 );
 size_t g_str_flush ( 
	FILE *file,
	g_string_t *g_str
	 );
