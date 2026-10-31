# TAS-MAX C++17

Au démarrage, le programme crée un jeu fixe d'un million d'entiers (graine 42), puis remplit les deux max-heaps avec les mêmes valeurs. Le temps de génération est exclu. Les résultats du remplissage initial sont écrits dans `resultats.csv`.

## Compilation et lancement

```sh
g++ -std=c++17 -O2 -Wall -Wextra main.cpp TasScratchVector.cpp TasSTL.cpp Benchmark.cpp -o tas
./tas
```

Sous Windows/MinGW, produire `tas.exe` et lancer `./tas.exe`. UTF-8 est configuré à l'exécution sous Windows.

## Fonctionnement

Les actions manuelles de recherche, insertion et suppression opèrent sur les deux heaps. A utilise un tableau `int*` avec agrandissement manuel par doublement, copies profondes et déplacements. B utilise réellement `std::priority_queue<int>`.

Le menu Benchmark affiche les durées mesurées au démarrage. Pour B, `priority_queue::push` réalise à la fois le placement et l'ajustement, qui ne peuvent pas être mesurés séparément via son interface ; ces deux sous-mesures sont indiquées N/A. Le test d'extraction travaille sur une copie de chaque tas et retire uniquement sa racine maximale une fois ; les tas principaux restent intacts. La recherche STL parcourt une copie avec `top()`/`pop()`.

Le tas A implémente manuellement ses percolations et ne dépend d'aucun conteneur STL. Le tas B repose sur le conteneur adaptateur `std::priority_queue`.
