#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
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
    /* TODO: How about freeing the list elements and the strings? */
    list_ele_t *current = q->head;
    while (current != NULL) {
        list_ele_t *toBeDeleted = current;
        free(toBeDeleted->value);
        current = current->next;
        free(toBeDeleted);
    }

    /* Free queue structure */
    free(q);
}

bool insert_q_is_NULL(queue_t *q, char *s)
{
    q = q_new();
    if (q != NULL) {
        list_ele_t *newQ;
        size_t _strlen = strlen(s) + 1;
        char *_string = malloc(_strlen);
        if (_string == NULL)
            return false;
        newQ = malloc(sizeof(list_ele_t));
        memcpy(_string, s, strlen(s));
        newQ->value = _string;
        newQ->next = NULL;
        q->head = newQ;
        q->tail = newQ;
        q->size++;
        return true;
    } else
        return false;
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
    /* TODO: What should you do if the q is NULL? */
    if (q == NULL) {
        if (insert_q_is_NULL(q, s))
            return true;
        else
            return false;
    }

    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    size_t _strlen = strlen(s) + 1;
    char *_string = malloc(_strlen);
    if (_string == NULL)
        return false;
    newh = malloc(sizeof(list_ele_t));
    memcpy(_string, s, strlen(s));
    newh->value = _string;
    newh->next = q->head;
    q->head = newh;
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
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q == NULL) {
        if (insert_q_is_NULL(q, s))
            return true;
        else
            return false;
    }
    list_ele_t *newTail;
    size_t _strlen = strlen(s) + 1;
    char *_string = malloc(_strlen);
    if (_string == NULL)
        return false;
    newTail = malloc(sizeof(list_ele_t));
    memcpy(_string, s, sizeof(strlen(s)));
    newTail->value = _string;
    newTail->next = NULL;
    q->tail->next = newTail;
    q->tail = newTail;
    q->size++;
    return false;
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
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q == NULL || !q->size)
        return false;

    list_ele_t *toBeDeleted = q->head;
    memcpy(sp, toBeDeleted->value, bufsize - 1);
    sp[bufsize] = '\0';
    free(toBeDeleted->value);
    q->head = q->head->next;
    free(toBeDeleted);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
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
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */

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
    if (!list1)
        return list1;
    if (!list2)
        return list2;

    if (strcmp(list1->value, list2->value) > 0) {  // increase
        list1->next = merge(list1->next, list2);
        return list1;
    } else {
        list2->next = merge(list1, list2->next);
        return list2;
    }
}

list_ele_t *mergeSort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    // spilt into 2 lists, show will be the half element of list
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;
    while (!fast || !fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *list1 = mergeSort(head);
    list_ele_t *list2 = mergeSort(fast);

    merge(list1, list2);
}


/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || !q->size)
        return;

    mergeSort(q->head);
}
