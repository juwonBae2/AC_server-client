project(chatHandler)

add_library(${PROJECT_NAME}
    src/chat_handler.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        spdlog::spdlog
        consoleStyle
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
        ${PROJECT_BINARY_DIR}/include
    PRIVATE
        ${PROJECT_BINARY_DIR}/src
)