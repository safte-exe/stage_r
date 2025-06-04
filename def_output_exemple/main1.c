#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h"

#define max_etats 50
#define max_actions 20
#define max_trans_par_etat 50
#define NB_VAR 1
#define nb_actions 3

char* etats[max_etats];
int num_etats;

char* actions[max_actions];

typedef struct Transition {
    int etat_in;
    int label_action;
} Transition;

Transition* transitions[max_etats];
int nb_trans_par_etat[max_etats] = {0};

void init_lts() {
    etats[0] = "up";
    etats[1] = "left";
    etats[2] = "right";
    etats[3] = "down";
    num_etats = 4;
    actions[0] = "a";
    actions[1] = "b";
    actions[2] = "c";

    transitions[0] = malloc(2 * sizeof(Transition));
    transitions[0][0] = (Transition){.etat_in = 1, .label_action = 0};
    transitions[0][1] = (Transition){.etat_in = 2, .label_action = 1};
    nb_trans_par_etat[0] = 2;

    transitions[1] = malloc(1 * sizeof(Transition));
    transitions[1][0] = (Transition){.etat_in = 3, .label_action = 1};
    nb_trans_par_etat[1] = 1;

    transitions[2] = malloc(1 * sizeof(Transition));
    transitions[2][0] = (Transition){.etat_in = 3, .label_action = 0};
    nb_trans_par_etat[2] = 1;

    transitions[3] = malloc(1 * sizeof(Transition));
    transitions[3][0] = (Transition){.etat_in = 0, .label_action = 2};
    nb_trans_par_etat[3] = 1;
}

typedef struct Variable {
    int v;
} Variable;

Variable variable;

void init_variables() {
    variable.v = 0;
}

Variable update_a( Variable var ) { if (var.v * 2 <= 10 ) { var.v *= 2;} return var; }
Variable update_b( Variable var ) { if (var.v + 2 <= 10 ) { var.v += 2;} return var; }
Variable update_c( Variable var ) { return var; }

typedef Variable (*UpdateFunction)(Variable);
UpdateFunction update_functions[nb_actions];

void init_update_functions() {
    update_functions[0] = update_a;
    update_functions[1] = update_b;
    update_functions[2] = update_c;
}

typedef struct Etat_x {
    int etat;
    Variable var;
    int id_interne;
    UT_hash_handle hh;
} Etat_x;

Etat_x *etats_x_hash = NULL;
int nb_etats_x = 0;

int etat_x_cle_hash(int etat, int var_v, Etat_x **res) {
    Etat_x *s;
    for (s = etats_x_hash; s != NULL; s = s->hh.next) {
        if (s->etat == etat && s->var.v == var_v) {
            *res = s;
            return 1;
        }
    }
    *res = NULL;
    return 0;
}

int ajouter_etat_x(int etat_id, Variable vars) {
    Etat_x *s;
    if (!etat_x_cle_hash(etat_id, vars.v, &s)) {
        s = malloc(sizeof(Etat_x));
        s->etat = etat_id;
        s->var = vars;
        s->id_interne = nb_etats_x;
        HASH_ADD(hh, etats_x_hash, etat, sizeof(Etat_x) - sizeof(UT_hash_handle), s);
        nb_etats_x++;
    }
    return s->id_interne;
}

typedef struct Transition_x {
    int cible;
    int action_id;
} Transition_x;

Transition_x** transitions_x = NULL;
int capacite_etats_x = 0;

void init_lts_x() {
    transitions_x = malloc(capacite_etats_x * sizeof(Transition_x*));
    for (int i = 0; i < capacite_etats_x; i++) {
        transitions_x[i] = NULL;
    }
}

void appliquer_transition1(int index_source) {
    Etat_x *s;
    for (s = etats_x_hash; s != NULL; s = s->hh.next) {
        if (s->id_interne == index_source) break;
    }
    if (!s) return;

    int etat_id = s->etat;
    int nb_transitions = nb_trans_par_etat[etat_id];
    transitions_x[index_source] = malloc(nb_transitions * sizeof(Transition_x));

    for (int i = 0; i < nb_transitions; i++) {
        Transition t = transitions[etat_id][i];
        Variable nouvelles_var = update_functions[t.label_action](s->var);
        int index_cible = ajouter_etat_x(t.etat_in, nouvelles_var);
        transitions_x[index_source][i].cible = index_cible;
        transitions_x[index_source][i].action_id = t.label_action;
    }
}

void appliquer_transitions() {
    int i = 0;
    while (i < nb_etats_x) {
        appliquer_transition1(i);
        i++;
    }
}

void print_lts_x() {
    Etat_x *s;
    for (s = etats_x_hash; s != NULL; s = s->hh.next) {
        int i = s->id_interne;
        printf("Etat etendu ID #%d\n", i);
        printf("  Etat de base    : %s (ID %d)\n", etats[s->etat], s->etat);
        printf("  Variables       : v = %d\n", s->var.v);
        printf("  Transitions sortantes :\n");

        for (int j = 0; j < nb_trans_par_etat[s->etat]; j++) {
            if (!transitions_x[i]) continue;
            int id_cible = transitions_x[i][j].cible;
            int action_id = transitions_x[i][j].action_id;
            printf("    [%d] --%s--> [%d]\n", i, actions[action_id], id_cible);
        }
        printf("\n");
    }
}

int main() {
    init_lts();
    init_variables();
    init_update_functions();
    capacite_etats_x = 45;
    init_lts_x();
    ajouter_etat_x(0, variable);


    appliquer_transitions();


    print_lts_x();
    printf("Total etats_x: %d\n", nb_etats_x);
    return 0;
}
