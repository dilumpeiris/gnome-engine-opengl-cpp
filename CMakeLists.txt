cmake_minimum_required(VERSION 3.16)
project(
  GnomeEngine
  VERSION 1.0.0
  LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Add source files (only main.cpp since GnomeEngine is header-only)
set(SOURCES src/main.cpp src/glad.c)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/engine
    ${CMAKE_SOURCE_DIR}/engine/core
    ${CMAKE_SOURCE_DIR}/engine/ecs
    ${CMAKE_SOURCE_DIR}/engine/input
    ${CMAKE_SOURCE_DIR}/engine/components
    ${CMAKE_SOURCE_DIR}/engine/entities
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/glad
)

# Explicitly set C++ standard library paths for LSP
if(APPLE)
  execute_process(
    COMMAND xcrun --show-sdk-path
    OUTPUT_VARIABLE MACOS_SDK_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  target_include_directories(
    ${PROJECT_NAME} SYSTEM PRIVATE ${MACOS_SDK_PATH}/usr/include/c++/v1
                                   ${MACOS_SDK_PATH}/usr/include)
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw glm::glm)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 ${CMAKE_BINARY_DIR}/bin)

# Platform-specific settings
if(APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework Cocoa" "-framework IOKit"
                        "-framework CoreVideo")
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                           "Clang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
