#ifndef MY_TREE_H_
#define MY_TREE_H_
#include <stdlib.h>
#include "utils.h"
#include <stdio.h>
#include <stdarg.h>

#ifndef NDEBUG
    #define TREE_DUMP(tree_to_dump, curr_node, ...) \
        tree_dump(tree_to_dump, curr_node, __func__, __FILE__, __LINE__, __VA_ARGS__);
    #define DEBUG_INFO , const char * funcname, const char * filename, const int fileline
    #define INIT_TREE(tree_name) my_tree_t tree_name = {};  tree_ctor(&tree_name);                     \
                                                            tree_name.rootname = #tree_name;           \
                                                            tree_name.funcname = __func__;             \
                                                            tree_name.filename = __FILE__;             \
                                                            tree_name.codeline = __LINE__;
#else
    #define TREE_DUMP(tree_to_dump, curr_node, action) \
        tree_dump(tree_to_dump, curr_node, action);
    #define DEBUG_INFO
    #define INIT_TREE(tree_name) my_tree_t tree_name = {};  tree_ctor(&tree_name);
#endif // NDEBUG

typedef int tree_val_t;

struct node_t
{
    tree_val_t  data;
    node_t *   right;
    node_t *    left;
    node_t *  parent;
};

struct my_tree_t
{
    node_t* root;
    size_t  size;
#ifndef NDEBUG
    const char * filename;
    const char * funcname;
    const char * rootname;
    int          codeline;
#endif
};

node_t* new_node(my_tree_t tree, tree_val_t data, node_t* left_node, node_t* right_node);

err_code_t tree_ctor(my_tree_t* tree);
err_code_t tree_dtor (node_t* tree);

err_code_t print_tree(node_t* tree);
err_code_t add_node(my_tree_t *tree, tree_val_t data_to_add);

err_code_t paste_instruction();
err_code_t tree_dump(my_tree_t* tree, node_t* curr_node DEBUG_INFO, const char * curr_action, ...);
err_code_t verificator(my_tree_t* tree, node_t* node, size_t recurs_level);

#endif // MY_TREE_H_
