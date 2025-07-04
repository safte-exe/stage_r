#ifndef STRUCTURE_LTS_EXTENDED_H
#define STRUCTURE_LTS_EXTENDED_H

#include "structure_lts.h"
#include "uthash.h"

#define TAILLE_MAX 1000

typedef struct Etat_x {
    int etat;
    Variable var;
} Etat_x;

typedef struct EtatXKey {
    int etat;
    Variable var;
} EtatXKey;

typedef struct EtatXHash {
    EtatXKey key;
    int index;
    UT_hash_handle hh;
} EtatXHash;

typedef struct Transition_x {
    int cible;
    int action_id;
} Transition_x;

typedef struct {
    Etat_x* etats_x;
    int nb_etats_x;
    int capacite_etats_x;
    Transition_x** transitions_x;
    int* nb_trans_x;
} LTS_Extended;

void build_lts_extended(LTS* lts, LTS_Extended* ltsx);
void print_lts_x(LTS_Extended* lts_ext);

#endif
