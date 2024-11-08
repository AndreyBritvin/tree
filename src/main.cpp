#include "my_tree.h"
#include <stdlib.h>
#include "my_log.h"
#include <time.h>

int main()
{
    enable_logging("tree_dump/dump.html");
    srand(time(NULL));

    INIT_TREE(node_root, 50, NULL, NULL);

    for (int i = 0; i < 10; i++)
    {
        node_root = add_node(node_root, rand() % 100);
        TREE_DUMP(node_root);
    }

    print_tree(node_root);
    printf("\n");

    tree_dtor(node_root);

    disable_logging();

    return 0;
}
