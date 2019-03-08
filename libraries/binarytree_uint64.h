#include <stdint.h>
#include "network.h"

#ifndef MY_HEADER_B_H_64
#define MY_HEADER_B_H_64

typedef struct binarytree_uint64 {
    uint64_t data;
    struct binarytree_uint64 *left;
    struct binarytree_uint64 *right;
} binarytree_uint64;

binarytree_uint64* insert_tree_uint64(binarytree_uint64 *node, uint64_t data);
int size_tree_uint64(binarytree_uint64* node);
void convert_tree_to_array_uint64(binarytree_uint64 *node, uint64_t* array, int* i);

#endif
