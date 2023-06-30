#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "chatHandler.hpp"
#include "consoleStyle.hpp"
#include "color.hpp"
#include "spdlog/fmt/fmt.h"

namespace
{
    int receive_message;
    std::array<char, 1024> buffer;
    // std::string buffer;
}

void Message::receiveMessage()
{
    buffer.fill('\0');
    receive_message = recv(client_socket, buffer.data(), buffer.size() - 1, 0);

    // ?????
    // receive_message = recv(client_socket, const_cast<char *>(buffer.data()), buffer.size() - 1, 0);

    if (receive_message <= 0)
    {
        fmt::print(color::setColor(color::ForeGround::RED) + "Failed to receive message. Please check server status.\n" + color::setColor(color::ForeGround::RESET));
    }
    // TODO: 서버를 통해서가 아닌 client 자체로 통신할지 안할지 여부
    // std::cerr << "Server: " << buffer << std::endl;
}

void Message::sendMessage()
{
    while (true)
    {
        // 메시지 입력
        std::cerr << (color::setColor(color::ForeGround::BRIGHT_GREEN) + "User: " + color::setColor(color::ForeGround::RESET));
        std::string message;
        std::getline(std::cin, message);

        // 메시지 서버로 전송
        send(client_socket, message.c_str(), message.length(), 0);

        // exit 메시지 수신 시 채팅 종료
        if (message == "exit")
        {
            fmt::print(color::setColor(color::ForeGround::BRIGHT_CYAN) + "Left the chat server.\n" + color::setColor(color::ForeGround::RESET));
            break;
        }
    }
}