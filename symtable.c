/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file symtable.c

 * @brief Symtable implementation - height balanced binary search tree

 * @author Juraj Remeň - xremen02
 */

#include <stdlib.h>

#include "scanner.h"
#include "symtable.h"

//BST functions
void bst_init(bst_node_ptr *root_ptr)
{
    *root_ptr = NULL;
}

void bst_insert(bst_node_ptr* root_ptr, char* key, void* data, node_data_type_t data_type)
{
    if (root_ptr == NULL && (*root_ptr) == NULL)
    {
        struct bst_node *tmp;
        tmp = (struct bst_node*)malloc(sizeof(struct bst_node));

        if (tmp == NULL)
        {
            return;
        }
        tmp->key = key;
        tmp->data = data;
        tmp->data_type = data_type;
        tmp->left_ptr = tmp->right_ptr = NULL;

        (*root_ptr) = tmp;
    }

    else
    {
        if (strcmp(key, (*root_ptr)->key) != 0)
        {
            if (strcmp(key, ((*root_ptr)->key)) < 0)
            {
                bst_insert(&((*root_ptr)->left_ptr), key, data, data_type);
            }

            else if (strcmp(key, (*root_ptr)->key) > 0)
            {
                bst_insert(&((*root_ptr)->right_ptr), key, data, data_type);
            }
        }

        else
        {
            (*root_ptr)->data = data;
        }
    }
}

void replace_by_right_most(bst_node_ptr target, bst_node_ptr *root_ptr)
{
    if ( (*root_ptr)->right_ptr == NULL)
    {
        target->key = (*root_ptr)->key;
        target->data = (*root_ptr)->data;
        bst_node_ptr to_delete = *root_ptr;
        *root_ptr = (*root_ptr)->left_ptr;
        free(to_delete);
    }

    else
    {
        replace_by_right_most(target, &((*root_ptr)->right_ptr));
    }
}

void bst_delete(bst_node_ptr *root_ptr, char* key)
{
    if (root_ptr && (*root_ptr))
    {
        if (strcmp(key, (*root_ptr)->key) < 0)
        {
            bst_delete(&(*root_ptr)->left_ptr, key);
        }

        else if (strcmp(key, (*root_ptr)->key) > 0)
        {
            bst_delete(&(*root_ptr)->right_ptr, key);
        }

        else
        {
            if (((*root_ptr)->left_ptr == NULL) && ((*root_ptr)->right_ptr == NULL))// leaf node
            {
                free((*root_ptr)->data);
                free(*root_ptr);
                *root_ptr = NULL;
            }

            else if (((*root_ptr)->left_ptr != NULL) && ((*root_ptr)->right_ptr == NULL)) // left subtree
            {
                free((*root_ptr)->data);
                free(*root_ptr);
                *root_ptr = (*root_ptr)->left_ptr;
            }

            else if (((*root_ptr)->left_ptr == NULL) && ((*root_ptr)->right_ptr != NULL)) // right subtree
            {
                free((*root_ptr)->data);
                free(*root_ptr);
                *root_ptr = (*root_ptr)->right_ptr;
            }

            else
            {
                replace_by_right_most((*root_ptr), &((*root_ptr)->left_ptr));
            }
        }
    }
}

bst_node_ptr bst_search(bst_node_ptr root_ptr, char* key)
{
    if (root_ptr == NULL)
    {
        return NULL;
    }

    else
    {
        if (strcmp(key, root_ptr->key) == 0)
        {
            return root_ptr;
        }

        else if (strcmp(key, root_ptr->key) > 0)
        {
            return bst_search(root_ptr->right_ptr, key);
        }

        else
        {
            return bst_search(root_ptr->left_ptr, key);
        }
    }
}

int init_string(string *str)
{
    if ((str->value = (char*)malloc(10 * sizeof(char))) == NULL)
    {
        return INTERNAL_ERROR;
    }
    str->value[0] = '\0';
    str->len = 0;
    str->allocated_len = 10;
    return SYNTAX_OK;
}

void dispose_string(string *str)
{
    free(str->value);
    str->value = NULL;
    str->len = str->allocated_len = 0;
}

void bst_dispose(bst_node_ptr *root_ptr)
{
    if ((*root_ptr) != NULL)
    {
        bst_dispose(&(*root_ptr)->left_ptr); //disposal of left subtree
        bst_dispose(&(*root_ptr)->right_ptr); // disposal of right subtree

        free((*root_ptr)->key);
        (*root_ptr)->key = NULL;

        if ((*root_ptr)->data_type == node_data_type_function)
        {
            dispose_string(&(((function_data_t*)(*root_ptr)->data)->params));
        }

        free((*root_ptr)->data);
        (*root_ptr)->data = NULL;

        free(*root_ptr);
        *root_ptr = NULL;
    }
}

// symtable functions
void symtable_init(symtable_t* table)
{
    bst_init(&(table)->root);
}

void symtable_insert_var(symtable_t* table, string key)
{
    var_data_t *data_ptr;
    if ((data_ptr = (var_data_t*)malloc(sizeof(var_data_t))) == NULL)
    {
        return;
    }

    data_ptr->data_type = -1;
    bst_insert(&(table->root), key.value, data_ptr, node_data_type_var);
}

void symtable_insert_function(symtable_t* table, string key)
{
    function_data_t *data_ptr;
    if ((data_ptr = (function_data_t*)malloc(sizeof(function_data_t))) == NULL)
    {
        return;
    }

    string params;
    init_string(&params);
    data_ptr->return_data_type = -1;
    data_ptr->declared = false;
    data_ptr->defined = false;
    data_ptr->params = params;

    bst_insert(&(table->root), key.value, data_ptr, node_data_type_function);
}

void symtable_delete(symtable_t* table, string key)
{
    bst_delete(&(table)->root, key.value);
}

bst_node_ptr symtable_search(symtable_t* table, string key)
{
    return bst_search(table->root, key.value);
}

void symtable_dispose(symtable_t* table)
{
    bst_dispose(&(table)->root);
}