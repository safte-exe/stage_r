#include <stdlib.h>
#include <stdio.h>
#include "structure_lts.h"
#include "structure_lts_extended.h"
#include "uthash.h"

// Déclaration externe des tableaux définis ailleurs
extern const char** etats;
extern const char** actions;

// Ajouter un nouvel état étendu
int ajouter_etat_x(LTS_Extended* ltsx, EtatXHash** hash, int etat_id, Variable var) {
    EtatXKey key = {etat_id, var};
    EtatXHash* s;

    HASH_FIND(hh, *hash, &key, sizeof(EtatXKey), s);
    if (s) return s->index;

    if (ltsx->nb_etats_x == ltsx->capacite_etats_x) {
        ltsx->capacite_etats_x *= 2;
        ltsx->etats_x = realloc(ltsx->etats_x, ltsx->capacite_etats_x * sizeof(Etat_x));
        ltsx->transitions_x = realloc(ltsx->transitions_x, ltsx->capacite_etats_x * sizeof(Transition_x*));
        ltsx->nb_trans_x = realloc(ltsx->nb_trans_x, ltsx->capacite_etats_x * sizeof(int));
    }

    int idx = ltsx->nb_etats_x;
    ltsx->etats_x[idx] = (Etat_x){etat_id, var};
    ltsx->transitions_x[idx] = NULL;
    ltsx->nb_trans_x[idx] = 0;

    s = malloc(sizeof(EtatXHash));
    s->key = key;
    s->index = idx;
    HASH_ADD(hh, *hash, key, sizeof(EtatXKey), s);

    return ltsx->nb_etats_x++;
}

// Création des transitions étendues
void trans_x(LTS* lts, LTS_Extended* ltsx, EtatXHash** hash, int i) {
    Etat_x source = ltsx->etats_x[i];
    int etat_id = source.etat;
    Variable var = source.var;

    int n = lts->nb_trans_par_etat[etat_id];
    int nb_valides = 0;

    for (int k = 0; k < n; k++) {
        int action = lts->transitions[etat_id][k].label_action;
        if (lts->constraints[action](var)) nb_valides++;
    }

    if (nb_valides == 0) return;

    ltsx->transitions_x[i] = malloc(nb_valides * sizeof(Transition_x));
    ltsx->nb_trans_x[i] = nb_valides;

    int j = 0;
    for (int k = 0; k < n; k++) {
        Transition t = lts->transitions[etat_id][k];
        int action = t.label_action;
        if (!lts->constraints[action](var)) continue;

        Variable new_var = lts->update_functions[action](var);
        int cible = ajouter_etat_x(ltsx, hash, t.etat_in, new_var);
        ltsx->transitions_x[i][j++] = (Transition_x){cible, action};
    }
}

// Construction du LTS étendu
void build_lts_extended(LTS* lts, LTS_Extended* ltsx) {
    ltsx->capacite_etats_x = TAILLE_MAX;
    ltsx->nb_etats_x = 0;

    ltsx->etats_x = malloc(ltsx->capacite_etats_x * sizeof(Etat_x));
    ltsx->transitions_x = malloc(ltsx->capacite_etats_x * sizeof(Transition_x*));
    ltsx->nb_trans_x = malloc(ltsx->capacite_etats_x * sizeof(int));

    EtatXHash* hash = NULL;
    ajouter_etat_x(ltsx, &hash, 0, lts->variable);

    for (int i = 0; i < ltsx->nb_etats_x; i++) {
        trans_x(lts, ltsx, &hash, i);
    }

    HASH_CLEAR(hh, hash);
}
void print_lts_x(LTS_Extended* lts_ext) {
    for (int i = 0; i < lts_ext->nb_etats_x; i++) {
        printf("Etat etendu ID #%d\n", i);
        printf("\t Etat de base    : %s (ID %d)\n", etats[lts_ext->etats_x[i].etat], lts_ext->etats_x[i].etat);
        printf("\t Variables       : v = %d\n", lts_ext->etats_x[i].var.v);
        printf("\t Transitions sortantes :\n");

        for (int j = 0; j < lts_ext->nb_trans_x[i]; j++) {
            int id_cible = lts_ext->transitions_x[i][j].cible;
            int action_id = lts_ext->transitions_x[i][j].action_id;
            printf("\t\t   [%d] --%s--> [%d] \n", i, actions[action_id], id_cible);
        }

        printf("\n");
    }
}