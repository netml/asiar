#include "binarytree_duple_unclassified.h"

void duple_completeness_reference_unclassified_merge(duple_unclassified* arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    duple_unclassified* L = (duple_unclassified*) malloc(sizeof(duple_unclassified) * n1);
    duple_unclassified* R = (duple_unclassified*) malloc(sizeof(duple_unclassified) * n2);

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
        if (L[i].completeness_reference >= R[j].completeness_reference) {
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
void duple_completeness_reference_unclassified_mergesort(duple_unclassified* arr, int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        duple_completeness_reference_unclassified_mergesort(arr, l, m);
        duple_completeness_reference_unclassified_mergesort(arr, m+1, r);

        duple_completeness_reference_unclassified_merge(arr, l, m, r);
    }
}

/*
int partition_duple_completeness_reference_unclassified(duple_unclassified* a, int l, int r) {
    double pivot;
    int i, j;
    duple_unclassified t;
    pivot = a[l].completeness_reference;
    i = l;
    j = r+1;

    while (1) {
        do ++i; while(a[i].completeness_reference >= pivot && i <= r);
        do --j; while(a[j].completeness_reference < pivot );
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

void quicksort_duple_completeness_reference_unclassified(duple_unclassified* a, int l, int r) {
    int j;

    if (l < r) {
        // divide and conquer
        j = partition_duple_completeness_reference_unclassified(a, l, r);
        quicksort_duple_completeness_reference_unclassified(a, l, j-1);
        quicksort_duple_completeness_reference_unclassified(a, j+1, r);
    }
}
*/

bool greater_than_with_ip_duple_unclassified(uint32_t data11, uint32_t data12, uint32_t data13, uint32_t data14, uint32_t data21, uint32_t data22, uint32_t data23, uint32_t data24) {
    if (data11 > data21)
        return true;
    else if (data11 == data21) {
        if (data12 > data22)
            return true;
        else if (data12 == data22) {
            if (data13 > data23)
                return true;
            else if (data13 == data23) {
                if (data14 > data24)
                    return true;
            }
        }
    }

    return false;
}

bool less_than_with_ip_duple_unclassified(uint32_t data11, uint32_t data12, uint32_t data13, uint32_t data14, uint32_t data21, uint32_t data22, uint32_t data23, uint32_t data24) {
    if (data11 < data21)
        return true;
    else if (data11 == data21) {
        if (data12 < data22)
            return true;
        else if (data12 == data22) {
            if (data13 < data23)
                return true;
            else if (data13 == data23) {
                if (data14 < data24)
                    return true;
            }
        }
    }

    return false;
}


binarytree_duple_unclassified* insert_tree_with_ip_duple_unclassified(binarytree_duple_unclassified *node, uint32_t data1, uint32_t data2, uint32_t data3, uint32_t data4, int data1_pos, int data2_pos, int data3_pos, int data4_pos, double completeness, int no_of_dist_match_for_1, int no_of_dist_match_for_2) {
    if(node==NULL) {
        binarytree_duple_unclassified *temp;
        temp = (binarytree_duple_unclassified*)malloc(sizeof(binarytree_duple_unclassified));
        temp -> data.data1 = data1;
        temp -> data.data2 = data2;
        temp -> data.data3 = data3;
        temp -> data.data4 = data4;
        temp -> data.data1_pos = data1_pos;
        temp -> data.data2_pos = data2_pos;
        temp -> data.data3_pos = data3_pos;
        temp -> data.data4_pos = data4_pos;
        temp -> data.completeness_reference = completeness;
        temp -> data.no_of_dist_match_for_1 = no_of_dist_match_for_1;
        temp -> data.no_of_dist_match_for_2 = no_of_dist_match_for_2;
        temp -> left = temp -> right = NULL;
        return temp;
    }

    if(greater_than_with_ip_duple_unclassified(data1, data2, data3, data4, node->data.data1, node->data.data2, node->data.data3, node->data.data4))
        node->right = insert_tree_with_ip_duple_unclassified(node->right, data1, data2, data3, data4, data1_pos, data2_pos, data3_pos, data4_pos, completeness, no_of_dist_match_for_1, no_of_dist_match_for_2);
    else if(less_than_with_ip_duple_unclassified(data1, data2, data3, data4, node->data.data1, node->data.data2, node->data.data3, node->data.data4))
        node->left = insert_tree_with_ip_duple_unclassified(node->left, data1, data2, data3, data4, data1_pos, data2_pos, data3_pos, data4_pos, completeness, no_of_dist_match_for_1, no_of_dist_match_for_2);

    /* Else there is nothing to do as the data is already in the tree. */
    return node;
}

void convert_duple_tree_to_array_unclassified(binarytree_duple_unclassified *node, duple_unclassified* array, int* i) {
    if(node==NULL)
        return;

    convert_duple_tree_to_array_unclassified(node->left, array, i);
    array[*i] = node->data;
    (*i)++;
    convert_duple_tree_to_array_unclassified(node->right, array, i);
}

int size_tree_duple_unclassified(binarytree_duple_unclassified* node) {
    if (node==NULL)
        return 0;
    else
        return(size_tree_duple_unclassified(node->left) + 1 + size_tree_duple_unclassified(node->right));
}

void delete_tree_duple_unclassified(binarytree_duple_unclassified* node) {
    if (node == NULL) return;

    /* first delete both subtrees */
    delete_tree_duple_unclassified(node->left);
    delete_tree_duple_unclassified(node->right);

    free(node);
}
