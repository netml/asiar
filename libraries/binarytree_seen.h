#include <stdlib.h>

#ifndef MY_HEADER_B_H_SEEN
#define MY_HEADER_B_H_SEEN

typedef struct binarytree_seen {
    int type;
    int vp;
    int ip;
    struct binarytree_seen *left;
    struct binarytree_seen *right;
} binarytree_seen;

binarytree_seen* insert_tree_seen(binarytree_seen *node, int type, int vp, int ip);
binarytree_seen* find_tree_seen(binarytree_seen *node, int type, int vp, int ip);
void delete_tree_seen(binarytree_seen* node);

#endif
