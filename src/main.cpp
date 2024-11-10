#include "my_tree.h"
#include <stdlib.h>
#include "my_log.h"
#include <time.h>

int main()
{
    enable_logging("tree_dump/dump.html");
    paste_instruction();
    srand(time(NULL));

    INIT_TREE(test_tree);

    for (int i = 0; i < 10; i++)
    {
        add_node(&test_tree, rand() % 100);
        TREE_DUMP(&test_tree, test_tree.root, "Inserting to this root");
    }

    print_tree(test_tree.root);
    printf("\n");

    tree_dtor(test_tree.root);

    disable_logging();

    return 0;
}
