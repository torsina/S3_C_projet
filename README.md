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

OUTILS IMPOSÉS :
- CLion avec mingw32 (gcc, g++, make)
- CMake

CONFIGURER CLANG-FORMAT EN LOCAL :
Aller dans Settings > Editor > Code Style.
- Changer Scheme de "Default" à "Project"
- Cliquer sur "Enable ClangFormat with clangd server"
- On peut faire CTRL+ALT+L ou Code > Reformat pour reformatter

- Avoir le .clang-format bien dans la racine
- Ne pas le modifier. Si besoin, taper un caractère et l'enlever puis sauvegarder pour forcer CLion à le recharger.
- TOUT EN BAS DE L'ECRAN (à côté de UTF-8) il doit y avoir : ClangFormat

CONFIGURER CCLINT :
- Avoir python 3 installé
- `pip install cclint` ou `pip3 install cclint`
