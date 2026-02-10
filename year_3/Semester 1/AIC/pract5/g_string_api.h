 int g_str_starts_with ( 
	const char *haystack,
	const char *needle
	 );
 int g_str_ends_with ( 
	const char *haystack,
	const char *needle
	 );
 void g_str_init ( 
	g_string_t *g_str
	 );
 void g_str_destroy ( 
	g_string_t *g_str
	 );
 void g_str_clear ( 
	g_string_t *g_str
	 );
 size_t g_str_alloc ( 
	g_string_t *g_str,
	size_t required_size
	 );
 char *g_str_printf ( 
	g_string_t *g_str,
	const char *format,
	...
	 );
 size_t g_str_n_copy ( 
	g_string_t *g_str,
	const char *src,
	size_t length
	 );
 size_t g_str_copy ( 
	g_string_t *g_str,
	const char *src
	 );
 size_t g_str_repeat ( 
	g_string_t *g_str,
	char ch,
	size_t times
	 );
 size_t g_str_indent ( 
	g_string_t *g_str,
	const char *src,
	size_t indent,
	size_t first
	 );
 size_t g_str_write ( 
	FILE *file,
	g_string_t *g_str
	 );
 size_t g_str_flush ( 
	FILE *file,
	g_string_t *g_str
	 );
