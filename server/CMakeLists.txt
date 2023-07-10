project(server)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/server.cpp
)

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

target_include_directories(${PROJECT_NAME}
  PUBLIC
    include
    ${PROJECT_BINARY_DIR}/include
  PRIVATE
    ${PROJECT_BINARY_DIR}/src
)
