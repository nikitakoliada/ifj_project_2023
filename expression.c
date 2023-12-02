/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file expression.c

 * @brief Implementation of the expression.c file part of parser

 * @author Pavlo Butenko - xbuten00
*/


//#include "expression.h"
#include "analysis.h"
#include "stack.h"
#include "error.h"
#include "scanner.h"
#include "symtable.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define FREE_RECOURCES(stack)do{\
        stack_dispose(stack);\
        free(stack);\
    }while(0)

#define GET_NEXT_NOT_EOL_TOKEN(token)do{\
        if((result = get_next_token(&token))){\
            FREE_RECOURCES(stack);\
            return result;\
        }\
        if(token.type == TOKEN_EOL) was_EOL = true;\
    }while(token.type == TOKEN_EOL);


#define PT_SIZE 9

typedef enum{
    S,      // <
    R,      // >
    E,      // =
    F,      // Failure
    P,      // Pass
    FUNC,   // Function
} precedence_value_t;

typedef enum{
    PMI,  // +-
    MDI,  // */
    LPI,  // (
    RPI,  // )
    RelI, // == != >= <= > <
    NCI,  // ??
    NI,   // !
    IdI,  // id
    DI    // $  
}pt_index;

precedence_value_t predence_table[PT_SIZE][PT_SIZE] = {
    //        +-  */  (     )   r   ??  !  id   $          
    /* +- */{ R,  S,  S,    R,  R,  R,  S,  S,  R},
    /* /* */{ S,  R,  S,    R,  R,  R,  S,  S,  R},
    /* (  */{ S,  S,  S,    E,  S,  S,  S,  S,  F},
    /* )  */{ R,  R,  F,    R,  R,  R,  S,  R,  R},
    /* r  */{ S,  S,  S,    R,  F,  R,  S,  S,  R},
    /* ?? */{ S,  S,  S,    R,  S,  S,  S,  S,  R},
    /* !  */{ R,  R,  S,    R,  R,  R,  F,  S,  R},
    /* id */{ R,  R, FUNC,  R,  R,  R,  R,  F,  R},
    /* $  */{ S,  S,  S,    F,  S,  S,  S,  S,  F}
};

int get_pt_index(eSymbol symbol){
    switch(symbol){
        case PlusS:
        case MinusS:
            return PMI;
        case MultS:
        case DivS:
            return MDI;
        case LPS:
            return LPI;
        case RPS:
            return RPI;
        case EqS:
        case GEqS:
        case LEqs:
        case LessS:
        case GreaterS:
            return RelI;
        case NilCS:
            return NCI;
        case NotS:
            return NI;
        case DoubleS:
        case IntS:
        case StringS:
        case IdS:
        case FunctionS:
        case NilS:
            return IdI;
        case DollarS:
        default:
            return DI;
    }
}


data_type get_data_type(token_t token, analyse_data_t* data, bool* is_nullable){
    switch(token.type){
        case IDENTIFIER:
            assert(data->var_id);
            int label_deep = data->label_deep;
            if(!strcmp(token.data.String, data->var_id->key) && data->in_var_definition){
                label_deep = label_deep > 0 ? label_deep - 1 : 0;
            }
            bst_node_ptr node = var_search(data, label_deep, token.data.String);
            if(!node) return Undefined;
            var_data_t* var_data = (var_data_t*)node->data;
            if(is_nullable) *is_nullable = var_data->q_type;
            return var_data->data_type;
        case INT_VALUE:
            return Int_Type;

        case DOUBLE_VALUE:
            return Double_Type;

        case STRING_VALUE:
            return String_Type;

        default: return Undefined;
    }
}

eSymbol token_to_esymbol(token_t token, analyse_data_t* data, data_type* type, bool* is_nullable){
    switch(token.type){
        case PLUS:  return PlusS;
        case MINUS: return MinusS;
        case MUL: return MultS;
        case DIV: return DivS;
        case TOKEN_LEFT_BRACKET: return LPS;
        case TOKEN_RIGHT_BRACKET: return RPS;
        case EQUAL: return EqS;
        case NOT_EQUAL: return NEqS;
        case MORE_THAN: return GreaterS;
        case LESS_THAN: return LessS;
        case LESS_THAN_OR_EQUAL: return LEqs;
        case MORE_THAN_OR_EQUAL: return GEqS;
        case NIL_COLL: return NilCS;
        case NOT: return NotS;
        case DOUBLE_VALUE: return DoubleS;
        case STRING_VALUE: return StringS;
        case INT_VALUE: return IntS;
        case KEYWORD:
            if(token.data.Keyword == Nil_KW) return NilS; 
            return DollarS;
        case IDENTIFIER:
            *type = get_data_type(token, data, is_nullable);
            return IdS;
        default: return DollarS; // Process
    }
}


void stack_print(stack_t* stack){
    for(int i = stack->index; i >= 0; i--){
        printf("%d)%d - %d\n", i, stack->array[i]->symbol, stack->array[i]->type);
    }
}

void swap(void** a, void** b){
    void* temp = *a;
    *a  = *b;
    *b = temp;
}

int reduce(stack_t* stack){
    stack_element* elements[3];
    int elements_count = 0;

    for(int i = 0; i < 3 && stack->array[stack->index - i]->symbol != Handle; i++){
        elements_count++;
        elements[i] = stack->array[stack->index - i];
        if(stack->index - i  == -1) return SYNTAX_ERROR;
    }

    // Needs to be reimplemented
    if(elements_count == 3){
        swap((void**)&elements[0], (void**)&elements[2]);
    }
    if(elements_count == 2){
        swap((void**)&elements[0], (void**)&elements[1]);
    }
    //


    if(stack->array[stack->index - elements_count]->symbol != Handle){
        return SYNTAX_ERROR;
    }

    stack_pop_elements(stack, elements_count + 1); // Check for error

    stack_element* new_element = malloc(sizeof(stack_element));
    new_element->symbol = NON_TERM;

    new_element->is_nil = false;
    new_element->nullable = false;
    new_element->nullable = false;


    // E -> id
    if(elements_count == 1){
        new_element->type = elements[0]->type;
        new_element->nullable = elements[0]->nullable;
        new_element->is_identifier = elements[0]->is_identifier;
        if(elements[0]->type == Undefined){
            if(elements[0]->symbol != NilS){
                return SEM_ERROR_UNDEF_VAR;
            }

            new_element->is_nil = true;
        }
    }
    // E -> E!
    else if(elements_count == 2){
        if(elements[1]->symbol != NotS || elements[0]->symbol != NON_TERM){
            return SYNTAX_ERROR;
        }

        if(elements[0]->type == Undefined){
            if(elements[0]->is_nil){
                return SEM_ERROR_TYPE_COMPAT;
            }
            return SEM_ERROR_UNDEF_VAR;
        }

        if(!elements[0]->nullable){
            return SEM_ERROR_TYPE_COMPAT;
        }

        new_element->nullable = false;
        new_element->type = elements[0]->type;
        new_element->is_identifier = elements[0]->is_identifier;

        // Generate ! code
    }
    else if(elements_count == 3){
        //printf("[1] = %d\n", elements[0]->symbol);
        //printf("[2] = %d\n", elements[1]->symbol);
        //printf("[3] = %d\n", elements[2]->symbol);
        // E -> (E)
        if(elements[0]->symbol == LPS && elements[1]->symbol == NON_TERM && elements[2]->symbol == RPS){
            new_element->nullable = elements[1]->nullable;
            new_element->type = elements[1]->type;
            new_element->is_nil = elements[1]->is_nil;
            new_element->is_identifier = elements[1]->is_identifier;
        }
        else{
            if(elements[0]->symbol != NON_TERM || elements[2]->symbol != NON_TERM){
                return SYNTAX_ERROR;
            }

            if(elements[1]->symbol != NilCS && get_pt_index(elements[1]->symbol) != RelI 
            && (elements[0]->nullable || elements[2]->nullable
            || elements[0]->is_nil || elements[2]->is_nil)){
                return SEM_ERROR_TYPE_COMPAT;
            }

            if((elements[0]->type == Undefined && !elements[0]->is_nil) 
            || (elements[2]->type == Undefined && !elements[2]->is_nil)){
                return SEM_ERROR_UNDEF_VAR;
            }

            new_element->is_identifier = elements[0]->is_identifier || elements[2]->is_identifier;

            if(elements[1]->symbol == NilCS){
                if((elements[0]->type != elements[2]->type && !elements[0]->is_nil) 
                || elements[2]->nullable || elements[2]->is_nil){
                    return SEM_ERROR_TYPE_COMPAT;
                }

                new_element->type = elements[2]->type;

                // Generate ?? code
            }
            else if(elements[1]->symbol == DivS){

                if((elements[0]->type != Int_Type && elements[0]->type != Double_Type) 
                || (elements[2]->type != Int_Type && elements[2]->type != Double_Type)){
                    return SEM_ERROR_TYPE_COMPAT;
                }

                if(elements[0]->type == Int_Type && elements[2]->type == Int_Type){
                    new_element->type = Int_Type;
                    // Generate decimal division
                }
                else{
                    if(elements[0]->type == Int_Type ) {
                        if(elements[0]->is_identifier) return SEM_ERROR_TYPE_COMPAT;
                        // Generate Int2Double for 1 operands
                    }

                    if(elements[2]->type == Int_Type) {
                        if(elements[2]->is_identifier) return SEM_ERROR_TYPE_COMPAT;
                        // Generate Int2Double for 2 operands
                    }

                    // Generate float division

                    new_element->type = Double_Type;
                }
            }
            else if(elements[1]->symbol == PlusS 
            || elements[1]->symbol == MinusS 
            || elements[1]->symbol == MultS){
                if(elements[0]->type == String_Type || elements[2]->type == String_Type){
                    
                    if(elements[0]->type != String_Type 
                    || elements[2]->type != String_Type 
                    || elements[1]->symbol != PlusS){
                        return SEM_ERROR_TYPE_COMPAT;
                    }

                    new_element->type = String_Type;

                    // Generate concatenation
                }
                else{
                    if((elements[0]->type != Int_Type && elements[0]->type != Double_Type) 
                    || (elements[2]->type != Int_Type && elements[2]->type != Double_Type)){
                        return SEM_ERROR_TYPE_COMPAT;
                    }

                    new_element->type = Int_Type;

                    if(elements[0]->type == Double_Type || elements[2]->type == Double_Type){
                        if(elements[0]->type == Int_Type){
                            if(elements[0]->is_identifier) return SEM_ERROR_TYPE_COMPAT;
                            // Generate Int2Double code
                        }

                        if(elements[2]->type == Int_Type){
                            if(elements[2]->is_identifier) return SEM_ERROR_TYPE_COMPAT;
                            // Generate Int2Double code
                        }

                        new_element->type = Double_Type;
                    }

                    switch(elements[1]->symbol){
                        case PlusS:
                            // Generate sum
                            break;
                        case MinusS:
                            // Generate difference
                            break;
                        case MultS:
                            // Generate multiplication
                            break;
                    }
                }
            }
            else if(elements[1]->symbol == EqS 
            || elements[1]->symbol == NEqS){
                if((elements[0]->type == Int_Type || elements[0]->type == Double_Type) 
                && (elements[2]->type == Int_Type || elements[2]->type == Double_Type)){

                    if(elements[0]->type == Double_Type && elements[2]->type == Int_Type){
                        if(elements[2]->is_identifier) return SEM_ERROR_TYPE_COMPAT;
                        // Generate Int2Double code
                    }

                    if(elements[2]->type == Double_Type && elements[0]->type == Int_Type){
                        if(elements[0]->is_identifier) return SEM_ERROR_TYPE_COMPAT;
                        // Generate Int2Double code
                    }
                }
                else if(elements[0]->type != elements[2]->type 
                && (!elements[0]->is_nil && !elements[2]->is_nil)){
                    return SEM_ERROR_TYPE_COMPAT;
                }

                switch(elements[1]->symbol){
                    case EqS:
                        // Generate ==
                        break;
                    case NEqS:
                        // Generate !=
                        break;
                }

                new_element->type = Bool_Type;
            }
            else if(elements[1]->symbol == LEqs
            || elements[1]->symbol == GEqS
            || elements[1]->symbol == LessS
            || elements[1]->symbol == GreaterS){
                if(elements[0]->type != elements[2]->type 
                || elements[0]->nullable || elements[2]->nullable){
                    return SEM_ERROR_TYPE_COMPAT;
                }

                switch(elements[1]->symbol){
                    case LessS:
                        // Generate <
                        break;
                    case GreaterS:
                        // Generate >
                        break;
                    case GEqS:
                        // Geenrate >= 
                        break;
                    case LEqs:
                        // Generate <=
                        break;
                }   

                new_element->type = Bool_Type;
            }
            else{
                return SYNTAX_ERROR;
            }
        }

        
    }
    else{
        return SYNTAX_ERROR;
    }

    if(!stack_push(stack, new_element)) return INTERNAL_ERROR;

    return SYNTAX_OK;
}

int compare_output_types(analyse_data_t* data, stack_element* final_element){

    data_type expected_type = 0;
    bool nullable = false;

    if(data->in_defintion){
        var_data_t type = ((function_data_t*)data->current_id->data)->params_types[data->args_index];
        expected_type = type.data_type;
        nullable = type.q_type;
    }else if(data->in_while_or_if){
        return final_element->type == Bool_Type && !final_element->nullable ? SYNTAX_OK : SEM_ERROR_TYPE_COMPAT;
    }else if(data->in_function && !data->in_var_definition && ((var_data_t*)data->var_id->data)->data_type == Undefined){
        function_data_t* func_data = (function_data_t*)data->current_id->data;
        expected_type = func_data->return_data_type;
        nullable = func_data->return_data_q_type;
    }else{
        var_data_t* var_data = (var_data_t*)data->var_id->data;
        expected_type = var_data->data_type;
        nullable = var_data->q_type;
    }

    if((final_element->nullable || final_element->is_nil) && !nullable && expected_type != Undefined){
        return SEM_ERROR_TYPE_COMPAT;
    }

    if(nullable && final_element->is_nil){
        return SYNTAX_OK;
    }

    switch(expected_type){
        case Int_Type:
            if(final_element->type != Int_Type){
                return SEM_ERROR_TYPE_COMPAT;
            }
            break;
        case Double_Type:
            if(final_element->type == Int_Type){
                if(final_element->is_identifier){
                    return SEM_ERROR_TYPE_COMPAT;
                }
            }
            else if(final_element->type != Double_Type){
                return SEM_ERROR_TYPE_COMPAT;
            }
            break;
        case String_Type:
            if(final_element->type != String_Type){
                return SEM_ERROR_TYPE_COMPAT;
            }
            break;
        case Undefined:
            break;
    }

    return SYNTAX_OK;
}


void process_parenthese(token_t token, int* count){
    if(token.type == TOKEN_LEFT_BRACKET) (*count)++;
    if(token.type == TOKEN_RIGHT_BRACKET) (*count)--;
}


int expression(analyse_data_t* data, bool* is_EOL){

    stack_t* stack = malloc(sizeof(stack_t));
    if(!stack) return INTERNAL_ERROR;


    if(!stack_init(&stack)){
        free(stack);
        return INTERNAL_ERROR;
    }

    stack_element dollar_element;
    
    dollar_element.symbol = DollarS;
    dollar_element.type = Undefined;

    if(!stack_push(stack, &dollar_element)){
        FREE_RECOURCES(stack);
        return INTERNAL_ERROR;
    }



    bool is_success = false;
    bool was_EOL = false;
    bool reduce_only = false;
    int parantheses_counter = 0;
    int result = 0;
    token_t prev_token;
    token_t token = data->token;
    if(token.type == TOKEN_EOL){
        GET_NEXT_NOT_EOL_TOKEN(token);
    }

    process_parenthese(token, &parantheses_counter);

    do{
        stack_print(stack);
        bool nullable = false;
        data_type input_id_data_type = Undefined;
        eSymbol input_symbol = token_to_esymbol(token, data, &input_id_data_type, &nullable);
        pt_index input_index = get_pt_index(input_symbol);
        stack_element* st_element = stack_top_terminal(stack);
        if(!st_element) return INTERNAL_ERROR;
        eSymbol stack_symbol = st_element->symbol;
        pt_index stack_symbol_index = get_pt_index(stack_symbol);

        precedence_value_t precedence_result = predence_table[stack_symbol_index][input_index];

        if(parantheses_counter < 0){
            if(stack->index == 1) precedence_result = P;
            else precedence_result = R;
        }

        printf("-****%d - %d*****\n", stack_symbol_index, input_index);
        stack_element* new_element = NULL;
        
        if(input_index == IdI && stack_symbol_index == IdI && was_EOL){
            reduce_only = true;
        }   

        if(reduce_only){
            if(stack_symbol != DollarS){
                precedence_result = R;
            }
            else{
                precedence_result = P;
            }
        }

        switch(precedence_result){
            case R:
                if((result = reduce(stack)) != SYNTAX_OK){
                    // Free recources
                    FREE_RECOURCES(stack);
                    return result;
                }

                break;

            case S:
                new_element = malloc(sizeof(stack_element));
                if(!new_element) return INTERNAL_ERROR;
                was_EOL = false;
                if(!stack_insert_after_top_terminal(stack, Handle, Undefined)){
                    FREE_RECOURCES(stack);
                    return INTERNAL_ERROR;
                }
                new_element->symbol = input_symbol;
                new_element->type = input_symbol == IdS 
                    ? input_id_data_type 
                    : get_data_type(token, data, &nullable);
                new_element->nullable = nullable;
                new_element->is_identifier = token.type == IDENTIFIER;
                if(!stack_push(stack, new_element))
                {
                    FREE_RECOURCES(stack);
                    return INTERNAL_ERROR;
                } 

                prev_token = token;

                GET_NEXT_NOT_EOL_TOKEN(token);

                process_parenthese(token, &parantheses_counter);

                break;

            case E:
                new_element = malloc(sizeof(stack_element));
                if(!new_element) return INTERNAL_ERROR;
                was_EOL = false;
                new_element->symbol = input_symbol;
                new_element->type = input_symbol == IdS 
                    ? input_id_data_type 
                    : get_data_type(token, data, &nullable);
                new_element->nullable = nullable;
                new_element->is_identifier = token.type == IDENTIFIER;
                if(!stack_push(stack, new_element))
                {
                    FREE_RECOURCES(stack);
                    return INTERNAL_ERROR;
                } 
                GET_NEXT_NOT_EOL_TOKEN(token);

                process_parenthese(token, &parantheses_counter);

                break;

            case FUNC:
                new_element = malloc(sizeof(stack_element));
                if(!new_element) return INTERNAL_ERROR;
                data_type type = Int_Type;
                data->token = token;
                if((result = f_expression_call(data, prev_token, &type, &nullable))){
                    FREE_RECOURCES(stack);
                    return result;
                }
                new_element->is_identifier = true;
                new_element->is_nil = false;
                new_element->nullable = nullable;
                new_element->type = type;
                new_element->symbol = FunctionS;
                printf("---%d\n", type);
                if(!stack_pop(stack))
                {
                    FREE_RECOURCES(stack);
                    return INTERNAL_ERROR;
                }
                if(!stack_push(stack, new_element))
                {
                    FREE_RECOURCES(stack);
                    return INTERNAL_ERROR;
                }

                GET_NEXT_NOT_EOL_TOKEN(token);

                process_parenthese(token, &parantheses_counter);

                break;

            case F:
                if(input_symbol == DollarS && stack_symbol == DollarS){
                    is_success = true;
                    printf("lol");
                }
                else{
                    // free recources
                    FREE_RECOURCES(stack);
                    return SYNTAX_ERROR;
                }

                break;

            case P:
                is_success = true;
                break;
        }
    }while(!is_success);

    stack_element* final_element = stack_pop(stack);


    if(!final_element){
        // Free recources
        FREE_RECOURCES(stack);
        return SYNTAX_ERROR;
    }

    if(final_element->symbol != NON_TERM){

        // Free recources
        FREE_RECOURCES(stack);
        return INTERNAL_ERROR;
    }


    if((result = compare_output_types(data, final_element))){
        FREE_RECOURCES(stack);
        return result;
    }

    else if(data->in_var_definition){
        var_data_t* var_data = (var_data_t*)data->var_id->data;
        var_data->data_type = final_element->type;
        var_data->q_type = final_element->nullable;
    }

    // Generate assignment

    data->token = token;
    *is_EOL = was_EOL; 

    FREE_RECOURCES(stack);

    printf("End of Expression\n");

    return SYNTAX_OK;
}


