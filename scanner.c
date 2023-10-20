/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file scanner.c

 * @brief Scanner implementation
 
 * @author Juraj Remeň - xremen02
*/

#include "scanner.h"

#include "ctype.h"

char *keywords[] = {"Double", "else", "func", "if", "Int", "let", "nil", "return", "String", "var", "while"};
char *built_in_functions[] = {"readString", "readInt", "readDouble", "write", "Int2Double", "Double2Int", "length", "substring", "ord", "chr"};


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

bool is_operator(int x)
{
    return (x == '+' || x == '-' || x == '*' || x == '/' || x == '=');
}

bool is_negative(token_t *prev_token)
{
    if (prev_token)
    {
        if ((prev_token->type == TOKEN_OPERATOR && strcmp(prev_token->data.String, "=") == 0)||
            (prev_token->type == TOKEN_SEPARATOR && strcmp(prev_token->data.String, ",") == 0)) {
            return true;
        }
    }
    return false;
}

bool is_built_in_function(char *token, char *built_in_functions[])
{
    for (int i = 0; i < 12; i++)
    {
        if (strcmp(token, built_in_functions[i]) == 0)
        {
            return true;
        }
    }

        return false;
}

bool is_keyword(char *token, char *keywords[])
{
    for (int i = 0; i < 12; i++)
    {
        if (strcmp(token, keywords[i]) == 0)
        {
            return true;
        }
    }

        return false;
}

bool is_function_type(token_t *prev_token)
{
    if (prev_token != NULL && prev_token->prev != NULL && prev_token->type == TOKEN_SEPARATOR) {

        if (prev_token->prev->type == TOKEN_FUNCTION_TYPE || prev_token->prev->type == TOKEN_RIGHT_BRACKET)
            return true;
    }
    return false;
}

bool scanner_set_nil_value(token_t *prev_token,token_t *token, char *token_array)
{
    if (prev_token && prev_token->type == TOKEN_OPERATOR && strcmp(prev_token->data.String,"=") == 0
        && token->type == KEYWORD && strcmp(token_array,"nil") == 0)
        return true;
    else if (prev_token && prev_token->type == TOKEN_SEPARATOR && strcmp(prev_token->data.String,",") == 0
             && token->type == KEYWORD && strcmp(token_array,"nil") == 0)
        return true;
    else
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
        if (ascii_val < 1 || ascii_val > 255)
            ERROR_EXIT("Invalid escape sequence.\n", LEX_ERROR);

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
        token[token_position -1] = '\0';
    }
}

token_t * scanner_get_first_token(token_t *token)
{
    token_t *next = token;
    while (next && next->prev)
        next = next->prev;

    return next;
}

void scanner_free_tokens(token_t *first_token)
{
    if (!first_token)
        return;

    if (first_token->prev)
        first_token = scanner_get_first_token(first_token);

    while (first_token->next)
    {
        token_t *token_free = first_token;
        first_token = first_token->next;

        free(token_free);
    }
}

char * scanner_get_token_array(unsigned long long size)
{
    char * token_array = (char *)calloc(size + 1, sizeof(char));
    if (token_array)
        return token_array;
    ERROR_EXIT("Could not allocate memory for token array!", INTERNAL_ERROR);
}

char * scanner_add_char_to_token_array(char * array, unsigned long long *size, unsigned long long *i, int c)
{
    if (*size == *i) {
        *(size) *= 2;
        char *reallocated = realloc(array, *size);
        if (reallocated)
            array = reallocated;
        ERROR_EXIT("Could not allocate memory for token!", INTERNAL_ERROR);
    }

    array[(*i)++] = (char) c;
    return array;
}

token_type_t scanner_get_token_type(char *token, scanner_states_t actual_state)
{
    if (is_keyword(token, keywords))
        return KEYWORD;
    else if (is_built_in_function(token, built_in_functions))
        return BUILT_IN_FUNCTION;
    else if (actual_state == KEYWORD_OR_IDENTIFIER)
        return IDENTIFIER;
    else if (actual_state == NUMBER || actual_state == EXPONENT) {
        if (is_integer(token))
            return INT_VALUE;
        return NUMBER_VALUE;
    } else if (actual_state == OPERATOR)
        return TOKEN_OPERATOR;
    else if (actual_state == SEPARATOR)
        return TOKEN_SEPARATOR;
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
}

token_t * scanner_get_token_struct(char *token_array, unsigned long long actual_line, scanner_states_t actual_state, token_t *prev_token)
{
    token_t *token = (token_t *)malloc(sizeof(token_t));
    if (!token)
        ERROR_EXIT("Could not allocate memory for token!", INTERNAL_ERROR);

    token->line = actual_line;
    token->type = scanner_get_token_type(token_array, actual_state);

    token->next = NULL;
    if (prev_token)
        prev_token->next = token;
    else
        token->prev = NULL;

    //if the current token_type = func_params, then previous token_type = function_name
    if (token->type == TOKEN_LEFT_BRACKET && prev_token->type != BUILT_IN_FUNCTION && prev_token->type != KEYWORD && prev_token->type != TOKEN_OPERATOR && prev_token->type != TOKEN_SEPARATOR && prev_token->type != TOKEN_RIGHT_BRACKET)
    {
        prev_token->type = TOKEN_FUNCTION_TYPE;
    }

    if (scanner_set_nil_value(prev_token, token, token_array))
    {
        token->data.String = NULL;
        token->type = NIL_VALUE;
    }

    if (is_function_type(prev_token))
        token->type = TOKEN_FUNCTION_TYPE;

    if(token->type == STRING_VALUE && token_array[0] != '\0') {
        size_t len = strlen(token_array);
        size_t new_len = len;
        size_t j = 0;
        char temp[3];

        char *temp_str = (char *)malloc(sizeof(char ) * (len + 1));

        for (size_t i = 0; i < len; i++)
        {
            if (token_array[i] < 33 || token_array[i] == 35 || (token_array[i] == 92 && !isdigit(token_array[i+1])))
            {
                new_len += 4;
                temp_str = realloc(temp_str, sizeof(char) * (new_len + 1));
                sprintf(temp, "%02d", token_array[i]);

                temp_str[j++] = '\\';
                temp_str[j++] = '0';
                temp_str[j++] = temp[0];
                temp_str[j++] = temp[1];
            } else
                temp_str[j++] = token_array[i];
        }
        temp_str[j] = 0;
        token_array = temp_str;
    } else if(token->type == IDENTIFIER && token_array[0] == '_') {
        token_array[0] = 'l';
    }
    if (token->type == KEYWORD || token->type == IDENTIFIER ||
        token->type == STRING_VALUE || token->type == TOKEN_OPERATOR ||
        token->type == BUILT_IN_FUNCTION ||
        token->type == TOKEN_SEPARATOR || token->type == TOKEN_FUNCTION_TYPE ||
        token->type == TOKEN_LEFT_BRACKET || token->type == TOKEN_RIGHT_BRACKET ||
        token->type == TOKEN_LEFT_CURLY_BRACKET || token->type == TOKEN_RIGHT_CURLY_BRACKET) {
        token->data.String = token_array;
    } else if (token->type == INT_VALUE) {
        token->data.Int = atoll(token_array);
    } else if (token->type == NUMBER_VALUE) {
        token->data.Double = atof(token_array);
    } else if (token->type == NIL_VALUE) {
        token->data.Int = 0;
    }
    token->prev = prev_token;

    return token;
}

token_t * scanner_get_token(FILE *file, unsigned long long *line, token_t *previous_token)
{
    int c;
    enum scanner_states state = NEW_TOKEN;
    enum scanner_states temp_state = NEW_TOKEN;
    enum scanner_states next_state = NEW_TOKEN;

    unsigned long long token_length = DEFAULT_TOKEN_LENGTH;
    unsigned long long token_position = 0;
    int escaped_counter = 0;
    int offset = 0;
    bool exponent = false;
    bool first_exp_char = false;
    bool double_quotes_found = false;


    char *raw_token = scanner_get_token_array(token_length);

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n' && state != STRING)
            (*line)++;

        bool add_character = true;
        switch (state)
        {
            case NEW_TOKEN:
                if (c == '\'')
                    ERROR_EXIT("Simple quotes are not supported.\n", LEX_ERROR);
                if (c == '"')
                {
                    double_quotes_found = true;
                    add_character = false;
                    temp_state = NEW_TOKEN;
                    next_state = STRING;
                }
                if (c == ')')
                    next_state = RIGHT_BRACKET;
                else if (c == '(') {
                    next_state = LEFT_BRACKET;
                }else if (c == '_' || isalpha(c))
                    next_state = KEYWORD_OR_IDENTIFIER;
                else if (is_operator(c)) {
                    if (c == '-' && is_negative(previous_token)) {
                        next_state = NUMBER;
                    } else {
                        next_state = OPERATOR;
                    }
                }else if (isdigit(c))
                    next_state = NUMBER;
                else if (c == ',' || c == ':')
                    next_state = SEPARATOR;
                else if(isspace(c))
                    continue;
                break;
            case STRING:
                if (c == '"'){
                    if (double_quotes_found)  //empty string
                        c = '\0';
                    else
                        add_character = false;
                    double_quotes_found = false;
                    next_state = temp_state;
                }else if (c == '\\')
                    next_state = ESCAPE;
                break;
            case ESCAPE:
                if (escaped_counter == 0 && (c == '"' || c == 'n' || c == 't' || c == '\\')) {
                    offset = 1;

                    next_state = STRING;
                } else if (isdigit(c))
                {
                    escaped_counter++;

                    if (escaped_counter == 3)
                    {
                        offset = 0;
                        escaped_counter = 0;
                        next_state = STRING;
                    }
                } else
                    ERROR_EXIT("Invalid escape sequence.\n", LEX_ERROR);

                if (next_state == STRING) {
                    scanner_add_char_to_token_array(raw_token, &token_length, &token_position, c);
                    scanner_process_escape_sequence(raw_token, token_position);
                    token_position = token_position - offset;
                    add_character = false;
                }
                break;
            case FUNCTION_TYPE:
                if (c == ')')
                    next_state = NEW_TOKEN;
                else if (c == '"')
                {
                    temp_state = FUNCTION_TYPE;
                    next_state = STRING;
                }
                break;
            case NUMBER:
                if (c == '.')
                    next_state = DOT;
                else if (c == 'E' || c == 'e'){
                    next_state = EXPONENT;
                    first_exp_char = true;
                } else if (!isdigit(c)) {
                    add_character = false;
                    ungetc(c, stdin);  // read character could not be the useless whitespace
                    next_state = NEW_TOKEN;
                }
                break;
            case DOT:
                if (exponent)
                    ERROR_EXIT("Invalid number format.\n", LEX_ERROR);
                if (!isdigit(c) || isspace(c))
                    ERROR_EXIT("Invalid number format\n", LEX_ERROR);
                next_state = NUMBER;
                break;
            case EXPONENT:
                exponent = true;
                if (c == '.')
                    ERROR_EXIT("Invalid number format.\n", LEX_ERROR);
                if (first_exp_char && c != '+' && c != '-' && !isdigit(c))
                    ERROR_EXIT("Invalid number format\n", LEX_ERROR);
                if (!isdigit(c) && c != '+' && c != '-') {
                    add_character = false;
                    ungetc(c, stdin);  // read character could not be the useless whitespace
                    next_state = NEW_TOKEN;
                }
                break;
            case KEYWORD_OR_IDENTIFIER:
                if(isspace(c) || c == ',' || c == ')' || is_operator(c) || c == '(' || c == ':' || c == '"')
                {
                    add_character = false;  // skip adding invalid character of identificator
                    ungetc(c, stdin);  // read character could not be the useless whitespace
                    next_state = NEW_TOKEN;
                } else if (!isalnum(c) && c != '_' ){
                    ERROR_EXIT("Invalid identifier.\n", SYNTAX_ERROR);
                }
                break;
            case SEPARATOR:
                next_state = NEW_TOKEN;

                add_character = false;
                ungetc(c, stdin);
                next_state = NEW_TOKEN;
                break;
            case OPERATOR:
                if (c == '/')
                    next_state = COMMENT;
                else if (!is_operator(c))
                {
                    add_character = false;
                    ungetc(c, stdin);  // read character could not be the useless whitespace
                    next_state = NEW_TOKEN;
                }
                break;
            case COMMENT:
                if (c == '/')
                    next_state = COMMENT;
                else {
                    while ((c = fgetc(file)) != EOF) {
                        if (c == '\n') {
                            token_position = 0;
                            raw_token[0] = '\0';
                            next_state = NEW_TOKEN;
                            break;
                        }
                    }
                }
                break;
            case BLOCK_COMMENT:
                if (c == '*')
                    next_state = BLOCK_COMMENT_END;
                else {
                    while ((c = fgetc(file)) != EOF) {
                        if (c == '/') {
                            next_state = BLOCK_COMMENT_END;
                            break;
                        }
                    }
                }
                break;
            case BLOCK_COMMENT_END:
                if (c == '/')
                {
                    token_position = 0;
                    raw_token[0] = '\0';
                    next_state = NEW_TOKEN;
                }else
                    next_state = BLOCK_COMMENT;
                break;
            case LEFT_BRACKET:
                add_character = false;
                ungetc(c, stdin);
                next_state = NEW_TOKEN;
                break;
            case RIGHT_BRACKET:
                add_character = false;
                ungetc(c, stdin);
                next_state = NEW_TOKEN;
                break;
            default:
                break;
        }
        if (add_character && state < COMMENT)
            scanner_add_char_to_token_array(raw_token, &token_length, &token_position, c);

        if (next_state == NEW_TOKEN && state != NEW_TOKEN && state != COMMENT && state != BLOCK_COMMENT_END && state != BLOCK_COMMENT)
            break;

        state = next_state;
    }
    if (state == BLOCK_COMMENT)
        ERROR_EXIT("Could not end inside comment", LEX_ERROR);

    if (!token_position)
        return NULL;

    return scanner_get_token_struct(raw_token, *line, state, previous_token);
}

token_type_t scanner_get_param_token_type(char *token_string)
{
    if (strcmp(token_string, "Int") == 0)
        return INT_VALUE;
    else if (strcmp(token_string, "Double") == 0)
        return NUMBER_VALUE;
    else if (strcmp(token_string, "String") == 0)
        return STRING_VALUE;
    else if (strcmp(token_string, "nil") == 0)
        return NIL_VALUE;
    else
        ERROR_EXIT("Unsupported param type", INTERNAL_ERROR);
}