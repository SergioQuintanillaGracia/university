 void g_opt_fatal_error ( 
	g_options_t *opts,
	show_help_t show_mode,
	const char *param,
	const char *format,
	...
	 );
 g_section_t *g_opt_init ( 
	g_options_t *options,
	int argc,
	char *argv[]
	 );
 g_section_t *g_opt_add_section ( 
	g_options_t *options,
	const char *title
	 );
 g_integer_option_t *g_opt_add_mandatory_integer ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	int *p_value
	 );
 g_float_option_t *g_opt_add_mandatory_float ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	float *p_value
	 );
 g_string_option_t *g_opt_add_mandatory_string ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	char **p_value
	 );
 g_enum_option_t *g_opt_add_mandatory_enum ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	int *p_value
	 );
 g_custom_option_t *g_opt_add_mandatory_custom ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	void *p_value
	 );
 g_flag_option_t *g_opt_add_optional_flag ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	boolean *p_value,
	boolean default_value
	 );
 g_integer_option_t *g_opt_add_optional_integer ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	int *p_value,
	int default_value
	 );
 g_float_option_t *g_opt_add_optional_float ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	float *p_value,
	float default_value
	 );
 g_string_option_t *g_opt_add_optional_string ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	char **p_value,
	char *default_value
	 );
 g_enum_option_t *g_opt_add_optional_enum ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	int *p_value,
	int default_value
	 );
 g_custom_option_t *g_opt_add_optional_custom ( 
	g_section_t *section,
	char *opt_short,
	char *opt_long,
	char *description,
	void *p_value,
	void *default_value,
	size_t size
	 );
 g_enum_case_t *g_opt_add_enum_case ( 
	g_enum_option_t *opt_enum,
	const char *label,
	const char *description,
	int value
	 );
 void g_opt_show_about ( 
	g_options_t *options
	 );
 void g_opt_print_option ( 
	g_base_option_t *p_option,
	g_string_t *help,
	const int tab,
	size_t maxlength
	 );
 void g_opt_show_help ( 
	g_options_t *options
	 );
 void g_opt_show_option ( 
	g_options_t *options,
	const char *param
	 );
 int g_opt_parse ( 
	g_options_t *options,
	int argn
	 );
