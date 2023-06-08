project(client)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

# # 빌드 타겟
# add_library(${PROJECT_NAME}
#             # src/tcp_client.cpp
#             # src/tcp_server.cpp
#             # src/client.cpp
#             # src/pipe_ert_t.cpp
#             # src/common.cpp
#             src/main.cpp
# )

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