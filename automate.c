#include "automate.h"

void lire_automate_sur_fichier(char *nom_fichier, Automate *AF){
    /*
     Fichier des Automates sont sous la forme :

         2 a b          |   Nombre de symboles dans l’alphabet suivi des symbole
         5              |   Nombre d’états (de 0 à 4)
         1 0            |   Nombre d’états initiaux suivi de l’état initial (état 0)
         2 3 4          |   Nombre d’états terminaux suivi des états terminaux (états 3 et 4)
         10             |   Nombre de transitions
         0 a 3 1 2 3    |   3 transition de l'état 0 aux états 1 2 3 du symbol a
         0 c 0          |   0 transition
         1 a 0          |   ect...
         1 c 1 2
         2 a 1 3
         2 c 0
         3 a 0
         3 c 1 4
         4 a 1 2
         4 c 0
     */
    FILE *file = fopen(nom_fichier, "r");
    if (!file) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", &AF->num_symbols);
    for (int i = 0; i < AF->num_symbols; i++) {
        fscanf(file, " %c", &AF->symbols[i]);
    }

    fscanf(file, "%d", &AF->num_states);
    for(int i = 0; i < AF->num_states; i++){
        AF->states[i].inter_states[0] = i;
        AF->states[i].num_inter_states = 1;
    }

    fscanf(file, "%d", &AF->num_initial_states);
    for (int i = 0; i < AF->num_initial_states; i++) {
        fscanf(file, "%d", &AF->initial_states[i].inter_states[0]);
        AF->initial_states[i].num_inter_states = 1;
    }

    fscanf(file, "%d", &AF->num_final_states);
    for (int i = 0; i < AF->num_final_states; i++) {
        fscanf(file, "%d", &AF->final_states[i].inter_states[0]);
        AF->final_states[i].num_inter_states = 1;
    }

    fscanf(file, "%d", &AF->num_transitions);
    for (int i = 0; i < AF->num_transitions; i++) {
        int from, num_to, to;
        char symbol;
        fscanf(file, "%d %c %d", &from, &symbol, &num_to);

        AF->transitions[i].from[0] = from;
        AF->transitions[i].num_depart = 1;
        AF->transitions[i].symbol = symbol;

        AF->transitions[i].num_destinations = num_to;
        for(int k = 0; k < num_to; k++){
            fscanf(file, "%d", &to);
            AF->transitions[i].to[k] = to;
        }
    }

    fclose(file);
}

void afficher_automate(Automate *AF) {
    int max_dest = 0;
    for(int k = 0; k < AF->num_transitions; k++){
        if(AF->transitions[k].num_destinations > max_dest){
            max_dest = AF->transitions[k].num_destinations;
        }
    }
    int max_inter_states = 0;
    for(int k = 0; k < AF->num_states; k++){
        if(AF->states[k].num_inter_states > max_inter_states){
            max_inter_states = AF->states[k].num_inter_states;
        }
    }
    int col_width = 1 + (max_dest * 2); // Largeur dynamique
    int state_col_width = 3; // Largeur fixe pour la colonne des états

    // Ligne de séparation horizontale
    printf("+");
    for (int i = 0; i < state_col_width*2 + max_inter_states*2 -1; i++) printf("-");
    printf("+");
    for (int i = 0; i < AF->num_symbols; i++) {
        for (int j = 0; j < col_width; j++) printf("-");
        printf("+");
    }
    printf("\n");

    // En-tête du tableau
    printf("| %-1s  ", "");
    printf(" %-*s |", max_inter_states*2 -1, "");
    for (int i = 0; i < AF->num_symbols; i++) {
        printf(" %-*c |", col_width - 2, AF->symbols[i]);
    }
    printf("\n");

    // Ligne de séparation horizontale
    printf("+");
    for (int i = 0; i < state_col_width; i++) printf("-");
    printf("+");
    for (int i = 0; i < max_inter_states*2 +1; i++) printf("-");
    printf("+");
    for (int i = 0; i < AF->num_symbols; i++) {
        for (int j = 0; j < col_width; j++) printf("-");
        printf("+");
    }
    printf("\n");

    // Affichage des états et transitions
    int count = 0;
    for (int i = 0; i < AF->num_states; i++) {

        int particular_state = 0;
        for(int z = 0; z < AF->num_initial_states; z++){
            if(AF->initial_states[z].num_inter_states == AF->states[i].num_inter_states){
                int yes = 1;
                for(int k = 0; k < AF->states[i].num_inter_states; k++){
                    if(AF->initial_states[z].inter_states[k] != AF->states[i].inter_states[k]) yes = 0;
                }
                if(yes){
                    particular_state = 1;
                    printf("| %-s |", "E");
                }
            }
        }
        for(int z = 0; z < AF->num_final_states; z++){
            if(AF->final_states[z].num_inter_states == AF->states[i].num_inter_states){
                int yes = 1;
                for(int k = 0; k < AF->states[i].num_inter_states; k++){
                    if(AF->final_states[z].inter_states[k] != AF->states[i].inter_states[k]) yes = 0;
                }
                if(yes){
                    particular_state = 1;
                    printf("| %-s |", "S");
                }
            }
        }
        if(!particular_state) printf("| %-1s |", "");

        printf(" ");
        for(int z = 0; z < AF->states[i].num_inter_states; z++){
            if (z > 0) printf(",");
            printf("%d", AF->states[i].inter_states[z]);
        }
        printf(" %-*s|",max_inter_states * 2 - AF->states[i].num_inter_states*2, "");

        for (int j = 0; j < AF->num_symbols; j++) {
            int found = 0;
            int temp = 0;
            count++;
            printf(" ");
            for (int d = 0; d < AF->transitions[count-1].num_destinations; d++) {
                if (d > 0) printf(",");
                printf("%d", AF->transitions[count-1].to[d]);
                temp = d;
                found = 1;
            }
            if (!found) printf("- ");
            printf("%*s|", col_width - (found ? 2 : 3) - (temp * 2), "");
        }
        printf("\n");

        // Ligne de séparation horizontale
        printf("+");
        for (int i = 0; i < state_col_width; i++) printf("-");
        printf("+");
        for (int i = 0; i < max_inter_states*2 +1; i++) printf("-");
        printf("+");
        for (int j = 0; j < AF->num_symbols; j++) {
            for (int k = 0; k < col_width; k++) printf("-");
            printf("+");
        }
        printf("\n");
    }
}

bool est_deterministe(Automate *AF) {
    // Vérifier qu'il n'y a qu'un seul état initial
    if (AF->num_initial_states != 1) {
        return false;
    }

    // Vérifier qu'aucune transition n'a plus d'une destination
    for (int i = 0; i < AF->num_transitions; i++) {
        if (AF->transitions[i].num_destinations > 1) {
            return false;
        }
    }

    // Cas Transition sans symbole
    for (int i = 0; i < AF->num_transitions; i++) {
        if (AF->transitions[i].symbol == 'E') {
            return false;
        }
    }

    return true;
}

bool est_standard(Automate *AF) {
    // Vérifier qu'il y a un unique état initial
    if (AF->num_initial_states != 1) {
        return false;
    }

    int etat_initial[MAX_STATES];
    memcpy(etat_initial, AF->initial_states[0].inter_states, sizeof(int)*AF->initial_states[0].num_inter_states);

    // Vérifier que l'état initial n'a aucune transition entrante
    for (int i = 0; i < AF->num_transitions; i++) {
        for (int j = 0; j < AF->transitions[i].num_destinations; j++) {
            for (int k = 0; k < AF->initial_states[0].num_inter_states; k++){
                if (AF->transitions[i].to[j] == etat_initial[k]) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool est_complet(Automate *AF) {
    // Vérifier que chaque état possède une transition pour chaque symbole de l'alphabet
    for (int i = 0; i < AF->num_states; i++) {
        for (int j = 0; j < AF->num_symbols; j++) {
            char symbole = AF->symbols[j];
            bool transition_trouvee = false;

            // Chercher si l'état 'i' a une transition avec ce symbole
            for (int k = 0; k < AF->num_transitions; k++) {
                if (AF->transitions[k].from == AF->transitions[i].from && AF->transitions[k].symbol == symbole && AF->transitions[k].num_destinations > 0) {
                    transition_trouvee = true;
                    break;
                }
            }

            // Si une transition manque, l'automate est incomplet
            if (!transition_trouvee) {
                return false;
            }
        }
    }

    return true;
}

void rendre_standard(Automate *AF) {
    States ancien_initial[AF->num_initial_states];
    int ancien_num_ini = AF->num_initial_states;
    for (int i = 0; i < ancien_num_ini; i++) {
        ancien_initial[i] = AF->initial_states[i];
    }
    States nouvel_initial; // Créer un nouvel état initial
    nouvel_initial.inter_states[0] = AF->num_states;
    nouvel_initial.num_inter_states = 1;
    AF->states[AF->num_states] = nouvel_initial;
    AF->num_states++;

    // Changement de l'état initial
    AF->initial_states[0] = nouvel_initial;
    AF->num_initial_states = 1;

    for (int i = 0; i < AF->num_symbols; i++) {
        Transition nouvel_transition;
        memcpy(nouvel_transition.from, nouvel_initial.inter_states, sizeof(int) * nouvel_transition.num_depart);
        nouvel_transition.num_destinations = 0;
        nouvel_transition.num_depart = 1;
        nouvel_transition.symbol = AF->symbols[i];

        for (int k = 0; k < ancien_num_ini; k++) {
            for (int j = 0; j < AF->transitions[ancien_initial[k].inter_states[0]*2 + i].num_destinations; j++) { /////////////////////////////////////////////////////////////////
                int in = false;
                for (int z = 0; z < nouvel_transition.num_destinations; z++) {
                    if (nouvel_transition.to[z] == AF->transitions[ancien_initial[k].inter_states[0]*AF->num_symbols + i].to[j]) in = true;
                }
                if (!in) {
                    nouvel_transition.to[nouvel_transition.num_destinations] = AF->transitions[ancien_initial[k].inter_states[0]*AF->num_symbols + i].to[j];
                    nouvel_transition.num_destinations++;
                }
            }
        }

        AF->transitions[AF->num_transitions] = nouvel_transition;
        AF->num_transitions++;
    }
    afficher_automate(AF);
}

bool etat_existe(States etats[MAX_STATES], int nb_etats, States nouvel_etat) {
    for (int i = 0; i < nb_etats; i++) {
        if (etats[i].num_inter_states == nouvel_etat.num_inter_states) {
            bool identique = true;
            for (int j = 0; j < nouvel_etat.num_inter_states; j++) {
                if (etats[i].inter_states[j] != nouvel_etat.inter_states[j]) {
                    identique = false;
                    break;
                }
            }
            if (identique) return true;
        }
    }
    return false;
}

void rendre_deterministe(Automate *AF) {
    Automate AFD;
    AFD.num_transitions = 0;
    AFD.num_states = 0;
    AFD.num_symbols = AF->num_symbols;
    memcpy(AFD.symbols, AF->symbols, sizeof(AF->symbols));

    // Création du premier état déterministe (ensemble des états initiaux)
    States initial;
    initial.num_inter_states = 0;
    for (int i = 0; i < AF->num_initial_states; i++) {
        initial.inter_states[initial.num_inter_states++] = AF->initial_states[i].inter_states[0];
    }
    AFD.states[0] = initial;
    AFD.num_states = 1;
    AFD.initial_states[0] = initial;
    AFD.num_initial_states = 1;

    int index = 1; // Indice des nouveaux états créés
    int count = 0; // Indice de l’état en cours de traitement

    while (count < index) {
        States current_state = AFD.states[count];

        // Fusionner les transitions des anciens états du groupe
        for (int i = 0; i < AF->num_symbols; i++) {
            char symbole = AF->symbols[i];
            States nouvel_etat;
            nouvel_etat.num_inter_states = 0;

            // Fusionner toutes les destinations des états regroupés
            for (int j = 0; j < current_state.num_inter_states; j++) {
                int etat = current_state.inter_states[j];

                for (int t = 0; t < AF->num_transitions; t++) {
                    if (AF->transitions[t].from[0] == etat && AF->transitions[t].symbol == symbole) {
                        for (int d = 0; d < AF->transitions[t].num_destinations; d++) {
                            int destination = AF->transitions[t].to[d];

                            // Ajouter destination si elle n'est pas déjà présente
                            bool deja_present = false;
                            for (int x = 0; x < nouvel_etat.num_inter_states; x++) {
                                if (nouvel_etat.inter_states[x] == destination) {
                                    deja_present = true;
                                    break;
                                }
                            }
                            if (!deja_present) {
                                nouvel_etat.inter_states[nouvel_etat.num_inter_states++] = destination;
                            }
                        }
                    }
                }
            }

            // Trier les états pour garantir l'ordre cohérent
            for (int x = 0; x < nouvel_etat.num_inter_states - 1; x++) {
                for (int y = x + 1; y < nouvel_etat.num_inter_states; y++) {
                    if (nouvel_etat.inter_states[x] > nouvel_etat.inter_states[y]) {
                        int temp = nouvel_etat.inter_states[x];
                        nouvel_etat.inter_states[x] = nouvel_etat.inter_states[y];
                        nouvel_etat.inter_states[y] = temp;
                    }
                }
            }

            // Vérifier si ce nouvel ensemble d’états existe déjà
            int index_nouvel_etat = -1;
            for (int k = 0; k < AFD.num_states; k++) {
                if (etat_existe(&AFD.states[k], 1, nouvel_etat)) {
                    index_nouvel_etat = k;
                    break;
                }
            }

            if (index_nouvel_etat == -1) {
                // Ajout du nouvel état
                AFD.states[index] = nouvel_etat;
                index_nouvel_etat = index;
                index++;
                AFD.num_states++;
            }

            // Création de la transition fusionnée vers le nouvel ensemble d’états
            Transition new_transition;
            new_transition.from[0] = count;
            new_transition.num_depart = 1;
            new_transition.symbol = symbole;
            for (int x = 0; x < nouvel_etat.num_inter_states; x++) {
                new_transition.to[x] = nouvel_etat.inter_states[x];
            }
            new_transition.num_destinations = nouvel_etat.num_inter_states;
            AFD.transitions[AFD.num_transitions++] = new_transition;
        }
        count++;
    }

    // Définition des états finaux
    AFD.num_final_states = 0;
    for (int i = 0; i < AFD.num_states; i++) {
        for (int j = 0; j < AFD.states[i].num_inter_states; j++) {
            for (int f = 0; f < AF->num_final_states; f++) {
                if (AFD.states[i].inter_states[j] == AF->final_states[f].inter_states[0]) {
                    AFD.final_states[AFD.num_final_states++] = AFD.states[i];
                    break;
                }
            }
        }
    }

    // Mise à jour de l’automate original avec l’AFD
    memcpy(AF, &AFD, sizeof(Automate));
}