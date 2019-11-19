# S3_C_projet

- Coder en anglais

- Noms de fichiers en minuscules, séparés par des tirets "-"
- Extensions .c .h .inc

- Noms de type en CamelCase sans tirets. Pour les struct/enum/union, utiliser un underscore devant "_"
  Ex : typedef struct _Polygon Polygon;

- Macros/constantes en majuscules avec un underscore "_" entre chaque mot

- Variables en minuscules avec un underscore "_" entre chaque mot

- Dans chaque header, utiliser un include guard au format :
           `<nom du projet>_<chemin>_<nom header>_H_`

- Dans les .c : inclure le .h (même nom que .c), saut de ligne, inclure librairie standard, saut de ligne, inclure librairie externes, saut de ligne, inclure headers du projet, saut de ligne, inclure .inc

- Utiliser le style de Google, formatter avec clang-format et vérifier avec cclint

- Protéger les champs (avec .inc) et faire des getters/setters

- Générer la documentation avec sphinx en respectant la norme doxygen

- Tests unitaires + valgrind + cclint. Créer un dossier "**test**"

## OUTILS IMPOSÉS :
- CLion avec mingw32 (gcc, g++, make)
- CMake

## CONFIGURER CLANG-FORMAT EN LOCAL :
Aller dans Settings > Editor > Code Style.
- Changer Scheme de "Default" à "Project"
- Cliquer sur "Enable ClangFormat with clangd server"
- On peut faire CTRL+ALT+L ou Code > Reformat pour reformatter

- Avoir le .clang-format bien dans la racine
- Ne pas le modifier. Si besoin, taper un caractère et l'enlever puis sauvegarder pour forcer CLion à le recharger.
- TOUT EN BAS DE L'ECRAN (à côté de UTF-8) il doit y avoir : ClangFormat

## CONFIGURER CCLINT :
- Avoir python 3 installé
- `pip install cclint` ou `pip3 install cclint`

## MINGW :
- Version **32 bits**
- Modules :

| Nom                 |
|---------------------|
| mingw32-gcc-bin     |
| mingw32-gcc-g++-bin |
| mingw32-gdb-bin     |
| mingw32-make-bin    |

- Ajouter `C:\MinGW\bin` au PATH (chercher "variables d'environnement" sur Windows, cliquer sur "variables d'environnement" puis modifier "path" et ajouter la ligne).
- Pour faire un make : aller (avec `cd`) dans `/cmake-build-debug` puis `mingw32-make`.

## SPHINX
- Utiliser la commande `pip install -U sphinx`.
- Utiliser la norme [Doxygen](http://www.doxygen.nl/manual/docblocks.html).
- [Voici la description de la norme](https://franckh.developpez.com/tutoriels/outils/doxygen/)
- Faire un CMakeLists.txt avec `make docs`
### Exemple :
```c

/**
 *  \file [<name>] OU \file main.c
 *  \brief Cette fonction calcule
 *         la racine carrée.
 *  \author Francis
 *  \version 1.2.0
 *  \date 12 octobre 2019
 *  \fn double sqrt(double val)
 *  \param val Valeur dont on souhaite
 *             trouver la racine carrée.
 *  \return La racine carrée de val.
 */
double sqrt(double val) {
    ...
}
```
