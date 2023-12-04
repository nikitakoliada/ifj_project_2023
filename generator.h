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


//built-in function definitions
void generate_readString(void);
void generate_readInt(void);
void generate_readDouble(void);
void generate_Double2Int(void);
void generate_Int2Double(void);
void generate_length(void);
void generate_substring(void);
void generate_write(void);
void generate_ord(void);
void generate_chr(void);
void define_built_in_functions(void);

// All about variables
void generate_var_declaration(char *id);
void generate_var_definition(char *id, data_type type);
void generate_var_assignment(char *id);

// Read and write
void generate_read(char *id, data_type type);
void generate_write_var(char *id);

// Stack
void gen_push(token_t *token);
void gen_term(token_t *token);
void gen_operation(rules rule);
void gen_int2double(void);
void gen_double2int(void);
void gen_concat(void);
void gen_int2double_2op(void);

// Call
void gen_call_start(void);
void add_param_to_call(char* param_name);
void gen_call(char* function_name);

// Start and end
void generator_start(void);
void generator_end(void);

// Function generation
void generate_function_start(char* function_name);
void generate_function_return_param(data_type type);
void generate_function_end(char* function_name);

// If
void gen_if_start(int if_counter);
void gen_if_else(int if_counter);
void gen_if_end(int if_counter);

// While
void gen_while_start(int while_counter);
void gen_while(int while_counter);
void gen_while_end(int while_counter);


#endif //GENERATOR_H
