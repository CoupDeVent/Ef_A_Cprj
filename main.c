#include "automate.h"
#include "automate.c"

int main() {
    Automate AF;
    AF.a_ete_rendu_deterministe = false;

    char *file_name = "automates/automate_5.txt";
    lire_automate_sur_fichier(file_name, &AF);
    afficher_automate(&AF);

    printf("\n Deter ? : %hhd", est_deterministe(&AF));
    printf("\n Standard ? : %hhd", est_standard(&AF));
    printf("\n complet ? : %hhd", est_complet(&AF));

    printf("\n\n");
    //rendre_standard(&AF);


    rendre_deterministe(&AF);
    afficher_automate(&AF);
    printf("\n%hhd", est_deterministe(&AF));
    //rendre_complet(&AF);
    //afficher_automate(&AF);

    complementaire_automate(&AF);
    afficher_automate(&AF);

    //printf("%hhd", reconnaitre_mot(&AF, "ab"));
    return 0;
}