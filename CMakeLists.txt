cmake_minimum_required(VERSION 3.0)

project("Genetic Algorithm" C)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
#Standard C11
set (CMAKE_C_STANDARD 11)

#----------------VERSION----------------
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})
#---------------------------------------

#----------------OUT DIR----------------
#CMAKE_BINARY_DIR == cmake-build-debug
message(${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#--------------------------------------


add_subdirectory(src/)
add_subdirectory(test/)


enable_testing()

find_program(VALGRIND "valgrind")

#Generate
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_IGNORE_FILES "~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

file(GLOB FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/ga/test-*.c")

foreach(FILENAME ${FILES})
	get_filename_component(SRC ${FILENAME} NAME)
	get_filename_component(TEST ${FILENAME} NAME_WE)
	add_executable(${TEST} "${CMAKE_CURRENT_SOURCE_DIR}/test/ga/${SRC}" src/ga/ga.c src/ga/includes/ga.h src/ga/ga.inc)
	add_dependencies(${TEST} ga)
	target_link_libraries(${TEST} ga)
	if(VALGRIND)
		add_test("${TEST}[valgrind]" ${VALGRIND} --leak-check=full --quiet --error-exitcode=1 ./${TEST})
    	add_test("${TEST}[normal]" ./${TEST})
    else()
    	add_test("${TEST}" ./${TEST})
	endif()
endforeach()

