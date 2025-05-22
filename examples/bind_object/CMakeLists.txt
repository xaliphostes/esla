# Example directory CMakeLists.txt

# Add the main executable
add_executable(bind_object main.cxx)

# Link against the ESLA library
target_link_libraries(bind_object PRIVATE esla)

# Add a custom command to run the test script
# add_custom_target(run-test
#     COMMAND bind_object ${CMAKE_CURRENT_BINARY_DIR}/test.esla
#     DEPENDS bind_object
#     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#     COMMENT "Running ESLA test script"
# )