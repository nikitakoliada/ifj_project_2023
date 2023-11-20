/**
 * Project: Implementation of imperative language compiler IFJ23
 * @file symtable.c

 * @brief Symtable implementation - height balanced binary search tree

 * @author Juraj Remeň - xremen02
 * @author Nikita Koliada - xkolia00
 */

#include <stdlib.h>

#include "scanner.h"
#include "symtable.h"

//BST functions
void bst_init(bst_node_ptr *root_ptr)
{
    *root_ptr = NULL;
}

int bst_height(bst_node_ptr *root_ptr)
{
    if (root_ptr == NULL)
    {
        return 0;
    }

    else
    {
        return ((*root_ptr)->height);
    }
}

int bst_calculate_balance(bst_node_ptr *root_ptr)
{
    struct bst_node *tmp;
    tmp = (struct bst_node*)malloc(sizeof(struct bst_node));
    if (root_ptr == NULL && tmp == NULL)
    {
        return 0;
    }

    else
    {
        int balance = (bst_height((bst_node_ptr *) (*root_ptr)->left_ptr)) - (bst_height((bst_node_ptr *) (*root_ptr)->right_ptr));
        return balance;
    }
}

int max(int x, int y)
{
    return ( (x > y) ? x : y );
}

bst_node_ptr bst_rotate_right(bst_node_ptr *root_ptr)
{
    bst_node_ptr (*tmp1) = (bst_node_ptr *) (*root_ptr)->left_ptr;
    bst_node_ptr (*tmp2) = (bst_node_ptr *) (*tmp1)->right_ptr;

    (*tmp1)->right_ptr =  (*root_ptr);
    (*root_ptr)->left_ptr = (*tmp2);

    (*root_ptr)->height = max(bst_height(&(*root_ptr)->left_ptr), bst_height(&(*root_ptr)->right_ptr)) + 1;
    (*tmp1)->height = max(bst_height(&(*tmp1)->left_ptr), bst_height(&(*tmp1)->right_ptr)) + 1;

    return (*tmp1);
}

bst_node_ptr bst_rotate_left(bst_node_ptr *root_ptr)
{
    bst_node_ptr *tmp1 = (bst_node_ptr *) (*root_ptr)->left_ptr;
    bst_node_ptr *tmp2 = (bst_node_ptr *) (*tmp1)->right_ptr;

    (*tmp1)->right_ptr =  (*root_ptr);
    (*root_ptr)->left_ptr = (*tmp2);

    (*root_ptr)->height = max(bst_height(&(*root_ptr)->left_ptr), bst_height(&(*root_ptr)->right_ptr)) + 1;
    (*tmp1)->height = max(bst_height(&(*tmp1)->left_ptr), bst_height(&(*tmp1)->right_ptr)) + 1;

    return (*tmp1);
}

bst_node_ptr bst_insert(bst_node_ptr* root_ptr, char* key, void* data, node_data_type_t data_type)
{
    if (root_ptr == NULL && (*root_ptr) == NULL)
    {
        struct bst_node *tmp;
        tmp = (struct bst_node*)malloc(sizeof(struct bst_node));

        if (tmp == NULL)
        {
            return NULL;
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
            if (strcmp(key, ((*root_ptr)->key)) < 0 )
            {
                bst_insert(&((*root_ptr)->left_ptr), key, data, data_type);
                if (bst_height(&(*root_ptr)->left_ptr) > bst_height(&(*root_ptr)->right_ptr))
                {
                    (*root_ptr)->height = bst_height(&(*root_ptr)->left_ptr);
                }
            }

            else if (strcmp(key, (*root_ptr)->key) > 0)
            {
                bst_insert(&((*root_ptr)->right_ptr), key, data, data_type);
                if (bst_height(&(*root_ptr)->left_ptr) < bst_height(&(*root_ptr)->right_ptr))
                {
                    (*root_ptr)->height = bst_height(&(*root_ptr)->right_ptr);
                }
            }

            int balance = bst_calculate_balance(root_ptr);

            // leaning left
            if (balance > 1 && key < (*root_ptr)->left_ptr->key)
            {
                return bst_rotate_right(root_ptr);
            }

            // leaning left and then right
            else if (balance > 1 && key > (*root_ptr)->left_ptr->key)
            {
                (*root_ptr)->left_ptr = bst_rotate_left(&(*root_ptr)->left_ptr);
                return bst_rotate_right(root_ptr);
            }
            // leaning right
            else if (balance < -1 && key > (*root_ptr)->right_ptr->key)
            {
                return bst_rotate_left(root_ptr);
            }

            // leaning right and then left
            else if (balance < -1 && key > (*root_ptr)->right_ptr->key)
            {
                (*root_ptr)->right_ptr = bst_rotate_right(&(*root_ptr)->left_ptr);
                return bst_rotate_left(root_ptr);
            }
        }

        else
        {
            (*root_ptr)->data = data;
        }
    }

    return (*root_ptr);
}

void bst_replace_by_right_most(bst_node_ptr target, bst_node_ptr *root_ptr)
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
        bst_replace_by_right_most(target, &((*root_ptr)->right_ptr));
    }
}

bst_node_ptr bst_delete(bst_node_ptr *root_ptr, char* key)
{
    if (*root_ptr)
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
                bst_replace_by_right_most((*root_ptr), &((*root_ptr)->left_ptr));
            }
        }
    }
    else
    {
        (*root_ptr)->height = 1 + max(bst_height(&(*root_ptr)->left_ptr), bst_height(&(*root_ptr)->right_ptr));

        int balance = bst_calculate_balance(root_ptr);
        if (balance > 1 && bst_calculate_balance(&(*root_ptr)->left_ptr) >= 0)
        {
            return bst_rotate_right(root_ptr);
        }

        else if (balance > 1 && bst_calculate_balance(&(*root_ptr)->left_ptr) < 0)
        {
            (*root_ptr)->left_ptr = bst_rotate_left(&(*root_ptr)->left_ptr);
            return bst_rotate_right(root_ptr);
        }

        else if (balance < -1 && bst_calculate_balance(&(*root_ptr)->right_ptr) <= 0)
        {
            return bst_rotate_left(root_ptr);
        }

        else if (balance < -1 && bst_calculate_balance(&(*root_ptr)->right_ptr) > 0)
        {
            (*root_ptr)->left_ptr = bst_rotate_right(&(*root_ptr)->left_ptr);
            return bst_rotate_left(root_ptr);
        }
    }

    return (*root_ptr);
}

bst_node_ptr bst_search(bst_node_ptr *root_ptr, char* key)
{
    if ((*root_ptr) == NULL)
    {
        return NULL;
    }

    else
    {
        if (strcmp(key, (*root_ptr)->key) == 0)
        {
            return (*root_ptr);
        }

        else if (strcmp(key, (*root_ptr)->key) > 0)
        {
            return bst_search(&(*root_ptr)->right_ptr, key);
        }

        else
        {
            return bst_search(&(*root_ptr)->left_ptr, key);
        }
    }
}

// int init_string(string *str)
// {
//     if ((str->value = (char*)malloc(10 * sizeof(char))) == NULL)
//     {
//         return INTERNAL_ERROR;
//     }
//     str->value[0] = '\0';
//     str->len = 0;
//     str->allocated_len = 10;
//     return SYNTAX_OK;
// }

// void dispose_string(string *str)
// {
//     free(str->value);
//     str->value = NULL;
//     str->len = str->allocated_len = 0;
// }

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

void symtable_insert_var(symtable_t* table, char* key, var_data_t* data)
{
    var_data_t *data_ptr;
    if ((data_ptr = (var_data_t*)malloc(sizeof(var_data_t))) == NULL)
    {
        return;
    }
    data_ptr->data_type = data->data_type;
    bst_insert(&(table->root), key, data_ptr, node_data_type_var);
}

void symtable_insert_function(symtable_t* table, char* key, function_data_t* data)
{
    function_data_t *data_ptr;
    if ((data_ptr = (function_data_t*)malloc(sizeof(function_data_t))) == NULL)
    {
        return;
    }

    data_ptr->return_data_type = data->return_data_type;
    data_ptr->defined = data->defined;
    data_ptr->param_names = data->param_names;
    data_ptr->param_len = data->param_len;
    data_ptr->params_types = data->params_types;
    data_ptr->params_identifiers = data->params_identifiers;

    bst_insert(&(table->root), key, data_ptr, node_data_type_function);
}


void symtable_delete(symtable_t* table, char* key)
{
    bst_delete(&(table)->root, key);
    //table->size--;
}

bst_node_ptr symtable_search(symtable_t* table, char* key)
{
    return bst_search(&(table)->root, key);
}

void symtable_dispose(symtable_t* table)
{
    bst_dispose(&(table)->root);
}
