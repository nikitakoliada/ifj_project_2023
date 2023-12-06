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

// Stack structure
typedef struct stack{
    int index;
    int size;
    stack_element** array;
}stack_t;

// Stack initialization
bool stack_init(stack_t**);
// Push element
bool stack_push(stack_t*, stack_element*);
// Pop element
stack_element* stack_pop(stack_t*);
// Pop specific amount of elements
void stack_pop_elements(stack_t*, int);
// Receive top stack element
stack_element* stack_top(stack_t*);
// receive top terminal from stack
stack_element* stack_top_terminal(stack_t*);
// insert element after top terminal
bool stack_insert_after_top_terminal(stack_t*, eSymbol, data_type);
// Checks if stack is empty
bool is_stack_empty(stack_t*);
// Dispose stack
void stack_dispose(stack_t*);