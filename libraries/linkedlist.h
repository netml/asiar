#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef MY_HEADER_H
#define MY_HEADER_H

typedef struct node {
    int data;
    uint32_t data2;
    int occurence;
    struct node *next;
} linkedlist;

linkedlist* find_list(linkedlist *head, int data);
linkedlist* find_list2(linkedlist *head, uint32_t data);
void print_list(linkedlist *head);
void print_list2(linkedlist *head);
void print_list2_file(linkedlist *head, FILE *file);
void print_list3(linkedlist *head);
void write_list(linkedlist *head, FILE *fp);
linkedlist* insert_list(linkedlist *head, int data);
linkedlist* insert_list_inorder(linkedlist *head, int data, int gap);
linkedlist* insert_list_alias(linkedlist *head, uint32_t data, int pos);
bool is_empty(linkedlist *head);
int length(linkedlist *head);
int get_min(linkedlist *head);
int get_max(linkedlist *head);
linkedlist* delete_list(linkedlist *head, int pos);
linkedlist* getTail(linkedlist *cur);
linkedlist* partition(linkedlist *head, linkedlist *end, linkedlist **newHead, linkedlist **newEnd);
linkedlist* quickSortRecur(linkedlist *head, linkedlist *end);
void quickSort(linkedlist **headRef);
void deleteList(linkedlist* head_ref);

#endif
