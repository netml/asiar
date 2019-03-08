#include "linkedlist.h"
#include "network.h"

#ifndef MY_HEADER_B_H
#define MY_HEADER_B_H

typedef struct mask {
    uint32_t data;
    int mask;
} mask;

typedef struct binarytree {
    uint32_t data;
    int mask;
    linkedlist *list;
    struct binarytree *left;
    struct binarytree *right;
} binarytree;

binarytree* find_min(binarytree *node);
binarytree* find_max(binarytree *node);
binarytree* insert_tree(binarytree *node, uint32_t data, int mask);
binarytree* delete_node(binarytree *node, uint32_t data);
binarytree* find_tree(binarytree *node, uint32_t data);
void print_inorder(binarytree *node);
void change_inorder(binarytree *node, int i, int j);
void print_preorder(binarytree *node);
void print_postorder(binarytree *node);
int size_tree(binarytree* node);
void convert_tree_to_array_mask(binarytree *node, mask* array, int* i);
void convert_tree_to_array(binarytree *node, uint32_t* array, int* i);
void delete_tree(binarytree* node);

#endif
