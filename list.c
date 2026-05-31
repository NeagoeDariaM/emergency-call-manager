#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "utils.h"

//functia de creare a unei liste dublu inlantuite
doubly_linked_list_t*
dll_create(unsigned int data_size)
{
	doubly_linked_list_t *list = malloc(sizeof(doubly_linked_list_t));
	DIE(!list, "malloc failed");

	list->head = malloc(sizeof(dll_node_t));
	DIE(!list->head, "malloc failed");

	list->head->next = list->head;
	list->head->prev = list->head;
	list->data_size = data_size;
	list->size = 0;

	return list;
}

//functia de adaugare intr-o lista dublu inlantuita
int
q_enqueue(doubly_linked_list_t *q, void *new_data)
{
	if (!q) {
		return 0;
	}
	dll_node_t *new = malloc(sizeof(dll_node_t));
	DIE(!new, "malloc failed");
	
	new->data = new_data;
	dll_node_t *p = q->head->prev;
	new->prev = p;
	new->next = q->head;
	p->next = new;
	q->head->prev = new;
	q->size++;
	return 1;
}

//functie pentru eliminarea unui element dintr-o lista dublu inlantuita
//(ultimul)
void*
s_dequeue(doubly_linked_list_t *q)
{
	if (!q || q->size == 0) {
		return 0;
	}
	dll_node_t *s;
	dll_node_t *p = q->head->prev;

	s = p->prev;
	s->next = q->head;
	q->head->prev = s;
	void *data = p->data;
	free(p);
	q->size--;

	return data;
}
