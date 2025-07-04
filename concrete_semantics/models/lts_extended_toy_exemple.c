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
    
    int nb_etats = 4;
    int nb_actions = 3;

    etats = malloc(nb_etats * sizeof(char*));
    actions = malloc(nb_actions * sizeof(char*));
    nb_trans_par_etat = malloc(nb_etats * sizeof(int));
    transitions = malloc(nb_etats * sizeof(Transition*));
    update_functions = malloc(nb_actions * sizeof(UpdateFunction));
    constraints = malloc(nb_actions * sizeof(Constraint));

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


////Initialisation data 

//Initialisation data variables
void init_variables() { 
    variable.v = 0;
}

//Initialisation des fonctions
Variable update_a( Variable var ) { if (var.v * 2 <= 10 ) { var.v *= 2;} return var; }
Variable update_b( Variable var ) { if (var.v + 2 <= 10 ) { var.v += 2;} return var; }
Variable update_c( Variable var ) { return var; }


void init_update_functions() {
    update_functions[0] = update_a;
    update_functions[1] = update_b;
    update_functions[2] = update_c;
}

//Initialisation des contraintes
bool const_a(Variable var) { return true; }
bool const_b(Variable var) { return true;}
bool const_c(Variable var) { return true;}


void init_constraints() {
    constraints[0] = const_a;
    constraints[1] = const_b;
    constraints[2] = const_c;
}

void fill_lts_struct(LTS* lts) {
    lts->nb_etats = nb_etats;
    lts->nb_actions = nb_actions;
    lts->etats = etats;
    lts->actions = actions;
    lts->nb_trans_par_etat = nb_trans_par_etat;
    lts->transitions = transitions;
    lts->variable = variable;
    lts->update_functions = update_functions;
    lts->constraints = constraints;
}
