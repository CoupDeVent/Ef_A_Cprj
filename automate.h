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
#define MAX_DEPART 10

typedef struct {
    int from[MAX_DEPART];
    char symbol;
    int to[MAX_DESTINATIONS];
    int num_depart;
    int num_destinations;
} Transition;

typedef struct {
    int inter_states[MAX_STATES];
    int num_inter_states;
} States;

typedef struct {
    int num_states;
    States states[MAX_STATES];

    int num_symbols;
    char symbols[MAX_SYMBOLS];

    int num_initial_states;
    States initial_states[MAX_STATES];

    int num_final_states;
    States final_states[MAX_STATES];

    int num_transitions;
    Transition transitions[MAX_TRANSITIONS];
    bool a_ete_rendu_deterministe;
} Automate;


void lire_automate_sur_fichier(char *nom_fichier, Automate *AF);
void afficher_automate(Automate *AF);

bool est_deterministe(Automate *AF);
bool est_standard(Automate *AF);
bool est_complet(Automate *AF);

void rendre_standard(Automate *AF);
void rendre_deterministe(Automate *AF);
void rendre_complet(Automate *AF);

bool reconnaitre_mot(Automate *AF, const char *mot);
void complementaire_automate(Automate *AF);

#endif //AUTOMATES_PRJ_AUTOMATE_H
