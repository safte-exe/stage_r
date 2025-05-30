#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_etats 50
#define max_actions 20
#define max_trans_par_etat 50
#define NB_VAR 1
#define nb_actions 3

// Définition de Variable avec champ dynamique
typedef struct Variable {
    int v;
    int* tab;
    int taille;
} Variable;

Variable create_variable(int taille) {
    Variable var;
    var.v = 0;
    var.taille = taille;
    var.tab = malloc(taille * sizeof(int));
    for (int i = 0; i < taille; i++) var.tab[i] = i + 1;
    return var;
}

Variable copy_variable(Variable original) {
    Variable copy;
    copy.v = original.v;
    copy.taille = original.taille;
    copy.tab = malloc(copy.taille * sizeof(int));
    memcpy(copy.tab, original.tab, original.taille * sizeof(int));
    return copy;
}

void free_variable(Variable* var) {
    if (var->tab) {
        free(var->tab);
        var->tab = NULL;
    }
    var->taille = 0;
}

// Définition des structures de LTS de base
char* etats[max_etats];
int num_etats;

char* actions[max_actions];

typedef struct Transition {
    int etat_in;
    int label_action;
} Transition;

Transition* transitions[max_etats];
int nb_trans_par_etat[max_etats] = {0};

int etat_exist(int s) {
    if (s < num_etats) return s;
    return -1;
}

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

void print_lts() {
    for (int i = 0; i < num_etats; i++) {
        printf("Etat etendu ID #%d\n", i);
        printf("\tNom: %s\n", etats[i]);
        printf("\tTransitions sortantes: %d\n", nb_trans_par_etat[i]);
        for (int j = 0; j < nb_trans_par_etat[i]; j++) {
            printf("\t\t[%d] --%s--> [%d]\n", i,
                   actions[transitions[i][j].label_action],
                   transitions[i][j].etat_in);
        }
        printf("\n");
    }
}

// Fonctions d'update
Variable update_a(Variable var) {
    if (var.v * 2 <= 10) var.v *= 2;
    return var;
}
Variable update_b(Variable var) {
    if (var.v + 2 <= 10) var.v += 2;
    return var;
}
Variable update_c(Variable var) {
    if (var.tab[0]<4){
        printf("l");
    }
    return var;
}

typedef Variable (*UpdateFunction)(Variable);
UpdateFunction update_functions[nb_actions];

void init_update_functions() {
    update_functions[0] = update_a;
    update_functions[1] = update_b;
    update_functions[2] = update_c;
}

// Définition Etat étendu
typedef struct Etat_x {
    int etat;
    Variable var;
} Etat_x;

Etat_x* etats_x = NULL;
int nb_etats_x = 0;
int capacite_etats_x = 0;

int ajouter_etat_x(int etat_id, Variable vars) {
    if (etats_x == NULL) {
        capacite_etats_x = 2;
        etats_x = malloc(capacite_etats_x * sizeof(Etat_x));
    } else if (nb_etats_x == capacite_etats_x) {
        capacite_etats_x *= 2;
        etats_x = realloc(etats_x, capacite_etats_x * sizeof(Etat_x));
    }

    etats_x[nb_etats_x].etat = etat_id;
    etats_x[nb_etats_x].var = copy_variable(vars);
    return nb_etats_x++;
}

int etat_x_exist(Etat_x s) {
    for (int i = 0; i < nb_etats_x; i++) {
        if (etats_x[i].etat == s.etat && etats_x[i].var.v == s.var.v)
            return i;
    }
    return -1;
}

// LTS étendu
typedef struct Transition_x {
    int cible;
    int action_id;
} Transition_x;

Transition_x** transitions_x = NULL;
int* nb_trans_par_etat_x = NULL;
int* capacite_trans_x = NULL;

void init_lts_x() {
    transitions_x = malloc(capacite_etats_x * sizeof(Transition_x*));
    nb_trans_par_etat_x = calloc(capacite_etats_x, sizeof(int));
    capacite_trans_x = calloc(capacite_etats_x, sizeof(int));
    for (int i = 0; i < capacite_etats_x; i++) {
        transitions_x[i] = NULL;
    }
}

void appliquer_transition1(int index_source) {
    Etat_x source = etats_x[index_source];
    int etat_id = source.etat;
    int nb_transitions = nb_trans_par_etat[etat_id];

    transitions_x[index_source] = malloc(nb_transitions * sizeof(Transition_x));
    nb_trans_par_etat_x[index_source] = nb_transitions;

    for (int i = 0; i < nb_transitions; i++) {
        Transition t = transitions[etat_id][i];
        Variable nouvelles_var = update_functions[t.label_action](source.var);

        Etat_x cible;
        cible.var = nouvelles_var;
        cible.etat = t.etat_in;

        int index_cible = etat_x_exist(cible);
        if (index_cible == -1) {
            index_cible = ajouter_etat_x(cible.etat, cible.var);
        }

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
    for (int i = 0; i < nb_etats_x; i++) {
        printf("Etat étendu ID #%d\n", i);
        printf("\tEtat de base    : %s (ID %d)\n", etats[etats_x[i].etat], etats_x[i].etat);
        printf("\tVariable        : v = %d | tab = [", etats_x[i].var.v);
        for (int k = 0; k < etats_x[i].var.taille; k++) {
            printf("%d ", etats_x[i].var.tab[k]);
        }
        printf("]\n");

        for (int j = 0; j < nb_trans_par_etat_x[i]; j++) {
            int id_cible = transitions_x[i][j].cible;
            int action_id = transitions_x[i][j].action_id;
            printf("\t\t[%d] --%s--> [%d]\n", i, actions[action_id], id_cible);
        }
        printf("\n");
    }
}

void free_all() {
    for (int i = 0; i < nb_etats_x; i++) {
        free_variable(&etats_x[i].var);
    }
    free(etats_x);
    for (int i = 0; i < num_etats; i++) {
        free(transitions[i]);
    }
    for (int i = 0; i < capacite_etats_x; i++) {
        if (transitions_x[i]) free(transitions_x[i]);
    }
    free(transitions_x);
    free(nb_trans_par_etat_x);
    free(capacite_trans_x);
}

// -------------------- MAIN --------------------
int main() {
    init_lts();
    init_update_functions();

    Variable variable = create_variable(5);
    variable.v = 1;

    capacite_etats_x = 45;
    init_lts_x();

    ajouter_etat_x(0, variable);
    appliquer_transitions();
    print_lts_x();

    printf("Nombre total d'états étendus : %d\n", nb_etats_x);

    free_variable(&variable);
    free_all();

    return 0;
}
