************
Presentation
************
This project was made by Group 14.
In order to use the ```libga`` library :

* run ``cmake``

* run ``make``

* run ``make docs``

* run ``make install``

You can then use the header file "``ga.h``".

This library allows you to create a "generator" which defines the range of values that an individual can take.
You can then create a population using this generator (you can of course specify the number of individuals).

You can make the population evolve until the problem is solved.

The "``libsudoku``" library, on the other hand, allows you to load/write a sudoku from/to a file.
It also provides the fitness function : ``evaluate``.

The executable "```sudoku/main.c``" contains the actual code of the sudoku program : it used both ``libga`` and ``libsudoku`` to solve a sudoku.