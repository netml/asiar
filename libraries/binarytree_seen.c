#include "binarytree_seen.h"

binarytree_seen * insert_tree_seen(binarytree_seen *node, int type, int vp, int ip) {
    if(node==NULL) {
        binarytree_seen *temp;
        temp = (binarytree_seen *)malloc(sizeof(binarytree_seen));
        temp -> type = type;
        temp -> vp = vp;
        temp -> ip = ip;
        temp -> left = temp -> right = NULL;
        return temp;
    }

    if(type > (node->type))
        node->right = insert_tree_seen(node->right, type, vp, ip);
    else if(type < (node->type))
        node->left = insert_tree_seen(node->left, type, vp, ip);
    else {
        if(vp > (node->vp))
            node->right = insert_tree_seen(node->right, type, vp, ip);
        else if(vp < (node->vp))
            node->left = insert_tree_seen(node->left, type, vp, ip);
        else {
            if(ip > (node->ip))
                node->right = insert_tree_seen(node->right, type, vp, ip);
            else if(ip < (node->ip))
                node->left = insert_tree_seen(node->left, type, vp, ip);

            /* Else there is nothing to do as the data is already in the tree. */
        }
    }

    return node;
}

binarytree_seen * find_tree_seen(binarytree_seen *node, int type, int vp, int ip) {
    if(node==NULL) /* Element is not found */
        return NULL;

    if(type > node->type) /* Search in the right sub tree. */
        return find_tree_seen(node->right, type, vp, ip);
    else if(type < node->type) /* Search in the left sub tree. */
        return find_tree_seen(node->left, type, vp, ip);
    else {
        if(vp > node->vp) /* Search in the right sub tree. */
            return find_tree_seen(node->right, type, vp, ip);
        else if(vp < node->vp) /* Search in the left sub tree. */
            return find_tree_seen(node->left, type, vp, ip);
        else {
            if(ip > node->ip) /* Search in the right sub tree. */
                return find_tree_seen(node->right, type, vp, ip);
            else if(ip < node->ip) /* Search in the left sub tree. */
                return find_tree_seen(node->left, type, vp, ip);
            else /* Element Found */
                return node;
        }
    }
}

void delete_tree_seen(binarytree_seen* node) {
    if (node == NULL) return;

    /* first delete both subtrees */
    delete_tree_seen(node->left);
    delete_tree_seen(node->right);

    free(node);
}
