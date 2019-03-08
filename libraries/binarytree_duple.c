#include "binarytree_duple.h"

void duple_completeness_merge(duple* arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    duple* L = (duple*) malloc(sizeof(duple) * n1);
    duple* R = (duple*) malloc(sizeof(duple) * n2);

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
void duple_completeness_mergesort(duple* arr, int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        duple_completeness_mergesort(arr, l, m);
        duple_completeness_mergesort(arr, m+1, r);

        duple_completeness_merge(arr, l, m, r);
    }
}

void duple_reference_merge(duple* arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    duple* L = (duple*) malloc(sizeof(duple) * n1);
    duple* R = (duple*) malloc(sizeof(duple) * n2);

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
        if (L[i].reference >= R[j].reference) {
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
void duple_reference_mergesort(duple* arr, int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        duple_reference_mergesort(arr, l, m);
        duple_reference_mergesort(arr, m+1, r);

        duple_reference_merge(arr, l, m, r);
    }
}

bool greater_than_duple(duple data1, duple data2) {
    if (data1.data1 > data2.data1)
        return true;
    else if (data1.data1 == data2.data1) {
        if (data1.data2 > data2.data2)
            return true;
    }

    return false;
}

bool less_than_duple(duple data1, duple data2) {
    if (data1.data1 < data2.data1)
        return true;
    else if (data1.data1 == data2.data1) {
        if (data1.data2 < data2.data2)
            return true;
    }

    return false;
}

bool greater_than_with_ip_duple(uint32_t data11, uint32_t data12, uint32_t data21, uint32_t data22) {
    if (data11 > data21)
        return true;
    else if (data11 == data21) {
        if (data12 > data22)
            return true;
    }

    return false;
}

bool less_than_with_ip_duple(uint32_t data11, uint32_t data12, uint32_t data21, uint32_t data22) {
    if (data11 < data21)
        return true;
    else if (data11 == data21) {
        if (data12 < data22)
            return true;
    }

    return false;
}

binarytree_duple* insert_tree_with_duple(binarytree_duple *node, duple data) {
    if(node==NULL) {
        binarytree_duple *temp;
        temp = (binarytree_duple*)malloc(sizeof(binarytree_duple));
        temp -> data = data;
        temp -> left = temp -> right = NULL;
        return temp;
    }

    if(greater_than_duple(data, (node->data)))
        node->right = insert_tree_with_duple(node->right, data);
    else if(less_than_duple(data, (node->data)))
        node->left = insert_tree_with_duple(node->left, data);

    /* Else there is nothing to do as the data is already in the tree. */
    return node;
}

binarytree_duple* insert_tree_with_ip_duple(binarytree_duple *node, uint32_t data1, uint32_t data2, int data1_pos, int data2_pos, double completeness, double reference) {
    if(node==NULL) {
        binarytree_duple *temp;
        temp = (binarytree_duple*)malloc(sizeof(binarytree_duple));
        temp -> data.data1 = data1;
        temp -> data.data2 = data2;
        temp -> data.data1_pos = data1_pos;
        temp -> data.data2_pos = data2_pos;
        temp -> data.completeness = completeness;
        temp -> data.reference = reference;
        temp -> left = temp -> right = NULL;
        return temp;
    }

    if(greater_than_with_ip_duple(data1, data2, node->data.data1, node->data.data2))
        node->right = insert_tree_with_ip_duple(node->right, data1, data2, data1_pos, data2_pos, completeness, reference);
    else if(less_than_with_ip_duple(data1, data2, node->data.data1, node->data.data2))
        node->left = insert_tree_with_ip_duple(node->left, data1, data2, data1_pos, data2_pos, completeness, reference);

    /* Else there is nothing to do as the data is already in the tree. */
    return node;
}

void convert_duple_tree_to_array(binarytree_duple *node, duple* array, int* i) {
    if(node==NULL)
        return;

    convert_duple_tree_to_array(node->left, array, i);
    array[*i] = node->data;
    (*i)++;
    convert_duple_tree_to_array(node->right, array, i);
}

int size_tree_duple(binarytree_duple* node) {
    if (node==NULL)
        return 0;
    else
        return(size_tree_duple(node->left) + 1 + size_tree_duple(node->right));
}

binarytree_duple* find_tree_duple(binarytree_duple *node, uint32_t data1, uint32_t data2) {
    if(node==NULL) /* Element is not found */
        return NULL;

    if(greater_than_with_ip_duple(data1, data2, node->data.data1, node->data.data2)) /* Search in the right sub tree. */
        return find_tree_duple(node->right, data1, data2);
    else if(less_than_with_ip_duple(data1, data2, node->data.data1, node->data.data2)) /* Search in the left sub tree. */
        return find_tree_duple(node->left, data1, data2);
    else /* Element Found */
        return node;
}

void delete_tree_duple(binarytree_duple* node) {
    if (node == NULL) return;

    /* first delete both subtrees */
    delete_tree_duple(node->left);
    delete_tree_duple(node->right);

    free(node);
}

void print_inorder_duple(binarytree_duple *node) {
    if(node==NULL)
        return;

    print_inorder_duple(node->left);
    printf("%s, %s\n", convert_int_to_ip(node->data.data1), convert_int_to_ip(node->data.data2));
    print_inorder_duple(node->right);
}

void print_preorder_duple(binarytree_duple *node) {
    if(node==NULL)
        return;

    printf("%s, %s\n", convert_int_to_ip(node->data.data1), convert_int_to_ip(node->data.data2));
    print_preorder_duple(node->left);
    print_preorder_duple(node->right);
}

void print_postorder_duple(binarytree_duple *node) {
    if(node==NULL)
        return;

    print_postorder_duple(node->left);
    print_postorder_duple(node->right);
    printf("%s, %s\n", convert_int_to_ip(node->data.data1), convert_int_to_ip(node->data.data2));
}
