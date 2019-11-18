#"ga" is the genetic algorithms library
add_library(ga SHARED ga.c includes/ga.h ga.inc)

install(
        TARGETS ga
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

install(
        FILES includes/ga.h
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