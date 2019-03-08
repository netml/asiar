#include <stdint.h>
#include <stdlib.h>

#ifndef MY_HEADER_B_H_IP
#define MY_HEADER_B_H_IP

typedef struct binarytree_ip {
    uint32_t data;
    struct binarytree_ip *left;
    struct binarytree_ip *right;
} binarytree_ip;

binarytree_ip* find_min_ip(binarytree_ip *node);
binarytree_ip* find_max_ip(binarytree_ip *node);
binarytree_ip* insert_tree_ip(binarytree_ip *node, uint32_t data);
binarytree_ip* delete_ip(binarytree_ip *node, uint32_t data);
binarytree_ip* find_tree_ip(binarytree_ip *node, uint32_t data);
void print_inorder_ip(binarytree_ip *node);
void print_preorder_ip(binarytree_ip *node);
void print_postorder_ip(binarytree_ip *node);
int size_tree_ip(binarytree_ip* node);
void convert_tree_to_array_ip(binarytree_ip *node, uint32_t* array, int* i);
void delete_tree_ip(binarytree_ip* node);

#endif
