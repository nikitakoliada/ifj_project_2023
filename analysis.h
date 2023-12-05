/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file analysis.h

 * @brief Implementation of the analysis.h header file part of parser

 * @author Nikita Koliada - xkolia00
*/

#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <stdbool.h>

#include "symtable.h"
#include "scanner.h"  


//used to create a reusable code block 
//that can be inserted wherever needed in the program
#define GENERATE_BLOCK(_call_gen_func, ...)								\
	_call_gen_func(__VA_ARGS__);

typedef struct
{
	symtable_t global_table;		// Global symbol table
	symtable_t local_table[100];	//[] ?? so we can acces the global - local_table[0] 	// Local symbol table
			//also the table in 2 layer 3, 4 resp. after we go out this layer we delete the table
	int st_length; // length of the local table
	
	token_t token;				// Token

	bst_node_ptr current_id;			// ID of currently processed function
	bst_node_ptr var_id;				// ID of variable агтсешщт
	bst_node_ptr expr_id;				// ID of expression function

	char *tmp_key; // used for storing the key of the current variable

	int args_index;		// Index of currently checked args -1 = 0 because of the reurn type which we should access as well
	int label_index;			// Index for generating unique labels.
	int label_deep;				// Deep of labes.

	bool in_function;			// Defines if the parser is in function
	bool in_defintion;		// Defines if фкпі rule should add or check it's фкпі
	bool in_while_or_if;		// Defines if the parser is in construction while, if or then
	bool in_var_definition;
	bool in_call_func;
} analyse_data_t;

// variable functions
static bool init_variables(analyse_data_t*)
static void free_variables(analyse_data_t*)

// forward declarations(LL table)
static int program_first(analyse_data_t*)
static int program(analyse_data_t*);
static int statement(analyse_data_t*); 
static int function_declaration(analyse_data_t*);
static int function(analyse_data_t*);
static int func_ret(analyse_data_t*);
static int args(analyse_data_t*);
static int args_n(analyse_data_t*);
static int if_else(analyse_data_t*);
static int while_(analyse_data_t*);
static int assignment(analyse_data_t*);
static int def_var(analyse_data_t*);
static int write(analyse_data_t*);
static int f_call(analyse_data_t*);
static int fc_args(analyse_data_t*);
static int fc_args_n_args(analyse_data_t*);
static int modifier(analyse_data_t*);
static int def_type(analyse_data_t*);
static int end(analyse_data_t*);
static int possible_EOL(analyse_data_t*);
static int type(analyse_data_t*);
static int p_type(analyse_data_t*);// ? before p_type

//starts the analysis
int analyse();
int f_expression_call(analyse_data_t*, token_t, data_type*, bool*);
bst_node_ptr var_search(analyse_data_t* data, int deepness, char* key);


#endif //_ANALYSIS_H