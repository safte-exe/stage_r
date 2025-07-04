#include <stdio.h>
#include "structure_lts.h"
#include "structure_lts_extended.h"

// Déclarations manuelles des fonctions de toy_exemple.c

void fill_lts_struct(LTS* lts);
void build_lts_extended(LTS* lts, LTS_Extended* ltsx);
void print_lts_x(LTS_Extended* lts_ext);


int main() {
    LTS lts;
    fill_lts_struct(&lts);

    LTS_Extended ltsx;
    build_lts_extended(&lts, &ltsx);

    print_lts_x(&ltsx);

    printf("Nombre total d'états étendus : %d\n", ltsx.nb_etats_x);
    return 0;
}

