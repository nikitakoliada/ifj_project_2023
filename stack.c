#include "stack.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define INIT_SIZE 8

bool stack_init(stack_t** stack){
    if(!((*stack)->array = malloc(INIT_SIZE * sizeof(void*)))) return false;
    (*stack)->index = -1;
    (*stack)->size = INIT_SIZE;
    return true; 
}

bool stack_extend(stack_t** stack){
    int new_stack_size = ((*stack)->size + 1) * 2;
    if(!((*stack)->array = realloc((*stack)->array, new_stack_size * sizeof(stack_element*)))) return false;
    (*stack)->size = new_stack_size;
}

bool stack_push(stack_t* stack, stack_element* item){
    assert(stack);

    if(stack->index + 1 == stack->size){
        int new_size = stack_extend(&stack);
        if(!new_size) return false;
        stack->size = new_size;
    }

    stack->array[++stack->index] = item;

    return true;
}

stack_element* stack_pop(stack_t* stack){
    assert(stack);

    if(is_stack_empty(stack)) return NULL;

    return stack->array[stack->index--];
}

void stack_pop_elements(stack_t* stack, int count){
    assert(stack);

    while(count-- && stack_pop(stack));
}


stack_element* stack_top(stack_t* stack){
    assert(stack);

    if(is_stack_empty(stack)) return NULL;

    return stack->array[stack->index];
}

int top_terminal_index(stack_t* stack){
    assert(stack);

    int index = stack->index;
    stack_element* element = NULL;

    do{
        if(index == -1) return index;
        element = stack->array[index--];

    }while(element->symbol == NON_TERM);

    return ++index;
}

stack_element* stack_top_terminal(stack_t* stack){
    assert(stack);

    int index = top_terminal_index(stack);

    if(index == -1) return NULL;

    return stack->array[index];
}

bool stack_insert_after_top_terminal(stack_t* stack, eSymbol symbol, data_type type){
    assert(stack);

    int index = top_terminal_index(stack);

    if(index == -1) return false;

    if(stack->index + 1 == stack->size){
        int new_size = stack_extend(&stack);
        if(!new_size) return false;
        stack->size = new_size;
    }

    for(int i = stack->index; i > index; i--){
        stack->array[i + 1] = stack->array[i];
    }

    stack_element* new_element = malloc(sizeof(stack_element));
    if(!new_element) return false;
    new_element->symbol = symbol;
    new_element->type = type;

    stack->array[index + 1] = new_element;
    stack->index++;

    return true;
}


bool is_stack_empty(stack_t* stack){
    assert(stack);

    return stack->index == -1;
}

void stack_dispose(stack_t* stack){
    assert(stack);

    free(stack->array);
}
