#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef MY_HEADER_LL
#define MY_HEADER_LL

typedef struct nodedistance {
    int distance;
    int occurence;
    struct nodedistance *next;
} linkedlist_distance;

linkedlist_distance* find_list_linkedlist_distance(linkedlist_distance *head, int data);
void print_list_linkedlist_distance(linkedlist_distance *head);
linkedlist_distance* insert_list_linkedlist_distance(linkedlist_distance *head, int data);
bool is_empty_linkedlist_distance(linkedlist_distance *head);
int length_linkedlist_distance(linkedlist_distance *head);
int get_min_linkedlist_distance(linkedlist_distance *head);
int get_max_linkedlist_distance(linkedlist_distance *head);
linkedlist_distance* getTail_linkedlist_distance(linkedlist_distance *cur);
linkedlist_distance* partition_linkedlist_distance(linkedlist_distance *head, linkedlist_distance *end, linkedlist_distance **newHead, linkedlist_distance **newEnd);
linkedlist_distance* quickSortRecur_linkedlist_distance(linkedlist_distance *head, linkedlist_distance *end);
void quickSort_linkedlist_distance(linkedlist_distance **headRef);

#endif
