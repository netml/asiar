#include "binarytree_alias.h"

int partition_alias_reference(alias* a, int l, int r) {
    int pivot;
    int i, j;
    alias t;
    pivot = a[l].reference;
    i = l;
    j = r+1;

    while (1) {
        do ++i; while(a[i].reference >= pivot && i <= r);
        do --j; while(a[j].reference < pivot );
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

void quicksort_alias_reference(alias* a, int l, int r) {
    int j;

    if (l < r) {
        // divide and conquer
        j = partition_alias_reference(a, l, r);
        quicksort_alias_reference(a, l, j-1);
        quicksort_alias_reference(a, j+1, r);
    }
}

int partition_alias_completeness(alias* a, int l, int r) {
    double pivot;
    int i, j;
    alias t;
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

void quicksort_alias_completeness(alias* a, int l, int r) {
    int j;

    if (l < r) {
        // divide and conquer
        j = partition_alias_completeness(a, l, r);
        quicksort_alias_completeness(a, l, j-1);
        quicksort_alias_completeness(a, j+1, r);
    }
}

bool greater_than(alias data1, alias data2) {
    if (data1.data1 > data2.data1)
        return true;
    else if (data1.data1 == data2.data1) {
        if (data1.data2 > data2.data2)
            return true;
        else if (data1.data2 == data2.data2) {
            if (data1.data3 > data2.data3)
                return true;
        }
    }

    return false;
}

bool less_than(alias data1, alias data2) {
    if (data1.data1 < data2.data1)
        return true;
    else if (data1.data1 == data2.data1) {
        if (data1.data2 < data2.data2)
            return true;
        else if (data1.data2 == data2.data2) {
            if (data1.data3 < data2.data3)
                return true;
        }
    }

    return false;
}

bool greater_than_with_ip(uint32_t data11, uint32_t data12, uint32_t data13, uint32_t data21, uint32_t data22, uint32_t data23) {
    if (data11 > data21)
        return true;
    else if (data11 == data21) {
        if (data12 > data22)
            return true;
        else if (data12 == data22) {
            if (data13 > data23)
                return true;
        }
    }

    return false;
}

bool less_than_with_ip(uint32_t data11, uint32_t data12, uint32_t data13, uint32_t data21, uint32_t data22, uint32_t data23) {
    if (data11 < data21)
        return true;
    else if (data11 == data21) {
        if (data12 < data22)
            return true;
        else if (data12 == data22) {
            if (data13 < data23)
                return true;
        }
    }

    return false;
}

binarytree_alias* insert_tree_with_alias(binarytree_alias *node, alias data) {
    if(node==NULL) {
        binarytree_alias *temp;
        temp = (binarytree_alias*)malloc(sizeof(binarytree_alias));
        temp -> data = data;
        temp -> left = temp -> right = NULL;
        return temp;
    }

    if(greater_than(data, (node->data)))
        node->right = insert_tree_with_alias(node->right, data);
    else if(less_than(data, (node->data)))
        node->left = insert_tree_with_alias(node->left, data);

    /* Else there is nothing to do as the data is already in the tree. */
    return node;
}

binarytree_alias* insert_tree_with_ip(binarytree_alias *node, uint32_t data1, uint32_t data2, uint32_t data3) {
    if(node==NULL) {
        binarytree_alias *temp;
        temp = (binarytree_alias*)malloc(sizeof(binarytree_alias));
        temp -> data.data1 = data1;
        temp -> data.data2 = data2;
        temp -> data.data3 = data3;
        temp -> left = temp -> right = NULL;
        return temp;
    }

    if(greater_than_with_ip(data1, data2, data3, node->data.data1, node->data.data2, node->data.data3))
        node->right = insert_tree_with_ip(node->right, data1, data2, data3);
    else if(less_than_with_ip(data1, data2, data3, node->data.data1, node->data.data2, node->data.data3))
        node->left = insert_tree_with_ip(node->left, data1, data2, data3);

    /* Else there is nothing to do as the data is already in the tree. */
    return node;
}

void convert_alias_tree_to_array(binarytree_alias *node, alias* array, int* i) {
    if(node==NULL)
        return;

    convert_alias_tree_to_array(node->left, array, i);
    array[*i] = node->data;
    (*i)++;
    convert_alias_tree_to_array(node->right, array, i);
}

int size_tree_alias(binarytree_alias* node) {
    if (node==NULL)
        return 0;
    else
        return(size_tree_alias(node->left) + 1 + size_tree_alias(node->right));
}

binarytree_alias* find_tree_alias(binarytree_alias *node, uint32_t data1, uint32_t data2, uint32_t data3) {
    if(node==NULL) /* Element is not found */
        return NULL;

    if(greater_than_with_ip(data1, data2, data3, node->data.data1, node->data.data2, node->data.data3)) /* Search in the right sub tree. */
        return find_tree_alias(node->right, data1, data2, data3);
    else if(less_than_with_ip(data1, data2, data3, node->data.data1, node->data.data2, node->data.data3)) /* Search in the left sub tree. */
        return find_tree_alias(node->left, data1, data2, data3);
    else /* Element Found */
        return node;
}

void delete_tree_alias(binarytree_alias* node) {
    if (node == NULL) return;

    /* first delete both subtrees */
    delete_tree_alias(node->left);
    delete_tree_alias(node->right);

    free(node);
}

void print_inorder_alias(binarytree_alias *node) {
    if(node==NULL)
        return;

    print_inorder_alias(node->left);
    printf("%s, %s, %s\n", convert_int_to_ip(node->data.data1), convert_int_to_ip(node->data.data2), convert_int_to_ip(node->data.data3));
    print_inorder_alias(node->right);
}

void print_preorder_alias(binarytree_alias *node) {
    if(node==NULL)
        return;

    printf("%s, %s, %s\n", convert_int_to_ip(node->data.data1), convert_int_to_ip(node->data.data2), convert_int_to_ip(node->data.data3));
    print_preorder_alias(node->left);
    print_preorder_alias(node->right);
}

void print_postorder_alias(binarytree_alias *node) {
    if(node==NULL)
        return;

    print_postorder_alias(node->left);
    print_postorder_alias(node->right);
    printf("%s, %s, %s\n", convert_int_to_ip(node->data.data1), convert_int_to_ip(node->data.data2), convert_int_to_ip(node->data.data3));
}
