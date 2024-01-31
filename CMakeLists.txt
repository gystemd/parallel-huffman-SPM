cmake_minimum_required(VERSION 3.10)
project(HuffmanProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -lpthread")
# Add all your .cpp files in the src directory
add_definitions(-DNO_DEFAULT_MAPPING)
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(HuffmanProject ${SOURCES})
# If you have any include directories, use target_include_directories
# target_include_directories(HuffmanProject PRIVATE include_directory)