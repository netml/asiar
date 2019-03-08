#include "network.h"

#ifndef BINARYTREE_DUPLE
#define BINARYTREE_DUPLE

typedef struct duple {
    uint32_t data1;
    uint32_t data2;
    int data1_pos;
    int data2_pos;
    double completeness;
    double reference;
} duple;

typedef struct binarytree_duple {
    duple data;
    struct binarytree_duple *left;
    struct binarytree_duple *right;
} binarytree_duple;

void duple_completeness_merge(duple* arr, int l, int m, int r);
void duple_completeness_mergesort(duple* arr, int l, int r);
void duple_reference_merge(duple* arr, int l, int m, int r);
void duple_reference_mergesort(duple* arr, int l, int r);
binarytree_duple* insert_tree_with_duple(binarytree_duple *node, duple data);
binarytree_duple* insert_tree_with_ip_duple(binarytree_duple *node, uint32_t data1, uint32_t data2, int data1_pos, int data2_pos, double completeness, double reference);
binarytree_duple* find_tree_duple(binarytree_duple *node, uint32_t data1, uint32_t data2);
void print_inorder_duple(binarytree_duple *node);
void print_preorder_duple(binarytree_duple *node);
void print_postorder_duple(binarytree_duple *node);
int size_tree_duple(binarytree_duple* node);
void convert_duple_tree_to_array(binarytree_duple *node, duple* array, int* i);
void delete_tree_duple(binarytree_duple* node);
bool greater_than_with_ip_duple(uint32_t data11, uint32_t data12, uint32_t data21, uint32_t data22);
bool less_than_with_ip_duple(uint32_t data11, uint32_t data12, uint32_t data21, uint32_t data22);
bool greater_than_duple(duple data1, duple data2);
bool less_than_duple(duple data1, duple data2);

#endif
