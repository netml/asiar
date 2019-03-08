#include "network.h"

#ifndef BINARYTREE_SUBNET
#define BINARYTREE_SUBNET

typedef struct subnet {
    uint32_t data;
    int mask;
    int no_of_ips;
    int total_ips_in_subnet;
    double completeness;
} subnet;

typedef struct binarytree_subnet {
    uint32_t data;
    int mask;
    int no_of_ips;
    int total_ips_in_subnet;
    double completeness;
    int distance;
    struct binarytree_subnet *left;
    struct binarytree_subnet *right;
} binarytree_subnet;

void subnet_completeness_merge(subnet* arr, int l, int m, int r);
void subnet_completeness_mergesort(subnet* arr, int l, int r);
//int partition_subnet_reference(subnet* a, int l, int r);
//void quicksort_subnet_reference(subnet* a, int l, int r);
//int partition_subnet_completeness(subnet* a, int l, int r);
//void quicksort_subnet_completeness(subnet* a, int l, int r);
binarytree_subnet* insert_tree_subnet(binarytree_subnet *node, uint32_t data, int mask, int no_of_ips, int total_ips_in_subnet, double completeness);
void convert_tree_to_array_subnet(binarytree_subnet *node, subnet* array, int* i);
int size_tree_subnet(binarytree_subnet* node);
void delete_tree_subnet(binarytree_subnet* node);

#endif
