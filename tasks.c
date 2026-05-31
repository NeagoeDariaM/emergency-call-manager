#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tasks.h"
#include "queue.h"
#include "utils.h"

//functie pentru adaugarea unui incident in lista de incidente si adaugarea in
//cozile pentru prioritate
void
add_incident(doubly_linked_list_t *incidente, int id_incident, char priority[7],
			 char *description, doubly_linked_list_t *queue_high,
			 doubly_linked_list_t *queue_medium,
			 doubly_linked_list_t *queue_low)
{
	struct incident *node = malloc(sizeof(struct incident));
	DIE(!node, "malloc failed");

	node->id = id_incident;
	strcpy(node->priority, priority);

	node->description = malloc(strlen(description) + 1);
	DIE(!node->description, "malloc failed");

	strcpy(node->description, description);
	strcpy(node->status, "queued");

	dll_node_t *p = malloc(sizeof(dll_node_t));
	DIE(!p, "malloc failed");

	p->data = node;
	dll_node_t *curr = incidente->head->prev;
	p->prev = curr;
	p->next = incidente->head;
	curr->next = p;
	incidente->head->prev = p;
	incidente->size++;

	if (strcmp(priority, "high") == 0) {
		q_enqueue(queue_high, node);
	}
	if (strcmp(priority, "medium") == 0) {
		q_enqueue(queue_medium, node);
	}
	if (strcmp(priority, "low") == 0) {
		q_enqueue(queue_low, node);
	}
}

//functie care citeste echipajele din fisier
int
read_unit(FILE *in, struct unit *interv,
		  doubly_linked_list_t *queue_available_units)
{
	int nr_inc;
	fscanf(in, "%d", &nr_inc);

	for (int i = 0; i < nr_inc; i++) {
		fscanf(in, "%d %c", &interv[i].id, &interv[i].type);
		interv[i].availability = 1;
		q_enqueue(queue_available_units, &interv[i]);
	}

	return nr_inc;
}

//functie care afiseaza numarul de echipaje disponibile
void
check_units_availability(doubly_linked_list_t *queue_available_units,
						 FILE *out)
{
	fprintf(out, "Number of available units: %d\n",
			queue_available_units->size);
}

//functie care demareaza interventia verificand existenta incidentelor
//cat si a echipajelor disponibile
void
dispatch(doubly_linked_list_t *stack_dem,
		 doubly_linked_list_t *queue_available_units,
		 doubly_linked_list_t *interventii, doubly_linked_list_t *queue_high,
		 doubly_linked_list_t *queue_medium, doubly_linked_list_t *queue_low,
		 FILE *out)
{
	if (queue_available_units && queue_available_units->size > 0 &&
		(queue_high->size > 0 || queue_medium->size > 0 ||
		 queue_low->size > 0)) {
		if (queue_high->size > 0) {
			struct intervention *nod = malloc(sizeof(struct intervention));
			DIE(!nod, "malloc failed");
	
			nod->incident = (struct incident *)q_dequeue(queue_high);
			nod->unit = (struct unit *)q_dequeue(queue_available_units);

			strcpy(nod->incident->status, "intervened");
			nod->unit->availability = 0;
			q_enqueue(interventii, nod);
			q_enqueue(stack_dem, nod);

		} else if (queue_medium->size > 0) {
			struct intervention *nod = malloc(sizeof(struct intervention));
			DIE(!nod, "malloc failed");
	
			nod->incident = (struct incident *)q_dequeue(queue_medium);
			nod->unit = (struct unit *)q_dequeue(queue_available_units);

			strcpy(nod->incident->status, "intervened");
			nod->unit->availability = 0;
			q_enqueue(interventii, nod);
			q_enqueue(stack_dem, nod);

		} else if (queue_low->size > 0) {
			struct intervention *nod = malloc(sizeof(struct intervention));
			DIE(!nod, "malloc failed");
	
			nod->incident = (struct incident *)q_dequeue(queue_low);
			nod->unit = (struct unit *)q_dequeue(queue_available_units);

			strcpy(nod->incident->status, "intervened");
			nod->unit->availability = 0;
			q_enqueue(interventii, nod);
			q_enqueue(stack_dem, nod);
		}

	} else {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
	}
}

//functie pentru anularea rezolvarii unui incident daca acesta nu a fost
//inca rezolvat
void
undo_last_dispatch(doubly_linked_list_t *interventii,
				   doubly_linked_list_t *stack_dem,
				   doubly_linked_list_t *queue_available_units,
				   doubly_linked_list_t *queue_high,
				   doubly_linked_list_t *queue_medium,
				   doubly_linked_list_t *queue_low, FILE *out)
{
	struct intervention *data = (struct intervention *)s_dequeue(stack_dem);

	while (data && strcmp(data->incident->status, "intervened") != 0) {
		data = (struct intervention *)s_dequeue(stack_dem);
	}
	if (data) {
		strcpy(data->incident->status, "queued");
		if (strcmp(data->incident->priority, "high") == 0) {
			q_add_first(queue_high, data->incident);
		}

		if (strcmp(data->incident->priority, "medium") == 0) {
			q_add_first(queue_medium, data->incident);
		}

		if (strcmp(data->incident->priority, "low") == 0) {
			q_add_first(queue_low, data->incident);
		}

		data->unit->availability = 1;
		q_enqueue(queue_available_units, data->unit);
		dll_node_t *nod = interventii->head->next;
		struct intervention *data1 = (struct intervention *)nod->data;

		while (nod != interventii->head &&
			   data->incident->id != data1->incident->id) {
			nod = nod->next;
			data1 = (struct intervention *)nod->data;
		}
		//sterg nod
		if (nod != interventii->head) {
			dll_node_t *curr = nod->next;
			curr->prev = nod->prev;
			nod->prev->next = curr;
			
			free(nod->data);
			free(nod);
			interventii->size--;
		}
	} else {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
	}
}

//functie pentru solutionarea unei interventii si marcarea specifica
void
solved_incident(doubly_linked_list_t *interventii, int id_incident,
				doubly_linked_list_t *queue_available_units, FILE *out)
{
	dll_node_t *nod = interventii->head->next;
	struct intervention *data = (struct intervention *)nod->data;

	while (nod != interventii->head && data->incident->id != id_incident) {
		data = (struct intervention *)nod->next->data;
		nod = nod->next;
	}

	if (nod != interventii->head &&
		strcmp(data->incident->status, "intervened") == 0) {
		strcpy(data->incident->status, "solved");
		data->unit->availability = 1;
		q_enqueue(queue_available_units, data->unit);
	} else {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
	}
}

//functie pentru afisarea informatiilor despre echipaje
void
show_unit(struct unit *interv, int nr_units, int id_unit, FILE *out)
{
	for (int i = 0; i < nr_units; i++) {
		if (interv[i].id == id_unit) {
			char mes[12];
			if (interv[i].availability == 1) {
				strcpy(mes, "available");
			} else {
				strcpy(mes, "unavailable");
			}

			fprintf(out, "Unit %d is type %c and is %s\n",
					id_unit, interv[i].type, mes);
			return;
		}
	}
	fprintf(out, "INVALID OPERATION! ERROR 404\n");
}

//functie pentru afisarea informatiilor despre incidente
void
show_incident(doubly_linked_list_t *incidente, int id_incident, FILE *out)
{
	dll_node_t *nod = incidente->head->next;
	struct incident *data = (struct incident *)nod->data;

	while (nod != incidente->head && data->id != id_incident) {
		data = (struct incident *)nod->next->data;
		nod = nod->next;
	}
	if (nod != incidente->head) {
		fprintf(out, "Incident %d has %s priority,",
				id_incident, data->priority);
		fprintf(out, " the following description: \"%s\" and is %s\n",
				data->description, data->status);
	} else {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
	}
}

//functie pentru afisarea informatiilor despre intreaga
//interventie(fiecarui incident + ce echipaj i-a fost asociat)
void
show_interventions(doubly_linked_list_t *interventii, FILE *out)
{
	if (!interventii || interventii->size == 0) {
		fprintf(out, "No intervention has been initiated\n");
		return;
	}
	dll_node_t *nod = interventii->head->next;
	struct intervention *data = (struct intervention *)nod->data;
	
	while (nod != interventii->head) {
		fprintf(out, "Incident %d was assigned to unit %d,",
				data->incident->id, data->unit->id);
		fprintf(out, " and has the following status: \"%s\"\n",
				data->incident->status);
		
		if (nod->next != interventii->head) {
			data = (struct intervention *)nod->next->data;
		}
		nod = nod->next;
	}
}
