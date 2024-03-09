# Space Invaders BENMBAREK Younes

Jeu inspiré du célèbre Space Invaders.

## Installation

### Prérequis

- Le compilateur clang
- Tout le dossier git est requis car des images ont été rajoutées ainsi que des fonctions dans les fichiers du dossier Graphique.

### Makefile : compilation et fonctionnalités

**À la première compilation, il est nécessaire d'utilser la commande `all` pour compiler les bibliothèques.**

Le makefile contient les fonctionnalités suivantes :

- `tidy` : clang-tidy --quiet -checks='*' main.c --
- `format` : clang-format -i main.c
- `var` : clang-tidy --checks=*"cppcoreguidelines-init-variables" main.c --
- `readability` : clang-tidy --checks="readability-*" main.c --

## Jeu

### Commandes du jeu

Le jeu utilise les commandes suivantes :

- "s" ==> aller à gauche
- "d" ==> aller à droite
- "m" ==> tirer un missile

### Bonus : score

J'ai rajouté un score en bonus. Ce dernier est lu dans un fichier score.txt.
Cette implémentation amène néanmoins des warnings avec clang tidy.
Le score est aussi limité à 340 car il n'y a pas d'implémentation de plus de niveaux.

## Améliorations possibles

Les améliorations suivantes peuvent être apportées au projet :

- Des niveaux de difficulté.
- Meilleure gestion de la mémoire.
- Mode multijoueur

## Auteur

Créé par BENMBAREK Younes.
