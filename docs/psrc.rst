=================================
Private Source Code documentation
=================================

.. hlist::
   :columns: 3

   * A list of
   * short items
   * that should be
   * displayed
   * horizontally

.. centered:: Ceci est un texte centré
.. rubric:: Ceci est une rubrique

Texte dans la rubrique.
*Texte en italique*, **en gras** et en ``formatté``.
Exemple de liste :

* Bullet list 1
    * Sous liste 1
    * Sous liste 2
* Bullet list 2
  Multiline

1. Numérotée 1
2. Numérotée 2

#. Numérotée auto 1
#. Numérotée auto 2

| Normalement, Sphinx ignore les retours
| à la ligne dans le texte
| avec une barre verticale "|" au début,
| on force la préservation des retours à
| la ligne.

>>> 1+1

========
SECTIONS
========
# with overline, for parts

* with overline, for chapters

=, for sections

-, for subsections

^, for subsubsections

", for paragraphs

.. code-block:: c
    :linenos:
    :lineno-start: 0
    :emphasize-lines: 3,4

    //Code
    int main(void) {
        double a = 0.0;
        return -1;
    }


.. seealso::
   Module :py:mod:`zipfile`
      Documentation of the :py:mod:`zipfile` standard module.
   `GNU tar manual, Basic Tar Format <http://link>`_
      Documentation for tar archive files, including GNU tar extensions.
   `Voir <https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html#rst-doctest-blocks>`_
