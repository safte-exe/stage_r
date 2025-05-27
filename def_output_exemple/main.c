#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int etat_exist(int s) {
   /*
    for (int i = 0; i < num_etats; i++) {
        if (s  == i) return i;
    }
    */

    if ( s < num_etats) {return s ;}
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

void print_lts(){
    int i, j;
    for (i=0; i< num_etats; i++){
        printf("Etat etendu ID #%d\n", i);
        printf("\tNom: %s\n", etats[i] );
             printf("\tTransitions sortantes: %d \n", nb_trans_par_etat[i] );
        for (j=0; j< nb_trans_par_etat[i] ; j++){
            printf("\t\t[%d] --%s--> [%d] \n", i, actions[transitions[i][j].label_action], transitions[i][j].etat_in);
        }
        printf("\n");
    }

}




// Definitiion des variables 

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


// Definiton d'un etat etendu

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

    // printf("HAlo\n");
   // printf("je suis à l'etat %d \n ", nb_etats_x);
    //printf("je passe en param l'id  %d \n ", etat_id);
    etats_x[nb_etats_x].etat = etat_id ;
    // printf("HAlo\n");
    etats_x[nb_etats_x].var = vars;
    //printf("Nombre d'etats etendus %d\n ", nb_etats_x);
    // printf("HAlo\n");

    return nb_etats_x++;
}


int etat_x_exist(Etat_x s) {
    for (int i = 0; i < nb_etats_x; i++) {
        if (etats_x[i].etat  == s.etat &&
            etats_x[i].var.v == s.var.v )
            return i;
    }
    return -1;
}


// Definiton du LTS etendu

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


void appliquer_transition(int index_source) {
    Etat_x source = etats_x[index_source];
    int etat_id = etat_exist(source.etat);
    //printf("Etat de LTS org %d\n", etat_id);
    if (etat_id == -1) return;

    for (int i = 0; i < nb_trans_par_etat[etat_id]; i++) {
        Transition t = transitions[etat_id][i];
        //int action_id = t.label_action;
        //int etat_cible_id = t.etat_in;



        Etat_x cible;
        // à revoir : allouer espace dedié 
        Variable nouvelles_var = source.var;
       // variable = nouvelles_var;
        cible.var = update_functions[t.label_action](nouvelles_var);
        //nouvelles_var = variable;

        
        cible.etat = t.etat_in;
        // cible.var = nouvelles_var;

        int index_cible = etat_x_exist(cible);
        
     //   printf("Etat cible de LTS org %d\n", index_cible);

        if (index_cible == -1)
            index_cible = ajouter_etat_x(cible.etat, cible.var);
        //    printf("nouvel etat ajouté cible de LTS org %d\n", index_cible);


        if (transitions_x[index_source] == NULL) {

            capacite_trans_x[index_source] = 2;
            transitions_x[index_source] = malloc(2 * sizeof(Transition_x));
        }

        // à revoir

        if (nb_trans_par_etat_x[index_source] >= capacite_trans_x[index_source]) {
            capacite_trans_x[index_source] *= 2;
            transitions_x[index_source] = realloc(transitions_x[index_source],
                capacite_trans_x[index_source] * sizeof(Transition_x));
        }

        transitions_x[index_source][nb_trans_par_etat_x[index_source]].cible = index_cible;
        transitions_x[index_source][nb_trans_par_etat_x[index_source]].action_id = t.label_action;
        nb_trans_par_etat_x[index_source]++;
    //    printf("nb trna spour l'etat %d est : %d \n",i, nb_trans_par_etat_x[index_source]);
    }
}


void appliquer_transitions() {
    int i = 0;
    while (i < nb_etats_x) {
        appliquer_transition(i);
        i++;
    }
}



void print_lts_x() {
    for (int i = 0; i < nb_etats_x; i++) {
        
        printf(" Etat etendu ID #%d\n", i);
        printf("\t Etat de base    : %s (ID %d)\n", etats[etats_x[i].etat], etats_x[i].etat);
        printf("\t Variables       : v = %d\n", etats_x[i].var.v);
        printf("\t Transitions sortantes :\n");

        if (nb_trans_par_etat_x[i] == 0) {
            printf("\t   (aucune transition)\n");
        }

        for (int j = 0; j < nb_trans_par_etat_x[i]; j++) {
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

// -------------------- MAIN --------------------

int main() {
    init_lts();
    //print_lts();
    init_variables();
    init_update_functions();

 //   printf("A\n");
capacite_etats_x = 45;

// printf("B\n");
init_lts_x();

//printf("C\n");
ajouter_etat_x(0, variable);

//printf("D\n");
 // print_lts();

//printf("E\n");
//printf("voila l'etat xtended num 1 %d\n", etats_x[0].etat);


  //  printf("########################################\n");

    // Appliquer les transitions depuis s1
   // appliquer_transition(0);
   // print_lts_x();
  //  printf("########################################\n");


  //  appliquer_transition(1);
  //  print_lts_x();
  //  printf("########################################\n");

    
 //   appliquer_transition(2);
   // print_lts_x();
   // printf("########################################\n");


  //  appliquer_transition(1);

    // Affichage
  //  print_lts_x();
   appliquer_transitions();
   print_lts_x();
   printf("%d", nb_etats_x);


    return 0;
}
