/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file generator.h

 * @brief Generator definitions and header file for generator.c - generator of code

 * @author Juraj Remeň - xremen02
 */

#include <stdio.h>
#include <stdbool.h>

#ifndef GENERATOR_H
#define GENERATOR_H

#define GENERATE(...)             \
  do                              \
  {                               \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n");        \
  } while (0)

void generate_readString();
void generate_readInt();
void generate_readDouble();
void generate_Double2Int();
void generate_length();
void generate_substring();
void generate_ord();
void generate_chr();
void define_built_in_functions();

#endif //GENERATOR_H
