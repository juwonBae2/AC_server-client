#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "spdlog/fmt/fmt.h"
#include "client.hpp"
#include "consoleStyle.hpp"

Client::Client(const std::string &server_IP, int port_num)
{
    // 소켓 생성
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    // 서버 주소 초기화
    memset(&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_num);
    if (inet_pton(AF_INET, server_IP.c_str(), &(server_address.sin_addr)) <= 0)
    {
        std::cerr << "IP address setting failed." << std::endl;
        return;
    }

    std::string text = "Unable to connect to server.";
    std::string inverseText = console_stlye::makeInverse(text);

    // 서버에 연결
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << inverseText << std::endl;
        // std::cerr << "Unable to connect to server." << std::endl;
        return;
    }
}

void Client::run()
{
    int n;
    char buffer[1024];

    // std::string serverIP = "127.0.0.1";
    // TODO: IP를 받게 할 지 안 할지는 미정
    // std::string serverIP;
    // std::cout << "서버의 IP 주소를 입력하세요: ";
    // std::cin >> serverIP;

    // TODO: 이거 왜 작동 안해
    // 서버의 포트 번호 입력 받기
    // std::cout << "서버의 포트 번호를 입력하세요: ";
    // std::cin >> portNum;

    while (true)
    {
        // 서버로부터 메시지 수신
        memset(buffer, '\0', sizeof(buffer));
        n = recv(client_socket, buffer, sizeof(buffer), 0);
        if (n <= 0)
        {
            std::cerr << "Failed to receive message." << std::endl;
            break;
        }

        // TODO: 서버를 통해서가 아닌 client 자체로 통신할지 안할지 여부
        // std::cout << "Server: " << buffer << std::endl;

        // 종료 메시지 수신 시 채팅 종료
        if (strcmp(buffer, "종료") == 0)
            break;

        // 사용자로부터 메시지 입력
        std::cout << "User: ";
        std::string message;
        std::getline(std::cin, message);

        // 메시지 서버로 전송
        send(client_socket, message.c_str(), message.length(), 0);
    }

    // 소켓 종료
    close(client_socket);
}
