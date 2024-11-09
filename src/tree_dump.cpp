#include "my_tree.h"
#include <assert.h>
#include "my_log.h"
#include <string.h>

static err_code_t tree_dot          (char *filename, my_tree_t* tree, node_t* curr_node, const char * curr_action);
static err_code_t make_tree_node    (FILE* dot_file, my_tree_t* tree, node_t* curr_node, const char * curr_action);
static int        make_node         (FILE* dot_file, node_t* curr_node, node_t* node_to_select, const char * curr_action);
static size_t     generate_dot_file (my_tree_t* tree, node_t* curr_node, const char * curr_action);
static err_code_t generate_root_info(FILE* dot_file, my_tree_t* tree);

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

    generate_root_info(dot_file, tree);
    make_tree_node(dot_file, tree, curr_node, curr_action);

    DOT_("}\n");

    fclose(dot_file);

    return OK;
}

err_code_t generate_root_info(FILE* dot_file, my_tree_t* tree)
{
    DOT_("root[shape = Mrecord; label = \"{Tree at %p|Tree size = %zd|<r1> Root at %p}\" color = chocolate]\n", tree, tree->size, tree->root);
    DOT_("root->tree%p [color = brown];\n", tree->root);

    return OK;
}

err_code_t make_tree_node(FILE* dot_file, my_tree_t* tree, node_t* curr_node, const char * curr_action)
{
    assert(tree);

    make_node(dot_file, tree->root,  curr_node, curr_action);

    return OK;
}

err_code_t make_node(FILE* dot_file, node_t* curr_node, node_t* node_to_select, const char * curr_action)
{
    assert(curr_node);
    assert(dot_file);
    assert(curr_action);

    const char *node_color = "saddlebrown";
    if (curr_node == node_to_select)
    {
        node_color = "saddlebrown, style=\"filled\", fillcolor = \"#d9b986\"";
        DOT_("info_node[shape = Mrecord, style = \"filled\", fillcolor = yellow, label = \"%s\", constraint = false];\n", curr_action);
        DOT_("info_node->tree%p;", curr_node);
    }

    DOT_("tree%p[shape = record; label = \"{addr = %p |data = %d| {<l%p> left | <r%p> right}}\"; color = %s];\n",
                                curr_node, curr_node, curr_node->data, curr_node, curr_node, node_color);

    if (curr_node->left != NULL)
    {
        make_node(dot_file, curr_node->left, node_to_select, curr_action);
        DOT_("tree%p:<l%p>:s->tree%p;\n", curr_node, curr_node, curr_node->left)
    }
    if (curr_node->right != NULL)
    {
        make_node(dot_file, curr_node->right, node_to_select, curr_action);
        DOT_("tree%p:<r%p:s>->tree%p;\n", curr_node, curr_node, curr_node->right)
    }

    return OK;
}
#undef DOT_
