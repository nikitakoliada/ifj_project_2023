/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file stack.h

 * @brief Header file for the stack

 * @author Pavlo Butenko - xbuten00
*/

#include <stdbool.h>
#include "expression.h"

typedef struct{
    eSymbol symbol;
    data_type type;
    bool nullable;
    bool is_nil;
    bool is_identifier;
}stack_element;

typedef struct stack{
    int index;
    int size;
    stack_element** array;
}stack_t;

bool stack_init(stack_t**);
bool stack_extend(stack_t**);
bool stack_push(stack_t*, stack_element*);
stack_element* stack_pop(stack_t*);
void stack_pop_elements(stack_t*, int);
stack_element* stack_top(stack_t*);
int top_terminal_index(stack_t*);
stack_element* stack_top_terminal(stack_t*);
bool stack_insert_after_top_terminal(stack_t*, eSymbol, data_type);
bool is_stack_empty(stack_t*);
void stack_dispose(stack_t*);