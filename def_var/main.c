#include <stdio.h>
#include <stdlib.h>

#define nb_actions 4
#define NB_VAR 4

typedef struct Variable{
    int x;
    int y;
    int z;
} Variable;

Variable variable;

void init_variables() {
    variable.x = 0;
    variable.y = 9;
    variable.z = 1;

}

void update_a () {
    variable.x++;
    variable.y--;

}

void update_b() {
    variable.y--;
}

void update_c() {
            variable.x  =   variable.x * 2;
}


void update_d() {
            variable.z = ( variable.z + 1) % 2;
}

typedef void (*UpdateFunction)();
 UpdateFunction update_functions[nb_actions];


void init_update_functions() {
    update_functions[0] = update_a;
    update_functions[1] = update_b;
    update_functions[2] = update_c;
    update_functions[3] = update_d;
}


void print_variables() { 

    printf("x = %d\n", variable.x );
    printf("y = %d\n", variable.y );
    printf("z = %d\n", variable.z );
}



int main()
{

    init_variables();
    print_variables();
    printf("================================\n");
    init_update_functions();

    update_functions[0]();
        print_variables();

        printf("================================\n");
    update_functions[1]();
        print_variables();

        printf("================================\n");
    update_functions[2]();
        print_variables();

        printf("================================\n");
    update_functions[3]();

    print_variables();

        printf("================================\n");

    return 0;

}
