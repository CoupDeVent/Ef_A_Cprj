#include "automate.h"
#include "automate.c"

int main() {
    Automate AF;
    char *file_name = "automates/automate_test.txt";
    lire_automate_sur_fichier(file_name, &AF);
    afficher_automate(&AF);
    return 0;
}