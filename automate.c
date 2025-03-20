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
    AF->a_ete_rendu_deterministe = false;
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

    for(int i = 0; i < AF->num_states; i++){
        memcpy(AF->states[i].inter_states, AF->transitions[i*AF->num_symbols].from, sizeof(int) * MAX_DEPART);
        AF->states[i].num_inter_states = AF->transitions[i*AF->num_symbols].num_depart;
    }

    fclose(file);
}

void afficher_automate(Automate *AF) {
    int max_dest = 1;
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
    for (int i = 0; i < state_col_width*2 + max_inter_states*2; i++) printf("-");
    printf("+");
    for (int i = 0; i < AF->num_symbols; i++) {
        for (int j = 0; j < col_width; j++) printf("-");
        printf("+");
    }
    printf("\n");

    // En-tête du tableau
    printf("| %-2s  ", "");
    printf(" %-*s |", max_inter_states*2 -1, "");
    for (int i = 0; i < AF->num_symbols; i++) {
        printf(" %-*c |", col_width - 2, AF->symbols[i]);
    }
    printf("\n");

    // Ligne de séparation horizontale
    printf("+");
    for (int i = 0; i < state_col_width+1; i++) printf("-");
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
        printf("| ");
        for(int z = 0; z < AF->num_initial_states; z++){
            if(AF->initial_states[z].num_inter_states == AF->states[i].num_inter_states){
                int yes = 1;
                for(int k = 0; k < AF->states[i].num_inter_states; k++){
                    if(AF->initial_states[z].inter_states[k] != AF->states[i].inter_states[k]) yes = 0;
                }
                if(yes){
                    particular_state++;
                    printf("%-1s", "E");
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
                    particular_state++;
                    printf("%-1s", "S");
                }
            }
        }
        if(particular_state == 0) printf(" %-1s", "");
        if(particular_state == 1) printf(" ");
        printf(" |");

        printf(" ");
        for(int z = 0; z < AF->states[i].num_inter_states; z++){
            if (z > 0) printf(",");
            if(AF->states[i].inter_states[z] == 9999) printf("%c", 'P');
            else printf("%d", AF->states[i].inter_states[z]);
        }
        printf(" %-*s|",max_inter_states * 2 - AF->states[i].num_inter_states*2, "");

        for (int j = 0; j < AF->num_symbols; j++) {
            int found = 0;
            int temp = 0;
            count++;
            printf(" ");
            for (int d = 0; d < AF->transitions[count-1].num_destinations; d++) {
                if (d > 0) printf(",");
                if (AF->transitions[count-1].to[d] == 9999) printf("%c", 'P');
                else printf("%d", AF->transitions[count-1].to[d]);
                temp = d;
                found = 1;
            }
            if (!found) printf("- ");
            printf("%*s|", col_width - (found ? 2 : 3) - (temp * 2), "");
        }
        printf("\n");

        // Ligne de séparation horizontale
        printf("+");
        for (int i = 0; i < state_col_width+1; i++) printf("-");
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
    if(AF->a_ete_rendu_deterministe == true) return true;

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
    for (int k = 0; k < AF->num_transitions; k++){
        if(AF->transitions[k].num_destinations == 0) return false;
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
    AF->a_ete_rendu_deterministe = true;
}

void rendre_complet(Automate *AF) {
    AF->states[AF->num_states].inter_states[0] = 9999; // 9999 code pour etat poubelle
    AF->states[AF->num_states].num_inter_states = 1;
    AF->num_states++;

    for(int k = 0; k < AF->num_transitions; k++){
        if(AF->transitions[k].num_destinations == 0){
            AF->transitions[k].to[0] = 9999;
            AF->transitions[k].num_destinations = 1;
        }
    }

    for(int k = 0; k < AF->num_symbols; k++){
        AF->transitions[AF->num_transitions].to[0] = 9999;
        AF->transitions[AF->num_transitions].num_destinations = 1;
        AF->transitions[AF->num_transitions].from[0] = 9999;
        AF->transitions[AF->num_transitions].num_depart = 1;
        AF->num_transitions++;
    }
}

bool reconnaitre_mot(Automate *AF, const char *mot) {
    int etat_actuel = AF->initial_states[0].inter_states[0];

    // Parcourir chaque symbole du mot
    for (int i = 0; mot[i] != '\0'; i++) {
        char symbole = mot[i];
        bool transition_trouvee = false;

        // Chercher la transition correspondant au symbole
        for (int j = 0; j < AF->num_transitions; j++) {
            if (AF->transitions[j].from[0] == etat_actuel && AF->transitions[j].symbol == symbole) {
                etat_actuel = AF->transitions[j].to[0];
                transition_trouvee = true;
                break;
            }
        }

        // Si aucune transition trouvée pour ce symbole, le mot est rejeté
        if (!transition_trouvee) {
            return false;
        }
    }

    // Vérifier si l'état final est atteint
    for (int i = 0; i < AF->num_final_states; i++) {
        if (etat_actuel == AF->final_states[i].inter_states[0]) {
            return true;
        }
    }

    return false;
}

bool etats_sont_egaux(States a, States b) {
    if (a.num_inter_states != b.num_inter_states) {
        return false;
    }
    for (int i = 0; i < a.num_inter_states; i++) {
        if (a.inter_states[i] != b.inter_states[i]) {
            return false;
        }
    }
    return true;
}

void complementaire_automate(Automate *AF) {
    bool est_final[MAX_STATES] = {false};

    for (int i = 0; i < AF->num_final_states; i++) {
        for (int j = 0; j < AF->num_states; j++) {
            if (etats_sont_egaux(AF->final_states[i], AF->states[j])) {
                est_final[j] = true; // Marque l'état comme final
            }
        }
    }

    // Création du nouvel automate complémentaire
    Automate AF_COMP;
    memcpy(&AF_COMP, AF, sizeof(Automate));
    AF_COMP.num_final_states = 0;

    // Inverser les états finaux et non finaux
    for (int i = 0; i < AF->num_states; i++) {
        if (!est_final[i]) { // Un état qui n'était pas final devient final
            AF_COMP.final_states[AF_COMP.num_final_states] = AF->states[i];
            AF_COMP.num_final_states++;
        }
    }

    // Mise à jour de l'automate d'origine
    memcpy(AF, &AF_COMP, sizeof(Automate));
}

void minimiser_automate(Automate *AF) {
    if (!est_deterministe(AF) || !est_complet(AF)) {
        printf("L'automate doit être déterministe et complet avant la minimisation.\n");
        return;
    }

    bool distinction[MAX_STATES][MAX_STATES] = {false};
    for (int i = 0; i < AF->num_states; i++) {
        for (int j = i + 1; j < AF->num_states; j++) {
            if (AF->states[i].inter_states[0] == 9999 || AF->states[j].inter_states[0] == 9999) {
                distinction[i][j] = distinction[j][i] = true;
                continue;
            }
            bool i_final = false, j_final = false;
            for (int f = 0; f < AF->num_final_states; f++) {
                if (etats_sont_egaux(AF->final_states[f], AF->states[i])) i_final = true;
                if (etats_sont_egaux(AF->final_states[f], AF->states[j])) j_final = true;
            }
            if (i_final != j_final) {
                distinction[i][j] = distinction[j][i] = true;
            }
        }
    }

    bool changement;
    do {
        changement = false;
        for (int i = 0; i < AF->num_states; i++) {
            for (int j = i + 1; j < AF->num_states; j++) {
                if (!distinction[i][j]) {
                    for (int s = 0; s < AF->num_symbols; s++) {
                        int dest_i[MAX_DESTINATIONS], dest_j[MAX_DESTINATIONS];
                        int num_dest_i = 0, num_dest_j = 0;
                        for (int t = 0; t < AF->num_transitions; t++) {
                            if (AF->transitions[t].from[0] == i && AF->transitions[t].symbol == AF->symbols[s]) {
                                memcpy(dest_i, AF->transitions[t].to, sizeof(int) * AF->transitions[t].num_destinations);
                                num_dest_i = AF->transitions[t].num_destinations;
                            }
                            if (AF->transitions[t].from[0] == j && AF->transitions[t].symbol == AF->symbols[s]) {
                                memcpy(dest_j, AF->transitions[t].to, sizeof(int) * AF->transitions[t].num_destinations);
                                num_dest_j = AF->transitions[t].num_destinations;
                            }
                        }
                        for (int x = 0; x < num_dest_i; x++) {
                            for (int y = 0; y < num_dest_j; y++) {
                                if (dest_i[x] == 9999 || dest_j[y] == 9999) continue;
                                if (distinction[dest_i[x]][dest_j[y]]) {
                                    distinction[i][j] = distinction[j][i] = true;
                                    changement = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    } while (changement);

    int classe[MAX_STATES];
    memset(classe, -1, sizeof(classe));
    int nb_classes = 0;
    for (int i = 0; i < AF->num_states; i++) {
        if (classe[i] == -1) {
            classe[i] = nb_classes;
            for (int j = i + 1; j < AF->num_states; j++) {
                if (!distinction[i][j]) {
                    classe[j] = nb_classes;
                }
            }
            nb_classes++;
        }
    }

    Automate AF_min = {0};
    AF_min.num_states = nb_classes;
    AF_min.num_symbols = AF->num_symbols;
    memcpy(AF_min.symbols, AF->symbols, sizeof(AF->symbols));

    for (int t = 0; t < AF->num_transitions; t++) {
        int from = classe[AF->transitions[t].from[0]];
        char symbol = AF->transitions[t].symbol;
        Transition new_transition = { .from = {from}, .num_depart = 1, .symbol = symbol, .num_destinations = 0 };
        for (int d = 0; d < AF->transitions[t].num_destinations; d++) {
            int to = classe[AF->transitions[t].to[d]];
            if (to == 9999) continue;
            bool existe = false;
            for (int k = 0; k < new_transition.num_destinations; k++) {
                if (new_transition.to[k] == to) {
                    existe = true;
                    break;
                }
            }
            if (!existe) {
                new_transition.to[new_transition.num_destinations++] = to;
            }
        }
        AF_min.transitions[AF_min.num_transitions++] = new_transition;
    }

    AF_min.num_initial_states = 0;
    for (int i = 0; i < AF->num_initial_states; i++) {
        int classe_init = classe[AF->initial_states[i].inter_states[0]];
        if (classe_init == 9999) continue;
        AF_min.initial_states[AF_min.num_initial_states++] = (States) { .inter_states = {classe_init}, .num_inter_states = 1 };
    }

    AF_min.num_final_states = 0;
    for (int i = 0; i < AF->num_final_states; i++) {
        int classe_finale = classe[AF->final_states[i].inter_states[0]];
        if (classe_finale == 9999) continue;
        bool deja_ajoute = false;
        for (int j = 0; j < AF_min.num_final_states; j++) {
            if (AF_min.final_states[j].inter_states[0] == classe_finale) {
                deja_ajoute = true;
                break;
            }
        }
        if (!deja_ajoute) {
            AF_min.final_states[AF_min.num_final_states++] = (States) { .inter_states = {classe_finale}, .num_inter_states = 1 };
        }
    }

    memcpy(AF, &AF_min, sizeof(Automate));
    printf("Automate minimisé avec succès !\n");
}

























void rendre_deterministe_asynchrone(Automate *AF) {
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
            if (symbole == 'E') continue; // Les transitions 'E' sont ignorées pour la création des nouvelles transitions

            States nouvel_etat;
            nouvel_etat.num_inter_states = 0;

            // Fusionner toutes les destinations des états regroupés
            for (int j = 0; j < current_state.num_inter_states; j++) {
                int etat = current_state.inter_states[j];

                for (int t = 0; t < AF->num_transitions; t++) {
                    if (AF->transitions[t].from[0] == etat && AF->transitions[t].symbol == symbole) {
                        for (int d = 0; d < AF->transitions[t].num_destinations; d++) {
                            int destination = AF->transitions[t].to[d];
                            // Ajouter la destination si elle n'est pas déjà présente
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

        // Gestion des transitions asynchrones 'E' pour la fusion des états
        for (int i = 0; i < AF->num_transitions; i++) {
            if (AF->transitions[i].symbol == 'E') {
                for (int j = 0; j < AF->transitions[i].num_destinations; j++) {
                    int etat_dest = AF->transitions[i].to[j];

                    // Fusionner l'état atteint par 'E' avec les états des ensembles actuels
                    for (int k = 0; k < AFD.num_states; k++) {
                        for (int m = 0; m < AFD.states[k].num_inter_states; m++) {
                            if (AFD.states[k].inter_states[m] == etat_dest) {
                                // Ajouter les états atteints par 'E' à la fusion
                                States merged_state = AFD.states[k];
                                for (int p = 0; p < AFD.num_states; p++) {
                                    if (!etat_existe(AFD.states, AFD.num_states, merged_state)) {
                                        AFD.states[AFD.num_states++] = merged_state;
                                    }
                                }
                            }
                        }
                    }
                }
            }
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
    AF->a_ete_rendu_deterministe = true;
}