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
// Gestion des intervalles et valuations

typedef struct {
    float lower;
    float upper;
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

void afficher_valuation(Valuation* v) {
     printf("\t Variables       : v =");
    for (int i = 0; i < VAR_COUNT; i++) {
        Interval ivl = v->vars[i];
        printf("[");
        printf(ivl.lower_closed ? "%f" : "(%d", ivl.lower);
        printf(",");
        printf(ivl.upper_closed ? "%f" : "%d)", ivl.upper);
        printf("]\n");
    }
}

Valuation update_a(Valuation* in) {
    Valuation out = *in;
    //out.vars[0].lower += 2;
    out.vars[0].upper += 2;
    return out;
}

Valuation update_b(Valuation* in) {
    Valuation out = *in;
    out.vars[0].lower *= 2;
    out.vars[0].upper *= 2;
    return out;
}

Valuation update_c(Valuation* in) {
    Valuation out = *in;
    out.vars[0].lower = out.vars[0].lower / 3.0;
    out.vars[0].upper = out.vars[0].upper / 2.0;
    return out;
}

typedef Valuation (*UpdateFunction)(Valuation*);

UpdateFunction update_functions[nb_actions] = {update_a, update_b, update_c};



// Contraintes sous forme de valuations 


Valuation const_a() {
    Valuation v;
    v.vars[0] = (Interval){0, INT_MAX, true, true};
    return v;
}

Valuation const_b() {
    Valuation v;
    v.vars[0] = (Interval){0, 5, true, true};
    return v;
}

Valuation const_c() {
    Valuation v;
    v.vars[0] = (Interval){8, INT_MAX, true, true};
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
bool egal_interval(Interval a, Interval b) {
    return a.lower == b.lower &&
           a.upper == b.upper &&
           a.lower_closed == b.lower_closed &&
           a.upper_closed == b.upper_closed;
}

bool egal_valuation(Valuation* v1, Valuation* v2) {
    for (int i = 0; i < VAR_COUNT; i++) {
        if (!egal_interval(v1->vars[i], v2->vars[i]))
            return false;
    }
    return true;
}


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
     //   afficher_valuation(&inter);
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
    val_init.vars[0] = (Interval){3, 3, true, true}; // état initial valuation ponctuelle
    ajouter_etat_x(0, val_init);

    appliquer_transitions();

    print_lts_x();
  printf("Nombre d'etats = %d", nb_etats_x);

    return 0;
}
