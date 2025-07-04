#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uthash.h"

// --------------------- TYPES -------------------------

typedef struct Variable {
    int v;
} Variable;

typedef Variable (*UpdateFunction)(Variable);
typedef bool (*Constraint)(Variable);

typedef struct Transition {
    int etat_in;
    int label_action;
} Transition;

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

#define TAILLE_MAX 1000

// ------------------ LTS STRUCTURE ---------------------

typedef struct {
    int nb_etats;
    int nb_actions;
    char** etats;
    char** actions;
    int* nb_trans_par_etat;
    Transition** transitions;
    Variable variable;
    UpdateFunction* update_functions;
    Constraint* constraints;
} LTS;

// ------------------ GLOBALS (used in original code) ---------------------

Etat_x* etats_x;
int nb_etats_x = 0;
int capacite_etats_x = TAILLE_MAX;

Transition_x** transitions_x;
int* nb_trans_x;
EtatXHash* etats_x_hash = NULL;

// ------------------ UPDATE FUNCTIONS ---------------------

Variable update_a(Variable var) { var.v += 2; return var; }
Variable update_b(Variable var) { var.v -= 1; return var; }
Variable update_c(Variable var) { var.v *= 2; return var; }

// ------------------ CONSTRAINT FUNCTIONS ---------------------

bool const_a(Variable var) { return true; }
bool const_b(Variable var) { return var.v > 0; }
bool const_c(Variable var) { return var.v <= 0; }

// ------------------ INIT LTS ---------------------

void init_lts(LTS* lts) {
    lts->nb_etats = 11;
    lts->nb_actions = 3;

    lts->etats = malloc(sizeof(char*) * lts->nb_etats);
    lts->actions = malloc(sizeof(char*) * lts->nb_actions);
    lts->nb_trans_par_etat = malloc(sizeof(int) * lts->nb_etats);
    lts->transitions = malloc(sizeof(Transition*) * lts->nb_etats);

    lts->etats[0] = "l0l1"; lts->etats[1] = "l1l3"; lts->etats[2] = "l0l40";
    lts->etats[3] = "l2l3"; lts->etats[4] = "l0l3";  lts->etats[5] = "l1l4";
    lts->etats[6] = "l0l3"; lts->etats[7] = "l2l4"; lts->etats[8] = "l0l41";
    lts->etats[9] = "l1l3"; lts->etats[10] = "l2l3";

    lts->actions[0] = "a"; lts->actions[1] = "b"; lts->actions[2] = "c";

    Transition* tr;
    tr = malloc(3 * sizeof(Transition)); tr[0]=(Transition){1,0}; tr[1]=(Transition){2,2}; tr[2]=(Transition){3,0}; lts->transitions[0]=tr; lts->nb_trans_par_etat[0]=3;
    tr = malloc(2 * sizeof(Transition)); tr[0]=(Transition){4,2}; tr[1]=(Transition){5,2}; lts->transitions[1]=tr; lts->nb_trans_par_etat[1]=2;
    tr = malloc(3 * sizeof(Transition)); tr[0]=(Transition){5,0}; tr[1]=(Transition){6,1}; tr[2]=(Transition){7,0}; lts->transitions[2]=tr; lts->nb_trans_par_etat[2]=3;
    tr = malloc(2 * sizeof(Transition)); tr[0]=(Transition){4,1}; tr[1]=(Transition){7,2}; lts->transitions[3]=tr; lts->nb_trans_par_etat[3]=2;
    tr = malloc(1 * sizeof(Transition)); tr[0]=(Transition){8,2}; lts->transitions[4]=tr; lts->nb_trans_par_etat[4]=1;
    tr = malloc(2 * sizeof(Transition)); tr[0]=(Transition){8,2}; tr[1]=(Transition){9,1}; lts->transitions[5]=tr; lts->nb_trans_par_etat[5]=2;
    tr = malloc(2 * sizeof(Transition)); tr[0]=(Transition){9,0}; tr[1]=(Transition){10,0}; lts->transitions[6]=tr; lts->nb_trans_par_etat[6]=2;
    tr = malloc(2 * sizeof(Transition)); tr[0]=(Transition){8,1}; tr[1]=(Transition){10,1}; lts->transitions[7]=tr; lts->nb_trans_par_etat[7]=2;
    tr = malloc(1 * sizeof(Transition)); tr[0]=(Transition){0,1}; lts->transitions[8]=tr; lts->nb_trans_par_etat[8]=1;
    tr = malloc(1 * sizeof(Transition)); tr[0]=(Transition){0,2}; lts->transitions[9]=tr; lts->nb_trans_par_etat[9]=1;
    tr = malloc(1 * sizeof(Transition)); tr[0]=(Transition){0,1}; lts->transitions[10]=tr; lts->nb_trans_par_etat[10]=1;

    lts->variable = (Variable){.v = 0};

    lts->update_functions = malloc(sizeof(UpdateFunction) * lts->nb_actions);
    lts->update_functions[0] = update_a;
    lts->update_functions[1] = update_b;
    lts->update_functions[2] = update_c;

    lts->constraints = malloc(sizeof(Constraint) * lts->nb_actions);
    lts->constraints[0] = const_a;
    lts->constraints[1] = const_b;
    lts->constraints[2] = const_c;
}

// ------------------ HELPER FUNCTIONS ---------------------

int ajouter_etat_x(int etat_id, Variable var) {
    EtatXKey key = {etat_id, var};
    EtatXHash* s;

    HASH_FIND(hh, etats_x_hash, &key, sizeof(EtatXKey), s);
    if (s) return s->index;

    if (nb_etats_x == capacite_etats_x) {
        capacite_etats_x *= 2;
        etats_x = realloc(etats_x, capacite_etats_x * sizeof(Etat_x));
        transitions_x = realloc(transitions_x, capacite_etats_x * sizeof(Transition_x*));
        nb_trans_x = realloc(nb_trans_x, capacite_etats_x * sizeof(int));
    }

    int idx = nb_etats_x;
    etats_x[idx] = (Etat_x){etat_id, var};
    transitions_x[idx] = NULL;
    nb_trans_x[idx] = 0;

    s = malloc(sizeof(EtatXHash));
    s->key = key;
    s->index = idx;
    HASH_ADD(hh, etats_x_hash, key, sizeof(EtatXKey), s);

    return nb_etats_x++;
}

void trans_x(LTS* lts, int i) {
    Etat_x source = etats_x[i];
    int etat_id = source.etat;
    Variable var = source.var;

    int n = lts->nb_trans_par_etat[etat_id];
    int nb_valides = 0;

    for (int k = 0; k < n; k++) {
        int action = lts->transitions[etat_id][k].label_action;
        if (lts->constraints[action](var)) nb_valides++;
    }

    if (nb_valides == 0) {
        transitions_x[i] = NULL;
        nb_trans_x[i] = 0;
        return;
    }

    transitions_x[i] = malloc(nb_valides * sizeof(Transition_x));
    nb_trans_x[i] = nb_valides;

    int j = 0;
    for (int k = 0; k < n; k++) {
        Transition t = lts->transitions[etat_id][k];
        int action = t.label_action;
        if (!lts->constraints[action](var)) continue;

        Variable new_var = lts->update_functions[action](var);
        int target = ajouter_etat_x(t.etat_in, new_var);
        transitions_x[i][j++] = (Transition_x){target, action};
    }
}

// ------------------ NEW: BUILD FUNCTION ---------------------

void build(LTS* lts) {
    etats_x = malloc(capacite_etats_x * sizeof(Etat_x));
    transitions_x = malloc(capacite_etats_x * sizeof(Transition_x*));
    nb_trans_x = malloc(capacite_etats_x * sizeof(int));

    nb_etats_x = 0;
    etats_x_hash = NULL;

    ajouter_etat_x(0, lts->variable);  // état initial

    int i = 0;
    while (i < nb_etats_x) {
        trans_x(lts, i);
        i++;
    }
}

// ------------------ MAIN ---------------------

int main() {
    LTS lts;
    init_lts(&lts);

    build(&lts);

    printf("Nombre total d'états étendus : %d\n", nb_etats_x);
    return 0;
}
