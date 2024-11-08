#ifndef MY_TREE_H_
#define MY_TREE_H_
#include <stdlib.h>
#include "utils.h"
#include <stdio.h>

#define TREE_DUMP(tree_to_dump) tree_dump(tree_to_dump, __func__, __FILE__, __LINE__);

#ifndef NDEBUG
    #define INIT_TREE(tree_name, value, left, right) node_t* tree_name = new_node(value, left, right);  \
                                                            tree_name->rootname = #tree_name;           \
                                                            tree_name->funcname = __func__;             \
                                                            tree_name->filename = __FILE__;             \
                                                            tree_name->codeline = __LINE__;
#else
    #define INIT_TREE(tree_name) node_t* tree_name = new_node();
#endif // NDEBUG

typedef int tree_val_t;

struct node_t
{
    tree_val_t  data;
    node_t *   right;
    node_t *    left;
    node_t *  parent;
#ifndef NDEBUG
    const char * filename;
    const char * funcname;
    const char * rootname;
    int          codeline;
#endif
};

node_t* new_node(tree_val_t data, node_t* left_node, node_t* right_node);

err_code_t tree_dtor (node_t* tree);
err_code_t print_tree(node_t* tree);
node_t* add_node(node_t* tree, tree_val_t data_to_add);

err_code_t tree_dump(node_t* tree, const char * funcname, const char * filename, const int fileline);
err_code_t verificator(node_t* tree, size_t recurs_level);

#endif // MY_TREE_H_
