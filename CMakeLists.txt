cmake_minimum_required(VERSION 3.10)

project(JsonParser VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES "*.cpp" "*.cc" "*.c")

if(NOT SOURCES)
    message(WARNING "No source files found!")
endif()

add_executable(JsonParser ${SOURCES})

target_include_directories(JsonParser PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
