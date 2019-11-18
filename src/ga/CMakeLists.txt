#"ga" is the genetic algorithms library
add_library(ga SHARED ga.c includes/ga.h ga.inc)

install(
        TARGETS ga
        LIBRARY DESTINATION lib COMPONENT "Library"
        ARCHIVE DESTINATION lib COMPONENT "Library"
        RUNTIME DESTINATION bin COMPONENT "Runtime"
)

install(
        FILES includes/ga.h
        DESTINATION include
        COMPONENT "Header"
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