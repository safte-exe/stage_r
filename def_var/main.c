#include <stdio.h>
#include <stdlib.h>

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



int main()
{

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
