/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file generator.c

 * @brief Generator implementation - generator of code

 * @author Juraj Remeň - xremen02
 * @author Maksym Podhornyi - xpodho08
 */

#include "generator.h"

/* ----- BUILT IN FUNCTIONS ----- */
/**
 * @brief BuiltIn function readString
 *
 * @return string without a line break symbol
 */
void generate_readString(func_params_t *params)
{
    (void)params;
    GENERATE("#readString");
    GENERATE("JUMP !_readString");
    GENERATE("LABEL !function_readString");
    GENERATE("PUSHFRAME");
    GENERATE("DEFVAR LF@%%retval0");

    GENERATE("READ LF@%%retval0 string");

    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_readString");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief BuiltIn function readInt
 *
 * @return integer
 */
void generate_readInt(func_params_t *params)
{
    (void)params;
    GENERATE("#readInt");
    GENERATE("JUMP !_readInt");
    GENERATE("LABEL !function_readInt");
    GENERATE("PUSHFRAME");
    GENERATE("DEFVAR LF@%%retval0");

    GENERATE("READ LF@%%retval0 int");

    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_readInt");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief BuiltIn function readDouble
 *
 * @return decimal number
 */
void generate_readDouble(func_params_t *params)
{
    (void)params;
    GENERATE("#readDouble");
    GENERATE("JUMP !_readDouble");
    GENERATE("LABEL !function_readDouble");
    GENERATE("PUSHFRAME");
    GENERATE("DEFVAR LF@%%retval0");

    GENERATE("READ LF@%%retval0 float");

    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_readDouble");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief BuiltIn function write
 *
 * @return void
 */

void generate_write(func_params_t *params)
{
    (void)params;
    GENERATE("#write");
    GENERATE("JUMP !_write");
    GENERATE("LABEL !function_write");
    GENERATE("PUSHFRAME");

    GENERATE("WRITE LF@%%0");

    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_write");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief BuiltIn function Int2Double
 *
 * @return The value of the integer parameter i converted to
 * a decimal value.
 *
 */

void generate_Int2Double(func_params_t *params)
{
    (void)params;
    GENERATE("#Int2Double");
    GENERATE("JUMP !_Int2Double");
    GENERATE("LABEL !function_Int2Double");
    GENERATE("PUSHFRAME");
    GENERATE("DEFVAR LF@%%retval0");

    GENERATE("MOVE LF@%%retval0 nil@nil");
    GENERATE("JUMPIFEQ nil$error_Int2Double nil@nil LF@_Int2Double$i");
    GENERATE("INT2FLOAT LF@%%retval0 LF@_Int2Double$i");

    GENERATE("LABEL nil$error_Int2Double");

    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_Int2Double");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief BuiltIn function Double2Int
 *
 * @return The value of the decimal parameter f converted to
 * an integer value by trimming the decimal part.
 *
 */
void generate_Double2Int(func_params_t *params)
{
    (void)params;
    GENERATE("#Double2Int");
    GENERATE("JUMP !_Double2Int");
    GENERATE("LABEL !function_Double2Int");
    GENERATE("PUSHFRAME");
    GENERATE("DEFVAR LF@%%retval0");

    GENERATE("MOVE LF@%%retval0 nil@nil");
    GENERATE("JUMPIFEQ nil$error_Double2Int nil@nil LF@_Double2Int$f");
    GENERATE("FLOAT2INT LF@%%retval0 LF@_Double2Int$f");

    GENERATE("LABEL nil$error_Double2Int");

    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_Double2Int");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief BuiltIn function length
 *
 * @return the length of given string
 */
void generate_length(func_params_t *params)
{
    (void)params;
    GENERATE("#length");
    GENERATE("JUMP !_length");
    GENERATE("LABEL !function_length");
    GENERATE("PUSHFRAME");
    GENERATE("DEFVAR LF@%%retval0");

    GENERATE("STRLEN LF@%%retval0 LF@%%0");

    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_length");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief BuiltIn function substr
 *
 * @return the substring of the specified string s.
 * If the index i or j is out of bounds or if j < i,
 * the function returns an empty string.
 * If any parameter is nil,
 * error 8 occurs.
 */

void generate_substring(func_params_t *params)
{
    (void)params;
    GENERATE("#substr");
    GENERATE("JUMP !_substr");
    GENERATE("LABEL !function_substr");
    GENERATE("PUSHFRAME");
    GENERATE("DEFVAR LF@%%retval0");
    GENERATE("DEFVAR LF@%%cond_range");
    GENERATE("DEFVAR LF@%%length");
    GENERATE("DEFVAR LF@%%char");

    GENERATE("MOVE LF@%%retval0 string@%%0");

    GENERATE("JUMPIFEQ nil$error_substr nil@nil LF@substr$s");
    GENERATE("JUMPIFEQ nil$error_substr nil@nil LF@substr$i");
    GENERATE("JUMPIFEQ nil$error_substr nil@nil LF@substr$j");

    GENERATE("LT LF@%%cond_range LF@substr$i int@1");
    GENERATE("JUMPIFEQ nil$return_substr LF@%%cond_range bool@true");
    GENERATE("STRLEN LF@%%length LF@substr$s");
    GENERATE("GT LF@%%cond_range LF@substr$i LF@%%length");
    GENERATE("JUMPIFEQ nil$return_substr LF@%%cond_range bool@true");

    GENERATE("LT LF@%%cond_range LF@substr$j int@1");
    GENERATE("JUMPIFEQ nil$return_substr LF@%%cond_range bool@true");
    GENERATE("GT LF@%%cond_range LF@substr$j LF@%%length");
    GENERATE("JUMPIFEQ nil$return_substr LF@%%cond_range bool@true");

    GENERATE("LT LF@%%cond_range LF@substr$j LF@substr$i");
    GENERATE("JUMPIFEQ nil$return_substr LF@%%cond_range bool@true");

    GENERATE("SUB LF@substr$i LF@substr$i int@1");
    GENERATE("SUB LF@substr$j LF@substr$j int@1");

    GENERATE("LABEL loop");
    GENERATE("GT LF@%%cond_range LF@substr$i LF@substr$j");
    GENERATE("JUMPIFEQ !end_substr LF@%%cond_range bool@true");
    GENERATE("GETCHAR LF@%%char LF@substr$s LF@substr$i");
    GENERATE("CONCAT LF@%%retval0 LF@%%retval0 LF@%%char");
    GENERATE("ADD LF@substr$i LF@substr$i int@1");
    GENERATE("JUMP loop");

    GENERATE("LABEL nil$error_substr");
    GENERATE("EXIT int@8");
    GENERATE("JUMP !end_substr");

    GENERATE("LABEL nil$return_substr");
    GENERATE("MOVE LF@%%retval0 string@%%0");

    GENERATE("LABEL !end_substr");
    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_substr");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief BuiltIn function ord
 *
 * @return the ordinal value of character at position i in the string s.
 * If one of the parameters is nil, error 8 occurs.
 * If the index i is outside the bounds of the string (1 to #s), the function returns nil.
 */
void generate_ord(func_params_t *params)
{
    (void)params;
    GENERATE("#ord");
    GENERATE("JUMP !_ord");
    GENERATE("LABEL !function_ord");
    GENERATE("PUSHFRAME");

    GENERATE("DEFVAR LF@%%retval0");
    GENERATE("DEFVAR LF@%%length");
    GENERATE("DEFVAR LF@%%cond_range");

    GENERATE("JUMPIFEQ nil$error_ord nil@nil LF@ord$s");
    GENERATE("JUMPIFEQ nil$error_ord nil@nil LF@ord$i");

    GENERATE("STRLEN LF@%%length LF@ord$s");
    GENERATE("GT LF@%%cond_range LF@ord$i LF@%%length");
    GENERATE("JUMPIFEQ nil$return_ord LF@%%cond_range bool@true");
    GENERATE("LT LF@%%cond_range LF@ord$i int@1");
    GENERATE("JUMPIFEQ nil$return_ord LF@%%cond_range bool@true");

    GENERATE("STRI2INT LF@%%retval0 LF@ord$s LF@ord$i");
    GENERATE("JUMP !end_ord");

    GENERATE("LABEL nil$error_ord");
    GENERATE("EXIT int@8");
    GENERATE("JUMP !end_ord");

    GENERATE("LABEL nil$return_ord");
    GENERATE("MOVE LF@%%retval0 nil@nil");

    GENERATE("LABEL !end_ord");
    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_ord");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief chr function
 *
 * @return a one-character string with a character
 * whose ASCII code is specified by the i parameter.
 * The case when i is outside the interval [0; 255], leads
 * to nil. If i nil, error 8 occurs.
 */
void generate_chr(func_params_t *params)
{
    (void)params;
    GENERATE("#chr");
    GENERATE("JUMP !_chr");
    GENERATE("LABEL !function_chr");
    GENERATE("PUSHFRAME");
    GENERATE("DEFVAR LF@%%retval0");

    GENERATE("GT LF@%%cond_range LF@chr$i int@255");
    GENERATE("JUMPIFEQ $chr$error LF@%%cond_range bool@true");
    GENERATE("LT LF@%%cond_range LF@chr$i int@0");
    GENERATE("JUMPIFEQ $chr$error LF@%%cond_range bool@true");

    GENERATE("INT2CHAR LF@%%retval0 LF@chr$i");
    GENERATE("JUMP !end_chr");

    GENERATE("LABEL $chr$error");
    GENERATE("EXIT int@8");

    GENERATE("LABEL !end_chr");

    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_chr");
    GENERATE_EMPTY_LINE();
}

void define_built_in_functions(func_params_t *params)
{
    generate_readString(params);
    generate_readInt(params);
    generate_readDouble(params);
    generate_Double2Int(params);
    generate_length(params);
    generate_substring(params);
    generate_ord(params);
    generate_chr(params);
    generate_write(params);
    generate_Int2Double(params);
}

/**
 * @brief Generate header of the code
 *
 * @return void
 */
void generator_start(func_params_t *params)
{
    GENERATE(".IFJcode23");
    GENERATE("DEFVAR GF@%%exp_result");
    define_built_in_functions(params);
    GENERATE("LABEL $main");
    GENERATE("CREATEFRAME");
    GENERATE("PUSHFRAME");
}

void generator_end(void) {
    GENERATE("POPFRAME");
    GENERATE("CLEARS");
}

void generate_var_declaration(char *id)
{
    GENERATE("DEFVAR LF@%s", id);
}

void generate_var_definition(char *id, data_type type)
{
    switch (type)
    {
        case Int_Type:
            GENERATE("MOVE LF@%s int@0", id);
            break;
        case String_Type:
            GENERATE("MOVE LF@%s string@", id);
            break;
        case Double_Type:
            GENERATE("MOVE LF@%s float@0.0", id);
            break;
        case Bool_Type:
            GENERATE("MOVE LF@%s bool@false", id);
            break;
    }
}

void generate_var_assignment(char *id)
{
    GENERATE("MOVE LF@%s LF@%%exp_result", id);
}

void generate_call(func_params_t *params)
{
  GENERATE("CREATEFRAME");
  for (size_t i = 0; i < params->params_size; i++)
  {
    GENERATE("DEFVAR TF@%s$%s", params->name, params->pair[i].target);
    GENERATE("MOVE TF@%s$%s %s", params->name, params->pair[i].target, params->pair[i].source);
  }
  GENERATE("CALL !FUNC_%s", params->name);
}

void generate_function_start(func_params_t *params)
{
  GENERATE("JUMP !BYPASS_%s", params->name);
  GENERATE("LABEL !FUNC_%s", params->name);
  GENERATE("PUSHFRAME");
  for (size_t i = 0; i < params->returns_size; i++)
  {
    GENERATE("DEFVAR LF@%%retval%zu", i);
    GENERATE("MOVE LF@%%retval%zu nil@nil", i);
  }
}

void generate_return_params(func_params_t *params)
{
  for (size_t i = 0; i < params->returns_size; i++)
  {
    if (params->returns)
    {
        GENERATE("MOVE LF@%%retval%zu %s", i, params->returns[i]);
    }

    else
    {
        GENERATE("MOVE LF@%%retval%zu nil@nil", i);
    }
  }
}

void generate_function_end(func_params_t *params)
{
  generate_return(params);
  GENERATE("LABEL !BYPASS_%s", params->name);
}

void generate_return(func_params_t *params)
{
  generate_return_params(params);
  GENERATE("POPFRAME");
  GENERATE("RETURN");
}

int main(func_params_t *params)
{
    generator_start(params);
    generator_end();
    return 0;
}


