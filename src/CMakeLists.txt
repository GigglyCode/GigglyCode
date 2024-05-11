cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(GigglyCode LANGUAGES CXX)

add_subdirectory(lexer)
add_subdirectory(parser)

add_executable(gigly main.cpp)

target_link_libraries(gigly lexer)
target_link_libraries(gigly parser)

target_include_directories(gigly PUBLIC
    "${PROJECT_SOURCE_DIR}/lexer"
    "${PROJECT_SOURCE_DIR}/parser"
)