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

char *keywords[] = {"Double", "else", "func", "if", "Int", "let", "nil", "return", "String", "var", "while", "Int?", "String?", "Double?"};
char *built_in_functions[] = {"readString", "readInt", "readDouble", "write", "Int2Double", "Double2Int", "length", "substring", "ord", "chr"};
char *types[] = {"Double", "Int", "String"};

FILE* file;

void set_source_file(FILE *file_arg){
    file = file_arg;
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
    for (int i = 0; i < 14; i++)
    {
        if (strcmp(token, keywords[i]) == 0)
        {
            return true;
        }
    }

        return false;
}

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

void get_keyword_type(char* token_raw, keyword_t* keyword){
    for(int i = 0; i < 14; i++){
        if(!strcmp(token_raw, keywords[i])){
            *keyword = (keyword_t)i;
        }
    }
}

bool is_pure_type(char* token){
    for(int i = 0; i < 3; i++){
        if(!strcmp(token, types[i])) return true;
    }
    return false;
}

int get_next_token(token_t* token){
    // current state of the scanner
    scanner_states_t state = NEW_TOKEN_S;

    // string representation of unicode code
    char* unicode_code = calloc(sizeof(char), 3);
    int unicode_index = 0;

    // creation token and its type
    int token_type = -1;
    char* raw_token = calloc(sizeof(char), DEFAULT_TOKEN_LENGTH);
    if(raw_token == NULL){
        ERROR_EXIT("Could not allocate memory for token!", INTERNAL_ERROR)
    }
    unsigned index = 0;
    unsigned str_size = DEFAULT_TOKEN_LENGTH;

    do{
        bool add_char = false;
        int symbol = getc(file);

        if(symbol == '\n') token->line++;

        switch (state) {
            case NEW_TOKEN_S:
                if(symbol == '"'){
                    state = STRING_S;
                }else if(isdigit(symbol)){
                    add_char = true;
                    state = NUMBER_S;
                }else if(symbol == '-') {
                    state = MINUS_S;
                }else if(symbol == '+') {
                    token_type = PLUS;
                }else if(isalpha(symbol) || symbol == '_') {
                    add_char = true;
                    state = KEYWORD_OR_IDENTIFIER_S;
                }else if(symbol == EOF){
                    token_type = TOKEN_EOF;
                }else if(symbol == ',') {
                    token_type = COMMA;
                }else if(symbol == ':') {
                    token_type = COLON;
                }else if(symbol == '?') {
                    state = NIL_S;
                }else if(symbol == '!'){
                    state = NOT_S;
                }else if(symbol == '{'){
                    token_type = TOKEN_LEFT_CURLY_BRACKET;
                }else if(symbol == '}'){
                    token_type = TOKEN_RIGHT_CURLY_BRACKET;
                }else if(symbol == '='){
                    state = ASSIGNMENT_S;
                }else if(symbol == '*'){
                    token_type = MUL;
                }else if(symbol == '/'){
                    state = DIVIDE_S;
                }else if(symbol == '>'){
                    state = MAYBE_MORE_THAN_S;
                }else if(symbol == '<'){
                    state = MAYBE_LESS_THAN_S;
                }else if(symbol == '('){
                    token_type = TOKEN_LEFT_BRACKET;
                }else if(symbol == ')'){
                    token_type = TOKEN_RIGHT_BRACKET;
                }else if(symbol == '\n'){
                    token_type = TOKEN_EOL;
                }else if(symbol == ' ') {
                    continue;
                }else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }
                break;

            case MINUS_S:
                if(symbol == '>'){
                    token_type = TOKEN_FUNCTION_TYPE;
                }else{
                    ungetc(symbol, file);
                    token_type = MINUS;
                }
                break;
            case NIL_S:
                if(symbol == '?'){
                    token_type = NIL_COLL; // Nullish coalescing operator
                }else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR);
                }
                break;
            case NOT_S:
                if(symbol == '='){
                    token_type = NOT_EQUAL;
                }else {
                    ungetc(symbol, file);
                    token_type = NOT;
                    //TODO
                }
                break;
            case ASSIGNMENT_S:
                if(symbol == '='){
                    // state = EQUAL;
                    token_type = EQUAL;
                }else {
                    ungetc(symbol, file);
                    token_type = ASSIGNMENT;
                    //TODO
                }
                break;
            case MAYBE_MORE_THAN_S:
                if(symbol == '='){
                    // state = MORE_THAN_OR_EQUAL;
                    token_type = MORE_THAN_OR_EQUAL;
                }else{
                    ungetc(symbol, file);
                    token_type = MORE_THAN;
                    // TODO
                }
                break;
            case MAYBE_LESS_THAN_S:
                if(symbol == '='){
                    // state = LESS_THAN_OR_EQUAL;
                    token_type = LESS_THAN_OR_EQUAL;
                }else{
                    ungetc(symbol, file);
                    token_type = LESS_THAN;
                    // TODO
                }
                break;
            case KEYWORD_OR_IDENTIFIER_S:
                if(isalnum(symbol) || symbol == '_'){
                    add_char = true;
                }else if(symbol == '?' && is_pure_type(raw_token)){
                    add_char = true;
                    token_type = KEYWORD;
                }
                else{
                    ungetc(symbol, file);
                    if(is_built_in_function(raw_token)){
                        token_type = BUILT_IN_FUNCTION;
                    }else if (is_keyword(raw_token)){
                        token_type = KEYWORD;
                    }else{
                        token_type = IDENTIFIER;
                    }
                }
                break;
            case NUMBER_S:
                if(symbol >= '0' && symbol <= '9') {
                    state = NUMBER_S;
                    add_char = true;
                    break;
                }else if(symbol == '.'){
                    state = DOT_S;
                    add_char = true;
                    break;
                }else if(symbol == 'e' || symbol == 'E') {
                    state = EXPONENT_S;
                    add_char = true;
                    break;
                }else{
                    token_type = INT_VALUE;
                    ungetc(symbol, file);
                }
                break;

            case DOT_S:
                if(symbol >= '0' && symbol <= '9'){
                    state = DOUBLE_S;
                    add_char = true;
                    break;
                }
                else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }

            case DOUBLE_S:
                if (symbol >= '0' && symbol <= '9') {
                    state = DOUBLE_S;
                    add_char = true;
                    break;
                }else if (symbol == 'e' || symbol == 'E'){
                    state = EXPONENT_S;
                    add_char = true;
                    break;
                }else{
                    token_type = DOUBLE_VALUE;
                    ungetc(symbol, file);
                }
                break;

            case EXPONENT_S:
                if (symbol >= '0' && symbol <= '9') {
                    state = EXPONENT_FINAL_S;
                    add_char = true;
                    break;
                }else if (symbol == '+' || symbol == '-'){
                    state = EXPONENT_SIGN_S;
                    add_char = true;
                    break;
                }
                else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }

            case EXPONENT_SIGN_S:
                if (symbol >= '0' && symbol <= '9') {
                    state = EXPONENT_FINAL_S;
                    add_char = true;
                    break;
                }else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }

            case EXPONENT_FINAL_S:
                if (symbol >= '0' && symbol <= '9') {
                    state = EXPONENT_FINAL_S;
                    add_char = true;
                    break;
                }else{
                    ungetc(symbol, file);
                    token_type = DOUBLE_VALUE;
                }
                break;

            case DIVIDE_S:
                if(symbol == '/'){
                    state = COMMENT_S;
                    break;
                }else if(symbol == '*'){
                    state = BLOCK_COMMENT_S;
                    break;
                }else{
                    token_type = DIV;
                    ungetc(symbol, file);
                }
                break;

            case COMMENT_S:
                if(symbol >= 32 && symbol <= 126){
                    state = COMMENT_S;
                    break;
                } else if(symbol == '\n'){
                    state = NEW_TOKEN_S;
                    break;
                }
                break;

            case BLOCK_COMMENT_S:
                if (symbol >= 32 && symbol <= 126 && symbol != '*') {
                    state = BLOCK_COMMENT_S;
                    break;
                }else if(symbol == '*'){
                    state = BLOCK_COMMENT_POSSIBLE_END_S;
                    break;
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in block comment", LEX_ERROR)
                }
                break;

            case BLOCK_COMMENT_POSSIBLE_END_S:
                if(symbol == '/'){
                    state = BLOCK_COMMENT_END_S;
                    break;
                }else if(symbol == '*'){
                    state = BLOCK_COMMENT_POSSIBLE_END_S;
                    break;
                }else if(symbol >= 32 && symbol <= 126){
                    state = BLOCK_COMMENT_S;
                    break;
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in block comment", LEX_ERROR)
                }
                break;

            case BLOCK_COMMENT_END_S:
                state = NEW_TOKEN_S;

                break;

            case STRING_S:
                if(symbol == '"'){
                    state = MAYBE_MULTILINE_S;
                }else if(symbol == '\\'){
                    state = ESCAPE_S;
                }else if(symbol >= 32 && symbol <= 126){
                    state = STRING_S;
                    add_char = true;
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in string", LEX_ERROR)
                }
                break;

            case ESCAPE_S:
                switch (symbol) {
                    case 'n':
                        add_char_to_string(raw_token, &index, &str_size, '\n');
                        state = STRING_S;
                        break;
                    case 't':
                        add_char_to_string(raw_token, &index, &str_size, '\t');
                        state = STRING_S;
                        break;
                    case 'r':
                        add_char_to_string(raw_token, &index, &str_size, '\r');
                        state = STRING_S;
                        break;
                    case '"':
                        add_char_to_string(raw_token, &index, &str_size, '"');
                        state = STRING_S;
                        break;
                    case '\\':
                        add_char_to_string(raw_token, &index, &str_size, '\\');
                        state = STRING_S;
                        break;
                    case 'u':
                        state = MAYBE_UNICODE_S;
                        break;
                    default:
                        ERROR_EXIT("Unexpected symbol", LEX_ERROR)

                }
                break;

            case MAYBE_UNICODE_S:
                if(symbol == '{'){
                    state = UNICODE_S;
                }else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }
                break;

            case UNICODE_S:
                if(isxdigit(symbol)){
                    if(unicode_index == 4){
                        ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                    }
                    unicode_code[unicode_index++] = (char)symbol;
                    state = UNICODE_S;
                }else if(symbol == '}'){
                    int unicode_int = (int) strtol(unicode_code, NULL, 16);
                    add_char_to_string(raw_token, &index, &str_size, (char)unicode_int);
                    state = STRING_S;
                }else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }
                break;

            case MAYBE_MULTILINE_S:
                if(symbol == '"'){
                    state = MAYBE_MULTILINE_START_S;
                }else if(raw_token != NULL){
                    state = NEW_TOKEN_S;
                    token_type = STRING_VALUE;
                    ungetc(symbol, file);
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in string", LEX_ERROR)
                }
                break;

            case MAYBE_MULTILINE_START_S:
                if(symbol == '\n'){
                    state = MULTILINE_STRING_S;
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in string", LEX_ERROR)
                }else{
                    ERROR_EXIT("Unexpected symbol", LEX_ERROR)
                }
                break;

            case MULTILINE_STRING_S:
                if(symbol == '\n'){
                    state = MAYBE_MULTILINE_END_START_S;
                    add_char = true;
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in string", LEX_ERROR)
                }else{
                    state = MULTILINE_STRING_S;
                    add_char = true;
                }
                break;

            case MAYBE_MULTILINE_END_START_S:
                if(symbol == '"'){
                    state = MAYBE_MULTILINE_END_S;
                    add_char = true;
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in string", LEX_ERROR)
                }else{
                    state = MULTILINE_STRING_S;
                    add_char = true;
                }
                break;

            case MAYBE_MULTILINE_END_S:
                if(symbol == '"'){
                    state = MAYBE_MULTILINE_END_FINAL_S;
                    add_char = true;
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in string", LEX_ERROR)
                }else{
                    state = MULTILINE_STRING_S;
                    add_char = true;
                }
                break;

            case MAYBE_MULTILINE_END_FINAL_S:
                if(symbol == '"'){
                    state = MULTILINE_END_S;
                }else if(symbol == EOF){
                    ERROR_EXIT("Unexpected EOF in string", LEX_ERROR)
                }else{
                    state = MULTILINE_STRING_S;
                    add_char = true;
                }
                break;

            case MULTILINE_END_S:
                state = NEW_TOKEN_S;
                raw_token[index - 3] = '\0';
                token_type = STRING_VALUE;
                ungetc(symbol, file);
                break;
       }

       if(add_char){
           add_char_to_string(raw_token, &index, &str_size, (char)symbol);
       }

   }while(token_type == -1);

    token->type = token_type;

    switch (token_type)
    {
        case KEYWORD:
            get_keyword_type(raw_token, &token->data.Keyword);
            break;
        case INT_VALUE:
            token->data.Int = atoll(raw_token);
            break;
        case DOUBLE_VALUE:
            token->data.Double = atof(raw_token);
            break;
        default:
            if(token_type == IDENTIFIER || token_type == STRING_VALUE){
                token->data.String = malloc(sizeof(char) * (strlen(raw_token) + 1));
                if(!token->data.String) ERROR_EXIT("Could not allocate memory for token!", INTERNAL_ERROR)
                strcpy(token->data.String, raw_token);
            }
    }
//    printf("%s\n", raw_token);
    free(raw_token);

    return 0;
}
