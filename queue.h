#include <stdlib.h>
#ifndef QUEUE_H
#define QUEUE_H
#include "list.h"

void
q_add_first(doubly_linked_list_t *q, void *data);

void*
q_dequeue(doubly_linked_list_t *q);

void
free_q(doubly_linked_list_t *q);

#endif
