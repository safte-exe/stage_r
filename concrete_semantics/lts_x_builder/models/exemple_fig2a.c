#include <stdlib.h>
#include <stdbool.h>
#include "../structure_lts.h"
////// Definition du modele

////Declaration des variables 

//La structure des variables pour le modele actuel est la suivante 
/*
typedef struct Variable {
    int v;
} Variable;
*/


char** etats;                     // Etats du LTS
char** actions;                   // Actions du LTS
int* nb_trans_par_etat;          // Nombre de transitions sortantes par état
Transition** transitions;        // Transitions sortantes de chaque état
Variable variable;               // Variable de données
UpdateFunction* update_functions; // Fonctions d'update
Constraint* constraints;         // Contraintes

////Initialisation LTS 
void init_lts() {

    int nb_etats = 11;
    int nb_actions = 3;

    etats = malloc(nb_etats * sizeof(char*));
    actions = malloc(nb_actions * sizeof(char*));
    nb_trans_par_etat = malloc(nb_etats * sizeof(int));
    transitions = malloc(nb_etats * sizeof(Transition*));
    update_functions = malloc(nb_actions * sizeof(UpdateFunction));
    constraints = malloc(nb_actions * sizeof(Constraint));

    //Initialisation etats
    etats[0] = "l0l1";
    etats[1] = "l1l3";
    etats[2] = "l0l40";
    etats[3] = "l2l3";
    etats[4] = "l0l3";
    etats[5] = "l1l4";
    etats[6] = "l0l3";
    etats[7] = "l2l4";
    etats[8] = "l0l41";
    etats[9] = "l1l3";
    etats[10] = "l2l3";

    //Initialisation actions
    actions[0] = "a";
    actions[1] = "b";
    actions[2] = "c";

    //Initialisation transitions
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

////Initialisation data 

//Initialisation data variables
void init_variables() { 
    variable.v = 0;
}

//Initialisation des fonctions
Variable update_a(Variable var) { if (var.v + 2 <= 50 && var.v + 2 >= -50) var.v += 2; return var; }
Variable update_b(Variable var) { if (var.v - 1 <= 50 && var.v - 1 >= -50) var.v -= 1; return var; }
Variable update_c(Variable var) { if (var.v * 2 <= 50 && var.v * 2 >= -50) var.v *= 2; return var; }

void init_update_functions() {
    update_functions[0] = update_a;
    update_functions[1] = update_b;
    update_functions[2] = update_c;
}

//Initialisation des contraintes
bool const_a(Variable var) { return true; }
bool const_b(Variable var) { if (var.v > 0) return true; return false;}
bool const_c(Variable var) { if (var.v <= 0) return true; return false;}


void init_constraints() {
    constraints[0] = const_a;
    constraints[1] = const_b;
    constraints[2] = const_c;
}


// --------------------- Remplir la structure LTS ---------------------

void fill_lts_struct(LTS* lts) {
    init_lts();
    init_variables();
    init_update_functions();
    init_constraints();

    lts->etats = etats;
    lts->actions = actions;
    lts->nb_trans_par_etat = nb_trans_par_etat;
    lts->transitions = transitions;
    lts->variable = variable;
    lts->update_functions = update_functions;
    lts->constraints = constraints;
}
