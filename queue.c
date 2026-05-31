#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "utils.h"

//functie pentru adaugarea la inceputul unei cozi
void
q_add_first(doubly_linked_list_t *q, void *data)
{
	dll_node_t *new = malloc(sizeof(dll_node_t));
	DIE(!new, "malloc failed");

	new->data = data;
	new->next = q->head->next;
	q->head->next = new;
	new->prev = q->head;
	new->next->prev = new;
	q->size++;
}

//functie pentru eliminarea unui element dintr-o coada(primul)
void*
q_dequeue(doubly_linked_list_t *q)
{
	if (!q || q->size == 0) {
		return 0;
	}
	dll_node_t *s;
	dll_node_t *p = q->head->next;

	s = p->next;
	s->prev = p->prev;
	p->prev->next = s;
	void *data = p->data;
	free(p);
	q->size--;

	return data;
}

//functie pentru eliberarea memoriei unei cozi
void
free_q(doubly_linked_list_t *q)
{
	if (!q) {
		return;
	}
	dll_node_t *p = q->head->next;

	for (unsigned int i = 0; i < q->size; i++) {
		dll_node_t *rm = p;
		p = p->next;
		free(rm);
	}

	free(q->head);
	free(q);
}
