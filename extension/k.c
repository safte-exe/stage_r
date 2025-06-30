#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int lower;
    int upper;
} Interval;

typedef enum { X, Y, VAR_COUNT } Var;

typedef struct {
    Interval vars[VAR_COUNT];
} Valuation;


// Intersection de deux intervalles


Interval intersect_interval(Interval a, Interval b) {
    Interval result;
    result.lower = (a.lower > b.lower) ? a.lower : b.lower;
    result.upper = (a.upper < b.upper) ? a.upper : b.upper;

    // Si intervalle vide
    if (result.lower > result.upper) {
        result.lower = 1;
        result.upper = 0; // convention : intervalle vide si lower > upper
    }
    return result;
}


// Appliquer une contrainte symbolique : v ∩ c

bool apply_constraint(Valuation* result, Valuation* v, Valuation* constraint) {
    for (int i = 0; i < VAR_COUNT; i++) {
        result->vars[i] = intersect_interval(v->vars[i], constraint->vars[i]);

        if (result->vars[i].lower > result->vars[i].upper) {
            // intersection vide → contrainte non satisfiable
            return false;
        }
    }
    return true;
}



// Fonction de mise à jour retourne une nouvelle valuation



Valuation update(Valuation v) {
    Valuation result;

    // Exemple : x := x + 2
    result.vars[X].lower = v.vars[X].lower + 2;
    result.vars[X].upper = v.vars[X].upper + 2;

    // Exemple : y := 2 * y
    result.vars[Y].lower = 2 * v.vars[Y].lower;
    result.vars[Y].upper = 2 * v.vars[Y].upper;

    return result;
}




void print_valuation(const char* label, Valuation v) {
    printf("%s: x = [%d, %d], y = [%d, %d]\n", label,
           v.vars[X].lower, v.vars[X].upper,
           v.vars[Y].lower, v.vars[Y].upper);
}


int main() {
    // État initial
    Valuation v = { .vars = { {1, 5}, {2, 4} } };
    print_valuation("Initial state", v);

    // Contrainte 1 : x ∈ [3,10], y ∈ [0,3]
    Valuation guard1 = { .vars = { {INT_MIN, 10}, {0, 3} } };

    Valuation filtered;
    if (apply_constraint(&filtered, &v, &guard1)) {
        print_valuation("After guard 1", filtered);
        Valuation updated = update(filtered);
        print_valuation("After update (1)", updated);
    } else {
        printf("Transition blocked: guard 1 unsatisfiable\n");
    }

    // Contrainte 2 : x ∈ [1,5], y ∈ [5,10]
    Valuation guard2 = { .vars = { {1, 8}, {0, 6} } };

    if (apply_constraint(&filtered, &v, &guard2)) {
        print_valuation("After guard 2", filtered);
        Valuation updated = update(filtered);
        print_valuation("After update (2)", updated);
    } else {
        printf("Transition blocked: guard 2 unsatisfiable\n");
    }

    return 0;
}

