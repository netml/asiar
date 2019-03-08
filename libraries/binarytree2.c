#include "binarytree2.h"

int size_tree2(binarytree2* node) {
    if (node==NULL)
        return 0;
    else
        return(size_tree2(node->left) + 1 + size_tree2(node->right));
}

void delete_tree2(binarytree2* node) {
    if (node == NULL) return;

    /* first delete both subtrees */
    delete_tree2(node->left);
    delete_tree2(node->right);

    free(node);
}

void convert_binarytree2_tree_to_array(binarytree2 *node, int* array, int* i) {
    if(node==NULL)
        return;

    convert_binarytree2_tree_to_array(node->left, array, i);
    array[*i] = node->data;
    (*i)++;
    convert_binarytree2_tree_to_array(node->right, array, i);
}

binarytree2 * insert_tree_2_with_occurence(binarytree2 *node, int data, int occurence) {
    if(node==NULL) {
        binarytree2 *temp;
        temp = (binarytree2 *)malloc(sizeof(binarytree2));
        temp -> data = data;
        temp -> left = temp -> right = NULL;
        temp -> occurence = occurence;
        return temp;
    }

    if(data > (node->data))
        node->right = insert_tree_2_with_occurence(node->right,data,occurence);
    else if(data < (node->data))
        node->left = insert_tree_2_with_occurence(node->left,data,occurence);
    else {
        node->occurence++;
    }
    /* Else there is nothing to do as the data is already in the tree. */
    return node;
}

binarytree2 * insert_tree_2(binarytree2 *node, int data) {
    if(node==NULL) {
        binarytree2 *temp;
        temp = (binarytree2 *)malloc(sizeof(binarytree2));
        temp -> data = data;
        temp -> left = temp -> right = NULL;
        temp -> occurence = 1;
        return temp;
    }

    if(data > (node->data))
        node->right = insert_tree_2(node->right,data);
    else if(data < (node->data))
        node->left = insert_tree_2(node->left,data);
    else {
        node->occurence++;
    }
    /* Else there is nothing to do as the data is already in the tree. */
    return node;
}

void print_inorder_2(binarytree2 *node) {
    if(node==NULL)
        return;

    print_inorder_2(node->left);
    printf("s:%d\t%d\n", node->data, node->occurence);
    print_inorder_2(node->right);
}

binarytree2* find_tree_2(binarytree2 *node, int data) {
    if(node==NULL) /* Element is not found */
        return NULL;

    if(data > node->data) /* Search in the right sub tree. */
        return find_tree_2(node->right,data);
    else if(data < node->data) /* Search in the left sub tree. */
        return find_tree_2(node->left,data);
    else /* Element Found */
        return node;
}
