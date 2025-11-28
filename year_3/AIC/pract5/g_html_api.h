 void h_set_compact_mode ( 
	boolean compact
	 );
 boolean h_get_compact_mode (void);
 void h_init_base_tag ( 
	h_tag_base_t *tag,
	const char *name,
	h_tag_type_t type
	 );
 T_SINGLE h_new_single_tag ( 
	const char *name
	 );
 T_COMPOSITE h_new_composite_tag ( 
	const char *name
	 );
 T_BASE h_set_attr ( 
	T_BASE tag,
	const char *attr
	 );
 T_BASE h_add_attr ( 
	T_BASE tag,
	const char *attr
	 );
 T_SINGLE h_add_text ( 
	T_SINGLE tag,
	const char *text
	 );
 T_SINGLE h_add_attr_text ( 
	T_SINGLE tag,
	const char *attr,
	const char *text
	 );
 T_BASE h_add_item ( 
	T_COMPOSITE parent,
	T_BASE item
	 );
 void h_tag_flush ( 
	T_BASE tag,
	g_string_t *str
	 );
 void h_destroy_base_tag ( 
	T_BASE tag
	 );
 void h_destroy_single_tag ( 
	T_SINGLE tag
	 );
 void h_destroy_composite_tag ( 
	T_COMPOSITE tag
	 );
 void h_destroy_tag ( 
	T_BASE tag
	 );
 T_COMPOSITE g_html_init ( 
	g_html_t *html,
	const char *name
	 );
 void g_html_flush ( 
	g_html_t *html,
	FILE *output
	 );
 void g_html_destroy ( 
	g_html_t *html
	 );
