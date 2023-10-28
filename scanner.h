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
    ONE_LINE_STRING,
    END_STRING,
    MAYBE_MULTILINE,
    MULTILINE,
    MULTILINE_ESCAPE,
    MULTILINE_MAYBE_UNICODE,
    MULTILINE_UNICODE,
    LEFT_BRACKET,
    PARAMETER_ID,
    PARAMETER_TYPE,
    RIGHT_BRACKET,
    NUMBER,
    DOT,
    DOUBLE,
    EXPONENT,
    EXPONENT_SIGN,
    EXPONENT_FINAL,
    KEYWORD_OR_IDENTIFIER,
    OPERATOR,
    NIL,
    FUNCTION_TYPE,
    COMMENT,
    BLOCK_COMMENT,
    BLOCK_COMMENT_END,
    RIGHT_CURLY_BRACKET,
    LEFT_CURLY_BRACKET
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

typedef enum keyword{
    Double_KW,
    Else_KW,
    Function_KW,
    If_KW,
    Int_KW,
    Return_KW,
    String_KW,
    Var_KW,
    While_KW,
    Nil_KW,
    Let_KW,
} keyword_t;

typedef union token_data {
    long long int Int;
    double Double;
    char *String;
    keyword_t Keyword;
} token_data_t;

typedef struct token {
    enum token_type type;
    token_data_t data;
    unsigned long long line;
} token_t;

void set_source_file(FILE *file);
int get_next_token(token_t *token);

#endif // SCANNER_H