find_package(yaml)

if(NOT DEFINED ${yaml_FOUND})
    message("yaml package not found, maybe it isn't installed correctly ?")
    find_library(yaml REQUIRED)
endif(NOT DEFINED ${yaml_FOUND})

include_directories(${CMAKE_SOURCE_DIR}/src/ga/includes)
add_executable(main main.c)
target_link_libraries(main yaml)
add_library(sudoku sudoku.c includes/sudoku.h sudoku.inc)
add_dependencies(sudoku ga)
target_link_libraries(sudoku ga)
target_link_libraries(sudoku yaml)
#find_package(GA REQUIRED)
#find_library(GA_LIB ga PATHS C:/Users/timgo/CLionProjects/S3_C_projet/cmake-build-debug)
#include_directories(${GA_INCLUDE_DIRS})
#link_directories(${GA_LIB_DIRS})

add_dependencies(sudoku ga)
add_dependencies(main sudoku)
target_link_libraries(sudoku ga)
target_link_libraries(sudoku m)
target_link_libraries(main sudoku)


if (VALGRIND)
    add_test("main[valgrind]" ${VALGRIND} --leak-check=full --error-exitcode=1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main ../../examples/teacher.yaml 0.2 .05 5 5 --verbose)
    add_test("main[normal]" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main ../../examples/teacher.yaml 0.2 .05 100 100)
else ()

    add_test("main[normal]" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main ../../examples/teacher.yaml 0.2 .05 100 100)
endif ()