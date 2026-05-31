#include <stdlib.h>
#include "list.h"

#ifndef TASK_H
#define TASK_H

//structura pentru echipajele de interventie
struct unit {
	int id;
	char type;
	int availability;
};

//structura pentru incidente
struct incident {
	int id;
	char priority[7];
	char *description;
	char status[11];
};

//structura pentru solutionarea incidentelor-interventii
struct intervention {
	struct incident *incident;
	struct unit *unit;
};

//structura pentru intreg sistemul de urgenta
struct system {
	struct unit *units;
	struct incident *incidents;
	struct intervention *interventions;
};

void
add_incident(doubly_linked_list_t *incidente, int id_incident, char priority[7],
			 char *description, doubly_linked_list_t *queue_high,
			 doubly_linked_list_t *queue_medium,
			 doubly_linked_list_t *queue_low);

int
read_unit(FILE *in, struct unit *interv,
		  doubly_linked_list_t *queue_available_units);

void
check_units_availability(doubly_linked_list_t *queue_available_units,
						 FILE *out);

void
dispatch(doubly_linked_list_t *stack_dem,
		 doubly_linked_list_t *queue_available_units,
		 doubly_linked_list_t *interventii, doubly_linked_list_t *queue_high,
		 doubly_linked_list_t *queue_medium, doubly_linked_list_t *queue_low,
		 FILE *out);

void
undo_last_dispatch(doubly_linked_list_t *interventii,
				   doubly_linked_list_t *stack_dem,
				   doubly_linked_list_t *queue_available_units,
				   doubly_linked_list_t *queue_high,
				   doubly_linked_list_t *queue_medium,
				   doubly_linked_list_t *queue_low, FILE *out);

void
solved_incident(doubly_linked_list_t *interventii, int id_incident,
				doubly_linked_list_t *queue_available_units, FILE *out);

void
show_unit(struct unit *interv, int nr_units, int id_unit, FILE *out);

void
show_incident(doubly_linked_list_t *incidente, int id_incident, FILE *out);

void
show_interventions(doubly_linked_list_t *interventii, FILE *out);

#endif
