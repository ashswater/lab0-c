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
    if (!q)
        return NULL;
    /* TODO: What if malloc returned NULL? */
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q)
        return;
    list_ele_t *ptr = q->head, *tmp = NULL;
    while (ptr) {
        tmp = ptr;
        ptr = ptr->next;
        free(tmp->value);
        free(tmp);
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
    if (!q)
        return false;

    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    if (!(newh = malloc(sizeof(list_ele_t))))
        return false;
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (!(newh->value = malloc((strlen(s) + 1) * sizeof(char)))) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s));
    newh->value[strlen(s)] = '\0';
    newh->next = q->head;

    q->head = newh;
    if (!q->tail)
        q->tail = newh;
    ++(q->size);
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
    if (!q)
        return false;

    list_ele_t *newt;
    if (!(newt = malloc(sizeof(list_ele_t))))
        return false;
    if (!(newt->value = malloc((strlen(s) + 1) * sizeof(char)))) {
        free(newt);
        return false;
    }

    strncpy(newt->value, s, strlen(s));
    newt->value[strlen(s)] = '\0';
    newt->next = NULL;


    if (q->tail) {
        q->tail->next = newt;
        q->tail = newt;
    } else {
        q->head = newt;
        q->tail = newt;
    }

    ++(q->size);
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
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q)
        return false;
    if (!q->head)
        return false;
    // sp = malloc((strlen(q->head->value)+1)*sizeof(char));
    if (sp) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    if (!q->head)
        q->tail = NULL;

    free(tmp->value);
    free(tmp);
    --(q->size);
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
    if (!q)
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
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q)
        return;
    if (!q->head)
        return;
    list_ele_t *prev = NULL, *ptr = q->head, *nxt = q->head->next;
    q->head = q->tail;
    q->tail = ptr;
    while (ptr) {
        // ptr->next = prev;
        ptr->next = prev;
        prev = ptr;
        ptr = nxt;
        if (nxt)
            nxt = nxt->next;
    }
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
    if (!q)
        return;
    if (!q->head)
        return;
    if (q->head == q->tail)
        return;
    // q->tail = ele_q_sort(&(q->head));
    // q->tail = ele_merge_sort(&(q->head));
    ele_merge_sort(&(q->head));

    q->tail = q->head;
    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}

list_ele_t *ele_q_sort(list_ele_t **head)
{
    // if (!head)
    //    return NULL;
    if (!(*head))
        return NULL;

    list_ele_t *pivot = *head;
    list_ele_t *ptr = pivot->next, *tmp = NULL;
    // printf("pivot %s\n", pivot->value);
    if (!ptr)
        return pivot;
    pivot->next = NULL;
    list_ele_t *left = NULL, *right = NULL;
    while (ptr) {
        tmp = ptr;
        ptr = ptr->next;
        add_ele((strcmp(tmp->value, pivot->value) < 0) ? &left : &right, tmp);
    }
    if (left)
        ele_q_sort(&left);

    list_ele_t *tail = NULL;
    if (right)
        tail = ele_q_sort(&right);

    list_ele_t *result = NULL;
    // list_ele_t *result = left;
    concat_ele(&result, left);
    concat_ele(&result, pivot);
    concat_ele(&result, right);
    // pivot->next = right;

    *head = result;
    //    head = left;
    //    concat_ele(head, pivot);
    //    tmp->next = right;
    return (right) ? tail : pivot;
}

list_ele_t *ele_merge_sort(list_ele_t **head)
{
    if (!*head)
        return NULL;
    if (!(*head)->next)
        return *head;

    list_ele_t *left, *right;
    ele_split(*head, &left, &right);
    ele_merge_sort(&left);
    list_ele_t *tail = NULL;
    tail = ele_merge_sort(&right);

    *head = ele_sorted_merge(left, right);
    return tail;
}

list_ele_t *ele_sorted_merge(list_ele_t *a, list_ele_t *b)
{
    list_ele_t *result = NULL, *tmp, *tail = NULL;
    if (strcmp(a->value, b->value) < 0) {
        // printf("%s\n", a->value);
        result = tail = a;
        a = a->next;
        tail->next = NULL;
        if (!a) {
            tail->next = b;
            return result;
        }
    } else {
        // printf("%s\n", b->value);
        result = tail = b;
        b = b->next;
        tail->next = NULL;
        if (!b) {
            tail->next = a;
            return result;
        }
    }

    while (1) {
        if (strcmp(a->value, b->value) < 0) {  // a<b
            tmp = a;
            a = a->next;
            tail->next = tmp;
            tail = tmp;
            tail->next = NULL;

            if (!a) {
                tail->next = b;
                break;
            }
        } else {
            tmp = b;
            b = b->next;
            tail->next = tmp;
            tail = tmp;
            tail->next = NULL;
            if (!b) {
                tail->next = a;
                break;
            }
        }
    }
    return result;
}

void ele_split(list_ele_t *head, list_ele_t **a, list_ele_t **b)
{
    list_ele_t *ap = head->next, *bp = head;
    while (ap) {
        ap = ap->next;
        if (ap) {  // false if even
            ap = ap->next;
            bp = bp->next;
        }
    }
    *a = head;
    *b = bp->next;
    bp->next = NULL;
}

void add_ele(list_ele_t **head, list_ele_t *node)
{
    node->next = *head;
    *head = node;
}

void concat_ele(list_ele_t **head, list_ele_t *node)
{
    if (!node)
        return;
    while (*head) {
        head = &((*head)->next);
    }
    *head = node;
}
