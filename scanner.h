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
    NEW_TOKEN_S,
    STRING_S,
    ESCAPE_S,
    MAYBE_UNICODE_S,
    UNICODE_S,
    MAYBE_MULTILINE_S,
    MAYBE_MULTILINE_START_S,
    MULTILINE_STRING_S,
    MAYBE_MULTILINE_END_START_S,
    MAYBE_MULTILINE_END_S,
    MAYBE_MULTILINE_END_FINAL_S,
    MULTILINE_END_S,
    NUMBER_S,
    DOT_S,
    DOUBLE_S,
    EXPONENT_S,
    EXPONENT_SIGN_S,
    EXPONENT_FINAL_S,
    KEYWORD_OR_IDENTIFIER_S,
    NIL_S,
    DIVIDE_S,
    COMMENT_S,
    BLOCK_COMMENT_S,
    BLOCK_COMMENT_POSSIBLE_END_S,
    BLOCK_COMMENT_END_S,
    ASSIGNMENT_S,
    MAYBE_MORE_THAN_S,
    MAYBE_LESS_THAN_S,
    MINUS_S,
    NOT_S,
} scanner_states_t;

typedef enum token_type {
    KEYWORD,
    IDENTIFIER,
    TOKEN_EOF,
    TOKEN_EOL,
    BUILT_IN_FUNCTION,
    TOKEN_FUNCTION_TYPE,

    // values of variables
    STRING_VALUE,
    INT_VALUE,
    NIL_VALUE, // ?
    DOUBLE_VALUE,

    // relation operators
    NOT_EQUAL, // !=
    EQUAL, // ==
    LESS_THAN, // <
    LESS_THAN_OR_EQUAL, // <=
    MORE_THAN, // >
    MORE_THAN_OR_EQUAL, // >=

    // operators
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
    ASSIGNMENT // =
} token_type_t;

typedef enum keyword{
    Double_KW,
    Else_KW,
    Function_KW,
    If_KW,
    Int_KW,
    Let_KW,
    Nil_KW,
    Return_KW,
    String_KW,
    Var_KW,
    While_KW,
    IntNullable_KW,
    StringNullable_KW,
    DoubleNullable_KW,
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
