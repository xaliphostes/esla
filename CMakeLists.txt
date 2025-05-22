cmake_minimum_required(VERSION 3.12)
project(ESLA VERSION 0.1.0 LANGUAGES C CXX)

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

set(LINENOISE_C_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/third/linenoise.c")

if(EXISTS ${LINENOISE_C_FILE})
    set(HAVE_LINENOISE TRUE)
    message(STATUS "✓ linenoise found: ${LINENOISE_C_FILE}")
    set(LINENOISE_SOURCES ${LINENOISE_C_FILE})
else()
    set(HAVE_LINENOISE FALSE)
    message(STATUS "✗ linenoise not found")
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
    src/Repl.cxx
)

set(ALL_SOURCES ${ESLA_SOURCES})
if(HAVE_LINENOISE)
    list(APPEND ALL_SOURCES ${LINENOISE_SOURCES})
endif()


# Create library
add_library(esla STATIC ${ALL_SOURCES})

# Set include directories
target_include_directories(esla
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(HAVE_LINENOISE)
    target_compile_definitions(esla PUBLIC HAVE_LINENOISE=1)
    message(STATUS "✓ HAVE_LINENOISE=1 defined")
else()
    target_compile_definitions(esla PUBLIC HAVE_LINENOISE=0)
    message(STATUS "✗ HAVE_LINENOISE=0 defined")
endif()


if(HAVE_LINENOISE)
    set_source_files_properties(${LINENOISE_C_FILE} PROPERTIES LANGUAGE C)
endif()

# Add examples
add_subdirectory(examples/repl)
add_subdirectory(examples/bind_object)
