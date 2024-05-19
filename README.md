# CDataX

---

###### Ce projet vous est présenté par Pierre CAMELERI & Raphaël GUIGNOLLE

## Qu'est-ce que c'est que CDataX ?

CDataX est un projet qui permet une gestion efficace et pratique de très grands sets de données en utilisant une API
codée sur le langage C. L'objectif est de fournir une API qui va permettre aux programmeurs d'avoir un set d'outils
pour manipuler ces données : en ajouter, en supprimer, les trier, les ranger, les sélectionner, les modifier et les
afficher.

Les types de données qui sont gérés par le programme s'étendent de l'entier non signé à des types plus complexes tels
que des chaines de caractères (liste exhaustive plus en bas). Les données peuvent être chargées sont en dur, soit via
une demande de l'utilisateur, soit par l'importation et l'exportation de fichier au format CSV.

## Comment fonctionne CDatax ?

Le projet se base sur une API divisée en deux fichiers `column.h` et `cdataframe.h`. Chacun d'entre-eux fonctionne sur
un paradigme différent : gestion de colonnes indépendantes pour le premier et gestion de plusieurs colonnes liés les
unes aux autres au sein d'un *CDataframe*. Pour cette raison, il est déconseillé aux utilisateurs d'importer et
d'utiliser en concurrence ces deux modules. Il est préférable de manipuler qu'un d'entre-eux à la fois.

* `column.h` est une interface surtout pensée pour être la base de l'API. Ce module contient toute la gestion des
  données sous forme de colonnes avec toutes les fonctions liées.
* `cdataframe.h` est une interface finale pensée pour la gestion de données sous format "tableau". Ce module est la
  gestion des données sous forme d'une liste de colonnes, il regroupe également toutes les fonctions liées.

Il y a également présent un module de gestion de Liste Linéaire Chainé (doublement chainé). Il est importé et adapté
(légèrement) des consignes du projet. Il est subdivisé deux fichiers : un header `llc.h` et un fichier source : `llc.c`

Le programme inclut également un set de tests dans le fichier principal : `main.c`.
Il est toujours up-to-date avec les dernières modifications du projet.
Un fichier CSV `example.csv` est aussi à disposition pour les tests liés à l'importation de CSV.

## Caractéristiques techniques

Types de colonnes supportés (pleinement) :

* NULL type (limited feature set)
* unsigned integer
* signed integer
* char
* float
* double
* string
* pointer (alias structure) (limited feature set)

Détail du (limited feature set) :

1. Les comparaisons ne sont pleinement supportés (renvoie l'égalité)
2. Le chargement et déchargement via CSV et entré utilisateur n'est pas supporté (la valeur sera convertie vers NULL)
3. L'affichage de ces types est supporté !

Ces décisions font suite aux problématiques suivantes :

1. Il n'est pas possible de comparer deux pointers qui sont deux addresses mémoires. Il faudrait comparer vers ce quoi
   ils pointent, mais cela est impossible, car les valeurs pointées ne sont pas nécessairement homogènes : on ne peut
   pas comparer patates et carottes de manière binaire (plus grand, plus petit, égale) !
2. Les pointeurs étant propre à l'état de la mémoire du programme spécifique, il n'y a aucun sens à pointer un espace
   mémoire spécifique qui de plus protégé ou bien randomisé le système d'exploitation.
3. L'affichage est supporté et nous invitons à implémenter en dur toute utilisation de ces pointeurs qui reste des
   types supportés dans leur stockage simple dans un CDataframe et qui sont autorisé à l'exportation vers des CSV !

Nota Bene sur les constantes & types:

* La taille d'une entrée utilisateur ne doit pas dépasser 255 caractères (le cas échant peut être modifié jusqu'à la
  taille d'un int)
* Le nombre de colonnes maximum supporté lors du chargement d'un CSV est de 1024 colonnes. Le nombre peut être changé
  sans contraintes particulières il doit juste rester en dessous de la taille supporté par le type `indexation`.
* La réallocation dans une colonne se fait par bloc de 256 cases
* La réallocation dans le buffer d'affichage se fait par bloc de 32 caractères.
* Le format du type indexation doit **toujours être à jour** avec le type associé.
* Le type `indexation` est un alias pour tout ce qui est de l'ordre de grandeur de la taille d'une colonne.
* La taille physique de ce type est de 64 bits (implémentation standard).
* Une colonne ne devrait donc jamais excéder 9223372036854775807 *par défaut* (`long long int`).
* Un CDataframe est une liste doublement chainée avec une taille et une taille de colonne qui est constant mis-à-jour
  (à chaque ajout et suppression) pour garder une complexité de constante pour obtenir la taille de liste.

## Dépendances

Ce repo ne contient que le code source. Il nécessite d'être compilé vers la plateforme voulue.
Aucun fichier binaire n'est distribué. Cependant, des sources stables peuvent être trouvées dans
les [Releases](https://github.com/Balafre78/CDataX/releases).

Bibliothèques standards utilisées :

* `stdio.h` pour la gestion de l'InputOutput
* `stdlib.h` pour la gestion de la mémoire
* `string.h` pour la gestion simplifiée des chaines de caractères.

Le code source est fourni avec des fichiers de configurations pour build sous windows et linux à l'aide de

* CMake
* MinGW (windows) ou gcc (linux)
* CLion (by JetBrains)

> [!WARNING]
> La version de C utilisé est ANSI C11

## Guide de démarrage

* Cloner le repo via `git clone https://github.com/Balafre78/CDataX`
* Se déplacer dans le **dossier courant du projet** `cd CDataX`
* Faire toutes les modifications souhaitées (notamment dans le `main.c`)
* Compiler le projet vers la plateforme souhaitée !

## Bug connu

L'interaction avec l'InputOutput en C étant ce qu'il est, il est tout à fait possible que l'ergonomie d'affichage
avec les fonctions qui manipule le buffer d'entrée `stdin` soit dégradé. Cependant, cela n'a AUCUN impact sur les
données saisies qui sont toujours traitées de manière régulière sans bugs connus à ce jour. **Nous conseillons donc
fortement d'interagir avec les fichiers CSV au lieu de l'input utilisateur.**

Si vous en trouvez, n'hésitez pas à les reporter via [issues](https://github.com/Balafre78/CDataX/issues)