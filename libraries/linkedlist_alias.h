#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "binarytree_alias.h"

#ifndef MY_HEADER_LLA
#define MY_HEADER_LLA

typedef struct nodel {
    int data;
    struct nodel *next;
} linkedlist_alias;

linkedlist_alias* find_list_binary_alias(linkedlist_alias *head, int data);
linkedlist_alias* insert_list_binary_alias(linkedlist_alias *head, int data);
//void print_list_binary_alias(linkedlist_alias *head);
//bool is_empty_alias(linkedlist_alias *head);
//int length_alias(linkedlist_alias *head);

#endif
