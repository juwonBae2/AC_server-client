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
        # TODO: consoleStlye내 cmake를 받게 되어 있으니 이건 지워도 되나..??
        # spdlog::spdlog
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
        consoleStlye
)