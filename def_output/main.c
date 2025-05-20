#include <stdio.h>
#include <stdlib.h>


// ############################ DEFINITION DU LTS #######################################################

#define max_etats  50
#define max_actions  20
#define max_trans_par_etat  50


char* etats[max_etats];
int num_etats = 0;


char* actions[max_actions];
int num_actions = 0;

typedef struct Transition {
    int etat_in;
    int action_id;
} Transition;

Transition* transitions [max_etats];
int nb_trans_par_etat [max_etats] = {0} ;


void init_lts () {
    etats[0] = "s1";
    etats[1] = "s2";
    etats[2] = "s3";
    num_etats = 3;
    actions[0] = "a";
    actions[1] = "b";
    num_actions = 2;

    transitions [0] = malloc (2 * sizeof(Transition));
    transitions [0][0] = (Transition){.etat_in = 1, .action_id = 0 };
    transitions [0][1] = (Transition){.etat_in = 2, .action_id = 1 };
    nb_trans_par_etat[0] = 2;

    transitions [1] = malloc (1 * sizeof(Transition));
    transitions [1][0] = (Transition){.etat_in = 2, .action_id = 0 };
    nb_trans_par_etat[1] = 1;


    nb_trans_par_etat[2] = 0;

}

void print_lts(){
    int i, j;
    for (i=0; i< num_etats; i++){
             printf("%d transitions depuis l'etat %s :\n", nb_trans_par_etat[i], etats[i] );
        for (j=0; j< nb_trans_par_etat[i] ; j++){
            printf("%s --%s--> %s \n", etats[i], actions[transitions[i][j].action_id], etats[transitions[i][j].etat_in]);
        }
    }

}



// ############################ DEFINITION DES VARIBLES #######################################################


#define NB_VAR 4

typedef struct {
    char* name;
    int value;
} Variable;


Variable variables[NB_VAR];


void init_variables() {
    variables[0] = (Variable){"a", 0};
    variables[1] = (Variable){"b", 1};
    variables[2] = (Variable){"c", 5};
    variables[3] = (Variable){"d", 0};

}

void update_a () {
        variables[0].value++;
}

void update_b() {
        variables[1].value--;
}

void update_c() {
            variables[2].value  =   variables[2].value * 2;
}


void update_d() {
            variables[3].value = (variables[3].value + 1) % 2;
}

typedef void (*UpdateFunction)();
 UpdateFunction update_functions[NB_VAR];


void init_update_functions() {
    update_functions[0] = update_a;
    update_functions[1] = update_b;
    update_functions[2] = update_c;
    update_functions[3] = update_d;
}


void update(int index) {
    if (index >= 0 && index < NB_VAR) {
        update_functions[index]();
    } else {
        printf("index invalide: %d\n", index);
    }
}

void print_variables() {
    for (int i = 0; i < NB_VAR; i++) {
        printf("%s = %d\n", variables[i].name, variables[i].value);
    }
}



// ############################ DEFINITION DE EXTENDED STATES #######################################################

typedef struct Etat_x {
    int etat;
    Variable variables[NB_VAR];
}Etat_x;

Etat_x* etats_x = NULL;
int nb_etats_x = 0;
int capacite_etats_x = 10;

void ajouter_etat_x(int etat, Variable* vars) {
    if (nb_etats_x == capacite_etats_x) {
        capacite_etats_x =  capacite_etats_x * 2;
        etats_x = realloc(etats_x, capacite_etats_x * sizeof(Etat_x));
    }

    etats_x[nb_etats_x].etat = etat;
    for (int i = 0; i < NB_VAR; i++) {
        etats_x[nb_etats_x].variables[i].nom = vars[i].nom;
        etats_x[nb_etats_x].variables[i].valeur = vars[i].valeur;
    }
    nb_etats_x++;
}

void init_etats_x () {

}


//// transitions depuis extended etats 

typedef struct Transition_x {
    Etat_x cible;
    int action_id;
}

Transition_x** transitions_x = NULL;
int* nb_trans_par_etat_x ;
int* capacite_trans_x = NULL;

void init_lts_x () {

}























// ########################################### MAIN ###########################################################


int main()
{

    init_lts();
    print_lts();


    init_variables();
    print_variables();
    init_update_functions();

    update(0);
    update(1);
    update(2);
    update(3);

    print_variables();

    return 0;

}
