#Indique au compilateur d'aller chercher les headers dans ce dossier
include_directories(${CMAKE_SOURCE_DIR}/src/includes)

file(GLOB FILES "${CMAKE_CURRENT_SOURCE_DIR}/test-*.c")
message(${CMAKE_SOURCE_DIR})
foreach (FILENAME ${FILES})
    get_filename_component(SRC ${FILENAME} NAME)
    get_filename_component(TEST ${FILENAME} NAME_WE)
    add_executable(${TEST} "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")

    add_dependencies(${TEST} sudoku)
    target_link_libraries(${TEST} sudoku)
    add_custom_command(
            TARGET ${TEST}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/examples
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../../examples)

    if (VALGRIND)
        #Rajouter --quiet si besoin
        add_test("${TEST}[valgrind]" ls ..; ${VALGRIND} --leak-check=full --error-exitcode=1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
        #add_test("${TEST}[normal]" ./${TEST} WORKING_DIRECTORY ${})
        add_test("${TEST}[normal]" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
        #add_test("${TEST}[normal]" ./${CMAKE_CURRENT_BINARY_DIR}/${filename} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    else ()
        #message("OUTPUT -> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
        #message("COMMAND -> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SRC}")
        #add_test("${TEST}[normal]" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Testing/${filename} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        add_test("${TEST}[normal]" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
        #add_test("${TEST}" ./${TEST})
    endif ()
endforeach ()
