#include "automate.h"
#include "automate.c"

int main() {
    Automate AF;
    char *file_name = "automates/automate_test.txt";
    lire_automate_sur_fichier(file_name, &AF);
    afficher_automate(&AF);

    printf("\n Deter ? : %hhd", est_deterministe(&AF));
    printf("\n Standard ? : %hhd", est_standard(&AF));
    printf("\n complet ? : %hhd", est_complet(&AF));

    printf("\n\n");
    //rendre_standard(&AF);

    rendre_deterministe(&AF);
    afficher_automate(&AF);
    return 0;
}