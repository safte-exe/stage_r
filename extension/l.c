#include <stdio.h>
#include <stdbool.h>

typedef enum { X, Y, VAR_COUNT } Var;

typedef struct {
    int lower;
    int upper;
    bool lower_closed;
    bool upper_closed;
} Interval;

typedef struct {
    Interval vars[VAR_COUNT];
} Valuation;

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

Valuation intersect_valuation( Valuation* v1,  Valuation* v2) {
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

bool is_empty_valuation(const Valuation* v) {
    for (int i = 0; i < VAR_COUNT; i++) {
        if (is_empty_interval(v->vars[i])) return true;
    }
    return false;
}

Valuation update(const Valuation* in) {
    Valuation out;

    out.vars[X].lower = in->vars[X].lower + 2;
    out.vars[X].upper = in->vars[X].upper + 2;
    out.vars[X].lower_closed = in->vars[X].lower_closed;
    out.vars[X].upper_closed = in->vars[X].upper_closed;

    out.vars[Y].lower = 2 * in->vars[Y].lower;
    out.vars[Y].upper = 2 * in->vars[Y].upper;
    out.vars[Y].lower_closed = in->vars[Y].lower_closed;
    out.vars[Y].upper_closed = in->vars[Y].upper_closed;

    return out;
}

void print_valuation(const char* label, const Valuation* v) {
    printf("%s:\n", label);
    for (int i = 0; i < VAR_COUNT; i++) {
        char name = (i == X) ? 'x' : 'y';
        char left = v->vars[i].lower_closed ? '[' : ']';
        char right = v->vars[i].upper_closed ? ']' : '[';
        printf("  %c = %c%d, %d%c\n", name, left, v->vars[i].lower, v->vars[i].upper, right);
    }
}

int main() {
    Valuation v = {
        .vars = {
            [X] = {1, 5, true, true},
            [Y] = {2, 4, true, true}
        }
    };

    Valuation guard = {
        .vars = {
            [X] = {3, 10, true, true},
            [Y] = {0, 3, true, true}
        }
    };

    Valuation filtered = intersect_valuation(&v, &guard);

    print_valuation("Initial state", &v);
    print_valuation("Guard", &guard);

    if (!is_empty_valuation(&filtered)) {
        print_valuation("After intersection", &filtered);

        Valuation updated = update(&filtered);
        print_valuation("After update", &updated);
    } else {
        printf("Transition blocked: constraint unsatisfiable\n");
    }

    return 0;
}
