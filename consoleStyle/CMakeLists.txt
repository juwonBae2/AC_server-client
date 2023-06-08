project(consoleStyle)

add_library(${PROJECT_NAME}
    src/consoleStyle.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
)