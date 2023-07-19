#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <iomanip>

#include "chat_handler.hpp"
#include "console_style.hpp"
#include "color.hpp"
#include "spdlog/fmt/fmt.h"

std::string Message::getLineWithPrompt()
{
    std::string message;
    // std::cout << "Please enter your message: ";
    std::cout << std::right << std::setw(50);
    std::getline(std::cin, message);

    return message;
}

// TODO: receiveMessage() 에 포트번호나 client의 이름을 받도록 인자를 추가 할 예정
void Message::receiveMessage()
{
    while (true)
    {
        int receive_message = recv(client_socket_, buffer_.data(), buffer_.size() - 1, 0);
        if (receive_message <= 0)
        {
            // TODO: send에 딜레이 추가 후 로그 추가
            // fmt::print(color::setColor(color::ForeGround::RED) + "\nFailed to receive message. Please check server status.\n" + color::setColor(color::ForeGround::RESET));
            break;
        }

        buffer_[receive_message] = '\0';
        std::string received_str(buffer_.data());

        if (received_str == "exit")
        {
            std::cerr << "The connection with the " << color::setColor(color::ForeGround::BRIGHT_BLUE) + "CLIENT #" + color::setColor(color::ForeGround::RESET)
                      << color::setColor(color::ForeGround::BRIGHT_RED) + std::to_string(client_socket_) + color::setColor(color::ForeGround::RESET) << " has been terminated." << std::endl;
            break;
        }
        else if (received_str.find("has left the chat.") != std::string::npos)
        {
            std::cerr << received_str << std::endl;
        }
        else
        {
            std::cerr << buffer_.data() << std::endl;
        }
    }
}

void Message::sendMessage()
{
    // 서버에 응답을 받기위해 딜레이 추가
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    while (true)
    {
        std::string message = getLineWithPrompt();

        // 메시지 서버로 전송
        send(client_socket_, message.c_str(), message.length(), 0);

        // 이전 라인으로 이동하고 그 라인을 지우는 역할 (ANSI 코드)
        std::cout << "\033[F\033[J";
        std::cerr << std::right << std::setw(50) << (color::setColor(color::ForeGround::BRIGHT_GREEN) + "[User]: " + color::setColor(color::ForeGround::RESET)) << message << std::endl;

        if (message == "exit")
        {
            fmt::print(color::setColor(color::ForeGround::BRIGHT_CYAN) + "Left the chat server.\n" + color::setColor(color::ForeGround::RESET));

            // // "exit" 메시지를 보낸 후, 1초 동안 메시지 수신 시도 근데 왜 안돼 ..
            // std::array<char, 1024> buffer{'\0'};
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            // recv(client_socket_, buffer.data(), buffer.size() - 1, 0);
            // fmt::print(color::setColor(color::ForeGround::BRIGHT_CYAN) + "{}\n" + color::setColor(color::ForeGround::RESET), buffer.data());

            close(client_socket_);
            break;
        }
    }
}