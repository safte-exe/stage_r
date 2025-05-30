#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_etats 50
#define max_actions 20
#define max_trans_par_etat 50
#define NB_VAR 3
#define nb_actions 4

char* etats[max_etats];
int num_etats;

char* actions[max_actions];

typedef struct Transition {
    int etat_in;
    int label_action;
} Transition;

Transition* transitions[max_etats];
int nb_trans_par_etat[max_etats] = {0};

int etat_exist(char* s) {
    for (int i = 0; i < num_etats; i++) {
        if (strcmp(s, etats[i]) == 0) return i;
    }
    return -1;
}

void init_lts() {
    etats[0] = "s1";
    etats[1] = "s2";
    etats[2] = "s3";
    num_etats = 3;
    actions[0] = "a";
    actions[1] = "b";

    transitions[0] = malloc(2 * sizeof(Transition));
    transitions[0][0] = (Transition){.etat_in = 1, .label_action = 0};
    transitions[0][1] = (Transition){.etat_in = 2, .label_action = 1};
    nb_trans_par_etat[0] = 2;

    transitions[1] = malloc(1 * sizeof(Transition));
    transitions[1][0] = (Transition){.etat_in = 2, .label_action = 0};
    nb_trans_par_etat[1] = 1;

    nb_trans_par_etat[2] = 0;
}

void print_lts(){
    int i, j;
    for (i=0; i< num_etats; i++){
            printf("L'etat %d", i);
            printf("  dit : %s", etats[i] );
             printf("   a %d transitions sortantes %s :\n", nb_trans_par_etat[i], etats[i] );
        for (j=0; j< nb_trans_par_etat[i] ; j++){
            printf("\t\t%s --%s--> %s \n", etats[i], actions[transitions[i][j].label_action], etats[transitions[i][j].etat_in]);
        }
    }

}

typedef struct Variable {
    int x, y, z;
} Variable;

Variable variable;

void init_variables() {
    variable.x = 0;
    variable.y = 9;
    variable.z = 1;
}

void update_a() { variable.x++; variable.y--; }
void update_b() { variable.y--; }
void update_c() { variable.x *= 2; }
void update_d() { variable.z = (variable.z + 1) % 2; }

typedef void (*UpdateFunction)();
UpdateFunction update_functions[nb_actions];

void init_update_functions() {
    update_functions[0] = update_a;
    update_functions[1] = update_b;
    update_functions[2] = update_c;
    update_functions[3] = update_d;
}

typedef struct Etat_x {
    char* etat;
    Variable var;
} Etat_x;

Etat_x* etats_x = NULL;
int nb_etats_x = 0;
int capacite_etats_x = 0;

int ajouter_etat_x(char* etat_nom, Variable vars) {
    if (nb_etats_x == capacite_etats_x) {
        capacite_etats_x = capacite_etats_x == 0 ? 2 : capacite_etats_x * 2;
        etats_x = realloc(etats_x, capacite_etats_x * sizeof(Etat_x));
    }
    etats_x[nb_etats_x].etat = malloc(strlen(etat_nom) + 1);
    strcpy(etats_x[nb_etats_x].etat, etat_nom);
    etats_x[nb_etats_x].var = vars;
    printf("Nombre d'etats etendus %d\n za  ", nb_etats_x);
    return nb_etats_x++;
}

int etat_x_exist(Etat_x s) {
    for (int i = 0; i < nb_etats_x; i++) {
        if (strcmp(etats_x[i].etat, s.etat) == 0 &&
            etats_x[i].var.x == s.var.x &&
            etats_x[i].var.y == s.var.y &&
            etats_x[i].var.z == s.var.z)
            return i;
    }
    return -1;
}

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

void appliquer_transitions1(int index_source) {
    Etat_x source = etats_x[index_source];
    int etat_id = etat_exist(source.etat);
    if (etat_id == -1) return;

    for (int i = 0; i < nb_trans_par_etat[etat_id]; i++) {
        Transition t = transitions[etat_id][i];
        int action_id = t.label_action;
        int etat_cible_id = t.etat_in;

        Variable nouvelles_var = source.var;
        variable = nouvelles_var;
        if (update_functions[action_id]) update_functions[action_id]();
        nouvelles_var = variable;

        Etat_x cible;
        cible.etat = etats[etat_cible_id];
        cible.var = nouvelles_var;

        int index_cible = etat_x_exist(cible);
        if (index_cible == -1)
            index_cible = ajouter_etat_x(cible.etat, cible.var);

        if (transitions_x[index_source] == NULL) {
            capacite_trans_x[index_source] = 2;
            transitions_x[index_source] = malloc(2 * sizeof(Transition_x));
        }

        if (nb_trans_par_etat_x[index_source] >= capacite_trans_x[index_source]) {
            capacite_trans_x[index_source] *= 2;
            transitions_x[index_source] = realloc(transitions_x[index_source],
                capacite_trans_x[index_source] * sizeof(Transition_x));
        }

        transitions_x[index_source][nb_trans_par_etat_x[index_source]].cible = index_cible;
        transitions_x[index_source][nb_trans_par_etat_x[index_source]].action_id = action_id;
        nb_trans_par_etat_x[index_source]++;
    }
}

void print_lts_x() {
    for (int i = 0; i < nb_etats_x; i++) {
        printf("Depuis l'etat %s :\n", etats_x[i].etat);
        printf("\tVariables : x = %d, y = %d, z = %d\n", etats_x[i].var.x, etats_x[i].var.y, etats_x[i].var.z);
        for (int j = 0; j < nb_trans_par_etat_x[i]; j++) {
            printf("\t%s --%s--> %s\n", etats_x[i].etat, actions[transitions_x[i][j].action_id], etats_x[transitions_x[i][j].cible].etat);
        }
    }
}

// -------------------- MAIN --------------------

int main() {
    init_lts();
    print_lts();
    init_variables();
    init_update_functions();

    // Ajoute état initial s1
    ajouter_etat_x("s1", variable);
    capacite_etats_x = 4;
    init_lts_x();

    // Appliquer les transitions depuis s1
    appliquer_transitions1(0);

    // Affichage
    print_lts_x();

    return 0;
}
