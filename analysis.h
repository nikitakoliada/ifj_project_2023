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
	symtable_t global_table;		/// Global symbol table
	symtable_t local_table;		/// Local symbol table

	token_data_t token;				/// Token

	bst_node_ptr* current_id;			/// ID of currently processed function
	bst_node_ptr* lhs_id;				/// ID of left-hand-side variable
	bst_node_ptr* rhs_id;				/// ID of right-hand-side function (expression?)

	unsigned param_index;		/// Index of currently checked param
	int label_index;			/// Index for generating unique labels.
	int label_deep;				/// Deep of labes.

	bool in_function;			/// Defines if the parser is in function
	bool in_declaration;		/// Defines if param rule should add or check it's params
	bool in_while_or_if;		/// Defines if the parser is in construction while, if or then
	bool non_declared_function;	/// Function that has been only defined
} analyse_data_t;

/**
 * Starts syntactic and semantic anlysis.
 *
 * @return Appropriate error code.
 */
int analyse();


#endif //_ANALYSIS_H