#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

#include "chat_handler.hpp"
#include "console_style.hpp"
#include "color.hpp"
#include "spdlog/fmt/fmt.h"

// TODO: 추후 receiveMessage() 에 포트번호나 client의 이름을 받도록 인자를 추가 할 예정
void Message::receiveMessage()
{
    while (true)
    {
        int receive_message = recv(client_socket_, buffer_.data(), buffer_.size() - 1, 0);
        if (receive_message <= 0)
        {
            fmt::print(color::setColor(color::ForeGround::RED) + "\nFailed to receive message. Please check server status.\n" + color::setColor(color::ForeGround::RESET));
            break;
        }

        // std::cerr << (color::setColor(color::ForeGround::BRIGHT_GREEN) + "[User]: " + color::setColor(color::ForeGround::RESET));

        buffer_[receive_message] = '\0';
        std::cerr << buffer_.data() << std::endl;

        if (std::string(buffer_.data()) == "exit")
        {
            break;
        }
    }
}

void Message::sendMessage()
{
    while (true)
    {
        // 서버에 응답을 받기위해 딜레이 추가
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        std::cerr << (color::setColor(color::ForeGround::BRIGHT_GREEN) + "[User]: " + color::setColor(color::ForeGround::RESET));

        std::string message;
        std::getline(std::cin, message);

        // 메시지 서버로 전송
        send(client_socket_, message.c_str(), message.length(), 0);

        if (message == "exit")
        {
            fmt::print(color::setColor(color::ForeGround::BRIGHT_CYAN) + "Left the chat server.\n" + color::setColor(color::ForeGround::RESET));
            close(client_socket_);
            break;
        }
    }
}