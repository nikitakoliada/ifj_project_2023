/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file main.c

 * @brief Implementation of the "entry"

 * @author Juraj Remeň - xremen02
*/

#include "analysis.h"

//void print_token(token_t *token)
//{
//    switch (token->type) {
//        case KEYWORD:
//            printf("KEYWORD\n");
//            switch (token->data.Keyword) {
//                case Double_KW:
//                    printf("Double_KW\n");
//                    break;
//                case Else_KW:
//                    printf("Else_KW\n");
//                    break;
//                case If_KW:
//                    printf("If_KW\n");
//                    break;
//                case Int_KW:
//                    printf("Int_KW\n");
//                    break;
//                case Return_KW:
//                    printf("Return_KW\n");
//                    break;
//                case String_KW:
//                    printf("String_KW\n");
//                    break;
//                case Var_KW:
//                    printf("Var_KW\n");
//                    break;
//                case While_KW:
//                    printf("While_KW\n");
//                    break;
//                case Nil_KW:
//                    printf("Nil_KW\n");
//                    break;
//                case Function_KW:
//                    printf("Function_KW\n");
//                    break;
//                case Let_KW:
//                    printf("Let_KW\n");
//                    break;
//                case String_Nullable_KW:
//                    printf("String_Nullable_KW\n");
//                    break;
//                case Int_Nullable_KW:
//                    printf("Int_Nullable_KW\n");
//                    break;
//                case Double_Nullable_KW:
//                    printf("Double_Nullable_KW\n");
//                    break;
//            }
//            break;
//        case IDENTIFIER:
//            printf("IDENTIFIER\n");
//            printf("%s\n", token->data.String);
//            break;
//        case TOKEN_EOF:
//            printf("TOKEN_EOF\n");
//            break;
//        case TOKEN_EOL:
//            printf("TOKEN_EOL\n");
//            break;
//        case BUILT_IN_FUNCTION:
//            printf("BUILT_IN_FUNCTION\n");
//            break;
//        case TOKEN_FUNCTION_TYPE:
//            printf("TOKEN_FUNCTION_TYPE\n");
//            break;
//        case STRING_VALUE:
//            printf("STRING_VALUE\n");
//            printf("%s\n", token->data.String);
//            break;
//        case INT_VALUE:
//            printf("INT_VALUE\n");
//            printf("%lld\n", token->data.Int);
//            break;
//        case (PLUS):
//            printf("PLUS\n");
//            break;
//        case (MINUS):
//            printf("MINUS\n");
//            break;
//        case (MUL):
//            printf("MULTIPLY\n");
//            break;
//        case (DIV):
//            printf("DIVIDE\n");
//            break;
//        case (EQUAL):
//            printf("EQUAL\n");
//            break;
//        case (NOT_EQUAL):
//            printf("NOT_EQUAL\n");
//            break;
//        case (LESS_THAN):
//            printf("LESS_THAN\n");
//            break;
//        case (LESS_THAN_OR_EQUAL):
//            printf("LESS_THAN_OR_EQUAL\n");
//            break;
//        case (MORE_THAN):
//            printf("MORE_THAN\n");
//            break;
//        case (MORE_THAN_OR_EQUAL):
//            printf("MORE_THAN_OR_EQUAL\n");
//            break;
//        case (TOKEN_LEFT_BRACKET):
//            printf("TOKEN_LEFT_BRACKET\n");
//            break;
//        case (TOKEN_RIGHT_BRACKET):
//            printf("TOKEN_RIGHT_BRACKET\n");
//            break;
//        case (TOKEN_LEFT_CURLY_BRACKET):
//            printf("TOKEN_LEFT_CURLY_BRACKET\n");
//            break;
//        case (TOKEN_RIGHT_CURLY_BRACKET):
//            printf("TOKEN_RIGHT_CURLY_BRACKET\n");
//            break;
//        case (COMMA):
//            printf("COMMA\n");
//            break;
//        case (COLON):
//            printf("COLON\n");
//            break;
//        case (NOT):
//            printf("NOT\n");
//            break;
//        case (DOUBLE_VALUE):
//            printf("DOUBLE_VALUE\n");
//            printf("%f\n", token->data.Double);
//            break;
//        case ASSIGNMENT:
//            printf("ASSIGNMENT\n");
//            break;
//        default:
//            printf("%d\n", token->type);
//            break;
//    }
//}


int main(void)
{
    return analyse();
}
