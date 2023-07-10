#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "spdlog/fmt/fmt.h"
#include "client.hpp"
#include "consoleStyle.hpp"
#include "color.hpp"
#include <thread>

ExecutionResult Client::connectTo(const std::string &server_IP, int port_num)
{
    // 소켓 생성
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        std::cerr << "Socket creation failed." << std::endl;
        return ExecutionResult::failure(strerror(errno));
    }

    // TODO: ExecutionResult를 사용해 성공 실패 여부를 결정
    // try
    // {
    this->server_IP = server_IP;
    this->port_num = port_num;
    std::cerr << "Server connected successfully\n";
    // 채팅 시작

    // 수요일까지 빼기 ㅈㅂㅈㅂ
    run();

    // }
    // catch (const std::runtime_error &error)
    // {
    //     return ExecutionResult::failure(error.what());
    // }

    return ExecutionResult::success("success");
}

// 서버 주소 초기화
void Client::initializeServerAddress(struct sockaddr_in &server_address, const std::string &server_IP, int port_num)
{
    memset(&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_num);
    if (inet_pton(AF_INET, server_IP.c_str(), &(server_address.sin_addr)) <= 0)
    {
        std::cerr << "IP address setting failed." << std::endl;
        return;
    }
}

bool Client::connectToServer()
{
    struct sockaddr_in server_address;
    initializeServerAddress(server_address, server_IP, port_num);

    // 서버에 연결
    int result = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    std::string server_connection_failure = console_stlye::makeInverse(color::setColor(color::ForeGround::GREEN) + "Unable to connect to server." + color::setColor(color::ForeGround::RESET));

    if (result < 0)
    {
        std::cerr << server_connection_failure << std::endl;
        return false;
    }

    return true;
}

void Client::run()
{
    if (!connectToServer())
    {
        std::cerr << color::setColor(color::ForeGround::BRIGHT_RED) + "Server connection timed out." + color::setColor(color::ForeGround::RESET) << std::endl;
        close(client_socket);
        return;
    }
    else
    {
        // 채팅 시작
        Message messageHandler(client_socket);
        std::thread receive_message(&Message::receiveMessage, &messageHandler);
        std::thread send_message(&Message::sendMessage, &messageHandler);

        receive_message.join();
        send_message.join();
    }

    // 서버의 포트 번호 입력 받기
    // int portNum;
    // std::cerr << "서버의 포트 번호를 입력하세요: ";
    // std::cin >> portNum;
}