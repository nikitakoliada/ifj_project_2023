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

// Helper Functions
int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(bst_node_ptr N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int getBalance(bst_node_ptr N) {
    if (N == NULL)
        return 0;
    return height(N->left_ptr) - height(N->right_ptr);
}

bst_node_ptr rightRotate(bst_node_ptr root_ptr) {
    bst_node_ptr tmp = root_ptr->left_ptr;
    bst_node_ptr tmp2 = tmp->right_ptr;

    // Perform rotation
    tmp->right_ptr = root_ptr;
    root_ptr->left_ptr = tmp2;

    // Update heights
    root_ptr->height = max(height(root_ptr->left_ptr), height(root_ptr->right_ptr)) + 1;
    tmp->height = max(height(tmp->left_ptr), height(tmp->right_ptr)) + 1;

    // Return new root
    return tmp;
}

bst_node_ptr leftRotate(bst_node_ptr root_ptr) {
    bst_node_ptr tmp1 = root_ptr->right_ptr;
    bst_node_ptr tmp2 = tmp1->left_ptr;

    // Perform rotation
    tmp1->left_ptr = root_ptr;
    root_ptr->right_ptr = tmp2;

    // Update heights
    root_ptr->height = max(height(root_ptr->left_ptr), height(root_ptr->right_ptr)) + 1;
    tmp1->height = max(height(tmp1->left_ptr), height(tmp1->right_ptr)) + 1;

    // Return new root
    return tmp1;
}

bst_node_ptr new_bst_node(char* key, void* data, node_data_type_t data_type) {
    bst_node_ptr node = (bst_node_ptr)malloc(sizeof(struct bst_node));
    if (node) {
        node->key = strdup(key);
        node->data = data;
        node->data_type = data_type;
        node->left_ptr = NULL;
        node->right_ptr = NULL;
        node->height = 1;
    }
    return node;
}

void bst_init(bst_node_ptr* root) {
    *root = NULL;
}

bst_node_ptr bst_insert(bst_node_ptr node, char* key, void* data, node_data_type_t data_type) {
    if (node == NULL)
        return(new_bst_node(key, data, data_type));

    if (strcmp(key, node->key) < 0)
        node->left_ptr = bst_insert(node->left_ptr, key, data, data_type);
    else if (strcmp(key, node->key) > 0)
        node->right_ptr = bst_insert(node->right_ptr, key, data, data_type);
    else 
        return node;

    node->height = 1 + max(height(node->left_ptr), height(node->right_ptr));

    int balance = getBalance(node);


    if (balance > 1 && strcmp(key, node->left_ptr->key) < 0)
        return rightRotate(node);

    if (balance < -1 && strcmp(key, node->right_ptr->key) > 0)
        return leftRotate(node);

    if (balance > 1 && strcmp(key, node->left_ptr->key) > 0) {
        node->left_ptr = leftRotate(node->left_ptr);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(key, node->right_ptr->key) < 0) {
        node->right_ptr = rightRotate(node->right_ptr);
        return leftRotate(node);
    }

    return node;
}

bst_node_ptr bst_search(bst_node_ptr root, char* key) {
    if (root == NULL || strcmp(root->key, key) == 0)
        return root;

    if (strcmp(root->key, key) < 0)
        return bst_search(root->right_ptr, key);

    return bst_search(root->left_ptr, key);
}


void bst_dispose(bst_node_ptr root) {
    if (root != NULL) {
        bst_dispose(root->left_ptr);
        bst_dispose(root->right_ptr);
        free(root->key); 
        free(root); 
    }
}

bst_node_ptr minValueNode(bst_node_ptr node) {
    bst_node_ptr current = node;

    while (current->left_ptr != NULL)
        current = current->left_ptr;

    return current;
}

bst_node_ptr bst_delete(bst_node_ptr root, char* key) {
    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the root's key, then it lies in left subtree
    if (strcmp(key, root->key) < 0)
        root->left_ptr = bst_delete(root->left_ptr, key);

    // If the key to be deleted is greater than the root's key, then it lies in right subtree
    else if (strcmp(key, root->key) > 0)
        root->right_ptr = bst_delete(root->right_ptr, key);

    // if key is same as root's key, then This is the node to be deleted
    else {
        // node with only one child or no child
        if ((root->left_ptr == NULL) || (root->right_ptr == NULL)) {
            bst_node_ptr temp = root->left_ptr ? root->left_ptr : root->right_ptr;

            // No child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else // One child case
                *root = *temp; // Copy the contents of the non-empty child

            free(temp);
        } else {
            // node with two children: Get the inorder successor (smallest in the right subtree)
            bst_node_ptr temp = minValueNode(root->right_ptr);

            // Copy the inorder successor's data to this node
            root->key = strdup(temp->key);
            root->data = temp->data; // might need deep copy depending on data structure

            // Delete the inorder successor
            root->right_ptr = bst_delete(root->right_ptr, temp->key);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left_ptr), height(root->right_ptr));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->left_ptr) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left_ptr) < 0) {
        root->left_ptr = leftRotate(root->left_ptr);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right_ptr) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right_ptr) > 0) {
        root->right_ptr = rightRotate(root->right_ptr);
        return leftRotate(root);
    }

    return root;
}


bst_node_ptr symtable_search(symtable_t* table, char* key) {
    return bst_search(table->root, key);
}

void symtable_dispose(symtable_t* table) {
    bst_dispose(table->root);
}

void symtable_init(symtable_t* table) {
    table->root = NULL;
}

void symtable_insert_var(symtable_t* table, char* key, var_data_t* data) {
    table->root = bst_insert(table->root, key, data, node_data_type_var);
}

void symtable_insert_function(symtable_t* table, char* key, function_data_t* data) {
    table->root = bst_insert(table->root, key, data, node_data_type_function);
}

void symtable_delete(symtable_t* table, char* key) {
    table->root = bst_delete(table->root, key);
}
