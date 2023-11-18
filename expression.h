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