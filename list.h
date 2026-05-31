#include <stdlib.h>
#ifndef LIST_H
#define LIST_H

typedef struct dll_node_t dll_node_t;
struct dll_node_t
{
	void *data; /* Pentru ca datele stocate sa poata avea orice tip, folosim un
				pointer la void. */
	dll_node_t *prev, *next;
};

typedef struct doubly_linked_list_t doubly_linked_list_t;
struct doubly_linked_list_t
{
	dll_node_t *head;
	unsigned int data_size;
	unsigned int size;
};

doubly_linked_list_t*
dll_create(unsigned int data_size);

int
q_enqueue(doubly_linked_list_t *q, void *new_data);

void*
s_dequeue(doubly_linked_list_t *q);

#endif
