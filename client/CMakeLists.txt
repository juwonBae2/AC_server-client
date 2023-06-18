project(client)

add_executable(${PROJECT_NAME}
    src/main.cpp
    # src/client.cpp
)

# target_include_directories(${PROJECT_NAME} PRIVATE "include")

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        spdlog::spdlog
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
        consoleStyle
)