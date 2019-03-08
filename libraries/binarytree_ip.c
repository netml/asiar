#include "binarytree_ip.h"
#include "network.h"

binarytree_ip* find_min_ip(binarytree_ip *node) {
    if(node==NULL) /* There is no element in the tree */
        return NULL;

    if(node->left) /* Go to the left sub tree to find the min element */
        return find_min_ip(node->left);
    else
        return node;
}

binarytree_ip* find_max_ip(binarytree_ip *node) {
    if(node==NULL) /* There is no element in the tree */
        return NULL;

    if(node->right) /* Go to the left sub tree to find the min element */
        return find_max_ip(node->right);
    else
        return node;
}

binarytree_ip * insert_tree_ip(binarytree_ip *node, uint32_t data) {
    if(node==NULL) {
        binarytree_ip *temp;
        temp = (binarytree_ip *)malloc(sizeof(binarytree_ip));
        temp -> data = data;
        temp -> left = temp -> right = NULL;
        return temp;
    }

    if(data > (node->data))
        node->right = insert_tree_ip(node->right,data);
    else if(data < (node->data))
        node->left = insert_tree_ip(node->left,data);

    /* Else there is nothing to do as the data is already in the tree. */
    return node;
}

void convert_tree_to_array_ip(binarytree_ip *node, uint32_t* array, int* i) {
    if(node==NULL)
        return;

    convert_tree_to_array_ip(node->left, array, i);
    array[*i] = node->data;
    (*i)++;
    convert_tree_to_array_ip(node->right, array, i);
}

int size_tree_ip(binarytree_ip* node) {
    if (node==NULL)
        return 0;
    else
        return(size_tree_ip(node->left) + 1 + size_tree_ip(node->right));
}

binarytree_ip * delete_ip(binarytree_ip *node, uint32_t data) {
    binarytree_ip *temp;

    if(node==NULL)
        printf("Element Not Found");
    else if(data < node->data)
        node->left = delete_ip(node->left, data);
    else if(data > node->data)
        node->right = delete_ip(node->right, data);
    else {
        /* Now We can delete this node and replace with either minimum element
           in the right sub tree or maximum element in the left subtree */
        if(node->right && node->left) {
            /* Here we will replace with minimum element in the right sub tree */
            temp = find_min_ip(node->right);
            node -> data = temp->data;
            /* As we replaced it with some other node, we have to delete that node */
            node -> right = delete_ip(node->right,temp->data);
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

binarytree_ip * find_tree_ip(binarytree_ip *node, uint32_t data) {
    if(node==NULL) /* Element is not found */
        return NULL;

    if(data > node->data) /* Search in the right sub tree. */
        return find_tree_ip(node->right,data);
    else if(data < node->data) /* Search in the left sub tree. */
        return find_tree_ip(node->left,data);
    else /* Element Found */
        return node;
}

void delete_tree_ip(binarytree_ip* node) {
    if (node == NULL) return;

    /* first delete both subtrees */
    delete_tree_ip(node->left);
    delete_tree_ip(node->right);

    free(node);
}

void print_inorder_ip(binarytree_ip *node) {
    if(node==NULL)
        return;

    print_inorder_ip(node->left);
    printf("%s  ",convert_int_to_ip(node->data));
    print_inorder_ip(node->right);
}

void print_preorder_ip(binarytree_ip *node) {
    if(node==NULL)
        return;

    printf("%s\n",convert_int_to_ip(node->data));
    print_preorder_ip(node->left);
    print_preorder_ip(node->right);
}

void print_postorder_ip(binarytree_ip *node) {
    if(node==NULL)
        return;

    print_postorder_ip(node->left);
    print_postorder_ip(node->right);
    printf("%s\n",convert_int_to_ip(node->data));
}
