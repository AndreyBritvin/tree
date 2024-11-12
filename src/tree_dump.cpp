#include "my_tree.h"
#include <assert.h>
#include "my_log.h"
#include <string.h>
#include <stdarg.h>
#include <dump_colors.h>

static err_code_t tree_dot          (char *filename, my_tree_t* tree, node_t* curr_node, const char * curr_action, va_list args);
static err_code_t make_tree_node    (FILE* dot_file, my_tree_t* tree, node_t* curr_node, const char * curr_action, va_list args);
static int        make_node         (FILE* dot_file, node_t* curr_node, node_t* node_to_select, const char * curr_action, size_t recurs_level, size_t max_recurs);
static size_t     generate_dot_file (my_tree_t* tree, node_t* curr_node, const char * curr_action, va_list args);
static err_code_t generate_root_info(FILE* dot_file, my_tree_t* tree);
static err_code_t init_graph        (FILE* dot_file);

#define DOT_(...) fprintf(dot_file, __VA_ARGS__);

err_code_t tree_dump(my_tree_t* tree, node_t* curr_node DEBUG_INFO, const char * curr_action, ...)
{
    assert(tree);
    assert(funcname);
    assert(filename);

    va_list args;
    va_start(args, curr_action);
    LOG("<pre>\n"
        "Tree dump called from %s() in %s:%d for tree %p\n\n",
                        funcname, filename, fileline, tree);
#ifndef NDEBUG
    LOG("Rootname = %s at [%p] created in %s() %s:%d\n\n",
         tree->rootname, tree, tree->funcname, tree->filename, tree->codeline);
#endif
    size_t tree_num = generate_dot_file(tree, curr_node, curr_action, args) - 1;
    LOG("<img src=img/%zd.png>\n", tree_num);
    LOG("End printing tree -----------------------------------------------------\n"
        "</pre>");

    va_end(args);

    return OK;
}

size_t generate_dot_file(my_tree_t* tree, node_t* curr_node, const char * curr_action, va_list args)
{
    assert(tree != NULL);

    static size_t graphs_counter = 0;

    const char *txt_filename = "tree_dump/dot/%lu.dot";
    const char *base_command = "dot tree_dump/dot/%lu.dot -o tree_dump/img/%lu.png -Tpng";
    char *implementation     = (char *) calloc(strlen(base_command) + 20, sizeof(char));
    char *txt_full_filename  = (char *) calloc(strlen(txt_filename) + 20, sizeof(char));

    sprintf(implementation, base_command, graphs_counter, graphs_counter);
    sprintf(txt_full_filename, txt_filename, graphs_counter);

    // printf("File to create:  %s\n", txt_full_filename);
    // printf("Command to call: %s\n", implementation);
    tree_dot(txt_full_filename, tree, curr_node, curr_action, args);
    system(implementation);

    free(implementation);
    free(txt_full_filename);

    graphs_counter++;

    return graphs_counter;
}

err_code_t tree_dot(char *filename, my_tree_t* tree, node_t* curr_node, const char * curr_action, va_list args)
{
    assert(filename != NULL);

    FILE * SAFE_OPEN_FILE(dot_file, filename, "w");

    init_graph(dot_file);

    generate_root_info(dot_file, tree);
    make_tree_node(dot_file, tree, curr_node, curr_action, args);

    DOT_("}\n");

    fclose(dot_file);

    return OK;
}

static err_code_t init_graph(FILE* dot_file)
{
    DOT_("digraph{\n"
         "rankdir = TB;\n"
         "bgcolor=" BACKGROUND_COLOR ";");

    return OK;
}

err_code_t generate_root_info(FILE* dot_file, my_tree_t* tree)
{
    DOT_("root[shape = Mrecord; label = \"{Tree at %p|Tree size = %zd|<r1> Root at %p}\" color = " ROOT_SHAPE_COLOR "]\n", tree, tree->size, tree->root);
    DOT_("root->tree%p [color = " ROOT_INFO_EDGE_COLOR "];\n", tree->root);

    return OK;
}

err_code_t make_tree_node(FILE* dot_file, my_tree_t* tree, node_t* curr_node, const char * curr_action, va_list args)
{
    assert(tree);

    char *formatted_action = (char *) calloc(strlen(curr_action) + 100, sizeof(char));
    vsprintf(formatted_action, curr_action, args);

    make_node(dot_file, tree->root, curr_node, formatted_action, 0, tree->size);
    free(formatted_action);

    return OK;
}

err_code_t make_node(FILE* dot_file, node_t* curr_node, node_t* node_to_select, const char * curr_action, size_t recurs_level, size_t max_recurs)
{
    // assert(curr_node);
    assert(dot_file);
    assert(curr_action);

    if (recurs_level > max_recurs)
    {
        return ERROR_TREE_LOOPED;
    }

    const char *fill_color = "";
    const char *shape_color = "";
    if (curr_node == node_to_select)
    {
        // shape_color = "shape";
        shape_color = ", color = \"" CURRENT_NODE_SHAPE_COLOR "\"" "; penwidth=3;"; // #d9b986
        DOT_("info_node[shape = Mrecord, style = \"filled\", fillcolor = " CURRENT_NODE_COLOR ", label = \"");
        DOT_("%s", curr_action);
        DOT_("\", constraint = false];\n");
        DOT_("info_node->tree%p;", curr_node);
    }
    if (curr_node->left == NULL && curr_node->right == NULL)
    {
        fill_color = ENDING_LEAVES_TREE;
    }
    else
    {
        fill_color = DEFAULT_SHAPE_COLOR;
    }

    DOT_("tree%p[shape = record;"  //penwidth = 2.5;"
         "label = \"{<p> parent = %p|addr = %p |data = %d| {<l> left %p | <r> right %p}}\"; style=\"filled\","
         "fillcolor = \"%s\" %s];\n",
                                curr_node, curr_node->parent, curr_node, curr_node->data, curr_node->left,
                                                                curr_node->right, fill_color, shape_color);

    const char* edge_color = DEFAULT_EDGE_COLOR;

    if (curr_node->left != NULL)
    {
        make_node(dot_file, curr_node->left, node_to_select, curr_action, recurs_level + 1, max_recurs);

        if (curr_node->left->parent != curr_node)
        {
            edge_color = BAD_EDGE_COLOR;
            if (curr_node->left->parent == NULL) DOT_("tree%p:<p>:n->NULL_PTR [color=%s];\n", curr_node->left, edge_color)
            else                                 DOT_("tree%p:<p>:n->tree%p [color=%s];\n", curr_node->left->parent, curr_node, edge_color)
        }
        DOT_("tree%p:<l>:s->tree%p [color=%s];\n", curr_node, curr_node->left, edge_color)
    }

    edge_color = DEFAULT_EDGE_COLOR;
    if (curr_node->right != NULL)
    {
        make_node(dot_file, curr_node->right, node_to_select, curr_action, recurs_level + 1, max_recurs);
        if (curr_node->right->parent != curr_node)
        {
            edge_color = BAD_EDGE_COLOR;
            if (curr_node->right->parent == NULL) DOT_("tree%p:<p>:n->NULL_PTR [color=%s];\n", curr_node->right, edge_color)
            else DOT_("tree%p:<p>:n->tree%p [color=%s];\n", curr_node->right->parent, curr_node, edge_color)
        }
        DOT_("tree%p:<r>:s->tree%p [color = %s];\n", curr_node, curr_node->right, edge_color)
    }

    return OK;
}

err_code_t paste_instruction()
{

    const char *txt_filename = "tree_dump/dot/instruction.dot";
    const char *base_command = "dot tree_dump/dot/instruction.dot -o tree_dump/img/instruction.png -Tpng";

    FILE * SAFE_OPEN_FILE(dot_file, txt_filename, "w");
    // printf("File to create:  %s\n", txt_full_filename);
    // printf("Command to call: %s\n", implementation);
    init_graph(dot_file);
    LOG("<pre>\n"
        "This is example for tree dump.\n"
        "<img src=img/instruction.png>"
        "</pre>");

    DOT_("test_tree1 [shape = record; label = \"{parent=addr|addr = node address |data = tree.data| {<l1> left subtree | <r1> right subtree}}\"; fillcolor = \"" DEFAULT_SHAPE_COLOR "\", style=filled];\n");
    DOT_("test_tree2 [shape = record; label = \"{parent=addr|addr = node address |data = tree.data| {<l1> left subtree | <r1> right subtree}}\"; penwidth=2.5; fillcolor = \"" ENDING_LEAVES_TREE "\" , style=filled, color = " CURRENT_NODE_SHAPE_COLOR "];\n");
    DOT_("root[shape = Mrecord; label = \"{Tree at tree_addr|Tree size = ...|<r1> Root at ADDR}\" color = \"" ROOT_SHAPE_COLOR "\"]\n");
    DOT_("info_to_color1[shape=Mrecord, label=\"This rounded form indicates that its additional information\"];\n")
    DOT_("info_to_color2[shape=Mrecord, label=\"{" CURRENT_NODE_COLOR " color means current node| " ENDING_LEAVES_TREE " color means ending of tree}\", style=filled, fillcolor=" CURRENT_NODE_COLOR "];\n");
    DOT_("root->test_tree1 [color=" ROOT_INFO_EDGE_COLOR "];")
    DOT_("test_tree1->test_tree2 [color = " DEFAULT_EDGE_COLOR "];\n")
    DOT_("info_to_color1->root; info_to_color2->test_tree2;\n")
    DOT_("}");
    fclose(dot_file);

    system(base_command);

    return OK;
}
#undef DOT_
