#include "linkedlist.h"
#include "network.h"

linkedlist* find_list(linkedlist *head, int data) {
    bool exists = false;

    linkedlist *ptr = head;
    while(ptr != NULL) {
        if (ptr->data == data) {
            exists = true;
            break;
        }
        ptr = ptr->next;
    }

    if (!exists)
        return NULL;

    return head;
}

linkedlist* find_list2(linkedlist *head, uint32_t data) {
    bool exists = false;

    linkedlist *ptr = head;
    while(ptr != NULL) {
        if (ptr->data2 == data) {
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
linkedlist *getTail(linkedlist *cur) {
    while (cur != NULL && cur->next != NULL)
        cur = cur->next;
    return cur;
}

// Partitions the list taking the last element as the pivot
linkedlist *partition(linkedlist *head, linkedlist *end, linkedlist **newHead, linkedlist **newEnd)
{
    linkedlist *pivot = end;
    linkedlist *prev = NULL, *cur = head, *tail = pivot;

    // During partition, both the head and end of the list might change
    // which is updated in the newHead and newEnd variables
    while (cur != pivot) {
        if (cur->data < pivot->data) {
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
            linkedlist *tmp = cur->next;
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
linkedlist* quickSortRecur(linkedlist *head, linkedlist *end) {
    // base condition
    if (!head || head == end)
        return head;

    linkedlist *newHead = NULL, *newEnd = NULL;

    // Partition the list, newHead and newEnd will be updated
    // by the partition function
    linkedlist *pivot = partition(head, end, &newHead, &newEnd);

    // If pivot is the smallest element - no need to recur for
    // the left part.
    if (newHead != pivot) {
        // Set the node before the pivot node as NULL
        linkedlist *tmp = newHead;
        while (tmp->next != pivot)
            tmp = tmp->next;
        tmp->next = NULL;

        // Recur for the list before pivot
        newHead = quickSortRecur(newHead, tmp);

        // Change next of last node of the left half to pivot
        tmp = getTail(newHead);
        tmp->next =  pivot;
    }

    // Recur for the list after the pivot element
    pivot->next = quickSortRecur(pivot->next, newEnd);

    return newHead;
}

// The main function for quick sort. This is a wrapper over recursive
// function quickSortRecur()
void quickSort(linkedlist **headRef) {
    (*headRef) = quickSortRecur(*headRef, getTail(*headRef));
    return;
}

int get_min(linkedlist *head) {
    if (head == NULL)
        return -1;

    linkedlist *ptr = head;
    int min = ptr->data;

    ptr = ptr->next;

    //start from the beginning
    while(ptr != NULL) {
        if (ptr->data < min)
            min = ptr->data;
        ptr = ptr->next;
    }

    return min;
}

int get_max(linkedlist *head) {
    if (head == NULL)
        return -1;

    linkedlist *ptr = head;
    int max = ptr->data;

    ptr = ptr->next;

    //start from the beginning
    while(ptr != NULL) {
        if (ptr->data > max)
            max = ptr->data;
        ptr = ptr->next;
    }

    return max;
}

//display the list
void print_list(linkedlist *head) {
    linkedlist *ptr = head;
    printf("[ ");

    //start from the beginning
    while(ptr != NULL) {
        printf("(%d:%d) ", ptr->data, ptr->occurence);
        ptr = ptr->next;
    }

    printf("]");

    // delete ptr
}

//display the list
void print_list2_file(linkedlist *head, FILE *file) {
    linkedlist *ptr = head;
    //start from the beginning
    while(ptr != NULL) {
        fprintf(file, "%s ", convert_int_to_ip(ptr->data2));
        ptr = ptr->next;
    }

    // delete ptr
}

//display the list
void print_list2(linkedlist *head) {
    linkedlist *ptr = head;
    printf("[ ");

    //start from the beginning
    while(ptr != NULL) {
        printf("(%s) ", convert_int_to_ip(ptr->data2));
        ptr = ptr->next;
    }

    printf("]");

    // delete ptr
}

//display the list
void print_list3(linkedlist *head) {
    linkedlist *ptr = head;
    printf("[ ");

    //start from the beginning
    while(ptr != NULL) {
        printf("(%d) ", ptr->data);
        ptr = ptr->next;
    }

    printf("]");

    // delete ptr
}

void write_list(linkedlist *head, FILE *fp) {
    linkedlist *ptr = head;
    fprintf(fp, "[ ");

    //start from the beginning
    while(ptr != NULL) {
        fprintf(fp, "(%d) ",ptr->data);
        ptr = ptr->next;
    }

    fprintf(fp, "]");

    // delete ptr
}

// insert link at the first location
linkedlist* insert_list(linkedlist *head, int data) {
    if (find_list(head, data) == NULL) {
        linkedlist *link = (linkedlist*) malloc(sizeof(linkedlist)); // create a link

        link->data = data; // add data
        link->next = head; // point it to old first node
        link->occurence = 1;

        return link; // point first to new first node
    }
    else {
        head->occurence = head->occurence + 1;
    }

    return head;
}

// insert at the end
linkedlist* insert_list_inorder(linkedlist *head, int data, int gap) {
    if (head == NULL) {
        head = (linkedlist*) malloc(sizeof(linkedlist)); // create a link

        head->data = data; // add data
        head->next = NULL;
        head->occurence = gap;
    }
    else if (find_list(head, data) == NULL) {
        linkedlist *temp = head;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = (linkedlist*) malloc(sizeof(linkedlist)); // create a link
        temp = temp->next;

        temp->data = data; // add data
        temp->next = NULL;
        temp->occurence = gap;
    }

    return head;
}

// insert link at the first location
linkedlist* insert_list_alias(linkedlist *head, uint32_t data, int pos) {
    if (find_list2(head, data) == NULL) {
        linkedlist *link = (linkedlist*) malloc(sizeof(linkedlist)); // create a link

        link->data2 = data; // add data
        link->occurence = pos; // add position
        link->next = head; // point it to old first node

        return link; // point first to new first node
    }

    return head;
}

// is list empty
bool is_empty(linkedlist *head) {
    return head == NULL;
}

int length(linkedlist *head) {
    int length = 0;
    linkedlist *current;

    for(current = head; current != NULL; current = current->next) {
        length++;
    }

    return length;
}

// delete a link with given key
linkedlist* delete_list(linkedlist *head, int pos) {
    if (pos == 0) {
        linkedlist *to_be_deleted = head;
        head = head->next;
    }
    else {
        linkedlist *temp = head;

        // traverse
	int i = 0;
        for (i=0; i<(pos - 1); i++)
            temp = temp->next;

        linkedlist *to_be_deleted = temp->next;

        temp->next = temp->next->next;
    }
    // delete to_be_deleted
    return head;
}

void deleteList(linkedlist* head_ref) {
   /* deref head_ref to get the real head */
   linkedlist* current = head_ref;
   linkedlist* next;

   while (current != NULL) {
       next = current->next;
       free(current);
       current = next;
   }

   /* deref head_ref to affect the real head back
      in the caller. */
   head_ref = NULL;
}
