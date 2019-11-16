# S3_C_projet

- Coder en anglais

- Noms de fichiers en minuscules, séparés par des tirets "-"
- Extensions .c .h .inc

- Noms de type en CamelCase sans tirets. Pour les struct/enum/union, utiliser un underscore devant "_"
  Ex : typedef struct _Polygon Polygon;

- Macros/constantes en majuscules avec un underscore "" entre chaque mot

- Variables en minuscules avec un underscore "" entre chaque mot

- Dans chaque header, utiliser un include guard au format :
           <nom du projet><chemin><nom header>_H

- Dans les .c : inclure le .h (même nom que .c), saut de ligne, inclure librairie standard, saut de ligne, inclure librairie externes, saut de ligne, inclure headers du projet, saut de ligne, inclure .inc

- Utiliser le style de Google, formatter avec clang-format et vérifier avec cclint

- Protéger les champs (avec .inc) et faire des getters/setters

- Générer la documentation avec sphinx en respectant la norme doxygen

- Tests unitaires + valgrind. Créer un dossier "test"

OUTILS IMPOSÉS :
- CLion avec mingw32 (gcc, g++, make)
- CMake
- Github ?
