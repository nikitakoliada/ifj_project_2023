/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file generator.c

 * @brief Generator implementation - generator of code

 * @author Maksym Podhornyi - xpodho08
 */

#include "generator.h"

/* ----- BUILT IN FUNCTIONS ----- */
/**
 * @brief BuiltIn function readString
 *
 * @return string without a line break symbol
 */
void generate_readString()
{
    GENERATE("\n#readString");
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
void generate_readInt()
{
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
void generate_readDouble()
{
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

void generate_write()
{
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

void generate_Int2Double()
{
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
void generate_Double2Int()
{
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
void generate_length()
{
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
 * the function returns a nil value.
 */

void generate_substring()
{
    GENERATE("#substr");
    GENERATE("JUMP !_substr");
    GENERATE("LABEL !function_substr");
    GENERATE("PUSHFRAME");

    GENERATE("DEFVAR LF@%%retval0");
    GENERATE("DEFVAR LF@%%cond_range");
    GENERATE("DEFVAR LF@%%length");
    GENERATE("DEFVAR LF@%%char");

    GENERATE("LT LF@%%cond_range LF@substr$i int@0");
    GENERATE("JUMPIFEQ nil$return_substr LF@%%cond_range bool@true");

    GENERATE("STRLEN LF@%%length LF@substr$s");

    GENERATE("LT LF@%%cond_range LF@substr$i LF@%%length");
    GENERATE("JUMPIFEQ nil$return_substr LF@%%cond_range bool@false");

    GENERATE("LT LF@%%cond_range LF@substr$j int@0");
    GENERATE("JUMPIFEQ nil$return_substr LF@%%cond_range bool@true");

    GENERATE("GT LF@%%cond_range LF@substr$j LF@%%length");
    GENERATE("JUMPIFEQ nil$return_substr LF@%%cond_range bool@true");

    GENERATE("GT LF@%%cond_range LF@substr$i LF@substr$j");
    GENERATE("JUMPIFEQ nil$return_substr LF@%%cond_range bool@true");

    GENERATE("LABEL loop");
    GENERATE("GT LF@%%cond_range LF@substr$i LF@substr$j");
    GENERATE("JUMPIFEQ !end_substr LF@%%cond_range bool@true");
    GENERATE("GETCHAR LF@%%char LF@substr$s LF@substr$i");
    GENERATE("CONCAT LF@%%retval0 LF@%%retval0 LF@%%char");
    GENERATE("ADD LF@substr$i LF@substr$i int@1");
    GENERATE("JUMP loop");

    GENERATE("LABEL nil$return_substr");
    GENERATE("MOVE LF@%%retval0 nil@nil");

    GENERATE("LABEL !end_substr");
    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_substr");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief BuiltIn function ord
 *
 * @return the ordinal value of first character of the string s.
 * If the string is empty, the function returns 0.
 */
void generate_ord()
{
    GENERATE("#ord");
    GENERATE("JUMP !_ord");
    GENERATE("LABEL !function_ord");
    GENERATE("PUSHFRAME");

    GENERATE("DEFVAR LF@%%retval0");
    GENERATE("DEFVAR LF@%%cond_range");

    GENERATE("JUMPIFEQ empty$error_ord string@ LF@ord$s");

    GENERATE("STRI2INT LF@%%retval0 LF@ord$s int@0");
    GENERATE("JUMP !end_ord");

    GENERATE("LABEL empty$error_ord");
    GENERATE("MOVE LF@%%retval0 int@0");
    GENERATE("JUMP !end_ord");

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
 */
void generate_chr()
{
    GENERATE("#chr");
    GENERATE("JUMP !_chr");
    GENERATE("LABEL !function_chr");
    GENERATE("PUSHFRAME");
    GENERATE("DEFVAR LF@%%retval0");

    GENERATE("INT2CHAR LF@%%retval0 LF@chr$i");

    GENERATE("POPFRAME");
    GENERATE("RETURN");
    GENERATE("LABEL !_chr");
    GENERATE_EMPTY_LINE();
}

/**
 * @brief Define all built in functions
 *
 * @param void
 * @return void
 */
void define_built_in_functions()
{
    generate_readString();
    generate_readInt();
    generate_readDouble();
    generate_Double2Int();
    generate_length();
    generate_substring();
    generate_ord();
    generate_chr();
    generate_write();
    generate_Int2Double();
}

void generator_start(void)
{
    GENERATE(".IFJcode23");
    GENERATE("DEFVAR GF@%%exp_result");
    GENERATE("DEFVAR GF@%%tmp1");
    GENERATE("DEFVAR GF@%%tmp2");
    GENERATE("DEFVAR GF@%%tmp3");
    define_built_in_functions();
    GENERATE("LABEL $$main");
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
            if(strcmp(id, "%%retval0") == 0) {
                GENERATE("MOVE TF@%%retval0 int@0");
            } else {
                GENERATE("MOVE LF@%s int@0", id);
            }
            break;
        case String_Type:
            if(strcmp(id, "%%retval0") == 0) {
                GENERATE("MOVE TF@%%retval0 string@");
            } else {
                GENERATE("MOVE LF@%s string@", id);
            }
            break;
        case Double_Type:
            if(strcmp(id, "%%retval0") == 0) {
                GENERATE("MOVE TF@%%retval0 float@0.0");
            } else {
                GENERATE("MOVE LF@%s float@0.0", id);
            }
            break;
        case Bool_Type:
            if(strcmp(id, "%%retval0") == 0) {
                GENERATE("MOVE TF@%%retval0 bool@false");
            } else {
                GENERATE("MOVE LF@%s bool@false", id);
            }
            break;
        case Undefined:
            if(strcmp(id, "%%retval0") == 0) {
                GENERATE("MOVE TF@%%retval0 nil@nil");
            } else {
                GENERATE("MOVE LF@%s nil@nil", id);
            }
        default:
            break;
    }
}

void generate_var_assignment(char *id)
{
    if(strcmp(id, "%%exp_result") == 0) {
        GENERATE("POPS GF@%%exp_result");
    } else {
        GENERATE("POPS LF@%s", id);
    }
}

void generate_read(char *id, data_type type)
{
    switch (type)
    {
        case Int_Type:
            GENERATE("CREATEFRAME");
            GENERATE("CALL !function_readInt");
            GENERATE("MOVE LF@%s TF@%%retval0", id);
            break;
        case String_Type:
            GENERATE("CREATEFRAME");
            GENERATE("CALL !function_readString");
            GENERATE("MOVE LF@%s TF@%%retval0", id);
            break;
        case Double_Type:
            GENERATE("CREATEFRAME");
            GENERATE("CALL !function_readDouble");
            GENERATE("MOVE LF@%s TF@%%retval0", id);
            break;
        default:
            break;
    }
}

void generate_write_val(void)
{
    GENERATE("CREATEFRAME");
    GENERATE("DEFVAR TF@%%0");
    GENERATE("MOVE TF@%%0 LF@%%exp_result");
    GENERATE("CALL !function_write");

}

void gen_term(token_t *token){
    switch (token->type){
        case INT_VALUE:
            GENERATE("int@%lld", token->data.Int);
            break;
        case DOUBLE_VALUE:
            GENERATE("float@%a", token->data.Double);
            break;
        case STRING_VALUE:
            GENERATE("string@%s", token->data.String);
            break;
        case IDENTIFIER:
            GENERATE("LF@%s", token->data.String);
            break;
        default:
            break;
    }
}

void gen_push(token_t *token){
    GENERATE_WITHOUT_NEW_LINE("PUSHS ");
    gen_term(token);
}

void gen_pop(void){
    GENERATE("POPS");
}

void gen_int2double(void){
    GENERATE("INT2FLOATS");
}

void gen_double2int(void){
    GENERATE("FLOAT2INTS");
}

void gen_int2double_2op(void){
    GENERATE("POPS GF@%%tmp1");
    GENERATE("INT2FLOATS");
    GENERATE("PUSHS GF@%%tmp1");
}

void gen_operation(rules rule){
    switch (rule){
        case PLUS_R:
            GENERATE("ADDS");
            break;
        case MINUS_R:
            GENERATE("SUBS");
            break;
        case MUL_R:
            GENERATE("MULS");
            break;
        case DIV_R:
            GENERATE("DIVS");
            break;
        case IDIV_R:
            GENERATE("POPS GF@%%tmp1");
            GENERATE("INT2FLOATS");
            GENERATE("PUSHS GF@%%tmp1");
            GENERATE("INT2FLOATS");
            GENERATE("DIVS");
            GENERATE("FLOAT2INTS");
            break;
        case EQ:
            GENERATE("EQS");
            break;
        case NEQ:
            GENERATE("EQS");
            GENERATE("NOTS");
            break;
        case L:
            GENERATE("LTS");
            break;
        case LEQ:
            GENERATE("GTS");
            GENERATE("NOTS");
            break;
        case G:
            GENERATE("GTS");
            break;
        case GEQ:
            GENERATE("LTS");
            GENERATE("NOTS");
            break;
        case NOT_NIL_R:
            GENERATE("POPS GF@%%tmp1");
            GENERATE("PUSHS GF@%%tmp1");
            GENERATE("PUSHS nil@nil");
            GENERATE("EQS");
            GENERATE("ORS");
            GENERATE("PUSHS GF@%%tmp1");
            break;
        case NOT_NULL_R:
            GENERATE("POPS GF@%%tmp1");
            GENERATE("POPS GF@%%tmp2");
            GENERATE("POPS GF@%%tmp3");
            GENERATE("PUSHS GF@%%tmp3");
            GENERATE("PUSHS GF@%%tmp2");
            GENERATE("EQS");
            GENERATE("PUSHS GF@%%tmp1");
            GENERATE("PUSHS GF@%%tmp3");
            GENERATE("EQS");
            GENERATE("ORS");
            GENERATE("PUSHS GF@%%tmp2");
            GENERATE("PUSHS GF@%%tmp1");
            GENERATE("EQS");
            GENERATE("ORS");
            break;
        default:
            break;
    }
}

void gen_concat(void){
    GENERATE("POPS GF@%%tmp1");
    GENERATE("POPS GF@%%tmp2");
    GENERATE("CONCAT GF@%%tmp3 GF@%%tmp2 GF@%%tmp1");
    GENERATE("PUSHS GF@%%tmp3");
}

void gen_call_start(void){
    GENERATE("CREATEFRAME");
}

void add_param_to_call(char* param_name){
    GENERATE("DEFVAR TF@%%%s", param_name);
    GENERATE("MOVE TF@%%%s GF@%%exp_result", param_name);
}

void gen_call(char* function_name){
    if(strcmp(function_name, "readInt") == 0){
        GENERATE("CALL !function_readInt");
    }else if(strcmp(function_name, "readDouble") == 0){
        GENERATE("CALL !function_readDouble");
    }else if(strcmp(function_name, "readString") == 0){
        GENERATE("CALL !function_readString");
    }else if(strcmp(function_name, "Int2Double") == 0){
        GENERATE("CALL !function_Int2Double");
    }else if(strcmp(function_name, "Double2Int") == 0){
        GENERATE("CALL !function_Double2Int");
    }else if(strcmp(function_name, "length") == 0){
        GENERATE("CALL !function_length");
    }else if(strcmp(function_name, "substring") == 0){
        GENERATE("CALL !function_substr");
    }else if(strcmp(function_name, "ord") == 0){
        GENERATE("CALL !function_ord");
    }else if(strcmp(function_name, "chr") == 0){
        GENERATE("CALL !function_chr");
    }
    GENERATE("CALL !FUNC_%s", function_name);
}

void generate_function_start(char *name)
{
    GENERATE("JUMP !BYPASS_%s", name);
    GENERATE("LABEL !FUNC_%s", name);
    GENERATE("PUSHFRAME");
}

void generate_function_param(char *param_name, data_type type)
{
    GENERATE("DEFVAR LF@%s", param_name);
    generate_var_definition(param_name, type);
}

void generate_function_return_param(data_type type)
{
    GENERATE("DEFVAR TF@%%retval0");
    generate_var_definition("%%retval0", type);
}

void generate_function_return(void)
{
    GENERATE("MOVE TF@%%retval0 GF@%%exp_result");
    GENERATE("POPFRAME");
    GENERATE("RETURN");
}

void generate_function_void_return(void)
{
    GENERATE("POPFRAME");
    GENERATE("RETURN");
}

void generate_function_end(char* function_name)
{
    GENERATE("LABEL !BYPASS_%s", function_name);
}

void gen_while_start(int while_counter)
{
    GENERATE("LABEL !while_start_%d", while_counter);
}

void gen_while(int while_counter)
{
    GENERATE("JUMPIFEQ !while_start_%d GF@%%exp_result bool@true", while_counter);
    GENERATE("LABEL !while_end_%d", while_counter);
}

void gen_while_end(int while_counter)
{
    GENERATE("JUMP !while_start_%d", while_counter);
    GENERATE("LABEL !while_end_%d", while_counter);
}

void gen_if_let(char *id, int let_if_counter)
{
    GENERATE("JUMPIFEQ !if_let_%d LF@%s nil@nil", let_if_counter, id);
    GENERATE("MOVE GF@%%exp_result bool@true");
    GENERATE("LABEL !if_let_%d", let_if_counter);
    GENERATE("MOVE GF@%%exp_result bool@false");
    gen_if_start(let_if_counter);
}

void gen_if_start(int if_counter)
{
    GENERATE("JUMPIFEQ !if_else_%d GF@%%exp_result bool@false", if_counter);
}

void gen_if_else(int if_counter)
{
    GENERATE("JUMP !if_end_%d", if_counter);
    GENERATE("LABEL !if_else_%d", if_counter);
}

void gen_if_end(int if_counter)
{
    GENERATE("LABEL !if_end_%d", if_counter);
}


int main()
{
    generator_start();
    generate_var_declaration("a");
    generate_read("a", String_Type);
    generate_write_var("a");
    generator_end();
    return 0;
}


