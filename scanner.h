/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file scanner.h

 * @brief Scanner definitions
 
 * @author Juraj Remeň - xremen02
*/

#include "error.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"

#ifndef SCANNER_H
#define SCANNER_H

typedef enum scanner_states {
    NEW_TOKEN,
    STRING,
    ESCAPE,
    HEX_STRING,
    LEFT_BRACKET,
    PARAMETER_ID,
    PARAMETER_TYPE,
    RIGHT_BRACKET,
    NUMBER,
    DOT,
    EXPONENT,
    SEPARATOR,
    RIGHT_CURLY_BRACKET,
    LEFT_CURLY_BRACKET,
    FUNCTION_TYPE,
    COMMENT,
    BLOCK_COMMENT,
    BLOCK_COMMENT_END
} scanner_states_t;

typedef enum token_type {
    KEYWORD,
    IDENTIFIER,
    TOKEN_EOF,
    TOKEN_EOL,

    // values of variables
    NUMBER_VALUE,
    STRING_VALUE,
    INT_VALUE,
    NIL_VALUE, // ?

    // relation operators
    NOT_EQUAL, // !=
    EQUAL, // ==
    LESS_THAN, // <
    LESS_THAN_OR_EQUAL, // <=
    MORE_THAN, // >
    MORE_THAN_OR_EQUAL, // >=

    // operators
    ASSIGNMENT, // =
    PLUS, // +
    MINUS, // -
    MUL, // *
    DIV, // /
    NIL_COLL, // ??

    // others
    TOKEN_LEFT_BRACKET, // (
    TOKEN_RIGHT_BRACKET, // )
    TOKEN_LEFT_CURLY_BRACKET, // {
    TOKEN_RIGHT_CURLY_BRACKET, // }
    COMMA, // ,
    COLON, // :
    NOT, // !
} token_type_t;

typedef union token_data {
    long long int Int;
    double Double;
    char *String;
} token_data_t;

typedef struct token {
    enum token_type type;
    struct token *prev;
    struct token *next;
    token_data_t data;
    unsigned long long line;
} token_t;

token_t * get_token(FILE *file, unsigned long long *line, token_t *token);
token_t * get_first_token(token_t *token);
void free_tokens(token_t *first_token);
token_type_t get_token_types(char *token_string);

#endif // SCANNER_H