/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file error.h

 * @brief Error definitions
 
 * @author Juraj Remeň - xremen02
*/

#ifndef ERROR_H
#define ERROR_H

// HELPERS
#define ERROR_EXIT(msg, err_code)   \
    do                              \
    {                               \
        fprintf(stderr, msg);       \
        exit(err_code);             \
    }                               \
    while (0);                      \

// Error codes
#define SYNTAX_OK 0
#define TOKEN_OK 0
#define LEX_ERROR 1 // structure of current lexeme error
#define SYNTAX_ERROR 2 // wrong syntax of program
#define SEM_ERROR_UNDEF_FUNC 3 // undefined function
#define SEM_ERROR_PARAM 4 // wrong amount or type of parameters in function call or wrong type of return value of function
#define SEM_ERROR_UNDEF_VAR 5 // use of undefined variable
#define SEM_ERROR_EXPR 6 // missing or extra expression in return from function
#define SEM_ERROR_TYPE_COMPAT 7 // type compatibility in arithmetic, string or relation expressions
#define SEM_ERROR_TYPE_DERIV 8 // type of variable or parameter isn't specified and can't be derivative from used expression
#define SEM_ERROR_OTHER 9 // other semantic errors
#define INTERNAL_ERROR 99 // internal error of compiler not caused by input program (memory allocation, etc.)

#endif // ERROR_H