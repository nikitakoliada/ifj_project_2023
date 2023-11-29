/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file analysis.h

 * @brief Implementation of the analysis.h header file part of parser

 * @author Nikita Koliada xkolia00
*/

#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <stdbool.h>

#include "symtable.h"
#include "scanner.h"  



//used to create a reusable code block 
//that can be inserted wherever needed in the program
#define GENERATE_CODE(_callback, ...)								\
	if (!_callback(__VA_ARGS__)) return ERROR_INTERNAL

typedef struct
{
	symtable_t global_table;		// Global symbol table
	symtable_t local_table;		// Local symbol table

	token_t token;				// Token

	bst_node_ptr current_id;			// ID of currently processed function
	bst_node_ptr var_id;				// ID of variable агтсешщт
	bst_node_ptr expr_id;				// ID of expression function

	unsigned args_index;		// Index of currently checked args
	int label_index;			// Index for generating unique labels.
	int label_deep;				// Deep of labes.

	bool in_defintion;		// Defines if фкпі rule should add or check it's фкпі
	bool in_while_or_if;		// Defines if the parser is in construction while, if or then
} analyse_data_t;

//starts the analysis
int analyse();
int f_expression_call(analyse_data_t*, token_t, data_type*);


#endif //_ANALYSIS_H