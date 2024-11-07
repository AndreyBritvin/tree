#include "my_tree.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

node_t* new_node(tree_val_t data, node_t* left_node, node_t* right_node)
{
    node_t* node = (node_t*) calloc(1, sizeof(node_t));

    node->data  =       data;
    node->left  =  left_node;
    node->right = right_node;

    return node;
}

err_code_t tree_dtor(node_t* tree)
{
    if (tree->left  != NULL) tree_dtor(tree->left);
    if (tree->right != NULL) tree_dtor(tree->right);

    free(tree);

    return OK;
}

node_t* add_node(node_t* tree, tree_val_t data_to_add)
{
    if (tree == NULL) return new_node(data_to_add, NULL, NULL);
    if (tree->data < data_to_add) tree->right = add_node(tree->right, data_to_add);
    else                          tree->left  = add_node(tree->left,  data_to_add);

    return tree;
}

err_code_t print_tree(node_t* tree)
{
    printf("(");
    if (tree->left  != NULL) print_tree(tree->left);
    printf("%d", tree->data);
    if (tree->right != NULL) print_tree(tree->right);
    printf(")");

    return OK;
}

#define DOT_(...) fprintf(dot_file, __VA_ARGS__);

size_t tree_dump(node_t* tree)
{
    assert(tree != NULL);

    static size_t graphs_counter = 0;

    const char *txt_filename = "tree_dump/dot/%lu.dot";
    const char *base_command = "dot tree_dump/dot/%lu.dot -o tree_dump/img/%lu.png -Tpng";
    char *implementation     = (char *) calloc(strlen(base_command) + 20, sizeof(char));
    char *txt_full_filename  = (char *) calloc(strlen(txt_filename) + 20, sizeof(char));

    sprintf(implementation, base_command, graphs_counter, graphs_counter);
    sprintf(txt_full_filename, txt_filename, graphs_counter);

    printf("File to create:  %s\n", txt_full_filename);
    printf("Command to call: %s\n", implementation);
    tree_dot(txt_full_filename, tree);
    system(implementation);

    free(implementation);
    free(txt_full_filename);

    graphs_counter++;

    return graphs_counter;
}

err_code_t tree_dot(char *filename, node_t *tree)
{
    assert(filename != NULL);

    FILE * SAFE_OPEN_FILE(dot_file, filename, "w");

    DOT_("digraph{\n"
         "rankdir = TB;\n");

    make_tree_node(dot_file, tree);

    DOT_("}\n");

    fclose(dot_file);

    return OK;
}

err_code_t make_tree_node(FILE* dot_file, node_t* tree)
{
    assert(dot_file != NULL);
    assert(tree     != NULL);

    DOT_("tree%p[shape = record; label = \"{addr = %p |data = %d| {<l%p> left | <r%p> right}}\"];\n",
                                tree, tree, tree->data, tree, tree);

    if (tree->left != NULL)
    {
        make_tree_node(dot_file, tree->left);
        DOT_("tree%p:<l%p>->tree%p;\n", tree, tree, tree->left)
    }
    if (tree->right != NULL)
    {
        make_tree_node(dot_file, tree->right);
        DOT_("tree%p:<r%p>->tree%p;\n", tree, tree, tree->right)
    }

    return OK;
}
#undef DOT_
