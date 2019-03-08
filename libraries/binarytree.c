#include "binarytree.h"

binarytree* find_min(binarytree *node) {
    if(node==NULL) /* There is no element in the tree */
        return NULL;

    if(node->left) /* Go to the left sub tree to find the min element */
        return find_min(node->left);
    else
        return node;
}

binarytree* find_max(binarytree *node) {
    if(node==NULL) /* There is no element in the tree */
        return NULL;

    if(node->right) /* Go to the left sub tree to find the min element */
        return find_max(node->right);
    else
        return node;
}

binarytree * insert_tree(binarytree *node, uint32_t data, int mask) {
    if(node==NULL) {
        binarytree *temp;
        temp = (binarytree *)malloc(sizeof(binarytree));
        temp -> data = data;
        temp -> mask = mask;
        temp -> left = temp -> right = NULL;
        temp -> list = NULL;
        return temp;
    }

    if(data > (node->data))
        node->right = insert_tree(node->right, data, mask);
    else if(data < (node->data))
        node->left = insert_tree(node->left, data, mask);

    /* Else there is nothing to do as the data is already in the tree. */
    return node;
}

void convert_tree_to_array_mask(binarytree *node, mask* array, int* i) {
    if(node==NULL)
        return;

    convert_tree_to_array_mask(node->left, array, i);
    array[*i].data = node->data;
    array[*i].mask = node->mask;
    (*i)++;
    convert_tree_to_array_mask(node->right, array, i);
}

void convert_tree_to_array(binarytree *node, uint32_t* array, int* i) {
    if(node==NULL)
        return;

    convert_tree_to_array(node->left, array, i);
    array[*i] = node->data;
    (*i)++;
    convert_tree_to_array(node->right, array, i);
}

int size_tree(binarytree* node) {
    if (node==NULL)
        return 0;
    else
        return(size_tree(node->left) + 1 + size_tree(node->right));
}

binarytree * delete_node(binarytree *node, uint32_t data) {
    binarytree *temp;

    if(node==NULL)
        printf("Element Not Found");
    else if(data < node->data)
        node->left = delete_node(node->left, data);
    else if(data > node->data)
        node->right = delete_node(node->right, data);
    else {
        /* Now We can delete this node and replace with either minimum element
           in the right sub tree or maximum element in the left subtree */
        if(node->right && node->left) {
            /* Here we will replace with minimum element in the right sub tree */
            temp = find_min(node->right);
            node -> data = temp->data;
            /* As we replaced it with some other node, we have to delete that node */
            node -> right = delete_node(node->right,temp->data);
        }
        else {
            /* If there is only one or zero children then we can directly
               remove it from the tree and connect its parent to its child */
            temp = node;
            if(node->left == NULL)
                node = node->right;
            else if(node->right == NULL)
                node = node->left;
            free(temp); /* temp is longer required */
        }
    }
    return node;
}

binarytree * find_tree(binarytree *node, uint32_t data) {
    if(node==NULL) /* Element is not found */
        return NULL;

    if(data > node->data) /* Search in the right sub tree. */
        return find_tree(node->right,data);
    else if(data < node->data) /* Search in the left sub tree. */
        return find_tree(node->left,data);
    else /* Element Found */
        return node;
}

void delete_tree(binarytree* node) {
    if (node == NULL) return;

    /* first delete both subtrees */
    delete_tree(node->left);
    delete_tree(node->right);

    free(node);
}

void change_inorder(binarytree *node, int i, int j) {
    if(node==NULL)
        return;

    change_inorder(node->left, i, j);
    if (node->mask == i)
        node->mask = j;
    change_inorder(node->right, i, j);
}

void print_inorder(binarytree *node) {
    if(node==NULL)
        return;

    print_inorder(node->left);
    printf("%s/%d\n",convert_int_to_ip(node->data), node->mask);
    print_inorder(node->right);
}

void print_preorder(binarytree *node) {
    if(node==NULL)
        return;

    printf("%s\n",convert_int_to_ip(node->data));
    print_preorder(node->left);
    print_preorder(node->right);
}

void print_postorder(binarytree *node) {
    if(node==NULL)
        return;

    print_postorder(node->left);
    print_postorder(node->right);
    printf("%s\n",convert_int_to_ip(node->data));
}
