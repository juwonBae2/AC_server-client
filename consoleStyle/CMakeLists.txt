project(consoleStyle)

add_library(${PROJECT_NAME}
    src/consoleStyle.cpp
    src/test.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        spdlog::spdlog
)