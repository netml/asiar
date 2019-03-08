#include "binarytree_unresponsive.h"

void print_binarytree_unresponsive_inner(binarytree_unresponsive_inner *node) {
    if(node==NULL)
        return;

    print_binarytree_unresponsive_inner(node->left);
    printf("\tend: %s\n", convert_int_to_ip(node->data));
    printf("\tlength: %d\n", node->length);
    printf("\tIPs: ");
    for (int i = 0; i < node->length; i++)
        printf("%s ", convert_int_to_ip(node->ip_start + i));
    printf("\n\n");
    print_binarytree_unresponsive_inner(node->right);
}

void print_binarytree_unresponsive(binarytree_unresponsive *node) {
    if(node==NULL)
        return;

    print_binarytree_unresponsive(node->left);
    printf("from: %s\n", convert_int_to_ip(node->data));
    print_binarytree_unresponsive_inner(node->inner);
    print_binarytree_unresponsive(node->right);
}


binarytree_unresponsive* insert_binarytree_unresponsive(binarytree_unresponsive *node, uint32_t data) {
    if(node==NULL) {
        binarytree_unresponsive *temp;
        temp = (binarytree_unresponsive*)malloc(sizeof(binarytree_unresponsive));
        temp->data = data;
        temp->inner = NULL;
        temp->left = temp->right = NULL;
        return temp;
    }

    if(greater_than_binarytree_unresponsive(data, node->data))
        node->right = insert_binarytree_unresponsive(node->right, data);
    else if(less_than_binarytree_unresponsive(data, node->data))
        node->left = insert_binarytree_unresponsive(node->left, data);

    // Else there is nothing to do as the data is already in the tree.
    return node;
}

bool greater_than_binarytree_unresponsive(uint32_t data1, uint32_t data2) {
    if (data1 > data2)
        return true;

    return false;
}

bool less_than_binarytree_unresponsive(uint32_t data1, uint32_t data2) {
    if (data1 < data2)
        return true;

    return false;
}

binarytree_unresponsive* find_binarytree_unresponsive(binarytree_unresponsive *node, uint32_t data) {
    if(node==NULL) // Element is not found
        return NULL;

    if(greater_than_binarytree_unresponsive(data, node->data))
        return find_binarytree_unresponsive(node->right, data);
    else if(less_than_binarytree_unresponsive(data, node->data))
        return find_binarytree_unresponsive(node->left, data);
    else // Element Found
        return node;
}

binarytree_unresponsive_inner* insert_binarytree_unresponsive_inner(binarytree_unresponsive_inner *node, uint32_t data, uint32_t ip_start, int length) {
    if(node==NULL) {
        binarytree_unresponsive_inner *temp;
        temp = (binarytree_unresponsive_inner*)malloc(sizeof(binarytree_unresponsive_inner));
        temp->data = data;
        temp->ip_start = ip_start;
        temp->length = length;
        temp->left = temp->right = NULL;
        return temp;
    }

    if(greater_than_binarytree_unresponsive(data, node->data))
        node->right = insert_binarytree_unresponsive_inner(node->right, data, ip_start, length);
    else if(less_than_binarytree_unresponsive(data, node->data))
        node->left = insert_binarytree_unresponsive_inner(node->left, data, ip_start, length);

    // Else there is nothing to do as the data is already in the tree.
    return node;
}

binarytree_unresponsive_inner* find_binarytree_unresponsive_inner(binarytree_unresponsive_inner *node, uint32_t data) {
    if(node==NULL) // Element is not found
        return NULL;

    if(greater_than_binarytree_unresponsive(data, node->data))
        return find_binarytree_unresponsive_inner(node->right, data);
    else if(less_than_binarytree_unresponsive(data, node->data))
        return find_binarytree_unresponsive_inner(node->left, data);
    else // Element Found
        return node;
}

void delete_tree_unresponsive_inner(binarytree_unresponsive_inner* node) {
    if (node == NULL) return;

    // first delete both subtrees
    delete_tree_unresponsive_inner(node->left);
    delete_tree_unresponsive_inner(node->right);

    free(node);
}

void delete_tree_unresponsive(binarytree_unresponsive* node) {
    if (node == NULL) return;

    // first delete both subtrees
    delete_tree_unresponsive(node->left);
    delete_tree_unresponsive(node->right);

    delete_tree_unresponsive_inner(node->inner);

    free(node);
}

int size_tree_unresponsive(binarytree_unresponsive* node) {
    if (node==NULL)
        return 0;
    else
        return(size_tree_unresponsive(node->left) + 1 + size_tree_unresponsive(node->right));
}

/*
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

    // Else there is nothing to do as the data is already in the tree.
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
*/
