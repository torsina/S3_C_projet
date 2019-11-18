cmake_minimum_required(VERSION 3.0)

project("Genetic Algorithm" C)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
#Standard C11
set (CMAKE_C_STANDARD 11)

#"ga" is the genetic algorithms library
add_library(ga SHARED ga.c ga.h ga.inc)


install(
	TARGETS ga
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

install(
	FILES ga.h
	DESTINATION include
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/GAConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/GAConfig.cmake
	INSTALL_DESTINATION cmake
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/GAConfig.cmake
	DESTINATION cmake
)

enable_testing()

find_program(VALGRIND "valgrind")

file(GLOB FILES "${CMAKE_CURRENT_SOURCE_DIR}/test-*.c")

foreach(FILENAME ${FILES})
	get_filename_component(SRC ${FILENAME} NAME)
	get_filename_component(TEST ${FILENAME} NAME_WE)
	add_executable(${TEST} ${SRC} ga.c ga.h ga.inc)
	add_dependencies(${TEST} ga)
	target_link_libraries(${TEST} ga)
	if(VALGRIND)
		add_test("${TEST}[valgrind]" ${VALGRIND} --leak-check=full --quiet --error-exitcode=1 ./${TEST})
    	add_test("${TEST}[normal]" ./${TEST})
    else()
    	add_test("${TEST}" ./${TEST})
	endif()
endforeach()

set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_IGNORE_FILES "~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

