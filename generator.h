/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file generator.h

 * @brief Generator definitions and header file for generator.c - generator of code

 * @author Juraj Remeň - xremen02
 * @author Maksym Podhornyi - xpodho08
 */

#include <stdio.h>
#include <stdbool.h>
#include "symtable.h"
#include "scanner.h"

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

typedef struct
{
  char *source;
  char *target;
} function_param;

typedef struct
{
  char *name;
  size_t params_size;
  token_t **params;
  size_t returns_size;
  char **returns;
  function_param *pair;
} func_params_t;


//built-in function definitions
void generate_readString(func_params_t *params);
void generate_readInt(func_params_t *params);
void generate_readDouble(func_params_t *params);
void generate_Double2Int(func_params_t *params);
void generate_length(func_params_t *params);
void generate_substring(func_params_t *params);
void generate_write(void);
void generate_ord(func_params_t *params);
void generate_chr(func_params_t *params);
void define_built_in_functions(func_params_t *params);

void generate_var_declaration(char *id);
void generate_var_definition(char *id, data_type type);
void generate_var_assignment(char *id);

void generate_header(void);

void generate_function_start(func_params_t *params);
void generate_call(func_params_t *params);
void generate_function_end(func_params_t *params);
void generate_return(func_params_t *params);
void generate_return_params(func_params_t *params);

#endif //GENERATOR_H
