/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file analysis.c

 * @brief Implementation of the analysis.c file part of parser

 * @author Nikita Koliada xkolia00
*/
#include "error.h"
#include "symtable.h"
#include "stdbool.h"
#include "analysis.h"
#include "scanner.h"
// include code generator


void print_token(token_t *token)
{
    switch (token->type) {
        case KEYWORD:
            printf("KEYWORD\n");
            switch (token->data.Keyword) {
                case Double_KW:
                    printf("Double_KW\n");
                    break;
                case Else_KW:
                    printf("Else_KW\n");
                    break;
                case If_KW:
                    printf("If_KW\n");
                    break;
                case Int_KW:
                    printf("Int_KW\n");
                    break;
                case Return_KW:
                    printf("Return_KW\n");
                    break;
                case String_KW:
                    printf("String_KW\n");
                    break;
                case Var_KW:
                    printf("Var_KW\n");
                    break;
                case While_KW:
                    printf("While_KW\n");
                    break;
                case Nil_KW:
                    printf("Nil_KW\n");
                    break;
                case Function_KW:
                    printf("Function_KW\n");
                    break;
                case Let_KW:
                    printf("Let_KW\n");
                    break;
            }
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
        case (DOUBLE_VALUE):
            printf("DOUBLE_VALUE\n");
            printf("%f\n", token->data.Double);
            break;
        case ASSIGNMENT:
            printf("ASSIGNMENT\n");
            break;
        default:
            printf("%d\n", token->type);
            break;
    }
}

//Defining macros for easier work with the parser 
//and for better readability of the code
// not sure if this is the best way to do it 
//can implement it in a different(functions) way if needed

int result = SYNTAX_ERROR;

#define IS_P_TYPE(token)												\
	(token).type == DOUBLE_VALUE						\
	|| (token).type == INT_VALUE						\
	|| (token).type == STRING_VALUE							

#define GET_TOKEN()													\
	if ((result = get_next_token(&data->token)) != 0) {        \
            print_token(&data->token);                             \
            return result;                                         \
        }                                                          \

#define CHECK_TYPE(_type)											\
	if (data->token.type != (_type)) return SYNTAX_ERROR

#define CHECK_RULE(rule)											\
	if ((result = rule(data)) != 0) return result

#define CHECK_KEYWORD(_keyword)										\
	if (															\
		data->token.type != KEYWORD						\
		|| data->token.data.Keyword != (_keyword)				\
	) return SYNTAX_ERR

#define GET_TOKEN_AND_CHECK_TYPE(_type)								\
	do {															\
		GET_TOKEN();												\
		CHECK_TYPE(_type);											\
	} while(0)

#define GET_TOKEN_AND_CHECK_RULE(rule)								\
	do {															\
		GET_TOKEN();												\
		CHECK_RULE(rule);											\
	} while(0)

#define GET_TOKEN_AND_CHECK_KEYWORD(_keyword)						\
	do {															\
		GET_TOKEN();												\
		CHECK_KEYWORD(_keyword);									\
	} while(0)

static bool init_variables(analyse_data_t* data)
{
	symtable_init(&data->global_table);
	symtable_init(&data->local_table);
	data->current_id = NULL	;
    data->var_id = NULL;
    data->expr_id = NULL;

	data->args_index = 0;
	data->label_index = 0;
	data->label_deep = -1;


	data->in_function = false;
	data->in_defintion = false;
	data->in_while_or_if = false;
    //token_t * tmp = malloc(sizeof(token_t));
    //data->token = *tmp;

	return true;
}



//Frees symbol tables
static void free_variables(analyse_data_t* data)
{
	symtable_dispose(&data->global_table);
	symtable_dispose(&data->local_table);
}

// forward declarations(LL table)
static int program(analyse_data_t* data);
static int statement(analyse_data_t* data); 
static int function(analyse_data_t* data);
static int func_ret(analyse_data_t* data);
static int args(analyse_data_t* data);
static int args_n(analyse_data_t* data);
static int if_else(analyse_data_t* data);
static int while_(analyse_data_t* data);
static int assignment(analyse_data_t* data);
static int def_var(analyse_data_t* data);
static int f_call(analyse_data_t* data);
static int fc_args(analyse_data_t* data);
static int fc_n_args(analyse_data_t* data);
static int modifier(analyse_data_t* data);
static int def_type(analyse_data_t* data);
static int end(analyse_data_t* data);
static int possible_EOL(analyse_data_t* data);
static int ident(analyse_data_t* data);
static int type(analyse_data_t* data);
static int q_value(analyse_data_t* data); // ? before p_type 
static int p_type(analyse_data_t* data);// ? before p_type 

static int program(analyse_data_t* data)
{
    // <program> -> <statement>
	return statement(data);
}

static int statement(analyse_data_t* data)
{
    // 〈 statement 〉 −→ 〈 function 〉 EOL 〈 statement 〉
    if(data->token.type == KEYWORD && data->token.data.Keyword == Function_KW)
    {
        data->label_deep++;
        CHECK_RULE(function);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    // 〈 statement 〉 −→ 〈 if_else 〉 EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && data->token.data.Keyword == If_KW){
        CHECK_RULE(if_else);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    // 〈 statement 〉 −→ 〈 while 〉EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && data->token.data.Keyword == While_KW){
        data->label_deep++;
        CHECK_RULE(while_);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    //5. 〈 statement 〉 −→ 〈 assignment 〉EOL 〈 statement 〉
    else if(data->token.type == ASSIGNMENT){
        CHECK_RULE(assignment);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    //6. 〈 statement 〉 −→ 〈def_var 〉 EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && (data->token.data.Keyword == Let_KW || data->token.data.Keyword == Var_KW)){
        CHECK_RULE(def_var);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    //7. 〈 statement 〉 −→ 〈 f_call 〉EOL 〈 statement 〉
    else if(data->token.type == IDENTIFIER){
        //TODO somwehow find out if it is a function call( should be inside f_call ig)
        data->current_id = symtable_search(&data->global_table, data->token.data.String); 
        CHECK_RULE(f_call);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    //8. 〈 statement 〉 −→ EOL 〈 statement 〉
    else if(data->token.type == TOKEN_EOL){
        GET_TOKEN();
        return statement(data);
    }
    //9. 〈 statement 〉 −→ 〈 end 〉
    else if(data->token.type == KEYWORD && data->token.data.Keyword == TOKEN_EOF){
        return end(data); // TODO ig idk maybe inside end
    }
    //10. 〈 statement 〉 −→ ε
    return SYNTAX_OK;
}

static int function(analyse_data_t* data){
    //11. 〈 function 〉 −→ func 〈 ID 〉( 〈 args 〉) 〈 func_ret 〉{ EOL 〈 statement 〉 }
    if(data->token.type == KEYWORD && data->token.data.Keyword == Function_KW){
        data->in_function = true;
		data->label_index = 0;
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        // if function wasn't declared add it to the global symbol table
        data->current_id = symtable_search(&data->global_table, data->token.data.String);
        function_data_t* func_data = NULL;
		if (data->current_id == NULL)
		{   	
            func_data = malloc(sizeof(function_data_t));
            symtable_insert_function(&data->global_table, data->token.data.String, func_data);
            *data->current_id = symtable_search(&data->global_table,  data->token.data.String);
        }
		else
		{
            func_data = (function_data_t*)(*data->current_id)->data;
			if (func_data->defined)
				return SEM_ERROR_UNDEF_VAR;
		}		
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_LEFT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(args);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_RIGHT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(func_ret);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        func_data->defined = true;
        data->in_function = false;
        data->label_deep--;
        symtable_dispose(&data->local_table);
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

static int func_ret(analyse_data_t* data){
    function_data_t* func_data = (function_data_t*)(*data->current_id)->data;
    //12. 〈 func_ret 〉 −→ 〈 FUNCTION_TYPE 〉〈 type 〉
    if(data->token.type == TOKEN_FUNCTION_TYPE){
        GET_TOKEN();
			switch (data->token.data.Keyword)
			{
			case Int_KW:
				func_data->return_data_type = INT_VALUE;
				break;

			case Double_KW:
				func_data->return_data_type = DOUBLE_VALUE;
				break;

			case String_KW:
				func_data->return_data_type = STRING_VALUE;
				break;
            case Nil_KW:
				func_data->return_data_type = NIL_VALUE;
				break;
			default:
				return SYNTAX_ERROR;
			}
    }
    //13. 〈 func_ret 〉 −→ ε
    func_data->return_data_type = NULL;
    return SYNTAX_OK;

}

static int args(analyse_data_t* data){
	data->args_index = 0;
    //14. 〈 args 〉 −→ id id : 〈type 〉〈args_n 〉
    if(data->token.type == IDENTIFIER){
        function_data_t* func_data = (function_data_t*)(*data->current_id)->data;
        func_data->param_names[data->args_index] = data->token.data.String;
        // if there is function named as parameter
		if (symtable_search(&data->global_table, data->token.data.String))
			return SEM_ERROR_UNDEF_VAR;

        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        // we are in defintion, we need to add parameters to the local symbol table
        func_data->params_identifiers[data->args_index] == data->token.data.String;
        GET_TOKEN_AND_CHECK_TYPE(COLON);
        GET_TOKEN_AND_CHECK_RULE(type);
        var_data_t var_data = func_data->params_types[data->args_index];
        symtable_insert_var(&data->local_table, data->token.data.String, &var_data);	
        GET_TOKEN_AND_CHECK_RULE(args_n);
        func_data->param_len = data->args_index;
    }    
    //15. 〈 args 〉 −→ ε

    return SYNTAX_OK;
}
static int args_n(analyse_data_t* data){
    //17. 〈 args_n 〉 −→ , id id : 〈type 〉〈args_n 〉
    if(data->token.type == COMMA){
        function_data_t* func_data = (function_data_t*)(*data->current_id)->data;
        func_data->param_names[data->args_index] = data->token.data.String;
        // if there is function named as parameter
		if (symtable_search(&data->global_table, data->token.data.String))
			return SEM_ERROR_UNDEF_VAR;

        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        // we are in defintion, we need to add parameters to the local symbol table
        func_data->params_identifiers[data->args_index] == data->token.data.String;
        GET_TOKEN_AND_CHECK_TYPE(COLON);
        GET_TOKEN_AND_CHECK_RULE(type);
        var_data_t var_data = func_data->params_types[data->args_index];
        symtable_insert_var(&data->local_table, data->token.data.String, &var_data);	
        data->args_index++;
        GET_TOKEN_AND_CHECK_RULE(args_n);
    }    
//16. 〈 args_n 〉 −→ ε
    return SYNTAX_OK;
}

// //TODO no rule for expression
static int if_else(analyse_data_t* data){
//     //18. 〈 if_else 〉 −→ if 〈 expression 〉{ 〈 statement 〉} 〈 possible_EOL 〉else { 〈 statement 〉}
//     if(data->token.type == KEYWORD && data->token.data.Keyword == If_KW){
//         GET_TOKEN_AND_CHECK_RULE(expression);
//         CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
//         data->label_deep++;
//         data->in_while_or_if = true;
//         GET_TOKEN_AND_CHECK_RULE(statement);
//         CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
//         GET_TOKEN_AND_CHECK_RULE(possible_EOL);
//         CHECK_TYPE(KEYWORD && data->token.data.Keyword == Else_KW);
//         CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
//         GET_TOKEN_AND_CHECK_RULE(statement);
//         CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);

//         data->label_deep--;
// 		data->in_while_or_if = false;
//         return SYNTAX_OK;
//     }
    return SYNTAX_ERROR;
}

static int while_(analyse_data_t* data){
//     //19. 〈 while 〉 −→ while 〈 expression 〉{ 〈 statement 〉}
//     if(data->token.type == KEYWORD && data->token.data.Keyword == While_KW){
//         data->label_deep++;
//         data->in_while_or_if = true;
//         GET_TOKEN_AND_CHECK_RULE(expression);
//         GET_TOKEN_AND_CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
//         GET_TOKEN_AND_CHECK_RULE(statement);
//         GET_TOKEN_AND_CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
//         data->label_deep--;
//         data->in_while_or_if = false;
//         return SYNTAX_OK;
//     }
    return SYNTAX_ERROR;
}

static int assignment(analyse_data_t* data){
//     //20. 〈 assignment 〉 −→ 〈 id 〉 = 〈 expression 〉
//     GET_TOKEN();
//     if(data->token.type == IDENTIFIER){
//         data->var_id = symtable_search(&data->global_table, data->token.data.String); 
//         if(data->var_id == NULL){
//             return SEM_ERROR_UNDEF_VAR;
//         }
//         GET_TOKEN_AND_CHECK_TYPE(ASSIGNMENT);
//         GET_TOKEN_AND_CHECK_RULE(expression);
//         return SYNTAX_OK;
//     }
//     //21. 〈 assignment 〉 −→ 〈 modifier 〉〈 id 〉〈 def_type 〉 = 〈 expression 〉
//     else if(data->token.type == KEYWORD)
//     {
//         CHECK_RULE(modifier);
//         GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
//         data->var_id = symtable_search(&data->global_table, data->token.data.String);
//         if(data->var_id != NULL){
//             return SYNTAX_ERROR;
//         }
//         GET_TOKEN_AND_CHECK_RULE(def_type);
//         GET_TOKEN_AND_CHECK_TYPE(ASSIGNMENT);
//         GET_TOKEN_AND_CHECK_RULE(expression);
//         return SYNTAX_OK;
//     }
    return SYNTAX_ERROR;
}

static int def_var(analyse_data_t* data){
//     //22. 〈def_var 〉−→ 〈 modifier 〉id :〈type 〉
//     if(data->token.type == KEYWORD){
//         CHECK_RULE(modifier);
//         GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
//         data->var_id = symtable_search(&data->global_table, data->token.data.String);
//         if(data->var_id != NULL){
//             return SYNTAX_ERROR;
//         }
//         GET_TOKEN_AND_CHECK_TYPE(COLON);
//         GET_TOKEN_AND_CHECK_RULE(type);
//         //TODO add type?
//         symtable_insert_var(&data->global_table, data->token.data.String);
//         return SYNTAX_OK;
//     }
    return SYNTAX_ERROR;
}

// //23. 〈f_call 〉−→ id ( 〈fc_args 〉)
static int f_call(analyse_data_t* data){
//     if(data->token.type == IDENTIFIER){
//         data->current_id = symtable_search(&data->global_table, data->token.data.String);
//         if(data->current_id == NULL){
//             return SEM_ERROR_UNDEF_VAR;
//         }
//         GET_TOKEN_AND_CHECK_TYPE(TOKEN_LEFT_BRACKET);
//         GET_TOKEN_AND_CHECK_RULE(fc_args);
//         GET_TOKEN_AND_CHECK_TYPE(TOKEN_RIGHT_BRACKET);
//         return SYNTAX_OK;
//     }
    return SYNTAX_ERROR;
}

// //24. 〈 fc_args 〉−→ id: expression 〈fc_ n_args 〉
static int fc_args(analyse_data_t* data){
//     data->args_index = 0;
//     if(data->token.type == IDENTIFIER){
//         data->var_id = symtable_search(&data->local_table, data->token.data.String);
//         if(data->var_id != data->current_id.param_names[data->args_index]){
//             return SEM_ERROR_UNDEF_VAR;
//         }
//         GET_TOKEN_AND_CHECK_TYPE(COLON);
//         GET_TOKEN_AND_CHECK_RULE(expression);
//         GET_TOKEN_AND_CHECK_RULE(fc_n_args);
//         return SYNTAX_OK;
//     }
//     //25. 〈fc_args 〉−→ ε
    return SYNTAX_OK;
}

// //26. 〈fc_n_args 〉−→ , id: expression 〈fc_ n_args 〉
static int fc_args_n_args(analyse_data_t* data){
//     if(data->token.type == COMMA){
//         data->args_index++;
//         GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
//         data->var_id = symtable_search(&data->local_table, data->token.data.String);
//         if(data->var_id != data->current_id.param_names[data->args_index]){
//             return SEM_ERROR_UNDEF_VAR;
//         }
//         GET_TOKEN_AND_CHECK_TYPE(COLON);
//         GET_TOKEN_AND_CHECK_RULE(expression);
//         GET_TOKEN_AND_CHECK_RULE(fc_n_args);
//         return SYNTAX_OK;
//     }
//     //25. 〈fc_args 〉−→ ε
    return SYNTAX_OK;
}

static int modifier(analyse_data_t* data){
//     //28. 〈 modifier 〉−→ let
//     if(data->token.type == KEYWORD && data->token.data.Keyword == Let_KW){
//         GET_TOKEN();
//         return SYNTAX_OK;
//     }
//     //29. 〈 modifier 〉−→ var
//     else if(data->token.type == KEYWORD && data->token.data.Keyword == Var_KW){
//         GET_TOKEN();
//         return SYNTAX_OK;
//     }
    return SYNTAX_ERROR;
}

static int def_type(analyse_data_t* data){
//     //30. 〈 def_type 〉−→ :〈type 〉
//     if(data->token.type == COLON){
//         GET_TOKEN_AND_CHECK_RULE(type);
//         return SYNTAX_OK;
//     }
//     //31. 〈def_type 〉−→ ε

    return SYNTAX_ERROR;
}

// //32. 〈 end 〉 −→ EOF
static int end(analyse_data_t* data){
//     if(data->token.type == TOKEN_EOF){
//         return SYNTAX_OK;
//     }
    return SYNTAX_ERROR;
}

// //33. 〈 possible_EOL 〉 −→ EOL
static int possible_EOL(analyse_data_t* data){
//     if(data->token.type == TOKEN_EOL){
//         GET_TOKEN();
//         return SYNTAX_OK;
//     }
//     //34. 〈 possible_EOL 〉 −→ ε
    return SYNTAX_ERROR;
}

// //35. 〈 id 〉 −→ identifier
static int id(analyse_data_t* data){
//     if(data->token.type == IDENTIFIER){
//         GET_TOKEN();
//         return SYNTAX_OK;
//     }
    return SYNTAX_ERROR;
}

static int type(analyse_data_t* data){
    //36. 〈 type 〉 −→ 〈 p_type 〉 〈 ?_value 〉
    if(data->token.type == KEYWORD && (data->token.data.Keyword == Int_KW || data->token.data.Keyword == Double_KW || data->token.data.Keyword == String_KW)){
        GET_TOKEN_AND_CHECK_RULE(p_type);
        GET_TOKEN_AND_CHECK_RULE(q_value);
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}


//37. 〈 ?_value 〉 −→ ?
static int q_value(analyse_data_t* data){
    if(data->token.type == NIL_COLL){
        if (data->in_defintion)
        {
            function_data_t* func_data = (function_data_t*)(*data->current_id)->data;
            func_data->params_types[data->args_index].q_type = true;
        }
        else{
            var_data_t* var_data = (var_data_t*)(*data->var_id)->data;
            var_data->q_type = true;
        }
        return SYNTAX_OK;
    }
    //38. 〈 ?_value 〉 −→ ε
    else{
        if (data->in_defintion)
        {
            function_data_t* func_data = (function_data_t*)(*data->current_id)->data;
            func_data->params_types[data->args_index].q_type = false;
        }
        else{
            var_data_t* var_data = (var_data_t*)(*data->var_id)->data;
            var_data->q_type = false;
        }
        return SYNTAX_OK;
    }
}

static int p_type(analyse_data_t* data){
    if(data->token.type == KEYWORD){
        switch (data->token.data.Keyword)
		{
        //39. 〈 p_type 〉 −→ Double
        case Double_KW:
            if (data->in_defintion)
			{
                function_data_t* func_data = (function_data_t*)(*data->current_id)->data;
                func_data->params_types[data->args_index].data_type = 'd';
			}
            else{
                var_data_t* var_data = (var_data_t*)(*data->var_id)->data;
                var_data->data_type = 'd';
            }
            return SYNTAX_OK;
        break;
        //40. 〈 p_type 〉 −→ Int
        case Int_KW:
            if (data->in_defintion)
			{
                function_data_t* func_data = (function_data_t*)(*data->current_id)->data;
                func_data->params_types[data->args_index].data_type = 'i';
			}
            else{
                var_data_t* var_data = (var_data_t*)(*data->var_id)->data;
                var_data->data_type = 'i';
            }
            return SYNTAX_OK;
        break;;
        
        //41. 〈 p_type 〉 −→ String
        case String_KW:
            if (data->in_defintion)
			{
                function_data_t* func_data = (function_data_t*)(*data->current_id)->data;
 
                func_data->params_types[data->args_index].data_type = 's';
			}
            else{
                var_data_t* var_data = (var_data_t*)(*data->var_id)->data;
                var_data->data_type = 's';
            }
            return SYNTAX_OK;
        break;
        default:
            return SYNTAX_ERROR;
        }
    }
    return SYNTAX_ERROR;
}
int main()
{
    // symtable_t *table = malloc(sizeof(symtable_t));
    // symtable_init(table);
    // var_data_t *var = malloc(sizeof(var_data_t));
    // var->data_type = 'i';
    // var->q_type = false;
    // symtable_insert_var(table, "a", var);
    // var_data_t *var2 = malloc(sizeof(var_data_t));
    // var2->data_type = 'd';
    // var2->q_type = false;
    // symtable_insert_var(table, "b", var2);
    // var_data_t *var23 = malloc(sizeof(var_data_t));
    // var23->data_type = 's';
    // var23->q_type = false;
    // symtable_insert_var(table, "c", var23);
    // bst_node_ptr n = symtable_search(table, "a");
    // printf("%c\n", ((var_data_t*)n->data)->data_type);
    // n = symtable_search(table, "b");
    // printf("%c\n", ((var_data_t*)n->data)->data_type);
    // n = symtable_search(table, "c");
    // printf("%c\n", ((var_data_t*)n->data)->data_type);
    // function_data_t *func = malloc(sizeof(function_data_t));
    // func->return_data_type = 1;
    // func->defined = false;
    // func->param_len = 1;
    // func->params_identifiers[0] = "a";
    // func->params_types[0].data_type = 'i';
    // func->params_types[0].q_type = false;
    // func->param_names[0] = "a";
    // symtable_insert_function(table, "f", func);
    // bst_node_ptr nn = symtable_search(table, "f");
    // if(nn == NULL)
    //     printf("null\n");
    // function_data_t* func_ret = ((function_data_t*)nn->data);
    // printf("%s\n", func_ret->params_identifiers[0]);

    // char *input = "func decrement(of n: Int, by m: Int) -> Int {\nreturn n - m\n}";
    // FILE *file = fmemopen(input, strlen(input), "r");
    // set_source_file(file);
    // analyse_data_t *data = malloc(sizeof(analyse_data_t));
    
    // if (!init_variables(data))
    //     return INTERNAL_ERROR;

    // result = get_next_token(&data->token);

    // result = program(data);
    // if(result == SYNTAX_OK)
    //     printf("OK\n");
    // else
    //     printf("ERROR\n");
    // free_variables(data);

    return result;
}

