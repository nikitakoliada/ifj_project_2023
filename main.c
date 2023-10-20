/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file main.c

 * @brief Implementation of the "entry"

 * @author Juraj Remeň - xremen02
*/

#include "scanner.h"
#include "stdio.h"

token_t *initializeTokens()
{
    token_t *token = (token_t *) malloc(sizeof(token_t));
    char *data = (char *)malloc(sizeof(char) * 8);

    token->type = KEYWORD;
    token->prev = NULL;
    token->line = 0;
    token->data.String = data;

    token_t *token2 = (token_t *)malloc(sizeof(token_t));
    char *data2 = (char *)malloc(sizeof(char) * 8);

    token2->type = STRING_VALUE;
    token2->next = NULL;
    token2->prev = token;
    token2->line = 0;
    token2->data.String = data2;
    token->next = token2;

    return token;
}

int main(int argc, char *argv[])
{
    token_t *token = NULL;
    token_t *next_token = NULL;
    unsigned long long line = 1;

    while (true)  // Load all the tokens
    {
        next_token = scanner_get_token(stdin, &line, token);
        if (next_token == NULL)
            break;
        token = next_token;
    }
    token = scanner_get_first_token(token);

    scanner_free_tokens(token);

    return 0;
}