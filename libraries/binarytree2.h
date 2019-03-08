#include <stdint.h>
#include "network.h"

#ifndef MY_HEADER_B_H_2
#define MY_HEADER_B_H_2

typedef struct binarytree2 {
    int data;
    int occurence;
    struct binarytree2 *left;
    struct binarytree2 *right;
} binarytree2;

binarytree2* insert_tree_2(binarytree2 *node, int data);
binarytree2* insert_tree_2_with_occurence(binarytree2 *node, int data, int occurence);
void print_inorder_2(binarytree2 *node);
binarytree2* find_tree_2(binarytree2 *node, int data);
void convert_binarytree2_tree_to_array(binarytree2 *node, int* array, int* i);
int size_tree2(binarytree2* node);
void delete_tree2(binarytree2* node);

#endif
