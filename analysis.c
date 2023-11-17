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

// data is defined in function ig
#define CHECK_TYPE(_type)											\
	if (data->token.type != (_type)) return SYNTAX_ERR

#define CHECK_RULE(rule)											\
	if ((result = rule(data))) return result

#define CHECK_KEYWORD(_keyword)										\
	if (															\
		data->token.type != KEYWORD						\
		|| data->token.attribute.keyword != (_keyword)				\
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
    if(data->token.type == KEYWORD && data->token.attribute.keyword == Function_KW)
    {
        data->label_deep++;
        GET_TOKEN_AND_CHECK_RULE(function);
        CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    // 〈 statement 〉 −→ 〈 if_else 〉 EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && data->token.attribute.keyword == If_KW){
        data->label_deep++;
        CHECK_RULE(if_else);
        CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    // 〈 statement 〉 −→ 〈 while 〉EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && data->token.attribute.keyword == While_KW){
        data->label_deep++;
        CHECK_RULE(while_);
        CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    //5. 〈 statement 〉 −→ 〈 assignment 〉EOL 〈 statement 〉
    else if(data->token.type == ASSIGNMENT){
        GET_TOKEN_AND_CHECK_RULE(assignment);
        CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    // TODO Should ask pasha as there is no def_var type for trhis rule
    //6. 〈 statement 〉 −→ 〈def_var 〉 EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && data->token.attribute.keyword == Let_KW){
        GET_TOKEN_AND_CHECK_RULE(def_var);
        CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    //7. 〈 statement 〉 −→ 〈 f_call 〉EOL 〈 statement 〉
    else if(data->token.type == IDENTIFIER){
        //TODO somwehow find out if it is a function call( should be inside f_call ig)
        data->lhs_id = symtable_search(&data->global_table, data->token.attribute.string); 
        GET_TOKEN_AND_CHECK_RULE(f_call);
        CHECK_TYPE(TOKEN_EOL);
        return statement(data);
    }
    //8. 〈 statement 〉 −→ EOL 〈 statement 〉
    else if(data->token.type == TOKEN_EOL){
        GET_TOKEN();
        return statement(data);
    }
    //9. 〈 statement 〉 −→ 〈 end 〉
    else if(data->token.type == KEYWORD && data->token.attribute.keyword == End_KW){
        GET_TOKEN_AND_CHECK_RULE(end);
        return SYNTAX_OK; // TODO ig idk maybe inside end
    }
    //10. 〈 statement 〉 −→ ε
    return SYNTAX_OK;
}

static int function(analyse_data_t* data){
    //11. 〈 function 〉 −→ func 〈 ID 〉( 〈 args 〉) 〈 func_ret 〉{ EOL 〈 statement 〉 }
    if(data->token.type == KEYWORD && data->token.attribute.keyword == Function_KW){
        GET_TOKEN_AND_CHECK_RULE(id);
        CHECK_TYPE(TOKEN_LEFT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(args);
        CHECK_TYPE(TOKEN_RIGHT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(func_ret);
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        data->label_deep++; // TODO ig here but should test
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;

}
static int func_ret(analyse_data_t* data){
    //12. 〈 func_ret 〉 −→ 〈 FUNCTION_TYPE 〉〈 type 〉
    if(data->token.type == TOKEN_FUNCTION_TYPE){
        GET_TOKEN_AND_CHECK_RULE(type);
        return SYNTAX_OK;
    }
    //13. 〈 func_ret 〉 −→ ε
    return SYNTAX_OK;

}

//TODO need clarification on this rule bc do not understand why double id
static int args(analyse_data_t* data){
    //14. 〈 args 〉 −→ id id : 〈type 〉〈args_n 〉
    if(data->token.type == IDENTIFIER){
        GET_TOKEN_AND_CHECK_RULE(id);
        GET_TOKEN_AND_CHECK_RULE(id);
        CHECK_TYPE(COLON);
        GET_TOKEN_AND_CHECK_RULE(type);
        GET_TOKEN_AND_CHECK_RULE(args_n);
        return SYNTAX_OK;
    }
}

//TODO no rule for expression
static int if_else(analyse_data_t* data){
    //18. 〈 if_else 〉 −→ if 〈 expression 〉{ 〈 statement 〉} 〈 possible_EOL 〉else { 〈 statement 〉}
    if(data->token.type == KEYWORD && data->token.attribute.keyword == If_KW){
        GET_TOKEN_AND_CHECK_RULE(expression);
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(possible_EOL);
        CHECK_TYPE(KEYWORD && data->token.attribute.keyword == Else_KW);
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

static int while_(analyse_data_t* data){
    //19. 〈 while 〉 −→ while 〈 expression 〉{ 〈 statement 〉}
    if(data->token.type == KEYWORD && data->token.attribute.keyword == While_KW){
        CHECK_RULE(expression);
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

//TODO no rule for expression 