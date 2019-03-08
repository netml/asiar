#include "linkedlist.h"
#include "network.h"

#ifndef BINARYTREE_ALIAS
#define BINARYTREE_ALIAS

typedef struct alias {
    uint32_t data1;
    uint32_t data2;
    uint32_t data3;
    double completeness;
    int reference;
} alias;

typedef struct binarytree_alias {
    alias data;
    struct binarytree_alias *left;
    struct binarytree_alias *right;
} binarytree_alias;

int partition_alias_reference(alias* a, int l, int r);
void quicksort_alias_reference(alias* a, int l, int r);
int partition_alias_completeness(alias* a, int l, int r);
void quicksort_alias_completeness(alias* a, int l, int r);
binarytree_alias* insert_tree_with_alias(binarytree_alias *node, alias data);
binarytree_alias* insert_tree_with_ip(binarytree_alias *node, uint32_t data1, uint32_t data2, uint32_t data3);
binarytree_alias* find_tree_alias(binarytree_alias *node, uint32_t data1, uint32_t data2, uint32_t data3);
void print_inorder_alias(binarytree_alias *node);
void print_preorder_alias(binarytree_alias *node);
void print_postorder_alias(binarytree_alias *node);
int size_tree_alias(binarytree_alias* node);
void convert_alias_tree_to_array(binarytree_alias *node, alias* array, int* i);
void delete_tree_alias(binarytree_alias* node);
bool greater_than_with_ip(uint32_t data11, uint32_t data12, uint32_t data13, uint32_t data21, uint32_t data22, uint32_t data23);
bool less_than_with_ip(uint32_t data11, uint32_t data12, uint32_t data13, uint32_t data21, uint32_t data22, uint32_t data23);
bool greater_than(alias data1, alias data2);
bool less_than(alias data1, alias data2);

#endif
