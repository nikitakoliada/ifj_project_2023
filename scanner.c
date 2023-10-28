/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file scanner.c

 * @brief Scanner implementation

 * @author Juraj Remeň - xremen02
 * @author Pavlo Butenko - xbuten00
 * @author Maksym Podhornyi - xpodho08

 */

#include "scanner.h"

#include "ctype.h"

char *keywords[] = {"Double", "else", "func", "if", "Int", "let", "nil", "return", "String", "var", "while"};
char *built_in_functions[] = {"readString", "readInt", "readDouble", "write", "Int2Double", "Double2Int", "length", "substring", "ord", "chr"};

FILE* file;


bool is_integer(char *token)
{
    for (unsigned int i = 0; i < strlen(token); i++)
    {
        if (isdigit(token[i]))
        {
            return true;
        }
    }

    return false;
}

bool is_built_in_function(char *token)
{
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(token, built_in_functions[i]) == 0)
        {
            return true;
        }
    }

        return false;
}

bool is_keyword(char *token)
{
    for (int i = 0; i < 11; i++)
    {
        if (strcmp(token, keywords[i]) == 0)
        {
            return true;
        }
    }

        return false;
}


void scanner_process_escape_sequence(char *token, unsigned long long token_position)
{
    if (isdigit(token[token_position-2])) {
        token_position = token_position - 4;
        char escaped[4];
        int ascii_val;

        memcpy(escaped,&token[token_position+1], 3);
        escaped[3] = '\0';
        ascii_val = atoi(escaped);
        if (ascii_val < 1 || ascii_val > 255){
            ERROR_EXIT("Invalid escape sequence.\n", LEX_ERROR);
        }

    } else {
        char c = token[token_position-1];

        switch (c){
            case 'n':
                token[token_position-2] = 10;
                break;
            case 't':
                token[token_position-2] = 9;
                break;
            case 'r':
                token[token_position-2] = 13;
                break;
            case '"':
                token[token_position-2] = 34;
                break;
            case '\\':
                token[token_position-2] = 92;
                break;
            default:
                break;
        }
        token[token_position - 1] = '\0';
    }
}

/*void scanner_free_tokens(token_t *first_token)
{

}*/

/*token_type_t scanner_get_token_type(char *token, scanner_states_t actual_state)
{
    if (is_keyword(token))
        return KEYWORD;
    else if (is_built_in_function(token))
        return BUILT_IN_FUNCTION;
    else if (actual_state == KEYWORD_OR_IDENTIFIER)
        return IDENTIFIER;
    else if (actual_state == NUMBER || actual_state == EXPONENT) {
        if (is_integer(token))
            return INT_VALUE;
        return NUMBER_VALUE;
    } else if (actual_state == OPERATOR)
        return TOKEN_OPERATOR;
    //else if (actual_state == SEPARATOR)
    //    return TOKEN_SEPARATOR;
    else if (actual_state == STRING)
        return STRING_VALUE;
    else if (actual_state == LEFT_BRACKET)
        return TOKEN_LEFT_BRACKET;
    else if (actual_state == RIGHT_BRACKET)
        return TOKEN_RIGHT_BRACKET;
    else if (actual_state == LEFT_CURLY_BRACKET)
        return TOKEN_LEFT_CURLY_BRACKET;
    else if (actual_state == RIGHT_CURLY_BRACKET)
        return TOKEN_RIGHT_CURLY_BRACKET;
    else
        return NIL_VALUE;
}*/

//token_t * scanner_get_token_struct(char *token_array, unsigned long long actual_line, scanner_states_t actual_state, token_t *prev_token)
//{
//    token_t *token = (token_t *)malloc(sizeof(token_t));
//    if (!token)
//        ERROR_EXIT("Could not allocate memory for token!", INTERNAL_ERROR);
//
//    token->line = actual_line;
//    //token->type = scanner_get_token_type(token_array, actual_state);
//
//    //token->next = NULL;
//    //if (prev_token)
//        //prev_token->next = token;
//    //else
//        //token->prev = NULL;
//
//    //if the current token_type = func_params, then previous token_type = function_name
//    if (token->type == TOKEN_LEFT_BRACKET && prev_token->type != BUILT_IN_FUNCTION && prev_token->type != KEYWORD && prev_token->type != TOKEN_OPERATOR && prev_token->type != TOKEN_SEPARATOR && prev_token->type != TOKEN_RIGHT_BRACKET)
//    {
//        prev_token->type = TOKEN_FUNCTION_TYPE;
//    }
//
//    /*if (scanner_set_nil_value(prev_token, token, token_array))
//    {
//        token->data.String = NULL;
//        token->type = NIL_VALUE;
//    }
//
//    if (is_function_type(prev_token))
//        token->type = TOKEN_FUNCTION_TYPE;*/
//
//    if(token->type == STRING_VALUE && token_array[0] != '\0') {
//        size_t len = strlen(token_array);
//        size_t new_len = len;
//        size_t j = 0;
//        char temp[3];
//
//        char *temp_str = (char *)malloc(sizeof(char ) * (len + 1));
//
//        for (size_t i = 0; i < len; i++)
//        {
//            if (token_array[i] < 33 || token_array[i] == 35 || (token_array[i] == 92 && !isdigit(token_array[i+1])))
//            {
//                new_len += 4;
//                temp_str = realloc(temp_str, sizeof(char) * (new_len + 1));
//                sprintf(temp, "%02d", token_array[i]);
//
//                temp_str[j++] = '\\';
//                temp_str[j++] = '0';
//                temp_str[j++] = temp[0];
//                temp_str[j++] = temp[1];
//            } else
//                temp_str[j++] = token_array[i];
//        }
//        temp_str[j] = 0;
//        token_array = temp_str;
//    } else if(token->type == IDENTIFIER && token_array[0] == '_') {
//        token_array[0] = 'l';
//    }
//    if (token->type == KEYWORD || token->type == IDENTIFIER ||
//        token->type == STRING_VALUE || token->type == TOKEN_OPERATOR ||
//        token->type == BUILT_IN_FUNCTION ||
//        token->type == TOKEN_SEPARATOR || token->type == TOKEN_FUNCTION_TYPE ||
//        token->type == TOKEN_LEFT_BRACKET || token->type == TOKEN_RIGHT_BRACKET ||
//        token->type == TOKEN_LEFT_CURLY_BRACKET || token->type == TOKEN_RIGHT_CURLY_BRACKET) {
//        token->data.String = token_array;
//    } else if (token->type == INT_VALUE) {
//        token->data.Int = atoll(token_array);
//    } else if (token->type == NUMBER_VALUE) {
//        token->data.Double = atof(token_array);
//    } else if (token->type == NIL_VALUE) {
//        token->data.Int = 0;
//    }
//    //token->prev = prev_token;
//
//    return token;
//}

void add_char_to_string(char *string, unsigned* index, unsigned* size, char c)
{
    unsigned len = strlen(string);

    if(len + 2 >= *size){
        if(realloc(string, *size * 2 * sizeof(char)) == NULL) {
            ERROR_EXIT("Could not reallocate memory for string!", INTERNAL_ERROR)
        }
        *size *= 2;
    }
    string[(*index)++] = c;
    string[*index] = '\0';
}

int get_next_token(token_t* token){
    scanner_states_t state = NEW_TOKEN;
    int token_type = -1;
    char* raw_token = malloc(DEFAULT_TOKEN_LENGTH * sizeof(char));
    if(raw_token == NULL){
        ERROR_EXIT("Could not allocate memory for token!", INTERNAL_ERROR)
    }
    unsigned index = 0;
    unsigned str_size = DEFAULT_TOKEN_LENGTH;
    do{
        bool add_char = false;
        int symbol = getc(file);

        switch (state) {
            case NEW_TOKEN:
                if(symbol == '"'){
                    state = STRING;
                }else if(isdigit(symbol)){
                    state = NUMBER;
                }else if(symbol == '-') {
                    state = S_MINUS;
                }else if(symbol == '+') {
                    token_type = PLUS;
                }else if(isalpha(symbol) || symbol == '_') {
                    state = KEYWORD_OR_IDENTIFIER;
                }else if(symbol == EOF){
                    break;
                }else if(symbol == ',') {
                    token_type = COMMA;
                }else if(symbol == ':') {
                    token_type = COLON;
                }else if(symbol == '?') {
                    state = NIL;
                }else if(symbol == '!'){
                    state = S_NOT;
                }else if(symbol == '{'){
                    token_type = TOKEN_LEFT_CURLY_BRACKET;
                }else if(symbol == '}'){
                    token_type = TOKEN_RIGHT_CURLY_BRACKET;
                }else if(symbol == '='){
                    state = ASSIGNMENT;
                }else if(symbol == '*'){
                    token_type = MUL;
                }else if(symbol == '/'){
                    state = DIVIDE;
                }else if(symbol == '>'){
                    state = MAYBE_MORE_THAN;
                }else if(symbol == '<'){
                    state = MAYBE_LESS_THAN;
                }else if(symbol == '('){
                    token_type = TOKEN_LEFT_BRACKET;
                }else if(symbol == ')'){
                    token_type = TOKEN_RIGHT_BRACKET;
                }else if(symbol == ' ' || symbol == '\n') {
                    continue;
                }else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }
                break;

            case S_MINUS:
                if(symbol == '>'){
                    // state = FUNCTION_TYPE;
                    token_type = TOKEN_FUNCTION_TYPE;
                }else{
                    ungetc(symbol, file);
                    token_type = MINUS;
                    //TODO
                }
                break;
            case NIL:
                if(symbol == '?'){
                    //state = NC;
                    token_type = NIL_COLL; // Nullish coalescing operator
                }else{
                    ungetc(symbol, file);
                    token_type = NIL_VALUE;
                }
                break;
            case S_NOT:
                if(symbol == '='){
                    //state = NOT_EQUAL;
                    token_type = NOT_EQUAL;
                }else {
                    ungetc(symbol, file);
                    token_type = NOT;
                    //TODO
                }
                break;
            case ASSIGNMENT:
                if(symbol == '='){
                    // state = EQUAL;
                    token_type = EQUAL;
                }else {
                    ungetc(symbol, file);
                    token_type = ASSIGNMENT;
                    //TODO
                }
                break;
            case MAYBE_MORE_THAN:
                if(symbol == '='){
                    // state = MORE_THAN_OR_EQUAL;
                    token_type = MORE_THAN_OR_EQUAL;
                }else{
                    ungetc(symbol, file);
                    token_type = MORE_THAN;
                    // TODO
                }
                break;
            case MAYBE_LESS_THAN:
                if(symbol == '='){
                    // state = LESS_THAN_OR_EQUAL;
                    token_type = LESS_THAN_OR_EQUAL;
                }else{
                    ungetc(symbol, file);
                    token_type = LESS_THAN;
                    // TODO
                }
                break;
            case KEYWORD_OR_IDENTIFIER:
                if(isalnum(symbol) || symbol == '_'){
                    add_char = true;
                }else{
                    ungetc(symbol, file);
                    if(is_built_in_function(raw_token)){
                        token_type = BUILT_IN_FUNCTION;
                    }else if (is_keyword(raw_token)){
                        token_type = KEYWORD;
                    }else{
                        token_type = IDENTIFIER;
                    }
                }
            case NUMBER:
                if(symbol >= '0' && symbol <= '9') {
                    state = NUMBER;
                    add_char = true;
                    break;
                }else if(symbol == '.'){
                    state = DOT;
                    add_char = true;
                    break;
                }else if(symbol == 'e' || symbol == 'E') {
                    state = EXPONENT;
                    add_char = true;
                    break;
                }else{
                    token_type = INT_VALUE;
                    ungetc(symbol, file);
                }
                break;

            case DOT:
                if(symbol >= '0' && symbol <= '9'){
                    state = DOUBLE;
                    add_char = true;
                    break;
                }
                else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }

            case DOUBLE:
                if (symbol >= '0' && symbol <= '9') {
                    state = DOUBLE;
                    add_char = true;
                    break;
                }else if (symbol == 'e' || symbol == 'E'){
                    state = EXPONENT;
                    add_char = true;
                    break;
                }else{
                    token_type = DOUBLE_VALUE;
                    ungetc(symbol, file);
                }
                break;

            case EXPONENT:
                if (symbol >= '0' && symbol <= '9') {
                    state = EXPONENT_FINAL;
                    add_char = true;
                    break;
                }else if (symbol == '+' || symbol == '-'){
                    state = EXPONENT_SIGN;
                    add_char = true;
                    break;
                }
                else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }

            case EXPONENT_SIGN:
                if (symbol >= '0' && symbol <= '9') {
                    state = EXPONENT_FINAL;
                    add_char = true;
                    break;
                }else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }

            case EXPONENT_FINAL:
                if (symbol >= '0' && symbol <= '9') {
                    state = EXPONENT_FINAL;
                    break;
                }else{
                    ungetc(symbol, file);
                    token_type = DOUBLE_VALUE;
                }
                break;

            case DIVIDE:
                if(symbol == '/'){
                    state = COMMENT;
                    break;
                }else if(symbol == '*'){
                    state = BLOCK_COMMENT;
                    break;
                }else{
                    token_type = DIV;
                    ungetc(symbol, file);
                }
                break;

            case COMMENT:
                if(symbol >= 32 && symbol <= 126){
                    state = COMMENT;
                    break;
                } else if(symbol == '\n'){
                    state = NEW_TOKEN;
                    break;
                }
                break;

            case BLOCK_COMMENT:
                if (symbol >= 32 && symbol <= 126 && symbol != '*') {
                    state = BLOCK_COMMENT;
                    break;
                }else if(symbol == '*'){
                    state = BLOCK_COMMENT_POSSIBLE_END;
                    break;
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in block comment", LEX_ERROR)
                }
                break;

            case BLOCK_COMMENT_POSSIBLE_END:
                if(symbol == '/'){
                    state = BLOCK_COMMENT_END;
                    break;
                }else if(symbol == '*'){
                    state = BLOCK_COMMENT_POSSIBLE_END;
                    break;
                }else if(symbol >= 32 && symbol <= 126){
                    state = BLOCK_COMMENT;
                    break;
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in block comment", LEX_ERROR)
                }
                break;

            case BLOCK_COMMENT_END:
                state = NEW_TOKEN;

                break;

            case STRING:
                if(symbol == '"'){
                    state = MAYBE_MULTILINE;
                    //token_type = STRING_VALUE;
                    break;
                }else if(symbol >= 32 && symbol <= 126 && symbol != '\\') {
                    state = ONE_LINE_STRING;
                    add_char = true;
                    break;
                }
                break;

            case ONE_LINE_STRING:
                if(symbol == '"'){
                    state = END_STRING;
                    break;
                }else if(symbol == '\n'){
                    ERROR_EXIT("Unexpected EOL in string", LEX_ERROR)
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in string", LEX_ERROR)
                }else if(symbol >= 32 && symbol <= 126){
                    //state = ONE_LINE_STRING;
                    add_char = true;
                    break;
                }
                break;

            case MAYBE_MULTILINE:
                if (symbol == '"') {
                    state = MULTILINE;
                    break;
                }else if(symbol == '\n' || symbol == EOF || symbol == '\0') {
                    state = STRING;
                    // Empty string
                    break;
                }
                break;

            case END_STRING:
                break;

            case MULTILINE:
                if(symbol == '"') {
                    state = END_STRING;
                    break;
                }


       }

       if(add_char){
           add_char_to_string(raw_token, &index, &str_size, (char)symbol);
       }

   }while(token_type == -1);
    return 0;
}

//token_type_t scanner_get_param_token_type(char *token_string)
//{
//    if (strcmp(token_string, "Int") == 0)
//        return INT_VALUE;
//    else if (strcmp(token_string, "Double") == 0)
//        return NUMBER_VALUE;
//    else if (strcmp(token_string, "String") == 0)
//        return STRING_VALUE;
//    else if (strcmp(token_string, "nil") == 0)
//        return NIL_VALUE;
//    else
//        ERROR_EXIT("Unsupported param type", INTERNAL_ERROR);
//}
