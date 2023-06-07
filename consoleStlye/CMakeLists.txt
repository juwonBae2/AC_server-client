project(consoleStlye)

add_library(${PROJECT_NAME}
    src/consoleStlye.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
)