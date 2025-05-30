#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>  



#define MAX_NAME 50
#define TABLE_TAILLE 10

typedef struct {
    char name[MAX_NAME];
    int age;

    // ... ajouter des trucs ici 
}personne;


personne * hash_table [TABLE_TAILLE];

void init_hash_table() {

    for (int i = 0; i < TABLE_TAILLE; i++){
        hash_table[i] = NULL;
    }
    //tableau vide
}

void print_tableau() {
    printf("Debut\n");
    for (int i = 0; i < TABLE_TAILLE; i++){
        if(hash_table[i] == NULL) {
            printf("%d \t -----\n",i);
        }
        else {
            printf("%d \t %s\n", i, hash_table[i]->name);
        }
    }
    printf("Fin\n");
}

unsigned int hash( char *name ){

    int length = strlen(name);
    unsigned int hash_val = 0;
    for (int i =0; i< length; i++){
        hash_val += name[i];
        hash_val = (hash_val * name[i]) % TABLE_TAILLE ;
    }

    return hash_val ;
}


bool hash_table_insert (personne *p){
    if (p == NULL) return false;
    
    int index = hash (p->name);
    if (hash_table[index] != NULL ) {
        return false;
    }
    hash_table[index] = p;
    return true;
}

 
void main(){

    init_hash_table();
    print_tableau();
    personne jacob = {.name = "Jacob", .age = 34};
    hash_table_insert(&jacob);

    print_tableau();

    /*
    printf("Jacob => %d \n", hash("Jacob"));
    printf("Mary => %d \n", hash("Mary"));
    printf("Jane => %d \n", hash("Jane"));
    printf("Ovid => %d \n", hash("Ovid"));
    */
}