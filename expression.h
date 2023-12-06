/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file expression.h

 * @brief Implementation of the expression.h header file part of parser

 * @author Pavlo Butenko - xbuten00
*/

#include "analysis.h"
#include <stdbool.h>

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
}eSymbol; // Symbol type

// Main expression function
// First argument accepts LL parser data
// Second argument sets if there was a new line after expression
int expression(analyse_data_t*, bool*);