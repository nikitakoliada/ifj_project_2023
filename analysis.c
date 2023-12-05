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
#include "generator.h"

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
        print_token(&data->token);                                 \

#define CHECK_TYPE(t_type)											\
    if(data->token.type == TOKEN_EOF && t_type == TOKEN_EOL) return SYNTAX_OK ;\
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
    symtable_init(&data->local_table[0]);
	data->current_id = malloc(sizeof(bst_node_ptr))	;
    data->var_id = malloc(sizeof(bst_node_ptr));
    data->expr_id = malloc(sizeof(bst_node_ptr));

	data->args_index = 0;
	data->label_index = 0;
	data->label_deep = 0;


	data->in_defintion = false;
	data->in_while_or_if = false;
    data->in_var_definition = false;
    data->in_call_func = false;

    var_data_t * var_data = malloc(sizeof(var_data_t));
    var_data->constant = false;
    var_data->data_type = Undefined;
    var_data->q_type = false;

    symtable_insert_var(&data->global_table, "%%exp_result", var_data);
    function_data_t * readString = malloc(sizeof(function_data_t));
    readString->defined = true;
    readString->param_len = -1;
    readString->return_data_type = String_Type;
    readString->return_data_q_type = true;
    symtable_insert_function(&data->global_table, "readString", readString);
    function_data_t * readInt = malloc(sizeof(function_data_t));
    readInt->defined = true;
    readInt->param_len = -1;
    readInt->return_data_type = Int_Type;
    readInt->return_data_q_type = true;
    symtable_insert_function(&data->global_table, "readInt", readInt);
    function_data_t * readDouble = malloc(sizeof(function_data_t));
    readDouble->defined = true;
    readDouble->param_len = -1; // -1 == 0 in our compiler
    readDouble->return_data_type = Double_Type;
    readDouble->return_data_q_type = true;
    symtable_insert_function(&data->global_table, "readDouble", readDouble);

    function_data_t * Int2Double = malloc(sizeof(function_data_t));
    Int2Double->defined = true;
    Int2Double->param_len = 0; // 0 == 1 in our compiler
    Int2Double->params_types[0].data_type = Int_Type;
    Int2Double->params_types[0].q_type = false;
    Int2Double->params_types[0].constant = true;
    Int2Double->param_names[0] = "_";
    Int2Double->params_identifiers[0] = "term";
    Int2Double->return_data_type = Double_Type;
    Int2Double->return_data_q_type = false;
    symtable_insert_function(&data->global_table, "Int2Double", Int2Double);
    function_data_t * Double2Int = malloc(sizeof(function_data_t));
    Double2Int->defined = true;
    Double2Int->param_len = 0; // 0 == 1 in our compiler
    Double2Int->params_types[0].data_type = Double_Type;
    Double2Int->params_types[0].q_type = false;
    Double2Int->params_types[0].constant = true;
    Double2Int->param_names[0] = "_";
    Double2Int->params_identifiers[0] = "term";
    Double2Int->return_data_type = Int_Type;
    Double2Int->return_data_q_type = false;
    symtable_insert_function(&data->global_table, "Double2Int", Double2Int);

    function_data_t * length = malloc(sizeof(function_data_t));
    length->defined = true;
    length->param_len = 0; // 0 == 1 in our compiler
    length->params_types[0].data_type = String_Type;
    length->params_types[0].q_type = false;
    length->params_types[0].constant = true;
    length->param_names[0] = "_";
    length->params_identifiers[0] = "s";
    length->return_data_type = Int_Type;
    length->return_data_q_type = false;
    symtable_insert_function(&data->global_table, "length", length);
    function_data_t * ord = malloc(sizeof(function_data_t));
    ord->defined = true;
    ord->param_len = 0; // 0 == 1 in our compiler
    ord->params_types[0].data_type = String_Type;
    ord->params_types[0].q_type = false;
    ord->params_types[0].constant = true;
    ord->param_names[0] = "_";
    ord->params_identifiers[0] = "c";
    ord->return_data_type = Int_Type;
    ord->return_data_q_type = false;
    symtable_insert_function(&data->global_table, "ord", ord);
    function_data_t * chr = malloc(sizeof(function_data_t));
    chr->defined = true;
    chr->param_len = 0; // 0 == 1 in our compiler
    chr->params_types[0].data_type = Int_Type;
    chr->params_types[0].q_type = false;
    chr->params_types[0].constant = true;
    chr->param_names[0] = "_";
    chr->params_identifiers[0] = "i";
    chr->return_data_type = String_Type;
    chr->return_data_q_type = false;
    symtable_insert_function(&data->global_table, "chr", chr);

    function_data_t * substring = malloc(sizeof(function_data_t));
    substring->defined = true;
    substring->param_len = 2;// 2 == 3 in our compiler 
    substring->params_types[0].data_type = String_Type;
    substring->params_types[0].q_type = false;
    substring->params_types[0].constant = true;
    substring->param_names[0] = "of";
    substring->params_identifiers[0] = "s";
    substring->params_types[1].data_type = Int_Type;
    substring->params_types[1].q_type = false;
    substring->params_types[1].constant = true;
    substring->param_names[1] = "startingAt";
    substring->params_identifiers[1] = "i";
    substring->params_types[2].data_type = Int_Type;
    substring->params_types[2].q_type = false;
    substring->params_types[2].constant = true;
    substring->param_names[2] = "endingBefore";
    substring->params_identifiers[2] = "j";
    substring->return_data_type = String_Type;
    substring->return_data_q_type = true;
    symtable_insert_function(&data->global_table, "substring", substring);

	return true;
}



//Frees symbol tables
static void free_variables(analyse_data_t* data)
{
	symtable_dispose(&data->global_table);
    symtable_dispose(&data->local_table[0]);
}

// forward declarations(LL table)
static int program(analyse_data_t* data);
static int statement(analyse_data_t* data); 
static int function(analyse_data_t* data);
static int function_declaration(analyse_data_t* data);
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
static int type(analyse_data_t* data);
static int p_type(analyse_data_t* data);// ? before p_type 
static int write(analyse_data_t* data);

// going thru file for the first time in order to check all definitions
static int program_first(analyse_data_t* data)
{
	if(data->token.type == KEYWORD && data->token.data.Keyword == Function_KW)
    {
        CHECK_RULE(function_declaration);
        GET_TOKEN_AND_CHECK_RULE(possible_EOL);
        return program_first(data);
    }
    else if(data->token.type == TOKEN_EOF){
        return SYNTAX_OK;
    }
    else{
        GET_TOKEN();
        return program_first(data);
    }
}

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
        CHECK_RULE(possible_EOL);
        return statement(data);
    }
    // 〈 statement 〉 −→ 〈 if_else 〉 EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && data->token.data.Keyword == If_KW){
        CHECK_RULE(if_else);
        CHECK_RULE(possible_EOL);
        return statement(data);
    }
    // 〈 statement 〉 −→ 〈 while 〉EOL 〈 statement 〉
    else if(data->token.type == KEYWORD && data->token.data.Keyword == While_KW){
        CHECK_RULE(while_);
        CHECK_RULE(possible_EOL);
        return statement(data);
    }
    else if(data->token.type == IDENTIFIER){
        //5. 〈 statement 〉 −→ 〈 assignment 〉EOL 〈 statement 〉
        data->var_id = var_search(data, data->label_deep, data->token.data.String);
        bst_node_ptr tmp_id = data->current_id;
        data->current_id = symtable_search(&data->global_table, data->token.data.String);
        data->tmp_key = data->token.data.String;
        GET_TOKEN();
        if(data->token.type == ASSIGNMENT){
            printf("%s", data->tmp_key);
            CHECK_RULE(assignment);

            CHECK_RULE(possible_EOL);
            data->tmp_key = "";
        }
        //7. 〈 statement 〉 −→ 〈 f_call 〉EOL 〈 statement 〉
        else{
            if(strcmp(data->tmp_key, "write") == 0){
                CHECK_RULE(write);
                GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
            }
            else{
                CHECK_RULE(f_call);
                GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
            }
            data->tmp_key = "";

        }
        data->current_id = tmp_id;
        return statement(data);
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
        if(data->in_function == false || data->current_id == NULL){
            return SYNTAX_ERROR;
        }
        if(((function_data_t*)(*data->current_id).data)->return_data_type == Undefined){
            GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
            GENERATE_BLOCK(generate_function_void_return);
            return statement(data);
        }
        else{
            data->var_id = symtable_search(&data->global_table, "%%exp_result");
            GET_TOKEN_AND_CHECK_EXPRESSION();
            GENERATE_BLOCK(generate_function_return);
            CHECK_RULE(possible_EOL);
            return statement(data);
        }

    }
   
    //8. 〈 statement 〉 −→ EOL 〈 statement 〉
    else if(data->token.type == TOKEN_EOL){
        GET_TOKEN();
        return statement(data);
    }
    //9. 〈 statement 〉 −→ 〈 end 〉
    else if(data->token.type == TOKEN_EOF){
        return end(data); // TODO ig idk maybe inside end
    }
    //10. 〈 statement 〉 −→ ε
    return SYNTAX_OK;
}

static int function_declaration(analyse_data_t* data){
    data->in_defintion = true;
	data->label_index = 0;
    data->label_deep++;
    symtable_init(&data->local_table[data->label_deep]);
    GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
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
    func_data->defined = true;
    symtable_dispose(&data->local_table[data->label_deep]);
    data->label_deep--;
    data->in_defintion = false;
    return SYNTAX_OK;
}

static int function(analyse_data_t* data){
    //11. 〈 function 〉 −→ func 〈 ID 〉( 〈 args 〉) 〈 func_ret 〉{ EOL 〈 statement 〉 }
    if(data->token.type == KEYWORD && data->token.data.Keyword == Function_KW){
        data->in_defintion = true;
		data->label_index = 0;
        data->label_deep++;
        symtable_init(&data->local_table[data->label_deep]);
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        char * func_name = data->token.data.String;
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
		}		
        GENERATE_BLOCK(generate_function_start, func_name);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_LEFT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(args);
        CHECK_TYPE(TOKEN_RIGHT_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(func_ret);
        data->args_index = 0;
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(possible_EOL);
        data->in_defintion = false;
        // so we can difine the variable with the same name as arguments 
        data->label_deep++;
        symtable_init(&data->local_table[data->label_deep]);
        data->in_function = true;
        CHECK_RULE(statement);
        data->in_function = false;
        symtable_dispose(&data->local_table[data->label_deep]);
        data->label_deep--;
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        symtable_dispose(&data->local_table[data->label_deep]);
        GENERATE_BLOCK(generate_function_end, func_name);
        func_data->defined = true;
        data->label_deep--;
        GET_TOKEN_AND_CHECK_RULE(possible_EOL);
        
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

static int func_ret(analyse_data_t* data){
    //12. 〈 func_ret 〉 −→ 〈 FUNCTION_TYPE 〉〈 type 〉

    if(data->token.type == TOKEN_FUNCTION_TYPE){
        data->args_index++;
        GET_TOKEN_AND_CHECK_RULE(type);

		((function_data_t*)(*data->current_id).data)->return_data_type = ((function_data_t*)(*data->current_id).data)->params_types[data->args_index].data_type;
        ((function_data_t*)(*data->current_id).data)->return_data_q_type = ((function_data_t*)(*data->current_id).data)->params_types[data->args_index].q_type;
    }
    //13. 〈 func_ret 〉 −→ ε
    else{
        ((function_data_t*)(*data->current_id).data)->return_data_type = Undefined;
        ((function_data_t*)(*data->current_id).data)->return_data_q_type = false;
    }
    GENERATE_BLOCK(generate_function_return_param, ((function_data_t*)(*data->current_id).data)->return_data_type);
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
        GENERATE_BLOCK(generate_function_param, ((function_data_t*)(*data->current_id).data)->params_identifiers[data->args_index], var_data->data_type)
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
        GENERATE_BLOCK(generate_function_param, ((function_data_t*)(*data->current_id).data)->params_identifiers[data->args_index], var_data->data_type)
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
        symtable_init(&data->local_table[data->label_deep]);
        data->label_index += 2;
        bst_node_ptr let_id = NULL;
        bool prev_q_type = false;
        GET_TOKEN();
        if(data->token.type == KEYWORD && data->token.data.Keyword == Let_KW){
            GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);

            let_id = var_search(data, data->label_deep, data->token.data.String);
            if(let_id == NULL){
                return SEM_ERROR_UNDEF_VAR;
            }
            if(((var_data_t*)(*let_id).data)->q_type == false){
                return SEM_ERROR_TYPE_COMPAT;
            }
            prev_q_type = ((var_data_t*) let_id->data)->q_type; 
            ((var_data_t*) let_id->data)->q_type = false;
            GENERATE_BLOCK(gen_if_let, let_id->key, data->label_index);
            GET_TOKEN();
        }
        else{
            data->var_id = symtable_search(&data->global_table, "%%exp_result");
            CHECK_EXPRESSION();
            GENERATE_BLOCK(gen_if_start, data->label_index);
        }
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
		data->in_while_or_if = false;
        GET_TOKEN_AND_CHECK_RULE(statement);
        symtable_dispose(&data->local_table[data->label_deep]);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        GET_TOKEN_AND_CHECK_RULE(possible_EOL);
        if(data->token.data.Keyword != Else_KW){
            return SYNTAX_ERROR;
        }
        GENERATE_BLOCK(gen_if_else, data->label_index);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        symtable_init(&data->local_table[data->label_deep]);
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        symtable_dispose(&data->local_table[data->label_deep]);
        data->label_deep--;
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        if(let_id){
            ((var_data_t*) let_id->data)->q_type = prev_q_type;
        }
        GENERATE_BLOCK(gen_if_end, data->label_index);
        
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
        GENERATE_BLOCK(gen_while_start, data->label_index);
        symtable_init(&data->local_table[data->label_deep]);
        data->var_id = symtable_search(&data->global_table, "%%exp_result");
        GET_TOKEN_AND_CHECK_EXPRESSION();
        CHECK_TYPE(TOKEN_LEFT_CURLY_BRACKET);
        GENERATE_BLOCK(gen_while, data->label_index);
        data->in_while_or_if = false;
        GET_TOKEN_AND_CHECK_RULE(statement);
        CHECK_TYPE(TOKEN_RIGHT_CURLY_BRACKET);
        symtable_dispose(&data->local_table[data->label_deep]);
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        GENERATE_BLOCK(gen_while_end, data->label_index);
        data->label_deep--;
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
        GET_TOKEN_AND_CHECK_EXPRESSION();
        GENERATE_BLOCK(generate_var_assignment, data->tmp_key);
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

        data->tmp_key = data->token.data.String;
        data->var_id = symtable_search(&data->local_table[data->label_deep], data->token.data.String); 
		if (data->var_id == NULL)
		{   	
            var_data = malloc(sizeof(var_data_t));
            symtable_insert_var(&data->local_table[data->label_deep], data->token.data.String, var_data);
            data->var_id = symtable_search(&data->local_table[data->label_deep],  data->token.data.String);
        }else {
            return SEM_ERROR_UNDEF_VAR;
        }
        GENERATE_BLOCK(generate_var_declaration, data->tmp_key);
        bool no_type = true;
        bool no_assignment = true;
        GET_TOKEN();
        //22. 〈 def_var 〉−→ 〈 modifier 〉id :〈type 〉
        if(data->token.type == COLON){
            no_type = false;
            CHECK_RULE(def_type);
            var_data->q_type = ((var_data_t*)(*data->var_id).data)->q_type;
            var_data->data_type = ((var_data_t*)(*data->var_id).data)->data_type;
            data->in_var_definition = false;
        }
        else{
            var_data->q_type = Undefined;
            var_data->data_type = Undefined;
            data->in_var_definition = true;
        }
        GENERATE_BLOCK(generate_var_definition, data->tmp_key, var_data->data_type);
        // = 〈 expression 〉
        if(data->token.type == ASSIGNMENT){
            no_assignment = false;
            GET_TOKEN_AND_CHECK_EXPRESSION();
            GENERATE_BLOCK(generate_var_assignment, data->tmp_key);
            data->in_var_definition = false;
        }
        else{
            CHECK_TYPE(TOKEN_EOL);
            GET_TOKEN();
        }
        if(no_type && no_assignment){
            return SYNTAX_ERROR;
        }
        var_data->constant = constanta;
        return SYNTAX_OK;
    }

    return SYNTAX_ERROR;
}

static int write(analyse_data_t* data){
    //???. 〈 write 〉 −→ write ( 〈 expression 〉, ...)
    CHECK_TYPE(TOKEN_LEFT_BRACKET);
    GET_TOKEN_AND_CHECK_RULE(possible_EOL);
    if(data->token.type == TOKEN_RIGHT_BRACKET){
        GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
        return SYNTAX_OK;
    }
    data->var_id = symtable_search(&data->global_table, "%%exp_result");
    CHECK_EXPRESSION();
    while(data->token.type == COMMA){
        CHECK_RULE(possible_EOL);
        data->var_id = symtable_search(&data->global_table, "%%exp_result");
        GET_TOKEN_AND_CHECK_EXPRESSION();
        GENERATE_BLOCK(generate_write_val);
    }
    CHECK_RULE(possible_EOL);
    CHECK_TYPE(TOKEN_RIGHT_BRACKET);
    //GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
    return SYNTAX_OK;

}

// //23. 〈f_call 〉−→ id ( 〈fc_args 〉)
static int f_call(analyse_data_t* data){
    data->in_call_func = true;
    data->args_index = -1;
    if(data->current_id == NULL){
        return SEM_ERROR_UNDEF_FUNC;
    }
    GENERATE_BLOCK(gen_call_start);
    CHECK_TYPE(TOKEN_LEFT_BRACKET);
    GET_TOKEN_AND_CHECK_RULE(possible_EOL);
    if(data->token.type == TOKEN_RIGHT_BRACKET && ((function_data_t*)data->current_id->data)->param_len != -1){
        return SEM_ERROR_PARAM;
    }
    if(((function_data_t*)data->current_id->data)->param_len != -1){
        CHECK_RULE(fc_args);
    }
    else{
        if(data->token.type != TOKEN_RIGHT_BRACKET){
            return SEM_ERROR_PARAM;
        }
    }
    if(data->args_index != ((function_data_t*)data->current_id->data)->param_len){
        return SEM_ERROR_PARAM;
    }
    data->args_index = 0;
    CHECK_RULE(possible_EOL);
    CHECK_TYPE(TOKEN_RIGHT_BRACKET);
    GENERATE_BLOCK(gen_call, data->tmp_key);
    data->in_call_func = false;
    //GET_TOKEN_AND_CHECK_TYPE(TOKEN_EOL);
    return SYNTAX_OK;
}

// //23.2. 〈f_call 〉−→ id ( 〈fc_args 〉)
int f_expression_call(analyse_data_t* data, token_t id, data_type* type, bool* nullable){
    bst_node_ptr prev_current = data->current_id;
    bst_node_ptr prev_var = data->var_id;
    data->current_id = symtable_search(&data->global_table, id.data.String);
    *type = ((function_data_t*)data->current_id->data)->return_data_type;
    *nullable = ((function_data_t*)data->current_id->data)->return_data_q_type;
    CHECK_RULE(f_call);
    data->current_id = prev_current;
    data->var_id = prev_var;
    return SYNTAX_OK;   
}

// //24. 〈 fc_args 〉−→ id: expression 〈fc_ n_args 〉
static int fc_args(analyse_data_t* data){
    data->args_index = 0;
    //if _ as name var
    // a kak delat s undefined functions

    if(((function_data_t*)(*data->current_id).data)->param_len < data->args_index){
        return SEM_ERROR_PARAM;
    }
    if (strcmp(((function_data_t*)(*data->current_id).data)->param_names[data->args_index], "_") == 0){
        data->var_id = symtable_search(&data->global_table, "%%exp_result");
        CHECK_EXPRESSION();
        GENERATE_BLOCK(add_param_to_call, ((function_data_t*)(*data->current_id).data)->params_identifiers[data->args_index]);
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
        GENERATE_BLOCK(add_param_to_call, ((function_data_t*)(*data->current_id).data)->params_identifiers[data->args_index]);
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
        if(((function_data_t*)(*data->current_id).data)->param_len < data->args_index){
            return SEM_ERROR_PARAM;
        }
        if (strcmp(((function_data_t*)(*data->current_id).data)->param_names[data->args_index], "_") == 0){
            data->var_id = symtable_search(&data->global_table, "%%exp_result");
            GET_TOKEN_AND_CHECK_EXPRESSION();
            GENERATE_BLOCK(add_param_to_call, ((function_data_t*)(*data->current_id).data)->params_identifiers[data->args_index]);
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
            GENERATE_BLOCK(add_param_to_call, ((function_data_t*)(*data->current_id).data)->params_identifiers[data->args_index]);
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

    return SYNTAX_ERROR;
}

// //32. 〈 end 〉 −→ EOF
static int end(analyse_data_t* data){
    if(data->token.type == TOKEN_EOF){
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

static int type(analyse_data_t* data){
    //36. 〈 type 〉 −→ 〈 p_type 〉
    if(data->token.type == KEYWORD){
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
/*int main()
{
    //char *    input = "write(\"Zadejte cislo pro vypocet faktorialu: \")\nlet inp = readInt()\n// pomocna funkce pro dekrementaci celeho cisla o zadane cislo\nfunc decrement(of n: Int, by m: Int) -> Int {\nreturn n - m\n}\n// Definice funkce pro vypocet hodnoty faktorialu\nfunc factorial(_ n : Int) -> Int {\nvar result : Int?\nif (n < 2) {\nresult = 1\n} else {\nlet decremented_n = decrement(of: n, by: 1)\nlet temp_result = factorial(decremented_n)\nresult = n * temp_result\n}\nreturn result!\n}\n// pokracovani hlavniho tela programu\nif let inp {\nif (inp < 0) { // Pokracovani hlavniho tela programu\nwrite(\"Faktorial nelze spocitat!\")\n} else {\nlet vysl = factorial(inp)\n\nwrite(\"Vysledek je: \", vysl)\n}\n} else {\nwrite(\"Chyba pri nacitani celeho cisla!\")\n}";
    char *    input = "func main(_ i: Int) -> Int? {\n    return nil\n}\nfunc new() -> Int {\n    while 1 == 1{}\n    var i = 3\n    var k: Double = 5.5\n\n    return main(6 / 5) ?? new() - 8\n}\nlet c: Int \nfunc empty(){\n\n}\nfunc concat(b x : String, with y : String) -> String {\n    let x = y + y\n    if c == 5 - 5 {\n        var x : Double\n    }else{\n    }\n    return x + \"\" + y\n}\nlet a = \"ahoj \"\nvar hohol = concat(b: concat(b: a, with: \"svete\"), with: \"svete\") + \"\"\nempty()";
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);
    analyse_data_t *data = malloc(sizeof(analyse_data_t));
    
    if (!init_variables(data))
        return INTERNAL_ERROR;

    result = get_next_token(&data->token);

    if (data->token.type == TOKEN_EOF)
        return -1;

    //first go through
    result = program_first(data);
    printf("\n\nEND OF FIRST GO THROUGH\n\n");
    bst_node_ptr node = symtable_search(&data->global_table, "add");
    fclose(file);
    FILE *file2 = fmemopen(input, strlen(input), "r");
    set_source_file(file2);
    //second and main
    result = get_next_token(&data->token);
    result = program(data);

    if(result != SYNTAX_OK){
        printf("ERROR: %d\n", result);
        printf("GLOBAL LABEL - ");
        print_all_keys(data->global_table.root);
        printf("\n");
        for(int i = 0; i <= data->label_deep; i++){
            printf("LABEL: %d - ", i);
            print_all_keys(data->local_table[i].root);
            printf("\n");
        }
        // fprintf(stderr, "ERROR: %d\n", result);
    }else{
        printf("OK\n");
    }
    free_variables(data);
    //fclose(file);

    return result;
}*/

int analyse(){
    set_source_file(stdin);
    analyse_data_t *data = malloc(sizeof(analyse_data_t));
    
    if (!init_variables(data))
        return INTERNAL_ERROR;

    result = get_next_token(&data->token);

    //result = program(data);

    result = program_first(data);
    printf("\n\nEND OF FIRST GO THROUGH\n\n");
    fseek(stdin, 0, SEEK_SET);
    //second and main
    GENERATE_BLOCK(generator_start);
    result = get_next_token(&data->token);
    result = program(data);
    GENERATE_BLOCK(generator_end);
    if(result != SYNTAX_OK){
        printf("ERROR: %d\n", result);
        for(int i = 0; i <= data->label_deep; i++){
            printf("LABEL: %d - ", i);
            print_all_keys(data->local_table[i].root);
            printf("\n");
        }
        // fprintf(stderr, "ERROR: %d\n", result);
    }else{
        printf("OK\n");
    }

    free_variables(data);

    return result;
}
