#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "uthash.h"

#define max_etats 50
#define max_actions 20
#define max_trans_par_etat 50
#define VAR_COUNT 1
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

// Gestion des intervalles et valuations

typedef struct {
    int lower;
    int upper;
    bool lower_closed;
    bool upper_closed;
} Interval;

typedef struct {
    Interval vars[VAR_COUNT];
} Valuation;

// Intersections

Interval intersect_interval(Interval a, Interval b) {
    Interval result;

    result.lower = (a.lower > b.lower) ? a.lower : b.lower;
    result.upper = (a.upper < b.upper) ? a.upper : b.upper;

    if (a.lower == b.lower)
        result.lower_closed = a.lower_closed && b.lower_closed;
    else if (result.lower == a.lower)
        result.lower_closed = a.lower_closed;
    else
        result.lower_closed = b.lower_closed;

    if (a.upper == b.upper)
        result.upper_closed = a.upper_closed && b.upper_closed;
    else if (result.upper == a.upper)
        result.upper_closed = a.upper_closed;
    else
        result.upper_closed = b.upper_closed;

    return result;
}

Valuation intersect_valuation(Valuation* v1, Valuation* v2) {
    Valuation out;
    for (int i = 0; i < VAR_COUNT; i++) {
        out.vars[i] = intersect_interval(v1->vars[i], v2->vars[i]);
    }
    return out;
}

bool is_empty_interval(Interval ivl) {
    if (ivl.lower > ivl.upper) return true;
    if (ivl.lower == ivl.upper && (!ivl.lower_closed || !ivl.upper_closed)) return true;
    return false;
}

bool is_empty_valuation(Valuation* v) {
    for (int i = 0; i < VAR_COUNT; i++) {
        if (is_empty_interval(v->vars[i])) return true;
    }
    return false;
}

// Fonctions d'update


Valuation update_a(Valuation* in) {
    Valuation out = *in;
    out.vars[0].lower += 2;
    out.vars[0].upper += 2;
    return out;
}

Valuation update_b(Valuation* in) {
    Valuation out = *in;
    out.vars[0].lower -= 1;
    out.vars[0].upper -= 1;
    return out;
}

Valuation update_c(Valuation* in) {
    Valuation out = *in;
    out.vars[0].lower *= 2;
    out.vars[0].upper *= 2;
    return out;
}

typedef Valuation (*UpdateFunction)(Valuation*);

UpdateFunction update_functions[nb_actions] = {update_a, update_b, update_c};



// Contraintes sous forme de valuations 


Valuation const_a() {
    Valuation v;
    v.vars[0] = (Interval){-10, 10, true, true};
    return v;
}

Valuation const_b() {
    Valuation v;
    v.vars[0] = (Interval){-10, 10, true, true};
    return v;
}

Valuation const_c() {
    Valuation v;
    v.vars[0] = (Interval){-10, 10, true, true};
    return v;
}

typedef Valuation (*Constraint)();
Constraint constraints[nb_actions] = {const_a, const_b, const_c};


// Definition de l'état étendu
typedef struct Etat_x {
    int etat;
    Valuation val;
} Etat_x;

Etat_x* etats_x = NULL;
int nb_etats_x = 0;
int capacite_etats_x ;

// Table de hachage
typedef struct {
    int etat;
    Valuation val;
} EtatXKey;

typedef struct {
    EtatXKey key;
    int index;
    UT_hash_handle hh;
} EtatXHash;

EtatXHash* etats_x_hash = NULL;

int ajouter_etat_x(int etat_id, Valuation val) {
    EtatXKey key = {etat_id, val};
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
    etats_x[nb_etats_x].val = val;

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
int* nb_trans_x = NULL;

void init_lts_x() {
    
    
    transitions_x = malloc(capacite_etats_x * sizeof(Transition_x*));
    nb_trans_x = malloc(capacite_etats_x * sizeof(int));
    for (int i = 0; i < capacite_etats_x; i++) {
        transitions_x[i] = NULL;
        nb_trans_x[i] = 0;
    }
}

bool is_transition_applicable(int etat, Valuation* val, int action) {
    Valuation garde = constraints[action]();
    Valuation inter = intersect_valuation(val, &garde);
    return !is_empty_valuation(&inter);
}

void appliquer_transition1(int index_source) {


    Etat_x source = etats_x[index_source];
    int etat_id = source.etat;
    int nb_transitions = nb_trans_par_etat[etat_id];

    
    int nb_valides = 0;
    for (int i = 0; i < nb_transitions; i++) {
        int action = transitions[etat_id][i].label_action;
        if (is_transition_applicable(etat_id, &source.val, action)) {
            nb_valides++;
        }
    }
    
    nb_trans_x[index_source] = nb_valides;

    

    if (nb_valides == 0) {
        transitions_x[index_source] = NULL;
        return;
    }

    
    transitions_x[index_source] = malloc(nb_valides * sizeof(Transition_x));

    
    int k = 0;
    for (int i = 0; i < nb_transitions; i++) {
        Transition t = transitions[etat_id][i];
        int action = t.label_action;

        
        Valuation garde = constraints[action]();
        Valuation inter = intersect_valuation(&source.val, &garde);
        if (is_empty_valuation(&inter)) continue;

        Valuation val_apres = update_functions[action](&inter);
        int index_cible = ajouter_etat_x(t.etat_in, val_apres);
        transitions_x[index_source][k].cible = index_cible;
        transitions_x[index_source][k].action_id = action;
        k++;
    }
}

void appliquer_transitions() {
    int i = 0;
    while (i < nb_etats_x) {
        appliquer_transition1(i);
        i++;
    }
}

void afficher_valuation(Valuation* v) {
     printf("\t Variables       : v =");
    for (int i = 0; i < VAR_COUNT; i++) {
        Interval ivl = v->vars[i];
        printf("[");
        printf(ivl.lower_closed ? "%d" : "(%d", ivl.lower);
        printf(",");
        printf(ivl.upper_closed ? "%d" : "%d)", ivl.upper);
        printf("]\n");
    }
}


void print_lts_x() {
    for (int i = 0; i < nb_etats_x; i++) {
        printf("Etat etendu ID #%d\n", i);
        printf("\t Etat de base    : %s (ID %d)\n", etats[etats_x[i].etat], etats_x[i].etat);
        afficher_valuation(&etats_x[i].val);
        printf("\t Transitions sortantes :\n");

        for (int j = 0; j < nb_trans_x[i]; j++) {
            int id_cible = transitions_x[i][j].cible;
            int action_id = transitions_x[i][j].action_id;
            printf("\t\t   [%d] --%s--> [%d]\n", i, actions[action_id], id_cible);
        }
        printf("\n");
    }
}


int main() {
    init_lts();
    
    capacite_etats_x = 10000;  
    init_lts_x();

    Valuation val_init;
    val_init.vars[0] = (Interval){3, 9, true, true}; // état initial valuation ponctuelle
    ajouter_etat_x(0, val_init);

    appliquer_transitions();

    print_lts_x();
  printf("Nombre d'etats = %d", nb_etats_x);

    return 0;
}
