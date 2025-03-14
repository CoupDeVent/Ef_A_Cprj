#ifndef AUTOMATES_PRJ_AUTOMATE_H
#define AUTOMATES_PRJ_AUTOMATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 10
#define MAX_TRANSITIONS 200
#define MAX_DESTINATIONS 10

typedef struct {
    int from;
    char symbol;
    int to[MAX_DESTINATIONS];
    int num_destinations;
} Transition;

typedef struct {
    int num_states;

    int num_symbols;
    char symbols[MAX_SYMBOLS];

    int num_initial_states;
    int initial_states[MAX_STATES];

    int num_final_states;
    int final_states[MAX_STATES];

    int num_transitions;
    int max_destination_in_all_transition;
    Transition transitions[MAX_TRANSITIONS];
} Automate;


void lire_automate_sur_fichier(char *nom_fichier, Automate *AF);
void afficher_automate(Automate *AF);

bool est_deterministe(Automate *AF);
bool est_standard(Automate *AF);
bool est_complet(Automate *AF);

#endif //AUTOMATES_PRJ_AUTOMATE_H
