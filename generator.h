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

/** @brief Macro for generating code */
#define GENERATE(...)             \
  do                              \
  {                               \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n");        \
  } while (0)

/** @brief Macro for generating empty line */
#define GENERATE_EMPTY_LINE() \
  do                          \
  {                           \
    fprintf(stdout, "\n");    \
  } while (0)

/** @brief Macro for generating code without new line */
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

/**
 * @brief Start generating call to function
 *
 * @return void
 */
void gen_call_start(void);

/**
 * @brief Add parameter to function call
 *
 * @param param_name - id of parameter
 *
 * @return void
 */
void add_param_to_call(char* param_name);

/**
 * @brief Generate code for function call
 *
 * @param function_name - name of function to call
 *
 * @return void
 */
void gen_call(char* function_name);

/**
 * @brief Start generating user function
 *
 * @param function_name - name of function
 *
 * @return void
 */
void generate_function_start(char* function_name);

/**
 * @brief Generate code for function return
 *
 * @param type - type of return value
 *
 * @return void
 */
void generate_function_return_param(data_type type);

/**
 * @brief End generating user function
 *
 * @param function_name - name of function
 *
 * @return void
 */
void generate_function_end(char* function_name);

/**
 * @brief Generate code for function parameter
 *
 * @param param_name - name of parameter
 * @param type - type of parameter
 *
 * @return void
 */
void generate_function_param(char *param_name, data_type type);

/**
 * @brief Generate code for function return if
 * return type is void
 *
 * @return void
 */
void generate_function_void_return(void);

/**
 * @brief Generate code for function return if
 * return type is not void
 *
 * @return void
 */
void generate_function_return(void);

/**
 * @brief Generate code for check if variable is nil
 *
 * @param id - name of variable
 * @param let_if_counter - unique number of let/if
 *
 * @return void
 */
void gen_if_let(char *id, int let_if_counter);

/**
 * @brief Start generating if
 *
 * @param if_counter - unique number of if
 *
 * @return void
 */
void gen_if_start(int if_counter);

/**
 * @brief Generate code for else part of if
 *
 * @param if_counter - unique number of if
 *
 * @return void
 */
void gen_if_else(int if_counter);

/**
 * @brief Generate code for end of if
 *
 * @param if_counter - unique number of if
 *
 * @return void
 */
void gen_if_end(int if_counter);

/**
 * @brief Start generating while
 *
 * @param while_counter - unique number of while
 *
 * @return void
 */
void gen_while_start(int while_counter);

/**
 * @brief Generate code for while body
 *
 * @param while_counter - unique number of while
 *
 * @return void
 */
void gen_while(int while_counter);

/**
 * @brief Generate code for end of while
 *
 * @param while_counter - unique number of while
 *
 * @return void
 */
void gen_while_end(int while_counter);

#endif //GENERATOR_H
