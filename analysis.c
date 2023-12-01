/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file analysis.c

 * @brief Implementation of the analysis.c file part of parser

 * @author Nikita Koliada xkolia00
 * @author Maksym Podhornyi xpodho08
*/
#include "error.h"
#include "symtable.h"
#include "stdbool.h"
#include "analysis.h"
#include "scanner.h"
#include "expression.h"
// include code generator

//Defining macros for easier work with the parser 
//and for better readability of the code
int result = SYNTAX_ERROR;


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
               case StringNullable_KW:
                   printf("String_Nullable_KW\n");
                   break;
               case IntNullable_KW:
                   printf("Int_Nullable_KW\n");
                   break;
               case DoubleNullable_KW:
                   printf("Double_Nullable_KW\n");
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

#define GET_TOKEN()													\
	if ((result = get_next_token(&data->token)) != 0) {        \
            return result;                                         \
        }                                                          \
                        print_token(&data->token);                         \

#define CHECK_TYPE(t_type)											\
	if (data->token.type != t_type) return SYNTAX_ERROR

#define CHECK_RULE(rule)											\
	if ((result = rule(data)) != 0) return result

#define GET_TOKEN_AND_CHECK_TYPE(t_type)								\
	do {															\
		GET_TOKEN();												\
		CHECK_TYPE(t_type);											\
	} while(0)

#define GET_TOKEN_AND_CHECK_RULE(rule)								\
	do {															\
		GET_TOKEN();												\
		CHECK_RULE(rule);											\
	} while(0)


#define CHECK_EXPRESSION()								\
	do {															\
        bool EOL = false;												\
		if ((result = expression(data, &EOL)) != 0) return result;											\
	} while(0)


#define GET_TOKEN_AND_CHECK_EXPRESSION()								\
	do {															\
		GET_TOKEN();                                                  \
        bool EOL = false;												\
		if ((result = expression(data, &EOL)) != 0) return result;											\
	} while(0)

// search for the variable starting with current deepness up until the global table
bst_node_ptr var_search(analyse_data_t* data, int deepness, char* key){
    for (int i = deepness; i >= 0; i--){
        bst_node_ptr ptr = symtable_search(&data->local_table[i], key);
        if(ptr != NULL){
            return ptr;
        }
    }
    return symtable_search(&data->global_table, key);
}

static bool init_variables(analyse_data_t* data)
{
    data->st_length = 10;
	symtable_init(&data->global_table);
    for (int length = 0; length < data->st_length; length++){
        symtable_init(&data->local_table[length]);
    }
	data->current_id = malloc(sizeof(bst_node_ptr))	;
    data->var_id = malloc(sizeof(bst_node_ptr));
    data->expr_id = malloc(sizeof(bst_node_ptr));

	data->args_index = 0;
	data->label_index = 0;
	data->label_deep = 0;


	data->in_defintion = false;
	data->in_while_or_if = false;
    data->in_var_definition = false;

    var_data_t * var_data = malloc(sizeof(var_data_t));
    var_data->constant = false;
    var_data->data_type = Undefined;
    var_data->q_type = false;

    symtable_insert_var(&data->global_table, "%%exp_result", var_data);

	return true;
}



//Frees symbol tables
static void free_variables(analyse_data_t* data)
{
	symtable_dispose(&data->global_table);
    for (int length = 0; length < 10; length++){
        symtable_dispose(&data->local_table[length]);
    }
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
static int fc_args_n_args(analyse_data_t* data);
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
        CHECK_RULE(function);
        GET_TOKEN_AND_CHECK_RULE(possible_EOL);
        return statement(data);
    }
    // 〈 statement 〉 −→ 〈 if_else 〉 EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && data->token.data.Keyword == If_KW){
        CHECK_RULE(if_else);
        GET_TOKEN_AND_CHECK_RULE(possible_EOL);
        return statement(data);
    }
    // 〈 statement 〉 −→ 〈 while 〉EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && data->token.data.Keyword == While_KW){
        CHECK_RULE(while_);
        GET_TOKEN_AND_CHECK_RULE(possible_EOL);
        return statement(data);
    }
    else if(data->token.type == IDENTIFIER){
        //5. 〈 statement 〉 −→ 〈 assignment 〉EOL 〈 statement 〉
        data->var_id = var_search(data, data->label_deep, data->token.data.String);
        data->current_id = symtable_search(&data->global_table, data->token.data.String);
        data->tmp_key = data->token.data.String;
        GET_TOKEN();
        if(data->token.type == ASSIGNMENT){
            CHECK_RULE(assignment);
            CHECK_RULE(possible_EOL);
            data->tmp_key = "";
            return statement(data);
        }
        //7. 〈 statement 〉 −→ 〈 f_call 〉EOL 〈 statement 〉
        else{
            CHECK_RULE(f_call);
            GET_TOKEN_AND_CHECK_RULE(possible_EOL);
            data->tmp_key = "";
            return statement(data);
        }
    }
    //6. 〈 statement 〉 −→ 〈def_var 〉 EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && (data->token.data.Keyword == Let_KW || data->token.data.Keyword == Var_KW)){
        CHECK_RULE(def_var);
        //printf("%s", data->var_id->key);
        CHECK_RULE(possible_EOL);
        return statement(data);
    }
    //???. 〈 statement 〉 −→ <return_kw> <expression> EOL <statement>
    else if(data->token.type == KEYWORD && data->token.data.Keyword == Return_KW){
        data->var_id = symtable_search(&data->global_table, "%%exp_result");
        GET_TOKEN_AND_CHECK_EXPRESSION();
        CHECK_RULE(possible_EOL);
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
        data->in_defintion = true;
		data->label_index = 0;
        data->label_deep++;
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        // if function wasn't declared add it to the global symbol table
        data->current_id = symtable_search(&data->global_table, data->token.data.String);
        function_data_t* func_data = NULL;
		if (data->current_id == NULL)
		{   	
            func_data = malloc(sizeof(function_data_t));
            symtable_insert_function(&data->global_table, data->token.data.String, func_data);
            data->current_id = symtable_search(&data->global_table,  data->token.data.String);
        }
		else
		{
            func_data = (function_data_t*)(*data->current_id).data;
			if (func_data->defined)
				return SEM_ERROR_UNDEF_VAR;
		}		
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_LEFT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(args);
        CHECK_TYPE(TOKEN_RIGHT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(func_ret);
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        // so we can difine the variable with the same name as arguments 
        data->label_deep++;
        symtable_init(&data->local_table[data->label_deep]);
        GET_TOKEN_AND_CHECK_RULE(statement);
        data->label_deep--;
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        symtable_dispose(&data->local_table[data->label_deep]);
        func_data->defined = true;
        data->in_defintion = false;
        data->label_deep--;
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

static int func_ret(analyse_data_t* data){
    //12. 〈 func_ret 〉 −→ 〈 FUNCTION_TYPE 〉〈 type 〉

    if(data->token.type == TOKEN_FUNCTION_TYPE){
        GET_TOKEN_AND_CHECK_RULE(type);
		((function_data_t*)(*data->current_id).data)->return_data_type = ((function_data_t*)(*data->current_id).data)->params_types[data->args_index + 1].data_type;
    }
    //13. 〈 func_ret 〉 −→ ε
    else{
        ((function_data_t*)(*data->current_id).data)->return_data_type = Undefined;
    }
    return SYNTAX_OK;

}

static int args(analyse_data_t* data){
	
    //14. 〈 args 〉 −→ id id : 〈type 〉〈args_n 〉
    if(data->token.type == IDENTIFIER){
        data->args_index = 0;
        ((function_data_t*)(*data->current_id).data)->param_names[data->args_index] = data->token.data.String;
        // if there is function named as parameter
		bst_node_ptr name_var = symtable_search(&data->global_table, data->token.data.String);
        if(name_var != NULL)
			return SEM_ERROR_UNDEF_VAR;

        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        var_data_t* var_data = malloc(sizeof(var_data_t));
        bst_node_ptr ident_var = symtable_search(&data->local_table[data->label_deep], data->token.data.String);
        if(ident_var != NULL)
			return SEM_ERROR_UNDEF_VAR;
        symtable_insert_var(&data->local_table[data->label_deep], data->token.data.String, var_data);
        ((function_data_t*)(*data->current_id).data)->params_identifiers[data->args_index] = data->token.data.String;


        GET_TOKEN_AND_CHECK_TYPE(COLON);
        GET_TOKEN_AND_CHECK_RULE(type);
        var_data->q_type = ((function_data_t*)(*data->current_id).data)->params_types[data->args_index].q_type;
        var_data->data_type = ((function_data_t*)(*data->current_id).data)->params_types[data->args_index].data_type;
        var_data->constant = true;
        CHECK_RULE(args_n);
        ((function_data_t*)(*data->current_id).data)->param_len = data->args_index;
        return SYNTAX_OK;
    }    
    //15. 〈 args 〉 −→ ε
    else{
        ((function_data_t*)(*data->current_id).data)->param_len = -1;
        return SYNTAX_OK;
    }
    
}
static int args_n(analyse_data_t* data){
    //17. 〈 args_n 〉 −→ , id id : 〈type 〉〈args_n 〉
    if(data->token.type == COMMA){
        data->args_index++;
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        ((function_data_t*)(*data->current_id).data)->param_names[data->args_index] = data->token.data.String;
        // if there is function named as parameter
		bst_node_ptr name_var = symtable_search(&data->global_table, data->token.data.String);
        if(name_var != NULL)
			return SEM_ERROR_UNDEF_VAR;

        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        bst_node_ptr ident_var = symtable_search(&data->local_table[data->label_deep], data->token.data.String);
        if(ident_var != NULL)
			return SEM_ERROR_UNDEF_VAR;
        var_data_t* var_data = malloc(sizeof(var_data_t));
        symtable_insert_var(&data->local_table[data->label_deep], data->token.data.String, var_data);
        ((function_data_t*)(*data->current_id).data)->params_identifiers[data->args_index] = data->token.data.String;


        GET_TOKEN_AND_CHECK_TYPE(COLON);
        GET_TOKEN_AND_CHECK_RULE(type);
        var_data->q_type = ((function_data_t*)(*data->current_id).data)->params_types[data->args_index].q_type;
        var_data->data_type = ((function_data_t*)(*data->current_id).data)->params_types[data->args_index].data_type;
        var_data->constant = true;
        CHECK_RULE(args_n);	
    }    
//16. 〈 args_n 〉 −→ ε
    return SYNTAX_OK;
}

// //TODO no rule for expression
static int if_else(analyse_data_t* data){
//     //18. 〈 if_else 〉 −→ if 〈 expression 〉{ 〈 statement 〉} 〈 possible_EOL 〉else { 〈 statement 〉}
    if(data->token.type == KEYWORD && data->token.data.Keyword == If_KW){
        data->label_deep++;
        data->in_while_or_if = true;
        data->label_index += 2;
        GET_TOKEN();
        if(data->token.type == KEYWORD && data->token.data.Keyword == Let_KW){
            GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
            bst_node_ptr let_id = var_search(data, data->label_deep, data->token.data.String);
            if(let_id == NULL){
                return SEM_ERROR_UNDEF_VAR;
            }
            else if(((var_data_t*)(*let_id).data)->constant == false){
                return SEM_ERROR_UNDEF_VAR;
            }
        }
        else{
            data->var_id = symtable_search(&data->global_table, "%%exp_result");
            CHECK_EXPRESSION();
        }
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(possible_EOL);
        if(data->token.data.Keyword != Else_KW){
            return SYNTAX_ERROR;
        }
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        symtable_dispose(&data->local_table[data->label_deep]);
        data->label_deep--;
		data->in_while_or_if = false;
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

static int while_(analyse_data_t* data){
//     //19. 〈 while 〉 −→ while 〈 expression 〉{ 〈 statement 〉}
    if(data->token.type == KEYWORD && data->token.data.Keyword == While_KW){
        data->label_deep++;
        data->in_while_or_if = true;
        data->label_index += 2;
        data->var_id = symtable_search(&data->global_table, "%%exp_result");
        GET_TOKEN_AND_CHECK_EXPRESSION();
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        symtable_dispose(&data->local_table[data->label_deep]);
        data->label_deep--;
        data->in_while_or_if = false;
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

static int assignment(analyse_data_t* data){
//     //20. 〈 assignment 〉 −→ 〈 id 〉 = 〈 expression 〉
    if(data->token.type == ASSIGNMENT){
        if(data->var_id == NULL){
            return SEM_ERROR_UNDEF_VAR;
        }
        else if(((var_data_t*)(*data->var_id).data)->constant){
            return SEM_ERROR_UNDEF_VAR;
        }
        GET_TOKEN_AND_CHECK_TYPE(ASSIGNMENT);
        GET_TOKEN_AND_CHECK_EXPRESSION();
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;

}

static int def_var(analyse_data_t* data){
    //21. 〈 def_var 〉 −→ 〈 modifier 〉〈 id 〉〈 def_type 〉 = 〈 expression 〉
    if(data->token.type == KEYWORD)
    {
        var_data_t* var_data = NULL;
        bool constanta = false;
        if(data->token.data.Keyword == Let_KW){
            constanta = true;
        }
        else if(data->token.data.Keyword == Var_KW){
            constanta = false;
        }
        else{
            return SYNTAX_ERROR;
        }
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);

        data->var_id = symtable_search(&data->local_table[data->label_deep], data->token.data.String); 
		if (data->var_id == NULL)
		{   	
            var_data = malloc(sizeof(var_data_t));
            symtable_insert_var(&data->local_table[data->label_deep], data->token.data.String, var_data);
            data->var_id = symtable_search(&data->local_table[data->label_deep],  data->token.data.String);
        }else {
            return SEM_ERROR_UNDEF_VAR;
        }
        
        bool no_type = true;
        bool no_assignment = true;
        GET_TOKEN();
        //22. 〈 def_var 〉−→ 〈 modifier 〉id :〈type 〉
        if(data->token.type == COLON){
            no_type = false;
            CHECK_RULE(def_type);
            var_data->q_type = ((var_data_t*)(*data->var_id).data)->q_type;
            var_data->data_type = ((var_data_t*)(*data->var_id).data)->data_type;
        }
        else{
            var_data->q_type = Undefined;
            var_data->data_type = Undefined;
        }
        // = 〈 expression 〉
        if(data->token.type == ASSIGNMENT){
            no_assignment = false;
            data->in_var_definition = true;
            GET_TOKEN_AND_CHECK_EXPRESSION();
            data->in_var_definition = false;
        }
        else{
            GET_TOKEN(); // bc GET_TOKEN_AND_CHECK_EXPRESSION also gets token after the expression
        }
        if(no_type && no_assignment){
            return SYNTAX_ERROR;
        }
        
        var_data->constant = constanta;
        return SYNTAX_OK;
    }

    return SYNTAX_ERROR;
}

// //23. 〈f_call 〉−→ id ( 〈fc_args 〉)
static int f_call(analyse_data_t* data){
    data->args_index = -1;
    if(data->current_id == NULL){
        function_data_t* func_data = malloc(sizeof(function_data_t));
        func_data->defined = false;
        symtable_insert_function(&data->global_table, data->tmp_key, func_data);
        data->current_id = symtable_search(&data->global_table, data->tmp_key);
        ((function_data_t*)data->current_id->data)->param_names[data->args_index] = "nenull";
        ((function_data_t*)data->current_id->data)->param_len = 100; // max of params
    }
    CHECK_TYPE(TOKEN_LEFT_BRACKET);
    if(((function_data_t*)data->current_id->data)->param_len != -1)
        GET_TOKEN_AND_CHECK_RULE(fc_args);
    else{
        GET_TOKEN();
    }
    if(data->args_index != ((function_data_t*)data->current_id->data)->param_len && ((function_data_t*)data->current_id->data)->defined == true){
        return SEM_ERROR_PARAM;
    }
    if(((function_data_t*)data->current_id->data)->defined == false)
        ((function_data_t*)data->current_id->data)->param_len = data->args_index;
    data->args_index = 0;
    CHECK_TYPE(TOKEN_RIGHT_BRACKET);
    return SYNTAX_OK;
}

// //23.2. 〈f_call 〉−→ id ( 〈fc_args 〉)
int f_expression_call(analyse_data_t* data, token_t id, data_type* type){
    data->current_id = symtable_search(&data->global_table, id.data.String);
    data->tmp_key = id.data.String;
    *type = ((function_data_t*)data->current_id->data)->return_data_type;
    return f_call(data);    
}

// //24. 〈 fc_args 〉−→ id: expression 〈fc_ n_args 〉
static int fc_args(analyse_data_t* data){
    data->args_index = 0;
    //if _ as name var
    // a kak delat s undefined functions
    if(!((function_data_t*)(*data->current_id).data)->defined && data->token.type == IDENTIFIER)
    {
        data->var_id = symtable_search(&data->global_table, "%%exp_result");
        CHECK_EXPRESSION();
        CHECK_RULE(fc_args_n_args);
        return SYNTAX_OK;
    }
    else if (strcmp(((function_data_t*)(*data->current_id).data)->param_names[data->args_index], "_") == 0){
        data->var_id = symtable_search(&data->global_table, "%%exp_result");
        CHECK_EXPRESSION();
        CHECK_RULE(fc_args_n_args);
        return SYNTAX_OK;
    }
    else{
        // functions' params are in local[0] table
        if(((function_data_t*)data->current_id->data)->defined){
            if(strcmp(data->token.data.String, ((function_data_t*)(*data->current_id).data)->param_names[data->args_index])){
                return SEM_ERROR_PARAM;
            }
        }
        GET_TOKEN_AND_CHECK_TYPE(COLON);
        data->var_id = symtable_search(&data->global_table, "%%exp_result");
        GET_TOKEN_AND_CHECK_EXPRESSION();
        CHECK_RULE(fc_args_n_args);
        return SYNTAX_OK;
    }
    
    //25. 〈fc_args 〉−→ ε
    return SYNTAX_OK;
}

// //26. 〈fc_n_args 〉−→ , id: expression 〈fc_ n_args 〉
static int fc_args_n_args(analyse_data_t* data){
    if(data->token.type == COMMA){
        data->args_index++;
        //if _ as name var
        if(!((function_data_t*)(*data->current_id).data)->defined && data->token.type == IDENTIFIER)
        {
            data->var_id = symtable_search(&data->global_table, "%%exp_result");
            CHECK_EXPRESSION();
            CHECK_RULE(fc_args_n_args);
            return SYNTAX_OK;
        }
        else if (strcmp(((function_data_t*)(*data->current_id).data)->param_names[data->args_index], "_") == 0){
            data->var_id = symtable_search(&data->global_table, "%%exp_result");
            GET_TOKEN_AND_CHECK_EXPRESSION();
            CHECK_RULE(fc_args_n_args);
            return SYNTAX_OK;
        }
        else{
            GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
            if(((function_data_t*)data->current_id->data)->defined){
                if(strcmp(data->token.data.String, ((function_data_t*)(*data->current_id).data)->param_names[data->args_index])){
                    return SEM_ERROR_PARAM;
                }
            }
            GET_TOKEN_AND_CHECK_TYPE(COLON);
            data->var_id = symtable_search(&data->global_table, "%%exp_result");
            GET_TOKEN_AND_CHECK_EXPRESSION();
            CHECK_RULE(fc_args_n_args);
            return SYNTAX_OK;
        }
    }

//     //25. 〈fc_args 〉−→ ε
    return SYNTAX_OK;
}

static int modifier(analyse_data_t* data){
    //28. 〈 modifier 〉−→ let
    if(data->token.type == KEYWORD && data->token.data.Keyword == Let_KW){
        var_data_t* var_data = (var_data_t*)(*data->var_id).data;
        var_data->constant = true;
        return SYNTAX_OK;
    }
    //29. 〈 modifier 〉−→ var
    else if(data->token.type == KEYWORD && data->token.data.Keyword == Var_KW){
        var_data_t* var_data = (var_data_t*)(*data->var_id).data;
        var_data->constant = false;
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

    return SYNTAX_OK;
}

// //32. 〈 end 〉 −→ EOF
static int end(analyse_data_t* data){
    if(data->token.type == TOKEN_EOF){
        // check if all functions are defined
		// for (int i = 0; i < data->global_table->length; i++)
		// 	for (function_data_t* it = data->global_table[i]; it != NULL; it = it->next)
		// 		if (!it->data.defined) return SEM_ERROR_UNDEF_FUNC;
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

// //33. 〈 possible_EOL 〉 −→ EOL
static int possible_EOL(analyse_data_t* data){
    if(data->token.type == TOKEN_EOL){
        GET_TOKEN();
        return SYNTAX_OK;
    }
//     //34. 〈 possible_EOL 〉 −→ ε
    return SYNTAX_OK;
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
    //36. 〈 type 〉 −→ 〈 p_type 〉
    if(data->token.type == KEYWORD && (data->token.data.Keyword == Int_KW || data->token.data.Keyword == Double_KW || data->token.data.Keyword == String_KW)){
        CHECK_RULE(p_type);
        GET_TOKEN();
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}


static int p_type(analyse_data_t* data){
    //36. 〈 p_type 〉 −→ 〈 p_type 〉
    if(data->token.type == KEYWORD){
        switch (data->token.data.Keyword)
		{
        //39. 〈 p_type 〉 −→ Double
        case Double_KW:
            if (data->in_defintion)
			{
                function_data_t* func_data = (function_data_t*)(*data->current_id).data;
                func_data->params_types[data->args_index].data_type = Double_Type;
                func_data->params_types[data->args_index].q_type = false;
			}
            else{
                var_data_t* var_data = (var_data_t*)(*data->var_id).data;
                var_data->data_type = Double_Type;
                var_data->q_type = false;
            }
            return SYNTAX_OK;
        break;
        //40. 〈 p_type 〉 −→ Int
        case Int_KW:
            if (data->in_defintion)
			{
                function_data_t* func_data = (function_data_t*)(*data->current_id).data;
                func_data->params_types[data->args_index].data_type = Int_Type;
                func_data->params_types[data->args_index].q_type = false;

			}
            else{
                var_data_t* var_data = (var_data_t*)(*data->var_id).data;
                var_data->data_type = Int_Type;
                var_data->q_type = false;
            }
            return SYNTAX_OK;
        break;
        
        //41. 〈 p_type 〉 −→ String
        case String_KW:
            if (data->in_defintion)
			{
                function_data_t* func_data = (function_data_t*)(*data->current_id).data;
                func_data->params_types[data->args_index].data_type = String_Type;
                func_data->params_types[data->args_index].q_type = false;
			}
            else{
                var_data_t* var_data = (var_data_t*)(*data->var_id).data;
                var_data->data_type = String_Type;
                var_data->q_type = false;
            }
            
            return SYNTAX_OK;
        break;
        //42. 〈 p_type 〉 −→ StringNullable
        case StringNullable_KW:
            if (data->in_defintion)
			{
                function_data_t* func_data = (function_data_t*)(*data->current_id).data;
                func_data->params_types[data->args_index].data_type = String_Type;
                func_data->params_types[data->args_index].q_type = true;

			}
            else{
                var_data_t* var_data = (var_data_t*)(*data->var_id).data;
                var_data->data_type = String_Type;
                var_data->q_type = true;
            }
            return SYNTAX_OK;
        break;
        //42. 〈 p_type 〉 −→ DoubleNullable_KW
        case DoubleNullable_KW:
            if (data->in_defintion)
			{
                function_data_t* func_data = (function_data_t*)(*data->current_id).data;
                func_data->params_types[data->args_index].data_type = Double_Type;
                func_data->params_types[data->args_index].q_type = true;

			}
            else{
                var_data_t* var_data = (var_data_t*)(*data->var_id).data;
                var_data->data_type = Double_Type;
                var_data->q_type = true;
            }
            return SYNTAX_OK;
        break;
        //43. 〈 p_type 〉 −→ IntNullable_KW
        case IntNullable_KW:
            if (data->in_defintion)
			{
                function_data_t* func_data = (function_data_t*)(*data->current_id).data;
                func_data->params_types[data->args_index].data_type = Int_Type;
                func_data->params_types[data->args_index].q_type = true;

			}
            else{
                var_data_t* var_data = (var_data_t*)(*data->var_id).data;
                var_data->data_type = Int_Type;
                var_data->q_type = true;
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
    char *input = "func empty(){\n}\nfunc concat(b x : String, with y : String) -> String {\nlet x = x + y\nreturn x + \" \" + y\n}\nlet a = \"ahoj \"\nvar ct : String\nconcat(b: a, with: \"svete\")\nempty()trachnutebe()\n";

    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);
    analyse_data_t *data = malloc(sizeof(analyse_data_t));
    
    if (!init_variables(data))
        return INTERNAL_ERROR;

    result = get_next_token(&data->token);

    if (data->token.type == TOKEN_EOF)
        return -1;

    result = program(data);

    if(result != SYNTAX_OK)
        printf("ERROR: %d\n", result);
        // fprintf(stderr, "ERROR: %d\n", result);
    else{
        printf("OK\n");
    }
    //free_variables(data);
    fclose(file);

    return result;
}

