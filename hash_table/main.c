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

personne *hash_table_lookup (char* name){
    int index = hash(name);
    if (hash_table[index] != NULL
         && strcmp(name, hash_table[index]->name) == 0){
            return hash_table[index];
         }
    return NULL;
}

 
void main(){

    init_hash_table();
//    print_tableau();
    personne jacob = {.name = "Jacob", .age = 34};
    personne nicole = {.name = "Nicole", .age = 34};
    personne phoa = {.name = "Phoa", .age = 24};
    hash_table_insert(&jacob);
    hash_table_insert(&nicole);
    hash_table_insert(&phoa);

    print_tableau();

    personne *temp = hash_table_lookup("Meow");
    if (temp == NULL){
        printf("Pas trouve! \n");
    } else {
        printf("%s Trouve.e \n", temp->name);
    }
    
    temp = hash_table_lookup("Nicole");
    if (temp == NULL){
        printf("Pas trouve! \n");
    } else {
        printf("%s Trouve.e \n", temp->name);
    }
    /*
    printf("Jacob => %d \n", hash("Jacob"));
    printf("Mary => %d \n", hash("Mary"));
    printf("Jane => %d \n", hash("Jane"));
    printf("Ovid => %d \n", hash("Ovid"));
    */
}