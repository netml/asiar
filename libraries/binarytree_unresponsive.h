#include "network.h"

#ifndef BINARYTREE_UNRESPONSIVE
#define BINARYTREE_UNRESPONSIVE

typedef struct binarytree_unresponsive_inner {
    uint32_t data;
    uint32_t ip_start;
    int length;
    struct binarytree_unresponsive_inner *left;
    struct binarytree_unresponsive_inner *right;
} binarytree_unresponsive_inner;

typedef struct binarytree_unresponsive {
    uint32_t data;
    binarytree_unresponsive_inner* inner;
    struct binarytree_unresponsive *left;
    struct binarytree_unresponsive *right;
} binarytree_unresponsive;

//int partition_alias_reference(alias* a, int l, int r);
//void quicksort_alias_reference(alias* a, int l, int r);
//int partition_alias_completeness(alias* a, int l, int r);
//void quicksort_alias_completeness(alias* a, int l, int r);
binarytree_unresponsive* insert_binarytree_unresponsive(binarytree_unresponsive *node, uint32_t data);
binarytree_unresponsive* find_binarytree_unresponsive(binarytree_unresponsive *node, uint32_t data);

binarytree_unresponsive_inner* insert_binarytree_unresponsive_inner(binarytree_unresponsive_inner *node, uint32_t data, uint32_t ip_start, int length);
binarytree_unresponsive_inner* find_binarytree_unresponsive_inner(binarytree_unresponsive_inner *node, uint32_t data);

void print_binarytree_unresponsive_inner(binarytree_unresponsive_inner *node);
void print_binarytree_unresponsive(binarytree_unresponsive *node);
//binarytree_alias* insert_tree_with_ip(binarytree_alias *node, uint32_t data1, uint32_t data2, uint32_t data3);
//void print_inorder_alias(binarytree_alias *node);
//void print_preorder_alias(binarytree_alias *node);
//void print_postorder_alias(binarytree_alias *node);
int size_tree_unresponsive(binarytree_unresponsive* node);
//void convert_alias_tree_to_array(binarytree_alias *node, alias* array, int* i);
void delete_tree_unresponsive(binarytree_unresponsive* node);
void delete_tree_unresponsive_inner(binarytree_unresponsive_inner* node);
//bool greater_than_with_ip(uint32_t data11, uint32_t data12, uint32_t data13, uint32_t data21, uint32_t data22, uint32_t data23);
//bool less_than_with_ip(uint32_t data11, uint32_t data12, uint32_t data13, uint32_t data21, uint32_t data22, uint32_t data23);
bool greater_than_binarytree_unresponsive(uint32_t data1, uint32_t data2);
bool less_than_binarytree_unresponsive(uint32_t data1, uint32_t data2);

#endif
