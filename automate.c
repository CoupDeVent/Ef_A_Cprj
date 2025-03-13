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

    fscanf(file, "%d", &AF->num_initial_states);
    for (int i = 0; i < AF->num_initial_states; i++) {
        fscanf(file, "%d", &AF->initial_states[i]);
    }

    fscanf(file, "%d", &AF->num_final_states);
    for (int i = 0; i < AF->num_final_states; i++) {
        fscanf(file, "%d", &AF->final_states[i]);
    }

    fscanf(file, "%d", &AF->num_transitions);
    for (int i = 0; i < AF->num_transitions; i++) {
        int from, num_to, to;
        char symbol;
        fscanf(file, "%d %c %d", &from, &symbol, &num_to);

        AF->transitions[i].from = from;
        AF->transitions[i].symbol = symbol;

        AF->transitions[i].num_destinations = num_to;
        for(int k = 0; k < num_to; k++){
            fscanf(file, "%d", &to);
            AF->transitions[i].to[k] = to;
        }
    }

    AF->max_destination_in_all_transition = 0;
    for(int k = 0; k < AF->num_transitions; k++){
        if(AF->transitions[k].num_destinations > AF->max_destination_in_all_transition){
            AF->max_destination_in_all_transition = AF->transitions[k].num_destinations;
        }
    }

    fclose(file);
}

void afficher_automate(Automate *AF) {
    int col_width = 1 + (AF->max_destination_in_all_transition * 2); // Largeur dynamique
    int state_col_width = 3; // Largeur fixe pour la colonne des états

    // Ligne de séparation horizontale
    printf("+");
    for (int i = 0; i < state_col_width*2+1; i++) printf("-");
    printf("+");
    for (int i = 0; i < AF->num_symbols; i++) {
        for (int j = 0; j < col_width; j++) printf("-");
        printf("+");
    }
    printf("\n");

    // En-tête du tableau
    printf("| %-1s  ", "");
    printf(" %-1s |", "");
    for (int i = 0; i < AF->num_symbols; i++) {
        printf(" %-*c |", col_width - 2, AF->symbols[i]);
    }
    printf("\n");

    // Ligne de séparation horizontale
    printf("+");
    for(int k = 0; k < 2; k++){
        for (int i = 0; i < state_col_width; i++) printf("-");
        printf("+");
    }
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
            if(AF->initial_states[z] == i){
                particular_state = 1;
                printf("| %-1s |", "E");
            }
        }
        for(int z = 0; z < AF->num_final_states; z++){
            if(AF->final_states[z] == i){
                particular_state = 1;
                printf("| %-1s |", "S");
            }
        }
        if(!particular_state) printf("| %-1s |", "");

        printf(" %-1d |", i);

        for (int j = 0; j < AF->num_symbols; j++) {
            int found = 0;
            int temp = 0;
            count++;
            printf(" ");
            if (AF->transitions[count-1].num_destinations == 0) found = 0;
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
        for(int k = 0; k < 2; k++){
            for (int i = 0; i < state_col_width; i++) printf("-");
            printf("+");
        }
        for (int j = 0; j < AF->num_symbols; j++) {
            for (int k = 0; k < col_width; k++) printf("-");
            printf("+");
        }
        printf("\n");
    }
}