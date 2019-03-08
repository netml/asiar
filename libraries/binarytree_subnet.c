#include "binarytree_subnet.h"

void subnet_completeness_merge(subnet* arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    subnet* L = (subnet*) malloc(sizeof(subnet) * n1);
    subnet* R = (subnet*) malloc(sizeof(subnet) * n2);

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].completeness >= R[j].completeness) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // delete arrays
    free(L);
    free(R);
}

/* l is for left index and r is right index of the sub-array of arr to be sorted */
void subnet_completeness_mergesort(subnet* arr, int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        subnet_completeness_mergesort(arr, l, m);
        subnet_completeness_mergesort(arr, m+1, r);

        subnet_completeness_merge(arr, l, m, r);
    }
}

/*
int partition_subnet_reference(subnet* a, int l, int r) {
    int pivot;
    int i, j;
    subnet t;
    pivot = a[l].no_of_reference_points;
    i = l;
    j = r+1;

    while (1) {
        do ++i; while(a[i].no_of_reference_points >= pivot && i <= r);
        do --j; while(a[j].no_of_reference_points < pivot );
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }

    t = a[l];
    a[l] = a[j];
    a[j] = t;

    return j;
}

void quicksort_subnet_reference(subnet* a, int l, int r) {
    int j;

    if (l < r) {
        // divide and conquer
        j = partition_subnet_reference(a, l, r);
        quicksort_subnet_reference(a, l, j-1);
        quicksort_subnet_reference(a, j+1, r);
    }
}
*/

/*
int partition_subnet_completeness(subnet* a, int l, int r) {
    double pivot;
    int i, j;
    subnet t;
    pivot = a[l].completeness;
    i = l;
    j = r+1;

    while (1) {
        do ++i; while(a[i].completeness >= pivot && i <= r);
        do --j; while(a[j].completeness < pivot );
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }

    t = a[l];
    a[l] = a[j];
    a[j] = t;

    return j;
}

void quicksort_subnet_completeness(subnet* a, int l, int r) {
    int j;

    if (l < r) {
        // divide and conquer
        j = partition_subnet_completeness(a, l, r);
        quicksort_subnet_completeness(a, l, j-1);
        quicksort_subnet_completeness(a, j+1, r);
    }
}
*/

binarytree_subnet * insert_tree_subnet(binarytree_subnet *node, uint32_t data, int mask, int no_of_ips, int total_ips_in_subnet, double completeness) {
    if(node==NULL) {
        binarytree_subnet *temp;
        temp = (binarytree_subnet *)malloc(sizeof(binarytree_subnet));
        temp -> data = data;
        temp -> mask = mask;
        temp -> no_of_ips = no_of_ips;
        temp -> total_ips_in_subnet = total_ips_in_subnet;
        temp -> completeness = completeness;
        temp -> left = temp -> right = NULL;
        return temp;
    }

    if(data > (node->data))
        node->right = insert_tree_subnet(node->right, data, mask, no_of_ips, total_ips_in_subnet, completeness);
    else if(data < (node->data))
        node->left = insert_tree_subnet(node->left, data, mask, no_of_ips, total_ips_in_subnet, completeness);
    else if (mask > (node->mask))
        node->right = insert_tree_subnet(node->right, data, mask, no_of_ips, total_ips_in_subnet, completeness);
    else if(mask < (node->mask))
        node->left = insert_tree_subnet(node->left, data, mask, no_of_ips, total_ips_in_subnet, completeness);
    /* Else there is nothing to do as the data is already in the tree. */

    return node;
}

void convert_tree_to_array_subnet(binarytree_subnet *node, subnet* array, int* i) {
    if(node==NULL)
        return;

    convert_tree_to_array_subnet(node->left, array, i);
    array[*i].data = node->data;
    array[*i].mask = node->mask;
    array[*i].no_of_ips = node->no_of_ips;
    array[*i].completeness = node->completeness;
    array[*i].total_ips_in_subnet = node->total_ips_in_subnet;
    (*i)++;
    convert_tree_to_array_subnet(node->right, array, i);
}

int size_tree_subnet(binarytree_subnet* node) {
    if (node==NULL)
        return 0;
    else
        return(size_tree_subnet(node->left) + 1 + size_tree_subnet(node->right));
}

void delete_tree_subnet(binarytree_subnet* node) {
    if (node == NULL) return;

    /* first delete both subtrees */
    delete_tree_subnet(node->left);
    delete_tree_subnet(node->right);

    free(node);
}
