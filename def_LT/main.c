#include <stdio.h>
#include <stdlib.h>

#define max_etats  50
#define max_actions  20
#define max_trans_par_etat  50


char* etats[max_etats];
int num_etats = 0;


char* actions[max_actions];
int num_actions = 0;

typedef struct Transition {
    int etat_in;
    int label_action;
} Transition;

Transition* transitions [max_etats];
int nb_trans_par_etat [max_etats] = {0} ;


void init_ () {
    etats[0] = "s1";
    etats[1] = "s2";
    etats[2] = "s3";
    num_etats = 3;
    actions[0] = "a";
    actions[1] = "b";
    num_actions = 2;

    transitions [0] = malloc (2 * sizeof(Transition));
    transitions [0][0] = (Transition){.etat_in = 1, .label_action = 0 };
    transitions [0][1] = (Transition){.etat_in = 2, .label_action = 1 };
    nb_trans_par_etat[0] = 2;

    transitions [1] = malloc (1 * sizeof(Transition));
    transitions [1][0] = (Transition){.etat_in = 2, .label_action = 0 };
    nb_trans_par_etat[1] = 1;


    nb_trans_par_etat[2] = 0;

}

void print_lts(){
    int i, j;
    for (i=0; i< num_etats; i++){
             printf("%d transitions depuis l'etat %s :\n", nb_trans_par_etat[i], etats[i] );
        for (j=0; j< nb_trans_par_etat[i] ; j++){
            printf("%s --%s--> %s \n", etats[i], actions[transitions[i][j].label_action], etats[transitions[i][j].etat_in]);
        }
    }

}

void main(){
    init_();
    print_lts();
}
