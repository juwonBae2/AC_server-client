project(server)

add_executable(${PROJECT_NAME}
    # src/main.cpp
    src/server.cpp
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${PROJECT_BINARY_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        spdlog::spdlog
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
)