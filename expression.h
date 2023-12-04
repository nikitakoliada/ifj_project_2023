/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file expression.h

 * @brief Implementation of the expression.h header file part of parser

 * @author Pavlo Butenko - xbuten00
*/

#include "analysis.h"
#include <stdbool.h>

typedef enum{
    EQ,    // ==
    NEQ,    // !=
    G,  // >
    GEQ,    // >=
    L,  // <
    LEQ,    // <=
    PLUS_R,   // +
    MINUS_R,  // -
    MUL_R,    // *
    DIV_R,    // /
    IDIV_R,   // //
    NOT_NIL_R,    // !
    NOT_NULL_R,    // ??
}rules;

typedef enum{
    IntS,
    DoubleS,
    StringS,
    IdS,
    FunctionS,
    PlusS,
    MinusS, 
    MultS,
    DivS,
    EqS,     // ==
    NEqS,    // !=
    GEqS,    // >=
    LEqs,    // <=
    GreaterS, // >
    LessS,   // <
    NotS,    // !
    NilCS,  // ??
    LPS,     // (
    RPS,     // )
    DollarS, // $
    NON_TERM,
    NilS,
    Handle
}eSymbol;

int expression(analyse_data_t*, bool*);