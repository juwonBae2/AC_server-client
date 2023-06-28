#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include "color.hpp"

std::vector<int> client_sockets;

void handleClient(int client_socket)
{
    std::array<char, 1024> buffer{};
    int receive_message;

    while (true)
    {
        buffer.fill('\0');

        // 클라이언트로부터 메시지 수신
        receive_message = recv(client_socket, buffer.data(), buffer.size() - 1, 0);
        if (receive_message <= 0)
        {
            std::cerr << "The connection with the " << color::setColor(color::ForeGround::BRIGHT_BLUE) + "CLIENT #" + color::setColor(color::ForeGround::RESET)
                      << color::setColor(color::ForeGround::BRIGHT_RED) + std::to_string(client_socket) + color::setColor(color::ForeGround::RESET) << " has been terminated." << std::endl;

            break;
        }

        std::cout << color::setColor(color::ForeGround::BRIGHT_BLUE) + "CLIENT #" + color::setColor(color::ForeGround::RESET) << client_socket
                  << ": " << color::setColor(color::ForeGround::WHITE) + buffer.data() + color::setColor(color::ForeGround::RESET) << std::endl;

        // 종료 메시지 수신 시 클라이언트 연결 종료
        if (strcmp(buffer.data(), "exit") == 0)
        {
            std::cerr << "The connection with the " << color::setColor(color::ForeGround::BRIGHT_BLUE) + "CLIENT #" + color::setColor(color::ForeGround::RESET)
                      << color::setColor(color::ForeGround::BRIGHT_RED) + std::to_string(client_socket) + color::setColor(color::ForeGround::RESET) << " has been terminated." << std::endl;

            break;
        }
        // 다른 클라이언트들에게 메시지 전달
        for (int client : client_sockets)
        {
            if (client != client_socket)
            {
                send(client, buffer.data(), receive_message, 0);
            }
        }
    }

    // 클라이언트 소켓 종료 및 제거
    close(client_socket);
    auto remove_socket = std::find(client_sockets.begin(), client_sockets.end(), client_socket);
    if (remove_socket != client_sockets.end())
    {
        client_sockets.erase(remove_socket);
    }
}

int main()
{
    int server_socket, new_socket, port_num;
    struct sockaddr_in server_address
    {
    }, client_address{};
    socklen_t client_address_length;

    std::string welcome_message = R"(  ▶ Welcome to chatting server ◀)";

    // TODO: 이거 왜 작동 안 해
    // 포트 번호 입력 받기
    // std::cout << "사용할 포트 번호를 입력하세요: ";
    // std::cin >> portNum;

    // 소켓 생성
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }

    // 서버 주소 초기화
    memset(&server_address, '\0', sizeof(server_address));
    port_num = 7777;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_num);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // 소켓 바인딩
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Binding failed." << std::endl;
        return 1;
    }

    // 연결 대기
    listen(server_socket, 1);

    if (listen(server_socket, 10) < 0)
    {
        std::cerr << "Failed to wait for connection." << std::endl;
        return 1;
    }

    // TODO: IP 까지 할 지 안 할지는 미정
    // 서버 IP 주소 출력
    // struct sockaddr_in serverInfo
    // {
    // };
    // socklen_t serverInfoLength = sizeof(serverInfo);
    // getsockname(server_socket, (struct sockaddr *)&serverInfo, &serverInfoLength);
    // char ip[INET_ADDRSTRLEN];
    // inet_ntop(AF_INET, &(serverInfo.sin_addr), ip, INET_ADDRSTRLEN);
    // std::cout << "서버 IP 주소: " << ip << std::endl;

    std::cout << color::setColor(color::ForeGround::GREEN) + R"(
 _____  _____ ______  _   _  _____ ______      _____ ______  _____  _   _  _____ ______
/  ___||  ___|| ___ \| | | ||  ___|| ___ \    /  _  \| ___ \|  ___|| \ | ||  ___||  _  \
\ `--. | |__  | |_/ /| | | || |__  | |_/ /    | | | || |_/ /| |__  |  \| || |__  | | | |
 `--. \|  __| |    / | | | ||  __| |    /     | | | ||  __/ |  __| | . ` ||  __| | | | |
/\__/ /| |___ | |\ \ \ \_/ /| |___ | |\ \     \ \_/ /| |    | |___ | |\  || |___ | |/ /
\____/ \____/ \_| \_| \___/ \____/ \_| \_|     \___/ \_|    \____/ \_| \_/\____/ |___/   o

    ∧_____∧
   ( ̳• ·̫ • ̳)
┏ー∪∪━━━━━━━━┓
  ♡°•. message .•°♡
┗━--━━━━━•━━━┛

 )" + color::setColor(color::ForeGround::RESET)
              // TODO: 아래가 작동하면 지울 예정
              << color::setColor(color::ForeGround::BRIGHT_WHITE) + welcome_message + color::setColor(color::ForeGround::RESET)
              << std::endl;

    while (true)
    {
        client_address_length = sizeof(client_address);

        // TODO: IP를 추가하여 아래 로그를 더 활용할 예정
        // 클라이언트로부터 연결 요청 대기
        new_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&client_address_length);
        if (new_socket < 0)
        {
            std::cerr << "Failed to accept connection." << std::endl;
            return 1;
        }

        // 클라이언트에게 환영 메시지 전송
        send(new_socket, welcome_message.c_str(), welcome_message.length(), 0);

        // 연결된 클라이언트 소켓을 저장
        client_sockets.push_back(new_socket);

        // 클라이언트를 처리하기 위한 스레드 생성
        std::thread client_thread(handleClient, new_socket);
        client_thread.detach();
    }

    // 소켓 종료
    close(server_socket);

    return 0;
}