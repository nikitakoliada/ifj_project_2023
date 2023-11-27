#include "analysis.h"
#include <stdbool.h>

typedef enum{
    IntS,
    DoubleS,
    StringS,
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
    IdS,     
    LPS,     // (
    RPS,     // )
    DollarS, // $
    NON_TERM,
    NilS,
    Handle
}eSymbol;

int expression(analyse_data_t*, bool*);