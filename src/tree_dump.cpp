#include "my_tree.h"
#include <assert.h>
#include "my_log.h"
#include <string.h>

static err_code_t tree_dot         (char *filename, my_tree_t* tree, node_t* curr_node, const char * curr_action);
static size_t     generate_dot_file(my_tree_t* tree, node_t* curr_node, const char * curr_action);
static int        make_tree_node   (FILE* dot_file, node_t* tree);

#define DOT_(...) fprintf(dot_file, __VA_ARGS__);

err_code_t tree_dump(my_tree_t* tree, node_t* curr_node, const char * curr_action DEBUG_INFO)
{
    assert(tree);
    assert(funcname);
    assert(filename);

    LOG("<pre>\n"
        "Tree dump called from %s() in %s:%d for tree %p\n\n",
                        funcname, filename, fileline, tree);
#ifndef NDEBUG
    LOG("Rootname = %s at [%p] created in %s() %s:%d\n\n",
         tree->rootname, tree, tree->funcname, tree->filename, tree->codeline);
#endif

    size_t tree_num = generate_dot_file(tree, curr_node, curr_action) - 1;
    LOG("<img src=img/%zd.png>\n", tree_num);
    LOG("End printing tree -----------------------------------------------------\n"
        "</pre>");

    return OK;
}

size_t generate_dot_file(my_tree_t* tree, node_t* curr_node, const char * curr_action)
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
    tree_dot(txt_full_filename, tree, curr_node, curr_action);
    system(implementation);

    free(implementation);
    free(txt_full_filename);

    graphs_counter++;

    return graphs_counter;
}

err_code_t tree_dot(char *filename, my_tree_t* tree, node_t* curr_node, const char * curr_action)
{
    assert(filename != NULL);

    FILE * SAFE_OPEN_FILE(dot_file, filename, "w");

    DOT_("digraph{\n"
         "rankdir = TB;\n");

    make_tree_node(dot_file, tree->root);

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
        DOT_("tree%p:<l%p>:s->tree%p;\n", tree, tree, tree->left)
    }
    if (tree->right != NULL)
    {
        make_tree_node(dot_file, tree->right);
        DOT_("tree%p:<r%p:s>->tree%p;\n", tree, tree, tree->right)
    }

    return OK;
}
#undef DOT_
