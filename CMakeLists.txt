cmake_minimum_required(VERSION 3.15)
project(HashMap)

# Add the src directory library to the build
add_library(source INTERFACE "")
add_subdirectory("src")
# Build executable
add_executable(${PROJECT_NAME} main.cc)
# Link in libraries.
target_link_libraries(${PROJECT_NAME} source)
