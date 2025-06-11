#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "uthash.h"

#define max_etats 50
#define max_actions 20
#define max_trans_par_etat 50
#define NB_VAR 1
#define nb_actions 3

// Definition du LTS de base 
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
    etats[0] = "l0l1"; etats[1] = "l1l3"; etats[2] = "l0l40";
    etats[3] = "l2l3"; etats[4] = "l0l3"; etats[5] = "l1l4";
    etats[6] = "l0l3"; etats[7] = "l2l4"; etats[8] = "l0l41";
    etats[9] = "l1l3"; etats[10] = "l2l3";

    num_etats = 11;
    actions[0] = "a"; actions[1] = "b"; actions[2] = "c";

    transitions[0] = malloc(3 * sizeof(Transition));
    transitions[0][0] = (Transition){1, 0}; transitions[0][1] = (Transition){2, 2}; transitions[0][2] = (Transition){3, 0};
    nb_trans_par_etat[0] = 3;

    transitions[1] = malloc(2 * sizeof(Transition));
    transitions[1][0] = (Transition){4, 2}; transitions[1][1] = (Transition){5, 2};
    nb_trans_par_etat[1] = 2;

    transitions[2] = malloc(3 * sizeof(Transition));
    transitions[2][0] = (Transition){5, 0}; transitions[2][1] = (Transition){6, 1}; transitions[2][2] = (Transition){7, 0};
    nb_trans_par_etat[2] = 3;

    transitions[3] = malloc(2 * sizeof(Transition));
    transitions[3][0] = (Transition){4, 1}; transitions[3][1] = (Transition){7, 2};
    nb_trans_par_etat[3] = 2;

    transitions[4] = malloc(1 * sizeof(Transition));
    transitions[4][0] = (Transition){8, 2};
    nb_trans_par_etat[4] = 1;

    transitions[5] = malloc(2 * sizeof(Transition));
    transitions[5][0] = (Transition){8, 2}; transitions[5][1] = (Transition){9, 1};
    nb_trans_par_etat[5] = 2;

    transitions[6] = malloc(2 * sizeof(Transition));
    transitions[6][0] = (Transition){9, 0}; transitions[6][1] = (Transition){10, 0};
    nb_trans_par_etat[6] = 2;

    transitions[7] = malloc(2 * sizeof(Transition));
    transitions[7][0] = (Transition){8, 1}; transitions[7][1] = (Transition){10, 1};
    nb_trans_par_etat[7] = 2;

    transitions[8] = malloc(1 * sizeof(Transition));
    transitions[8][0] = (Transition){0, 1};
    nb_trans_par_etat[8] = 1;

    transitions[9] = malloc(1 * sizeof(Transition));
    transitions[9][0] = (Transition){0, 2};
    nb_trans_par_etat[9] = 1;

    transitions[10] = malloc(1 * sizeof(Transition));
    transitions[10][0] = (Transition){0, 1};
    nb_trans_par_etat[10] = 1;
}

// Définition des variables
typedef struct Variable {
    int v;
} Variable;

Variable variable;

void init_variables() { variable.v = 0; }

Variable update_a(Variable var) { if (var.v + 2 <= 500000 && var.v + 2 >= -500000) var.v += 2; return var; }
Variable update_b(Variable var) { if (var.v - 1 <= 500000 && var.v - 1 >= -500000) var.v -= 1; return var; }
Variable update_c(Variable var) { if (var.v * 2 <= 500000 && var.v * 2 >= -500000) var.v *= 2; return var; }



typedef Variable (*UpdateFunction)(Variable);
UpdateFunction update_functions[nb_actions];

void init_update_functions() {
    update_functions[0] = update_a;
    update_functions[1] = update_b;
    update_functions[2] = update_c;
}

//Défenition des contraintes 

bool const_a(Variable var) { return true; }
bool const_b(Variable var) { if (var.v > 0) return true; return false;}
bool const_c(Variable var) { if (var.v <= 0) return true; return false;}


typedef bool (*Constraint)(Variable);
Constraint constraints[nb_actions];

void init_constraints() {
    constraints[0] = const_a;
    constraints[1] = const_b;
    constraints[2] = const_c;
}


int nb_trans_applicables(int etat, Variable var){
    int val = 0;
    for (int i = 0; i < nb_trans_par_etat[etat]; i++) {
        int action = transitions[etat][i].label_action;
        if (constraints[action](var)) {
            val++;
        }
    }
    return val;
}




// Définition de l'état étendu
typedef struct Etat_x {
    int etat;
    Variable var;
} Etat_x;

Etat_x* etats_x = NULL;
int nb_etats_x = 0;
int capacite_etats_x = 0;

// Table de hachage
typedef struct {
    int etat;
    Variable v;
} EtatXKey;

typedef struct {
    EtatXKey key;
    int index;
    UT_hash_handle hh;
} EtatXHash;

EtatXHash* etats_x_hash = NULL;

int ajouter_etat_x(int etat_id, Variable vars) {
    EtatXKey key = {etat_id, vars};
    EtatXHash* s;

    HASH_FIND(hh, etats_x_hash, &key, sizeof(EtatXKey), s);
    if (s) return s->index;

    if (etats_x == NULL) {
        capacite_etats_x = 2;
        etats_x = malloc(capacite_etats_x * sizeof(Etat_x));
    } else if (nb_etats_x == capacite_etats_x) {
        capacite_etats_x *= 2;
        etats_x = realloc(etats_x, capacite_etats_x * sizeof(Etat_x));
    }

    etats_x[nb_etats_x].etat = etat_id;
    etats_x[nb_etats_x].var = vars;

    s = malloc(sizeof(EtatXHash));
    s->key = key;
    s->index = nb_etats_x;
    HASH_ADD(hh, etats_x_hash, key, sizeof(EtatXKey), s);

    return nb_etats_x++;
}

// Transitions étendues
typedef struct Transition_x {
    int cible;
    int action_id;
} Transition_x;

Transition_x** transitions_x = NULL;

void init_lts_x() {
    transitions_x = malloc(capacite_etats_x * sizeof(Transition_x*));
    for (int i = 0; i < capacite_etats_x; i++) {
        transitions_x[i] = NULL;
    }
}


void appliquer_transition1(int index_source) {
    int nb_transitions = nb_trans_par_etat[etat_id];
transitions_x[index_source] = malloc(nb_transitions * sizeof(Transition_x));
int j = 0;
for (int i = 0; i < nb_transitions; i++) {
    Transition t = transitions[etat_id][i];
    int action = t.label_action;
    if (constraints[action](source.var)) {
        Variable nouvelles_var = update_functions[action](source.var);
        int index_cible = ajouter_etat_x(t.etat_in, nouvelles_var);

        transitions_x[index_source][j].cible = index_cible;
        transitions_x[index_source][j].action_id = action;
        j++;
    }
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
    for (int i = 0; i < nb_etats_x; i++) {
        printf("Etat etendu ID #%d\n", i);
        printf("\t Etat de base    : %s (ID %d)\n", etats[etats_x[i].etat], etats_x[i].etat);
        printf("\t Variables       : v = %d\n", etats_x[i].var.v);
        printf("\t Transitions sortantes :\n");

        for (int j = 0; j < nb_trans_par_etat[etats_x[i].etat]; j++) {
            int id_cible = transitions_x[i][j].cible;
            int action_id = transitions_x[i][j].action_id;
            printf("\t\t   [%d] --%s--> [%d] \n",
                   i,
                   actions[action_id],
                   id_cible);
        }
        printf("\n");
    }
}

// MAIN
int main() {
    init_lts();
    init_variables();
    init_update_functions();

    capacite_etats_x = 1000000000;
    init_lts_x();
    ajouter_etat_x(0, variable);
    appliquer_transitions();
   // print_lts_x();
    printf("Nombre total d'états étendus : %d\n", nb_etats_x);

    return 0;
}