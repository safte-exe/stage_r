# LTS Extended Builder

Ce projet implémente la **construction d’un système de transitions étiquetées étendu (espace d'etat)** à partir d’un LTS de base enrichi de **variables d’état**, de **fonctions de mise à jour** et de **contraintes logiques**.

Il permet de simuler l'évolution d’un système en fonction de conditions et de données.

---

## Structure des fichiers

| Fichier                    | Rôle                                                                |
|----------------------------|----------------------------------------------------------------------|
| `main.c`                   | Point d’entrée. Appelle la construction du LTS étendu                |
| `toy_exemple.c`            | Définit le LTS de base (états, transitions, actions, contraintes)    |
| `lts_extended_builder.c`   | Contient l'algorithme de construction du LTS étendu                  |
| `structure_lts.h`          | Définitions des structures : LTS, Transition, Contrainte, etc.       |
| `structure_lts_extended.h` | Structures pour les états étendus, transitions étendues, etc.        |

---

##  Utilisation de la bibliothèque `uthash`

Le projet utilise [`uthash`](https://troydhanson.github.io/uthash/). `uthash` est une bibliothèque **header-only** en C utilisée pour la gestion de l'espace d'etat du LTS, afin de:
    -creer une table de hachage
    -eviter les doublons
    -accelerer la recherche drastiquement

---

## Utiliser les modèles dans le dossier Models

1. Ouvrir `toy_exemple.c`
2. Supprimer ou commenter les fonctions suivantes :
   - `init_lts()`
   - `init_variables()`
   - `init_update_functions()`
   - `init_constraints()`

3. Copier-coller les mêmes fonctions depuis **le fichier exemple voulu** : `models/exemple_fig2a.c` ou `models/lts_extended_toy_exemple.c`


---

##  Compilation et Execution

1. La compilation est faite avec la ligne de commande : gcc -o executable_name main.c toy_exemple.c lts_extended_builder.c
2. L'execution : ./executable_name 

