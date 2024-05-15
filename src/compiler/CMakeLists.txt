add_subdirectory(
    "${PROJECT_SOURCE_DIR}/parser/AST"
    "${PROJECT_BINARY_DIR}/lexer"
)
add_library(compiler compiler.cpp)