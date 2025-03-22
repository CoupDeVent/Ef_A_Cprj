#include "automate.h"
#include "automate.c"

int main() {
    bool end = false;
    while(!end){
        Automate AF;
        AF.a_ete_rendu_deterministe = false;

        int num_automate = -1;
        printf("Quel automate voulez-vous etudier ?\n");
        scanf("%d", &num_automate);
        while(num_automate < 0 || num_automate > 44){
            scanf("%d", &num_automate);
        }
        char nom_fichier[27];
        if(num_automate < 10) snprintf(nom_fichier, 27, "automates/automate_0%d.txt", num_automate);
        else snprintf(nom_fichier, 27, "automates/automate_%d.txt", num_automate);
        lire_automate_sur_fichier(nom_fichier, &AF);

        printf("\nVoila l'automate numero %d :\n\n", num_automate);
        afficher_automate(&AF);
        printf("\n\nL'automate est :");
        if(est_deterministe(&AF)) printf("\n   - Deterministe");
        else printf("\n   - Non deterministe");
        if(est_standard(&AF)) printf("\n   - Standard");
        else printf("\n   - Non Standard");
        if(est_complet(&AF)) printf("\n   - Complet");
        else printf("\n   - Non Complet");

        bool inter_end = false;
        while(!inter_end){
            printf("\n\nQue voulez vous faire ?");
            printf("\n    1- Rendre standard l'automate si il ne l'est pas deja.");
            printf("\n    2- Rendre deterministe l'automate si il ne l'est pas deja.");
            printf("\n    3- Rendre complet l'automate si il ne l'est pas deja.");
            printf("\n    4- Rendre complementaire l'automate.");
            printf("\n    5- Reconnaitre un mot dans l'automate.");
            printf("\n    6- Afficher l'automate.");
            printf("\n    7- Reinitialise l'automate.");
            printf("\n    8- Quitter.");
            printf("\n");
            int faire = -1;
            while(faire < 1 || faire > 8){
                scanf("%d", &faire);
            }

            switch (faire) {
                case 1:{
                    if(est_standard(&AF)) printf("\nL'automate est deja standard.");
                    else{
                        rendre_standard(&AF);
                        printf("\nL'automate a ete rendu standard :\n");
                        afficher_automate(&AF);
                    }
                    break;
                }
                case 2:{
                    if(est_deterministe(&AF)) printf("\nL'automate est deja deterministe.");
                    else{
                        if(num_automate == 31 || num_automate == 32 || num_automate == 33 || num_automate == 34 || num_automate == 35){
                            rendre_deterministe_asynchrone(&AF);
                        }
                        else rendre_deterministe(&AF);
                        printf("\nL'automate a ete rendu deterministe :\n");
                        afficher_automate(&AF);
                    }
                    break;
                }
                case 3:{
                    if(est_complet(&AF)) printf("\nL'automate est deja complet.");
                    else{
                        rendre_complet(&AF);
                        printf("\nL'automate a ete rendu complet :\n");
                        afficher_automate(&AF);
                    }
                    break;
                }
                case 4:{
                    complementaire_automate(&AF);
                    printf("\nVoici l'automate complementaire :\n");
                    afficher_automate(&AF);
                    break;
                }
                case 5:{
                    int num_mot_reco = 0;
                    printf("\nCombien de mot voulez vous reconnaitre :");
                    scanf("%d", &num_mot_reco);

                    char mots[255][255];
                    for(int k = 1; k < num_mot_reco+1; k++){
                        if(k==1){
                            printf("\nEntrer le 1er mot :");
                            scanf("%s", mots[k]);
                        }
                        else {
                            printf("Entrer le %deme mot : ", k);
                            scanf("%s", mots[k]);
                        }
                    }

                    printf("\n");

                    for(int k = 1; k < num_mot_reco+1; k++){
                        if(k==1){
                            if(reconnaitre_mot(&AF, mots[k])) printf("\nLe 1er mot est reconnu");
                            else printf("\nLe 1er mot n'est pas reconnu");
                        }
                        else{
                            if(reconnaitre_mot(&AF, mots[k])) printf("\nLe %deme mot est reconnu", k);
                            else printf("\nLe %deme mot n'est pas reconnu", k);
                        }
                    }
                    break;
                }
                case 6:{
                    printf("\nVoici l'automate :\n\n");
                    afficher_automate(&AF);
                    break;
                }
                case 7:{
                    lire_automate_sur_fichier(nom_fichier, &AF);
                    break;
                }
                case 8:{
                    inter_end = true;
                    break;
                }
            }
        }
    }
    return 0;
}