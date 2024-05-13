add_subdirectory(
    "${PROJECT_SOURCE_DIR}/parser/AST"
    "${PROJECT_BINARY_DIR}/parser/AST"
)
add_library(compiler compiler.cpp)