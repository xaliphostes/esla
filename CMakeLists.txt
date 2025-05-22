cmake_minimum_required(VERSION 3.12)
project(ESLA VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compile options
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Define source files
set(ESLA_SOURCES
    src/AST.cxx
    src/Environment.cxx
    src/Interpreter.cxx
    src/Lexer.cxx
    src/Node.cxx
    src/Parser.cxx
    src/Token.cxx
    src/Value.cxx
)

# Create library
add_library(esla STATIC ${ESLA_SOURCES})

# Set include directories
target_include_directories(esla
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Add examples
add_subdirectory(examples/repl)
add_subdirectory(examples/bind_object)

# Install configuration
# include(GNUInstallDirs)

# install(TARGETS esla
#     EXPORT esla-targets
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# install(DIRECTORY include/
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# # Export targets
# install(EXPORT esla-targets
#     FILE esla-targets.cmake
#     NAMESPACE ESLA::
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/esla
# )

# # Generate package configuration
# include(CMakePackageConfigHelpers)

# configure_package_config_file(
#     ${CMAKE_CURRENT_SOURCE_DIR}/cmake/esla-config.cmake.in
#     ${CMAKE_CURRENT_BINARY_DIR}/esla-config.cmake
#     INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/esla
# )

# write_basic_package_version_file(
#     ${CMAKE_CURRENT_BINARY_DIR}/esla-config-version.cmake
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY SameMajorVersion
# )

# install(FILES
#     ${CMAKE_CURRENT_BINARY_DIR}/esla-config.cmake
#     ${CMAKE_CURRENT_BINARY_DIR}/esla-config-version.cmake
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/esla
# )

# # Add uninstall target
# if(NOT TARGET uninstall)
#     configure_file(
#         ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
#         ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
#         @ONLY
#     )
#     add_custom_target(uninstall
#         COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
#     )
# endif()