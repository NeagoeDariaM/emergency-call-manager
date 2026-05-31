#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "queue.h"
#include "tasks.h"
#include "utils.h"

//functie pentru adaugarea santinelei la incidente
void
santinela(struct incident *sant)
{
	sant->id = 0;
	strcpy(sant->status, "solved");
	strcpy(sant->priority, "low");

	sant->description = malloc(14);
	DIE(!sant->description, "malloc failed");

	strcpy(sant->description, "test incident");
}

//functie pentru adaugarea santinelei la interventii
void
int_santinela(struct intervention *sant1, struct incident *sant)
{
	sant1->incident = sant;
	sant1->unit = malloc(sizeof(struct unit));
	DIE(!sant1->unit, "malloc failed");

	sant1->unit->availability = 0;
	sant1->unit->type = 'A';
	sant1->unit->id = 0; 
}

//functia pentru citirea si apelarea functiilor corespunszatoare comenzilor
void
read_command(FILE *in, FILE *out, int nr_units, struct unit *interv,
			 doubly_linked_list_t *incidente, doubly_linked_list_t *interventii,
			 doubly_linked_list_t *queue_high, doubly_linked_list_t *queue_low,
			 doubly_linked_list_t *queue_medium,
			 doubly_linked_list_t *queue_available_units,
			 doubly_linked_list_t *stack_dem)
{
	int n;
	fscanf(in, "%d", &n);
	char cmd[25];
	for (int i = 0; i < n; i++) {
		fscanf(in, "%s", cmd);
	
		if (strcmp(cmd, "ADD_INCIDENT") == 0) {
			int id_incident;
			fscanf(in, "%d", &id_incident);
			char priority[7], desc[256];
			fscanf(in, "%s %[^\n]", priority, desc);
			char *descc = strtok(desc, "\"");
			add_incident(incidente, id_incident, priority, descc,
						 queue_high, queue_medium, queue_low);

		} else if (strcmp(cmd, "CHECK_UNITS_AVAILABILITY") == 0) {
			check_units_availability(queue_available_units, out);

		} else if (strcmp(cmd, "DISPATCH") == 0) {
			dispatch(stack_dem, queue_available_units, interventii,
					 queue_high, queue_medium, queue_low, out);

		} else if (strcmp(cmd, "UNDO_LAST_DISPATCH") == 0) {
			undo_last_dispatch(interventii, stack_dem,
							   queue_available_units, queue_high,
							   queue_medium, queue_low, out);

		} else if (strcmp(cmd, "SOLVED_INCIDENT") == 0) {
			int id_unit;
			fscanf(in, "%d", &id_unit);
			solved_incident(interventii, id_unit, queue_available_units, out);

		} else if (strcmp(cmd, "SHOW_UNIT") == 0) {
			int id_unit;
			fscanf(in, "%d", &id_unit);
			show_unit(interv, nr_units, id_unit, out);

		} else if (strcmp(cmd, "SHOW_INCIDENT") == 0) {
			int id_unit;
			fscanf(in, "%d", &id_unit);
			show_incident(incidente, id_unit, out);

		} else if (strcmp(cmd, "SHOW_INTERVENTIONS") == 0) {
			show_interventions(interventii, out);
		}
	}
}

//functie pentru eliberarea memoriei alocate intregului program
void
free_all(struct unit *interv, doubly_linked_list_t *incidente,
		 doubly_linked_list_t *interventii,
		 doubly_linked_list_t *queue_high,
		 doubly_linked_list_t *queue_low,
		 doubly_linked_list_t *queue_medium,
		 doubly_linked_list_t *queue_available_units,
		 doubly_linked_list_t *stack_dem)
{
	if (incidente) {
		dll_node_t *p = incidente->head->next;

		for (unsigned int i = 0; i < incidente->size; i++) {
			struct incident *data = (struct incident *)p->data;
			free(data->description);
			free(data);
			dll_node_t *rm = p;
			p = p->next;
			free(rm);
		}

		struct incident *sant = (struct incident *)incidente->head->data;
		
		free(sant->description);
		free(sant);
		free(incidente->head);
		free(incidente);
	}

	if (interventii) {
		dll_node_t *p = interventii->head->next;

		for (unsigned int i = 0; i < interventii->size; i++) {
			free(p->data);
			dll_node_t *rm = p;
			p = p->next;
			free(rm);
		}

		struct intervention *sant1 = (struct intervention *)
		interventii->head->data;
		
		free(sant1->unit);
		free(sant1);
		free(interventii->head);
		free(interventii);
	} 
	
	free_q(queue_available_units);
	free_q(queue_high);
	free_q(queue_low);
	free_q(queue_medium);
	free_q(stack_dem);
	free(interv);
}

int main(void)
{
	//deschidere fisiere intrare/iesire
	FILE *in = fopen("tema1.in", "r");
	DIE(!in, "file not open");

	FILE *out = fopen("tema1.out", "w");
	DIE(!out, "file not open");
	
	struct unit *interv = malloc(50 * sizeof(struct unit));
	DIE(!interv, "malloc failed");
	//creare lista pentru incidente + adaugare santinela
	doubly_linked_list_t *incidente = dll_create(sizeof(struct incident));
	struct incident *sant = malloc(sizeof(struct incident));
	DIE(!sant, "malloc failed");
	
	santinela(sant);
	incidente->head->data = sant;
	//creare lista pentru interventii + adaugare santinela
	doubly_linked_list_t *interventii = dll_create(sizeof(struct intervention));
	struct intervention *sant1 = malloc(sizeof(struct intervention));
	DIE(!sant1, "malloc failed");
	
	int_santinela(sant1, sant);
	interventii->head->data = sant1;
	//creare cozi
	doubly_linked_list_t *queue_high = dll_create(sizeof(struct incident));
	doubly_linked_list_t *queue_medium = dll_create(sizeof(struct incident));
	doubly_linked_list_t *queue_low = dll_create(sizeof(struct incident));
	doubly_linked_list_t
	*queue_available_units = dll_create(sizeof(struct unit));
	doubly_linked_list_t *stack_dem = dll_create(sizeof(struct intervention));
	
	//apelarea si rezolvarea task-urilor
	int nr_units = read_unit(in, interv, queue_available_units);
	read_command(in, out, nr_units, interv, incidente, interventii, queue_high,
				 queue_low, queue_medium, queue_available_units, stack_dem);
	
	//apelarea functiei oentru eliberarea memoriei
	free_all(interv, incidente, interventii, queue_high, queue_low,
			 queue_medium, queue_available_units, stack_dem);
	
	//inchiderea fisierelor
	fclose(in);
	fclose(out);
	return 0;
}
