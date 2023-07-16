project(consoleStyle)

add_library(${PROJECT_NAME}
    src/console_style.cpp
    src/function_test.cpp
    src/execution_result.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        spdlog::spdlog
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
        ${PROJECT_BINARY_DIR}/include
    PRIVATE
        ${PROJECT_BINARY_DIR}/src
)

