#ifndef MY_TREE_H_
#define MY_TREE_H_
#include <stdlib.h>
#include "utils.h"
#include <stdio.h>

typedef int tree_val_t;

struct node_t
{
    tree_val_t  data;
    node_t *   right;
    node_t *    left;
    node_t *  parent;
};

node_t* new_node(tree_val_t data, node_t* left_node, node_t* right_node);

err_code_t tree_dtor (node_t* tree);
err_code_t print_tree(node_t* tree);
node_t* add_node(node_t* tree, tree_val_t data_to_add);

size_t tree_dump(node_t* tree);
err_code_t tree_dot(char *filename, node_t *tree);
int make_tree_node(FILE* dot_file, node_t* tree);

#endif // MY_TREE_H_
