#ifndef STRUCTURE_LTS_H
#define STRUCTURE_LTS_H

#include "structure_variable.h"

typedef Variable (*UpdateFunction)(Variable);
typedef bool (*Constraint)(Variable);

typedef struct Transition {
    int etat_in;
    int label_action;
} Transition;

typedef struct {
    char** etats;  // pointeurs vers vos noms d’états
    char** actions;
    int* nb_trans_par_etat;
    Transition** transitions;
    Variable variable;
    UpdateFunction* update_functions;
    Constraint* constraints;
} LTS;

#endif
