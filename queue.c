#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL)
        return NULL;  // bad alloc
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;
    list_ele_t *current = q->head;
    while (current != NULL) {
        list_ele_t *toBeDeleted = current;
        current = current->next;
        free(toBeDeleted->value);
        free(toBeDeleted);
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q)
        return false;

    size_t _strlen = strlen(s) + 1;
    char *_string = malloc(_strlen);
    if (_string == NULL)
        return false;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        free(_string);
        return false;
    }
    memcpy(_string, s, strlen(s));
    _string[_strlen - 1] = '\0';

    newh->value = _string;
    newh->next = q->head;
    q->head = newh;

    if (q->tail == NULL)
        q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    size_t _strlen = strlen(s) + 1;
    char *_string = malloc(_strlen);
    if (_string == NULL)
        return false;
    memcpy(_string, s, strlen(s));
    _string[_strlen - 1] = '\0';

    list_ele_t *newTail = malloc(sizeof(list_ele_t));
    if (newTail == NULL) {
        free(_string);
        return false;
    }
    newTail->value = _string;
    newTail->next = NULL;

    if (q->tail != NULL)
        q->tail->next = newTail;
    q->tail = newTail;
    if (q->head == NULL)
        q->head = newTail;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || !q->head || sp == NULL)
        return false;

    list_ele_t *toBeDeleted = q->head;
    memcpy(sp, toBeDeleted->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    q->head = q->head->next;
    q->size--;

    free(toBeDeleted->value);
    free(toBeDeleted);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || !q->size)
        return;

    list_ele_t *current = q->head->next;
    q->tail = q->head;
    while (current) {
        /*
         * Use q->tail->next to be the cache.
         * Because q->tail->next is not using, and it can reduce declare a
         * variable.
         */
        q->tail->next = current->next;
        current->next = q->head;
        q->head = current;
        current = q->tail->next;
    }
}

list_ele_t *merge(list_ele_t *list1, list_ele_t *list2)
{
    list_ele_t *result, *current;
    if (!list1)
        return list2;
    if (!list2)
        return list1;

    // init result and current
    if (strcmp(list1->value, list2->value) < 0) {
        result = list1;
        list1 = list1->next;
    } else {
        result = list2;
        list2 = list2->next;
    }
    current = result;

    // merge value of list nodes until one's end
    while (list1 && list2) {
        if (strcmp(list1->value, list2->value) < 0) {
            current->next = list1;
            list1 = list1->next;
        } else {
            current->next = list2;
            list2 = list2->next;
        }
        current = current->next;
    }

    if (list1)
        current->next = list1;
    if (list2)
        current->next = list2;

    return result;
}

list_ele_t *mergeSort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    // spilt into 2 lists, slow will be the half element of list
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *list1 = mergeSort(head);
    list_ele_t *list2 = mergeSort(fast);

    return merge(list1, list2);
}


/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size < 2)
        return;

    q->head = mergeSort(q->head);
    list_ele_t *last = q->head;
    while (last->next) {
        last = last->next;
    }
    q->tail = last;
}
