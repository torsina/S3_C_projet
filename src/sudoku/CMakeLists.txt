#Rien pour l'instant
#project(KEK)
#message(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/ga/includes)
add_executable(main main.c)
add_library(sudoku sudoku.c includes/sudoku.h sudoku.inc)
add_dependencies(sudoku ga)
target_link_libraries(sudoku ga)
#find_package(GA REQUIRED)
#find_library(GA_LIB ga PATHS C:/Users/timgo/CLionProjects/S3_C_projet/cmake-build-debug)
#include_directories(${GA_INCLUDE_DIRS})
#link_directories(${GA_LIB_DIRS})

add_dependencies(sudoku ga)
add_dependencies(main sudoku)
target_link_libraries(sudoku ga)
target_link_libraries(sudoku m)
target_link_libraries(main sudoku)

