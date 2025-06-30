#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct {
    int lower;
    int upper;
} Interval;
    Interval vars[VAR_COUNT];

//ajouter diff possibilities d'intervalle




typedef enum { X, Y, VAR_COUNT } Var;
//typedef Interval valuation
typedef struct {
} Valuation;




typedef enum { LT, LE, EQ, GE, GT } Operator;

typedef struct {
    Var variable;
    Operator op;
    int value;
} Constraint;


typedef struct {
    Constraint* conditions;
    int count;
} ConstraintSet;


int apply_constraint(Valuation* v, ConstraintSet* cs) {
    for (int i = 0; i < cs->count; i++) {
        Constraint c = cs->conditions[i];
        Interval* ivl = &v->vars[c.variable];
        switch (c.op) {
            case GE:
                if (ivl->lower < c.value) ivl->lower = c.value;
                break;
            case GT:
                if (ivl->lower <= c.value) ivl->lower = c.value + 1;
                break;
            case LE:
                if (ivl->upper > c.value) ivl->upper = c.value;
                break;
            case LT:
                if (ivl->upper >= c.value) ivl->upper = c.value - 1;
                break;
            case EQ:
                ivl->lower = ivl->upper = c.value;
                break;
        }
        if (ivl->lower > ivl->upper) return 0; // Constraint not satisfiable
    }
    return 1; // Satisfiable
}

void update(Valuation* v) {
    v->vars[X].lower += 2;
    v->vars[X].upper += 2;

    v->vars[Y].lower *= 2;
    v->vars[Y].upper *= 2;
}


int main() {
    Valuation v = { .vars = { {1, 5}, {2, 4} } };

    printf("Before transition: x = [%d, %d], y = [%d, %d]\n",
            v.vars[X].lower, v.vars[X].upper,
            v.vars[Y].lower, v.vars[Y].upper);
    Constraint conditions[] = {
        {X, GE, 3},
        {Y, LT, 4}
    };
    ConstraintSet cset = { .conditions = conditions, .count = 2 };

    if (apply_constraint(&v, &cset)) {
        update(&v);
        printf("After transition: x = [%d, %d], y = [%d, %d]\n",
            v.vars[X].lower, v.vars[X].upper,
            v.vars[Y].lower, v.vars[Y].upper);
    } else {
        printf("Transition blocked: constraint unsatisfiable.\n");
    }


    printf("Before transition: x = [%d, %d], y = [%d, %d]\n",
            v.vars[X].lower, v.vars[X].upper,
            v.vars[Y].lower, v.vars[Y].upper);
    


    Constraint conditions1[] = {
        {X, LT, 6},
        {Y, GE, 5}
    };

    ConstraintSet cset1 = { .conditions = conditions1, .count = 2 };

    if (apply_constraint(&v, &cset1)) {
        update(&v);
        printf("After transition: x = [%d, %d], y = [%d, %d]\n",
            v.vars[X].lower, v.vars[X].upper,
            v.vars[Y].lower, v.vars[Y].upper);
    } else {
        printf("Transition blocked: constraint unsatisfiable.\n");
    }

    return 0;
}
