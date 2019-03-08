#include "linkedlist_distance.h"
#include "network.h"

linkedlist_distance* find_list_linkedlist_distance(linkedlist_distance *head, int distance) {
    bool exists = false;

    linkedlist_distance *ptr = head;
    while(ptr != NULL) {
        if (ptr->distance == distance) {
            exists = true;
            break;
        }
        ptr = ptr->next;
    }

    if (!exists)
        return NULL;

    return head;
}

// Returns the last node of the list
linkedlist_distance *getTail_linkedlist_distance(linkedlist_distance *cur) {
    while (cur != NULL && cur->next != NULL)
        cur = cur->next;
    return cur;
}

// Partitions the list taking the last element as the pivot
linkedlist_distance *partition_linkedlist_distance(linkedlist_distance *head, linkedlist_distance *end, linkedlist_distance **newHead, linkedlist_distance **newEnd)
{
    linkedlist_distance *pivot = end;
    linkedlist_distance *prev = NULL, *cur = head, *tail = pivot;

    // During partition, both the head and end of the list might change
    // which is updated in the newHead and newEnd variables
    while (cur != pivot) {
        if (cur->distance > pivot->distance) {
            // First node that has a value less than the pivot - becomes
            // the new head
            if ((*newHead) == NULL)
                (*newHead) = cur;

            prev = cur;
            cur = cur->next;
        }
        else { // If cur node is greater than pivot
            // Move cur node to next of tail, and change tail
            if (prev)
                prev->next = cur->next;
            linkedlist_distance *tmp = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    // If the pivot data is the smallest element in the current list,
    // pivot becomes the head
    if ((*newHead) == NULL)
        (*newHead) = pivot;

    // Update newEnd to the current last node
    (*newEnd) = tail;

    // Return the pivot node
    return pivot;
}


//here the sorting happens exclusive of the end node
linkedlist_distance* quickSortRecur_linkedlist_distance(linkedlist_distance *head, linkedlist_distance *end) {
    // base condition
    if (!head || head == end)
        return head;

    linkedlist_distance *newHead = NULL, *newEnd = NULL;

    // Partition the list, newHead and newEnd will be updated
    // by the partition function
    linkedlist_distance *pivot = partition_linkedlist_distance(head, end, &newHead, &newEnd);

    // If pivot is the smallest element - no need to recur for
    // the left part.
    if (newHead != pivot) {
        // Set the node before the pivot node as NULL
        linkedlist_distance *tmp = newHead;
        while (tmp->next != pivot)
            tmp = tmp->next;
        tmp->next = NULL;

        // Recur for the list before pivot
        newHead = quickSortRecur_linkedlist_distance(newHead, tmp);

        // Change next of last node of the left half to pivot
        tmp = getTail_linkedlist_distance(newHead);
        tmp->next =  pivot;
    }

    // Recur for the list after the pivot element
    pivot->next = quickSortRecur_linkedlist_distance(pivot->next, newEnd);

    return newHead;
}

// The main function for quick sort. This is a wrapper over recursive
// function quickSortRecur()
void quickSort_linkedlist_distance(linkedlist_distance **headRef) {
    (*headRef) = quickSortRecur_linkedlist_distance(*headRef, getTail_linkedlist_distance(*headRef));
    return;
}

int get_min_linkedlist_distance(linkedlist_distance *head) {
    if (head == NULL)
        return -1;

    linkedlist_distance *ptr = head;
    int min = ptr->distance;

    ptr = ptr->next;

    //start from the beginning
    while(ptr != NULL) {
        if (ptr->distance < min)
            min = ptr->distance;
        ptr = ptr->next;
    }

    return min;
}

int get_max_linkedlist_distance(linkedlist_distance *head) {
    if (head == NULL)
        return -1;

    linkedlist_distance *ptr = head;
    int max = ptr->distance;

    ptr = ptr->next;

    //start from the beginning
    while(ptr != NULL) {
        if (ptr->distance > max)
            max = ptr->distance;
        ptr = ptr->next;
    }

    return max;
}

//display the list
void print_list_linkedlist_distance(linkedlist_distance *head) {
    linkedlist_distance *ptr = head;
    printf("[ ");

    //start from the beginning
    while(ptr != NULL) {
        printf("(%d:%d) ", ptr->distance, ptr->occurence);
        ptr = ptr->next;
    }

    printf("]");

    // delete ptr
}

// insert link at the first location
linkedlist_distance* insert_list_linkedlist_distance(linkedlist_distance *head, int distance) {
    if (find_list_linkedlist_distance(head, distance) == NULL) {
        linkedlist_distance *link = (linkedlist_distance*) malloc(sizeof(linkedlist_distance)); // create a link

        link->distance = distance; // add data
        link->next = head; // point it to old first node
        link->occurence = 1;

        return link; // point first to new first node
    }
    else {
        head->occurence = head->occurence + 1;
    }

    return head;
}

// is list empty
bool is_empty_linkedlist_distance(linkedlist_distance *head) {
    return head == NULL;
}

int length_linkedlist_distance(linkedlist_distance *head) {
    int length = 0;
    linkedlist_distance *current;

    for (current = head; current != NULL; current = current->next)
        length++;

    return length;
}
