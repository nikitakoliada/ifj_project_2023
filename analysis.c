/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file analysis.c

 * @brief Implementation of the analysis.c file part of parser

 * @author Nikita Koliada xkolia00
*/
#include "error.h"
#include "symtable.h"
#include "analysis.h"
// include code generator


//Defining macros for easier work with the parser 
//and for better readability of the code
// not sure if this is the best way to do it 
//can implement it in a different(functions) way if needed

#define IS_P_TYPE(token)												\
	(token).type == DOUBLE_VALUE						\
	|| (token).type == INT_VALUE						\
	|| (token).type == STRING_VALUE							

#define GET_TOKEN()													\
	if ((result = get_next_token(&data->token)) != 0)\
		return result

#define CHECK_TYPE(_type)											\
	if (data->token.type != (_type)) return SYNTAX_ERR

#define CHECK_RULE(rule)											\
	if ((result = rule(data))) return result

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

	data->current_id = NULL;
	data->lhs_id = NULL;
	data->rhs_id = NULL;

	data->param_index = 0;
	data->label_index = 0;
	data->label_deep = -1;

	data->in_function = false;
	data->in_declaration = false;
	data->in_while_or_if = false;
	data->non_declared_function = false;

	return true;
}



//Frees symbol tables
static void free_variables(analyse_data_t* data)
{
	sym_table_free(&data->global_table);
	sym_table_free(&data->local_table);
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
static int possible_EOL(analyse_data_t* data)
static int id(analyse_data_t* data);
static int type(analyse_data_t* data);
static int q_value(analyse_data_t* data) // ? before p_type 
static int p_value(analyse_data_t* data) // ? before p_type 

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
        data->lhs_id = symtable_search(&data->global_table, data->token.attribute.string); 
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
    else if(data->token.type == KEYWORD && data->token.Keyword == End_KW){
        return end(data); // TODO ig idk maybe inside end
    }
    //10. 〈 statement 〉 −→ ε
    return SYNTAX_OK;
}

static int function(analyse_data_t* data){
    //11. 〈 function 〉 −→ func 〈 ID 〉( 〈 args 〉) 〈 func_ret 〉{ EOL 〈 statement 〉 }
    if(data->token.type == KEYWORD && data->token.Keyword == Function_KW){
        data->in_function = true;
		data->label_index = 0;
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        // if function wasn't declared add it to the global symbol table
        data->current_id = symtable_search(&data->global_table, data->token.data.String);	
		if (data->current_id == NULL)
		{   data->non_declared_function = true;		
            symtable_insert_function(&data->global_table, data->token.data.String);	
            data->current_id = symtable_search(&data->global_table,  data->token.data.String);
        }
		else
		{
			if (data->current_id->defined)
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
        data->current_id->defined = true;
        data->in_function = false;
        data->label_deep--;
        sym_table_free(&data->local_table);
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;

}
static int func_ret(analyse_data_t* data){
    //12. 〈 func_ret 〉 −→ 〈 FUNCTION_TYPE 〉〈 type 〉
    if(data->token.type == TOKEN_FUNCTION_TYPE){
        GET_TOKEN();
			switch (data->token.data.Keyword)
			{
			case Int_KW:
				data->current_id->data->return_data_type = INT_VALUE;
				break;

			case Double_KW:
				data->current_id->data->return_data_type = DOUBLE_VALUE;
				break;

			case String_KW:
				data->current_id->data->return_data_type = STRING_VALUE;
				break;
            case Nil_KW:
				data->current_id->data->return_data_type = NIL_VALUE;
				break;
			default:
				return SYNTAX_ERROR;
			}
		
		else
			return SYNTAX_ERROR;
    }
    //13. 〈 func_ret 〉 −→ ε
    data->current_id->data->return_data_type = NULL;
    return SYNTAX_OK;

}

static int args(analyse_data_t* data){
	data->param_index = 0;
    //14. 〈 args 〉 −→ id id : 〈type 〉〈args_n 〉
    if(data->token.type == IDENTIFIER){
        data->current_id.param_names += data->token.data.String;
        // if there is function named as parameter
		if (symtable_search(&data->global_table, data->token.data.String))
			return SEM_ERROR_UNDEF_VAR;

        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        // we are in defintion, we need to add parameters to the local symbol table
        symtable_insert_var(&data->local_table, data->token.data.String);	
        data->current_id.param += data->token.data.String;
        GET_TOKEN_AND_CHECK_TYPE(COLON);
        GET_TOKEN_AND_CHECK_RULE(type);
        GET_TOKEN_AND_CHECK_RULE(args_n);
        data->current_id.param_len = data->param_index;
    }    
    //15. 〈 args 〉 −→ ε

    return SYNTAX_OK;
}
static int args_n(analyse_data_t* data){
    //17. 〈 args_n 〉 −→ , id id : 〈type 〉〈args_n 〉
    if(data->token.type == COMMA){
        data->current_id.param_names += data->token.data.String;
        // if there is function named as parameter
		if (symtable_search(&data->global_table, data->token.data.String))
			return SEM_ERROR_UNDEF_VAR;

        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        // we are in defintion, we need to add parameters to the local symbol table
        symtable_insert_var(&data->local_table, data->token.data.String)	
        data->current_id.param += data->token.data.String;
        GET_TOKEN_AND_CHECK_TYPE(COLON);
        GET_TOKEN_AND_CHECK_RULE(type);
        data->param_index++;
        GET_TOKEN_AND_CHECK_RULE(args_n);
    }    
//16. 〈 args_n 〉 −→ ε
    return SYNTAX_OK;
}

//TODO no rule for expression
static int if_else(analyse_data_t* data){
    //18. 〈 if_else 〉 −→ if 〈 expression 〉{ 〈 statement 〉} 〈 possible_EOL 〉else { 〈 statement 〉}
    if(data->token.type == KEYWORD && data->token.data.Keyword == If_KW){
        GET_TOKEN_AND_CHECK_RULE(expression);
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        data->label_deep++;
        data->in_while_or_if = true;
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(possible_EOL);
        CHECK_TYPE(KEYWORD && data->token.data.Keyword == Else_KW);
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);

        data->label_deep--;
		data->in_while_or_if = false;
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

static int while_(analyse_data_t* data){
    //19. 〈 while 〉 −→ while 〈 expression 〉{ 〈 statement 〉}
    if(data->token.type == KEYWORD && data->token.data.Keyword == While_KW){
        data->label_deep++;
        data->in_while_or_if = true;
        GET_TOKEN_AND_CHECK_RULE(expression);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(statement);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        data->label_deep--;
        data->in_while_or_if = false;
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

static int assignment(analyse_data_t* data){
    //20. 〈 assignment 〉 −→ 〈 id 〉 = 〈 expression 〉
    GET_TOKEN();
    if(data->token.type == IDENTIFIER){
        data->var_id = symtable_search(&data->global_table, data->token.data.String); 
        if(data->var_id == NULL){
            return SEM_ERROR_UNDEF_VAR;
        }
        GET_TOKEN_AND_CHECK_TYPE(ASSIGNMENT);
        GET_TOKEN_AND_CHECK_RULE(expression);
        return SYNTAX_OK;
    }
    //21. 〈 assignment 〉 −→ 〈 modifier 〉〈 id 〉〈 def_type 〉= 〈 expression 〉
    else if(data->token.type == KEYWORD)
    {
        CHECK_RULE(modifier);
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        data->var_id = symtable_search(&data->global_table, data->token.data.String);
        if(data->var_id != NULL){
            return SYNTAX_ERROR;
        }
        GET_TOKEN_AND_CHECK_RULE(def_type);
        GET_TOKEN_AND_CHECK_TYPE(ASSIGNMENT);
        GET_TOKEN_AND_CHECK_RULE(expression);
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

static int def_var(analyse_data_t* data){
    //22. 〈def_var 〉−→ 〈 modifier 〉id :〈type 〉
    if(data->token.type == KEYWORD){
        CHECK_RULE(modifier);
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        data->var_id = symtable_search(&data->global_table, data->token.data.String);
        if(data->var_id != NULL){
            return SYNTAX_ERROR;
        }
        GET_TOKEN_AND_CHECK_TYPE(COLON);
        GET_TOKEN_AND_CHECK_RULE(type);
        //TODO add type?
        symtable_insert_var(&data->global_table, data->token.data.String);
        return SYNTAX_OK;
    }
        if(data->token.type == KEYWORD){

    return SYNTAX_ERROR;
}

//23. 〈f_call 〉−→ id ( 〈fc_args 〉)
static int f_call(analyse_data_t* data){
    if(data->token.type == IDENTIFIER){
        data->current_id = symtable_search(&data->global_table, data->token.data.String);
        if(data->current_id == NULL){
            return SEM_ERROR_UNDEF_VAR;
        }
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_LEFT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(fc_args);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_RIGHT_BRACKET);
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

//24. 〈 fc_args 〉−→ id: expression 〈fc_ n_args 〉
static int fc_args(analyse_data_t* data){
    data->args_index = 0;
    if(data->token.type == IDENTIFIER){
        data->var_id = symtable_search(&data->local_table, data->token.data.String);
        if(data->var_id != data->current_id.param_names[data->args_index]){
            return SEM_ERROR_UNDEF_VAR;
        }
        GET_TOKEN_AND_CHECK_TYPE(COLON);
        GET_TOKEN_AND_CHECK_RULE(expression);
        GET_TOKEN_AND_CHECK_RULE(fc_n_args);
        return SYNTAX_OK;
    }
    //25. 〈fc_args 〉−→ ε
    return SYNTAX_OK;
}

//26. 〈fc_n_args 〉−→ , id: expression 〈fc_ n_args 〉
static int fc_args(analyse_data_t* data){
    if(data->token.type == COMMA){
        data->args_index++;
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        data->var_id = symtable_search(&data->local_table, data->token.data.String);
        if(data->var_id != data->current_id.param_names[data->args_index]){
            return SEM_ERROR_UNDEF_VAR;
        }
        GET_TOKEN_AND_CHECK_TYPE(COLON);
        GET_TOKEN_AND_CHECK_RULE(expression);
        GET_TOKEN_AND_CHECK_RULE(fc_n_args);
        return SYNTAX_OK;
    }
    //25. 〈fc_args 〉−→ ε
    return SYNTAX_OK;
}

static int modifier(analyse_data_t* data){
    //28. 〈 modifier 〉−→ let
    if(data->token.type == KEYWORD && data->token.data.Keyword == Let_KW){
        GET_TOKEN();
        return SYNTAX_OK;
    }
    //29. 〈 modifier 〉−→ var
    else if(data->token.type == KEYWORD && data->token.data.Keyword == Var_KW){
        GET_TOKEN();
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

static int def_type(analyse_data_t* data){
    //30. 〈 def_type 〉−→ :〈type 〉
    if(data->token.type == COLON){
        GET_TOKEN_AND_CHECK_RULE(type);
        return SYNTAX_OK;
    }
    //31. 〈def_type 〉−→ ε

    return SYNTAX_ERROR;
}

//32. 〈 end 〉 −→ EOF
static int end(analyse_data_t* data){
    if(data->token.type == TOKEN_EOF){
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

//33. 〈 possible_EOL 〉 −→ EOL
static int possible_EOL(analyse_data_t* data){
    if(data->token.type == TOKEN_EOL){
        GET_TOKEN();
        return SYNTAX_OK;
    }
    //34. 〈 possible_EOL 〉 −→ ε
    return SYNTAX_ERROR;
}

//35. 〈 id 〉 −→ identifier
static int id(analyse_data_t* data){
    if(data->token.type == IDENTIFIER){
        GET_TOKEN();
        return SYNTAX_OK;
    }
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
            data->current_id->data->param_types[data->args_index].q_type = true;
        }
        else{
            data->var_id->data->q_type = true;
        }
        return SYNTAX_OK;
    }
    //38. 〈 ?_value 〉 −→ ε
    else{
        f (data->in_defintion)
        {
            data->current_id->data->param_types[data->args_index].q_type = false;
        }
        else{
            data->var_id->data->q_type = false;
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
                data->current_id->data->param_types[data->args_index].data_type = 'd';
			}
            else{
                data->var_id->data->data_type += 'd';
            }
			break;
            return SYNTAX_OK;
        }
        //40. 〈 p_type 〉 −→ Int
        case Int_KW:
            if (data->in_defintion)
            {
                data->current_id->data->param_types[data->args_index].data_type = 'i';
            }
            else{
                data->var_id->data->data_type += 'i';
            }
            
        }
        //41. 〈 p_type 〉 −→ String
        case String_KW:
            if (data->in_defintion)
            {
                data->current_id->data->param_types[data->args_index].data_type = 's';
            }
            else{
                data->var_id->data->data_type += 's';
            }
        }
    }
    return SYNTAX_ERROR;
}
int analyse()
{
    analyse_data_t data;
    int result = SYNTAX_OK;

    if (!init_variables(&data))
        return ERROR_INTERNAL;

    GET_TOKEN();
    CHECK_RULE(program);

    free_variables(&data);

    return result;
}