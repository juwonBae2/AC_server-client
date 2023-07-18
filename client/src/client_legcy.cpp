// 전부 지울 예정

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "spdlog/fmt/fmt.h"
#include "client_legcy.hpp"
#include "console_style.hpp"
#include "color.hpp"
#include <thread>

Client::Client(const std::string &server_IP, int port_num)
{
    // 소켓 생성
    client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_ < 0)
    {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    initializeServerAddress(server_address_, server_IP, port_num);

    bool connected = false;
    auto start_time = std::chrono::steady_clock::now();
    int loop_count = 0;

    while (!connectToServer(client_socket_, server_address_))
    {
        loop_count++;
        std::cerr << color::setColor(color::ForeGround::BRIGHT_CYAN) + "Client failed to connect.\n" + color::setColor(color::ForeGround::RESET)
                  << "Make sure the server is open and listening\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        std::cerr << color::setColor(color::ForeGround::GREEN) + "Retrying to connect...\n\n" + color::setColor(color::ForeGround::RESET);

        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);

        if (elapsed_time.count() >= 10)
        {
            std::cerr << color::setColor(color::ForeGround::BRIGHT_RED) + "Server connection timed out." + color::setColor(color::ForeGround::RESET) << std::endl;
            close(client_socket_);
            return;
        }
    }

    std::cerr << "Server connected successfully\n";
    // 채팅 시작
    run();
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

bool Client::connectToServer(int client_socket, const struct sockaddr_in &server_address)
{
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
    // 서버의 포트 번호 입력 받기
    int portNum;
    std::cerr << "서버의 포트 번호를 입력하세요: ";
    std::cin >> portNum;

    Message messageHandler(client_socket_);
    std::thread receive_message(&Message::receiveMessage, &messageHandler);
    std::thread send_message(&Message::sendMessage, &messageHandler);

    receive_message.join();
    send_message.join();
}