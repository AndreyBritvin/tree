#include "my_tree.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "my_log.h"

static node_t* add_node_by_root(my_tree_t* tree, node_t* curr_node, tree_val_t data_to_add);

node_t* new_node(tree_val_t data, node_t* left_node, node_t* right_node)
{
    node_t* node = (node_t*) calloc(1, sizeof(node_t));

    node->data  =       data;
    node->left  =  left_node;
    node->right = right_node;

    return node;
}

err_code_t tree_ctor(my_tree_t* tree)
{
    tree->size = 1;
    tree->root = new_node(0, NULL, NULL);

    return OK;
}

err_code_t tree_dtor(node_t* tree)
{
    assert(tree);

    if (tree->left  != NULL) tree_dtor(tree->left);
    if (tree->right != NULL) tree_dtor(tree->right);

    free(tree);

    return OK;
}

err_code_t add_node(my_tree_t *tree, tree_val_t data_to_add)
{
    add_node_by_root(tree, tree->root, data_to_add);
    tree->size += 1;

    return OK;
}

static node_t* add_node_by_root(my_tree_t* tree, node_t* curr_node, tree_val_t data_to_add)
{
    if (curr_node == NULL) return new_node(data_to_add, NULL, NULL);

    char *string_to_format = "Comparing data node.data = %d with data_to_add = %d";
    char *formatted_string = (char *) calloc(strlen(string_to_format) + 20, sizeof(char));
    sprintf(formatted_string, string_to_format, curr_node->data, data_to_add);
    TREE_DUMP(tree, curr_node, formatted_string);
    free(formatted_string);

    if (curr_node->data < data_to_add) curr_node->right = add_node_by_root(tree, curr_node->right, data_to_add);
    else                               curr_node->left  = add_node_by_root(tree, curr_node->left,  data_to_add);

    return curr_node;
}

err_code_t print_tree(node_t* tree)
{
    if (tree == NULL) return ERROR_TREE_IS_NULL; // TODO: parse this error

    printf("(");
    if (tree->left  != NULL) print_tree(tree->left);
    printf("%d", tree->data);
    if (tree->right != NULL) print_tree(tree->right);
    printf(")");

    return OK;
}

//TODO redo
static node_t* all_nodes[100] = {}; // TODO remove this cringe

err_code_t verificator(node_t* tree, size_t recurs_level)
{
    size_t index_to_append = 0;
    for (size_t i = 0; i < sizeof(all_nodes); i++)
    {
        if (tree == all_nodes[i])
        {
            return ERROR_TREE_LOOPED;
        }
        if (all_nodes[i] == NULL)
        {
            index_to_append = i;
            break;
        }
    }
    all_nodes[index_to_append] = tree;

    err_code_t err_code = OK;
    if (tree->left  != NULL) err_code = verificator(tree->left,  recurs_level + 1);
    if (tree->right != NULL) err_code = verificator(tree->right, recurs_level + 1);

    if (recurs_level == 0)
    {
        for (size_t i = 0; i < sizeof(all_nodes); i++)
        {
            all_nodes[i] = NULL;
        }
    }

    return err_code;
}
