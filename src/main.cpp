#include "my_tree.h"
#include "stdlib.h"

int main()
{
    node_t* node_4 = new_node(30, NULL, NULL);
    node_t* node_3 = new_node(60, NULL, NULL);
    node_t* node_2 = new_node(40, node_4, NULL);
    node_t* node_1 = new_node(50, node_2, node_3);
    printf("Node_1 = %p\n"
           "Node_2 = %p\n"
           "Node_3 = %p\n"
           "Node_4 = %p\n", node_1, node_2, node_3, node_4);
    add_node(node_1, 55);
    tree_dump(node_1);
    print_tree(node_1);

    tree_dtor(node_1);

    return 0;
}
