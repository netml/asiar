#include "linkedlist_alias.h"

//display the list
/*
void print_list_binary_alias(linkedlist_alias *head) {
    linkedlist_alias *ptr = head;
    //start from the beginning
    while(ptr != NULL) {
        printf("\t\t\t%s, %s, %s\n", convert_int_to_ip(ptr->data->data.data1), convert_int_to_ip(ptr->data->data.data2), convert_int_to_ip(ptr->data->data.data3));
        ptr = ptr->next;
    }

    // delete ptr
}
*/

linkedlist_alias* find_list_binary_alias(linkedlist_alias *head, int data) {
    linkedlist_alias *ptr = head;
    while(ptr != NULL) {
        if (ptr->data == data)
            return head;
        ptr = ptr->next;
    }

    return NULL;
}

// insert link at the first location
linkedlist_alias* insert_list_binary_alias(linkedlist_alias *head, int data) {
    if (find_list_binary_alias(head, data) == NULL) {
        linkedlist_alias *link = (linkedlist_alias*) malloc(sizeof(linkedlist_alias)); // create a link

        link->data = data; // add data
        link->next = head; // point it to old first node

        return link; // point first to new first node
    }

    return head;
}

/*
// is list empty
bool is_empty_alias(linkedlist_alias *head) {
    return head == NULL;
}
*/

/*
int length_alias(linkedlist_alias *head) {
    int length = 0;
    linkedlist_alias *current;

    for (current = head; current != NULL; current = current->next)
        length++;

    return length;
}
*/
