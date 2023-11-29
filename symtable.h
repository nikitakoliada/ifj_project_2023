/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file symtable.h

 * @brief Symtable definitions and header file for symtable.c

 * @author Juraj Remeň - xremen02
 * @author Nikita Koliada - xkolia00
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "scanner.h"

#ifndef SYMTABLE_H
#define SYMTABLE_H

typedef enum{
    Int_Type,
    String_Type,
    Double_Type, 
    Bool_Type,
    Undefined
}data_type;

// BST helpers and functions
typedef enum node_data_type {
    node_data_type_var,
    node_data_type_function,
} node_data_type_t;

// node of the tree
typedef struct bst_node {
    char* key;
    int height;
    node_data_type_t data_type;
    void* data;
    struct bst_node *left_ptr;
    struct bst_node *right_ptr;
} *bst_node_ptr;

// BST functions
bst_node_ptr new_bst_node(char*, void*, node_data_type_t);
void bst_init(bst_node_ptr *);
int bst_height(bst_node_ptr);
int bst_calculate_balance(bst_node_ptr);
int max(int, int);
bst_node_ptr bst_rotate_right(bst_node_ptr);
bst_node_ptr bst_rotate_left(bst_node_ptr);
bst_node_ptr bst_insert (bst_node_ptr, char*, void*, node_data_type_t);
void bst_replace_by_right_most(bst_node_ptr, bst_node_ptr);
bst_node_ptr bst_delete(bst_node_ptr, char*);
bst_node_ptr bst_search(bst_node_ptr, char*);
void bst_dispose(bst_node_ptr);

// symtable helpers and functions
typedef struct variable {
    int data_type;
    bool q_type; // true if f.e. int? or string? or double?
} var_data_t;

typedef struct function {
    int return_data_type;
    bool defined;
    int param_len;
    var_data_t params_types[100];
    char* params_identifiers[100];
    char* param_names[100];
} function_data_t;

typedef struct symtable {
    bst_node_ptr root;
} symtable_t;

// symtable functions
void symtable_init(symtable_t*);
void symtable_insert_var(symtable_t* table, char* key, var_data_t* data);
void symtable_insert_function(symtable_t* table, char* key, function_data_t* data);
void symtable_delete(symtable_t*, char*);
bst_node_ptr symtable_search(symtable_t*, char*);
void symtable_dispose(symtable_t*);

#endif // SYMTABLE_H