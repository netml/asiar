#include "binarytree_uint64.h"

binarytree_uint64 * insert_tree_uint64(binarytree_uint64 *node, uint64_t data) {
    if(node==NULL) {
        binarytree_uint64 *temp;
        temp = (binarytree_uint64*)malloc(sizeof(binarytree_uint64));
        temp -> data = data;
        temp -> left = temp -> right = NULL;
        return temp;
    }

    if(data > (node->data))
        node->right = insert_tree_uint64(node->right, data);
    else if(data < (node->data))
        node->left = insert_tree_uint64(node->left, data);

    /* Else there is nothing to do as the data is already in the tree. */
    return node;
}

void convert_tree_to_array_uint64(binarytree_uint64 *node, uint64_t* array, int* i) {
    if(node==NULL)
        return;

    convert_tree_to_array_uint64(node->left, array, i);
    array[*i] = node->data;
    (*i)++;
    convert_tree_to_array_uint64(node->right, array, i);
}

int size_tree_uint64(binarytree_uint64* node) {
    if (node==NULL)
        return 0;
    else
        return(size_tree_uint64(node->left) + 1 + size_tree_uint64(node->right));
}
