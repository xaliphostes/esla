# Example directory CMakeLists.txt

# Add the main executable
add_executable(esla-interpreter main.cxx)

# Link against the ESLA library
target_link_libraries(esla-interpreter PRIVATE esla)

# Copy the test.esla file to the build directory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/test.esla
    ${CMAKE_CURRENT_BINARY_DIR}/test.esla
    COPYONLY
)

# Add a custom command to run the test script
add_custom_target(run-test
    COMMAND esla-interpreter ${CMAKE_CURRENT_BINARY_DIR}/test.esla
    DEPENDS esla-interpreter
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running ESLA test script"
)