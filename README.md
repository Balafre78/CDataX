# CDataX

---

###### Projet proposé par Pierre CAMELERI & Raphaël GUIGNOLLE

## Qu'est-ce que c'est que CDataX ?

CDataX est un projet qui permet une gestion efficace et pratique de très grands sets de données en utilisant une API
codée sur le language C. L'objectif est de fournir une API qui va permettre aux programmeurs d'avoir un set d'outils
pour manipuler ces donnnées : en ajouter, en supprimer, les trier, les ranger, les sélectionner, les modifier et les
afficher.

Les types de données se limitent pour l'instant à des entiers mais ils s'étendront avec une prochaine version sur
plusieurs types de données qui pourront être utilisé simultanément. Une plus forte interraction avec l'IO sera aussi
envisagé - notamment le support du chargement et d'écriture d'un set de données à partir d'un fichier externe au format
csv.

## Comment fonctionne CDatax ?

Le projet se base sur une API divisé en deux fichiers `column.h` et `cdataframe.h` :

* `column.h` est une interface surtout pensée pour être la base de l'API. Ce module contient toute la gestion des 
  données sous forme de colonnes avec toutes les fonctions liés.
* `cdataframe.h` est une interface finale pensée pour la gestion de donnnées sous format "tableau". Ce module est la 
  gestion des données sous forme d'une liste de colonnes, il regroupe également toutes les fonctions liés.

Le programme inclue également un set de test_column dans le fichier principal : `main.c`.
Il est toujours up-to-date avec les dernières modifications du projet.

## Dépendances

Ce repo ne contient que le code source. Il nécessite d'être compilé vers la plateforme voulue.
Des binaires peuvent être distribué via [Releases](https://github.com/Balafre78/CDataX/releases).

Bibliothèques standards utilisés:

* `stdio.h` pour la gestion de l'InputOutput
* `stdlib.h` pour la gestion de la mémoire

Le code source est fournit avec des fichiers de configurations pour build sous windows et linux à l'aide de

* CMake
* MinGW (windows) ou gcc (linux)
* CLion (by JetBrains)

> [!WARNING]
> La version de C utilisé est ANSI C11

## Guide de démarrage

* Cloner le repo via `git clone https://github.com/Balafre78/CDataX`
* Se déplacer dans le **dossier courant du projet** `cd CDataX`
* Faire toutes les modifications souhaitée (notamment dans le `main.c`)
* Compiler le projet vers la plateforme souhaitée!

## Bugs connus

Il n'existe pas de bug majeur connu.

Si vous en trouvez, n'hésitez pas à les reporter via [issues](https://github.com/Balafre78/CDataX/issues)