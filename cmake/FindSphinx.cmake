#Documentation
find_program(SPHINX_EXECUTABLE
        NAMES sphinx-build
        HINTS $ENV{SPHINX_DIR}
        PATH_SUFFIXES bin
        DOC "Sphinx documentation generator"
        )

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Sphinx DEFAULT_MSG
        SPHINX_EXECUTABLE
        )

mark_as_advanced(SPHINX_EXECUTABLE)

#find_program(SPHINX_EXECUTABLE NAMES sphinx-build
#        HINTS
#        $ENV{SPHINX_DIR}
#        PATH_SUFFIXES bin
#        DOC "Sphinx documentation generator"
#        )
#
#include(FindPackageHandleStandardArgs)
#
#find_package_handle_standard_args(Sphinx DEFAULT_MSG
#        SPHINX_EXECUTABLE
#        )
#
#mark_as_advanced(SPHINX_EXECUTABLE)
#
#
## configured documentation tools and intermediate build results
#set(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/../docs/_build")
#
## Sphinx cache with pickled ReST documents
#set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/../docs/_doctrees")
#
## HTML output directory
#set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/../docs/html")
#
#if(NOT DEFINED SPHINX_THEME)
#    set(SPHINX_THEME default)
#endif()
#
#if(NOT DEFINED SPHINX_THEME_DIR)
#    set(SPHINX_THEME_DIR)
#endif()
#
#configure_file(
#        "conf.py.in"
#        "${BINARY_BUILD_DIR}/conf.py"
#        @ONLY)