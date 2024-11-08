#include "my_tree.h"
#include "stdlib.h"

#include <time.h>

int main()
{
    srand(time(NULL));

    node_t* node_root = NULL;

    for (int i = 0; i < 10; i++)
    {
        node_root = add_node(node_root, rand() % 100);
    }

    tree_dump(node_root);
    print_tree(node_root);

    tree_dtor(node_root);

    return 0;
}
