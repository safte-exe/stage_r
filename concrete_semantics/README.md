# LTS Extended Builder

Ce projet implémente la **construction d’un système de transitions étiquetées étendu (espace d'etat)** à partir d’un LTS de base enrichi de **variables d’état**, de **fonctions de mise à jour** et de **contraintes logiques**.

Il permet de simuler l'évolution d’un système en fonction de conditions et de données.

---

## Structure des fichiers

| Fichier                    | Rôle                                                                 |
|----------------------------|----------------------------------------------------------------------|
| `main.c`                   | Point d’entrée. Appelle la construction du LTS étendu                |
| `toy_exemple.c`            | Définit le LTS de base (états, transitions, actions, contraintes)    |
| `lts_extended_builder.c`   | Contient l'algorithme de construction du LTS étendu                  |
| `structure_lts.h`          | Définitions des structures : LTS, Transition, Contrainte, etc.       |
| `structure_lts_extended.h` | Structures pour les états étendus, transitions étendues, etc.        |
| `structure_variable.h`     | Définition de la structure des variables                             |

---

##  Utilisation de la bibliothèque `uthash`

Le projet utilise [`uthash`](https://troydhanson.github.io/uthash/userguide.html#_a_hash_in_c). `uthash` est une bibliothèque **header-only** en C utilisée pour la gestion de l'espace d'etat du LTS, afin de:
    -creer une table de hachage
    -eviter les doublons
    -accelerer la recherche drastiquement

---

## Utiliser les modèles dans le dossier Models

On peut créer un nouveau modèle en modifiant les fonctions suivantes :
   - `init_lts()`
   - `init_variables()`
   - `init_update_functions()`
   - `init_constraints()`
ainsi que la déclaration des variables suivant la définition donnée dans le header `structure_variable`


---

##  Compilation et Execution

1. La compilation est faite avec la ligne de commande : gcc -o executable_name main.c toy_exemple.c lts_extended_builder.c
    On peut compiler avec l’un des fichiers exemples souhaités, comme `models/exemple_fig2a.c` ou `models/lts_extended_toy_exemple.c`, à la place de `toy_exemple.c`
2. L'execution : ./executable_name 

