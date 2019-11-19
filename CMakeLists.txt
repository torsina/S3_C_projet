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
#message(${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#--------------------------------------

enable_testing()
find_program(VALGRIND "valgrind")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Sphinx REQUIRED)

#Mettre les enable_testing et find_program avant.
add_subdirectory(src/)
add_subdirectory(test/)
add_subdirectory(docs/)

#Generate zip
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_IGNORE_FILES "~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

#add_custom_target(docs ALL
#        ${SPHINX_EXECUTABLE}
#        -q -b html
#        -c "${BINARY_BUILD_DIR}"
#        -d "${SPHINX_CACHE_DIR}"
#        "${CMAKE_CURRENT_SOURCE_DIR}"
#        "${SPHINX_HTML_DIR}"
#        COMMENT "Building HTML documentation with Sphinx")