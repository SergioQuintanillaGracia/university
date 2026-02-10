 table_state_t *ht_create ( 
	int size
	 );
 void ht_set ( 
	table_state_t *table_state,
	char *key,
	char *text,
	char *class
	 );
 table_value_t *ht_get ( 
	table_state_t *table_state,
	char *key
	 );
