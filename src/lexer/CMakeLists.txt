add_subdirectory(
    "${PROJECT_SOURCE_DIR}/errors"
    "${PROJECT_BINARY_DIR}/errors"
)

add_library(lexer lexer.cpp string.cpp helper.cpp tokens.cpp)

target_link_libraries(lexer errors)

target_include_directories(lexer PUBLIC
    "${PROJECT_SOURCE_DIR}/errors"
)