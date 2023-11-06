/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file main.c

 * @brief Implementation of the "entry"

 * @author Juraj Remeň - xremen02
*/

#include "scanner.h"
#include "stdio.h"

void print_token(token_t *token)
{
    switch (token->type) {
        case KEYWORD:
            printf("KEYWORD\n");
            printf("%s\n", token->data.String);
            break;
        case IDENTIFIER:
            printf("IDENTIFIER\n");
            printf("%s\n", token->data.String);
            break;
        case TOKEN_EOF:
            printf("TOKEN_EOF\n");
            break;
        case TOKEN_EOL:
            printf("TOKEN_EOL\n");
            break;
        case BUILT_IN_FUNCTION:
            printf("BUILT_IN_FUNCTION\n");
            break;
        case TOKEN_FUNCTION_TYPE:
            printf("TOKEN_FUNCTION_TYPE\n");
            break;
        case NUMBER_VALUE:
            printf("NUMBER_VALUE\n");
            break;
        case STRING_VALUE:
            printf("STRING_VALUE\n");
            printf("%s\n", token->data.String);
            break;
        case INT_VALUE:
            printf("INT_VALUE\n");
            printf("%lld\n", token->data.Int);
            break;
        case NIL_VALUE:
            printf("NIL_VALUE\n");
            break;
        case (PLUS):
            printf("PLUS\n");
            break;
        case (MINUS):
            printf("MINUS\n");
            break;
        case (MUL):
            printf("MULTIPLY\n");
            break;
        case (DIV):
            printf("DIVIDE\n");
            break;
        case (EQUAL):
            printf("EQUAL\n");
            break;
        case (NOT_EQUAL):
            printf("NOT_EQUAL\n");
            break;
        case (LESS_THAN):
            printf("LESS_THAN\n");
            break;
        case (LESS_THAN_OR_EQUAL):
            printf("LESS_THAN_OR_EQUAL\n");
            break;
        case (MORE_THAN):
            printf("MORE_THAN\n");
            break;
        case (MORE_THAN_OR_EQUAL):
            printf("MORE_THAN_OR_EQUAL\n");
            break;
        case (TOKEN_LEFT_BRACKET):
            printf("TOKEN_LEFT_BRACKET\n");
            break;
        case (TOKEN_RIGHT_BRACKET):
            printf("TOKEN_RIGHT_BRACKET\n");
            break;
        case (TOKEN_LEFT_CURLY_BRACKET):
            printf("TOKEN_LEFT_CURLY_BRACKET\n");
            break;
        case (TOKEN_RIGHT_CURLY_BRACKET):
            printf("TOKEN_RIGHT_CURLY_BRACKET\n");
            break;
        case (COMMA):
            printf("COMMA\n");
            break;
        case (COLON):
            printf("COLON\n");
            break;
        case (NOT):
            printf("NOT\n");
            break;
        default:
            printf("%d\n", token->type);
            break;
    }
}


int main()
{
    set_source_file(stdin);
    token_t *token = malloc(sizeof(token_t));
    if(!token) return 1;

    while (true)  // Load all the tokens
    {
        get_next_token(token);
        if(token->type == TOKEN_EOF) break;
        print_token(token);
    }


    free(token);

    return 0;
}