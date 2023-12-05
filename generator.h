/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file generator.h

 * @brief Generator definitions and header file for generator.c - generator of code

 * @author Maksym Podhornyi - xpodho08
 */

#include <stdio.h>
#include <stdbool.h>

#include "symtable.h"
#include "scanner.h"
#include "expression.h"

#ifndef GENERATOR_H
#define GENERATOR_H

#define GENERATE(...)             \
  do                              \
  {                               \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n");        \
  } while (0)

#define GENERATE_EMPTY_LINE() \
  do                          \
  {                           \
    fprintf(stdout, "\n");    \
  } while (0)

#define GENERATE_WITHOUT_NEW_LINE(...) \
  do                                   \
  {                                    \
    fprintf(stdout, __VA_ARGS__);      \
  } while (0)


/**
 * @brief Generate header of the code,
 * define all built in functions and global variables
 * and jump to main function.
 *
 * @return void
 */
void generator_start(void);

/**
 * @brief Generate end of the code
 *
 * @return void
 */
void generator_end(void);

/**
 * @brief Generate code for variable declaration
 *
 * @param id - name of variable
 * @return void
 */
void generate_var_declaration(char *id);

/**
 * @brief Generate code for variable definition
 *
 * @param id - name of variable
 * @param type - type of variable
 * @return void
 */
void generate_var_definition(char *id, data_type type);

/**
 * @brief Generate code for variable assignment
 * from stack to variable
 *
 * @param id - name of variable
 * @return void
 */
void generate_var_assignment(char *id);

/**
 * @brief Generate code to read variable from stdin
 *
 * @param id - name of variable
 * @param type - type of variable to read
 *
 * @return void
 */
void generate_read(char *id, data_type type);

/**
 * @brief Generate code to write value of variable
 * exp_result to stdout
 *
 * @return void
 */
void generate_write_val(void);

/**
 * @brief Push value of variable to stack
 *
 * @param token - token with value of variable
 */
void gen_push(token_t *token);

/**
 * @brief Remove value from stack
 *
 * @return void
 */
void gen_pop(void);

/**
 * @brief Generate code to assign value of variable
 *
 * @param token - token with value of variable
 * @return void
 */
void gen_term(token_t *token);

/**
 * @brief Generate code with operation for stack
 *
 * @param rule - rule of operation
 * @return void
 */
void gen_operation(rules rule);

/**
 * @brief Generate code for int to double conversion
 * from stack
 *
 * @return void
 */
void gen_int2double(void);

/**
 * @brief Generate code for double to int conversion
 * from stack
 *
 * @return void
 */
void gen_double2int(void);

/**
 * @brief Generate code for concatenation of two strings
 * from stack
 *
 * @return void
 */
void gen_concat(void);

/**
 * @brief Generate code for int to double conversion
 * with two operands
 *
 *
 * @return void
 */
void gen_int2double_2op(void);

void gen_call_start(void);
void add_param_to_call(char* param_name);
void gen_call(char* function_name);

// Function generation
void generate_function_start(char* function_name);
void generate_function_return_param(data_type type);
void generate_function_end(char* function_name);
void generate_function_param(char *param_name, data_type type);
void generate_function_void_return(void);
void generate_function_return(void);

// If
void gen_if_let(char *id, int let_if_counter);
void gen_if_start(int if_counter);
void gen_if_else(int if_counter);
void gen_if_end(int if_counter);

// While
void gen_while_start(int while_counter);
void gen_while(int while_counter);
void gen_while_end(int while_counter);


#endif //GENERATOR_H
