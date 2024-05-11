add_subdirectory(AST)

add_library(parser parser.cpp)

target_link_libraries(parser AST)
target_link_libraries(parser errors)

target_include_directories(parser PUBLIC
    "${PROJECT_SOURCE_DIR}/AST"
    "${PROJECT_SOURCE_DIR}/lexer"
    "${PROJECT_SOURCE_DIR}/errors"
)