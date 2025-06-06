#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>  



#define MAX_NAME 50
#define TABLE_TAILLE 10

typedef struct personne {
    char name[MAX_NAME];
    int age;
    struct personne *next;

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
            printf("%d \t ", i);
            personne *tmp = hash_table[i];
            while (tmp != NULL){
                printf("%s - ", tmp->name );
                tmp = tmp->next;
            }
            printf("\n");
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
    p->next = hash_table[index];
    hash_table[index] = p;
    return true;
}

personne *hash_table_lookup (char* name){
    int index = hash(name);
    personne *tmp = hash_table[index];

    while (tmp != NULL && strcmp(tmp->name, name) != 0 ){
        tmp = tmp->next;
    }

    return tmp;
}


personne *hash_table_delete (char *name){

    int index = hash(name);
    personne *tmp = hash_table[index];
    personne *prev = NULL;
    while (tmp != NULL && strcmp(tmp->name, name) != 0 ){
        prev = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL) {return NULL;}
    if (prev == NULL) {
        //supprimer la tete de liste 
        hash_table[index] = tmp->next;
    } else {
        prev->next = tmp->next;
    }
    return tmp;
}

 
void main(){

    init_hash_table();
//    print_tableau();
    personne jacob = {.name = "Jacob", .age = 34};
    personne nicole = {.name = "Nicole", .age = 34};
    personne phoa = {.name = "Phoa", .age = 24};
    personne mia = {.name = "Mia", .age = 24};
    personne lorent = {.name = "Lorent", .age = 24};
    personne isa = {.name = "Isabelle", .age = 24};
    personne david = {.name = "David", .age = 24};
    personne kyle = {.name = "Kyle", .age = 24};
    personne laura = {.name = "Laura", .age = 24};
    personne brie = {.name = "Brie", .age = 24};
    personne john = {.name = "Johnny", .age = 24};
    hash_table_insert(&jacob);
    hash_table_insert(&nicole);
    hash_table_insert(&phoa);
    hash_table_insert(&mia);
    hash_table_insert(&lorent);
    hash_table_insert(&isa);
    hash_table_insert(&david);
    hash_table_insert(&kyle);
    hash_table_insert(&laura);
    hash_table_insert(&brie);
    hash_table_insert(&john);

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

    print_tableau();

    temp = hash_table_delete("Phoa");
    if (temp == NULL){
        printf("Pas trouve! \n");
    } else {
        printf("%s Trouve.e \n", temp->name);
    }

    print_tableau();

    /*
    printf("Jacob => %d \n", hash("Jacob"));
    printf("Mary => %d \n", hash("Mary"));
    printf("Jane => %d \n", hash("Jane"));
    printf("Ovid => %d \n", hash("Ovid"));
    */
}