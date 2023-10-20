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

#define DEFAULT_TOKEN_LENGTH 256

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
    COMMENT,
    BLOCK_COMMENT,
    BLOCK_COMMENT_END,
    KEYWORD_OR_IDENTIFIER,
    OPERATOR,
    NIL,
    FUNCTION_TYPE
} scanner_states_t;

typedef enum token_type {
    KEYWORD,
    IDENTIFIER,
    TOKEN_EOF,
    TOKEN_EOL,
    BUILT_IN_FUNCTION,
    TOKEN_OPERATOR,
    TOKEN_SEPARATOR,
    TOKEN_FUNCTION_TYPE,

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

token_t * scanner_get_first_token(token_t *token);
void scanner_free_tokens(token_t *first_token);
char * scanner_get_token_array(unsigned long long size);
char * scanner_add_char_to_token_array(char * array, unsigned long long *size, unsigned long long *i, int c);
token_type_t scanner_get_token_type(char *token, scanner_states_t actual_state);
token_t * scanner_get_token_struct(char *token_array, unsigned long long actual_line, scanner_states_t actual_state, token_t *prev_token);
token_t * scanner_get_token(FILE *file, unsigned long long *line, token_t *token);
token_type_t scanner_get_param_token_type(char *token_string);

#endif // SCANNER_H