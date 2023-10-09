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


bool isInt(char *token)
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

bool isBuiltInFunction(char *token, char *built_in_functions[])
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

bool isKeyword(char *token, char *keywords[])
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