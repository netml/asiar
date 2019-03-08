#include <stdint.h>
#include "linkedlist.h"
#include "network.h"

#ifndef BINARYTREE_DUPLE_UNCLASSIFIED
#define BINARYTREE_DUPLE_UNCLASSIFIED

typedef struct duple_unclassified {
    uint32_t data1;
    uint32_t data2;
    uint32_t data3;
    uint32_t data4;
    int data1_pos;
    int data2_pos;
    int data3_pos;
    int data4_pos;
    int no_of_dist_match_for_1;
    int no_of_dist_match_for_2;
    double completeness_reference;
} duple_unclassified;

typedef struct binarytree_duple_unclassified {
    duple_unclassified data;
    struct binarytree_duple_unclassified *left;
    struct binarytree_duple_unclassified *right;
} binarytree_duple_unclassified;

void duple_completeness_reference_unclassified_merge(duple_unclassified* arr, int l, int m, int r);
void duple_completeness_reference_unclassified_mergesort(duple_unclassified* arr, int l, int r);
//int partition_duple_completeness_reference_unclassified(duple_unclassified* a, int l, int r);
//void quicksort_duple_completeness_reference_unclassified(duple_unclassified* a, int l, int r);
binarytree_duple_unclassified* insert_tree_with_ip_duple_unclassified(binarytree_duple_unclassified *node, uint32_t data1, uint32_t data2, uint32_t data3, uint32_t data4, int data1_pos, int data2_pos, int data3_pos, int data4_pos, double completeness, int no_of_dist_match_for_1, int no_of_dist_match_for_2);
int size_tree_duple_unclassified(binarytree_duple_unclassified* node);
void convert_duple_tree_to_array_unclassified(binarytree_duple_unclassified *node, duple_unclassified* array, int* i);
void delete_tree_duple_unclassified(binarytree_duple_unclassified* node);
bool greater_than_with_ip_duple_unclassified(uint32_t data11, uint32_t data12, uint32_t data13, uint32_t data14, uint32_t data21, uint32_t data22, uint32_t data23, uint32_t data24);
bool less_than_with_ip_duple_unclassified(uint32_t data11, uint32_t data12, uint32_t data13, uint32_t data14, uint32_t data21, uint32_t data22, uint32_t data23, uint32_t data24);

#endif
