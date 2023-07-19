#include "server.hpp"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <array>
#include <thread>

ChatServer::ChatServer()
{
    server_socket_ = -1;
    is_server_running_ = true;
    welcome_message_ = R"(  ▶ Welcome to chatting server ◀)";
}

void ChatServer::startServer(int port_num)
{
    if (createSocket() && bindSocket(port_num) && listenForConnection())
    {
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
                  << color::setColor(color::ForeGround::BRIGHT_WHITE) + welcome_message_ + color::setColor(color::ForeGround::RESET) << "\n"
                  << std::endl;

        acceptClients();
    }
}

void ChatServer::stopServer()
{
    is_server_running_ = false;
}

bool ChatServer::createSocket()
{
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0)
    {
        std::cerr << "Socket creation failed." << std::endl;
        return false;
    }
    return true;
}

bool ChatServer::bindSocket(int port_num)
{
    struct sockaddr_in server_address
    {
    };
    memset(&server_address, '\0', sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_num);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // TODO: 바로 server 부활 용, 포트 번호를 받게 되면 그떄 다시 생각하자
    int reuse = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "Failed to set SO_REUSEADDR option." << std::endl;
        return false;
    }

    if (bind(server_socket_, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Binding failed." << std::endl;
        return false;
    }
    return true;
}

bool ChatServer::listenForConnection()
{
    if (listen(server_socket_, 10) < 0)
    {
        std::cerr << "Failed to wait for connection." << std::endl;
        return false;
    }
    return true;
}

void ChatServer::updateClientIdentifier(int client_socket, const std::string &identifier)
{
    client_identifiers_[client_socket] = identifier;
}

// TODO: 역 충
void ChatServer::acceptClients()
{
    struct sockaddr_in client_address
    {
    };
    socklen_t client_address_length;

    // TODO: while 보단 서버 상태에 따라 수정해ㅠ
    while (true)
    {
        client_address_length = sizeof(client_address);
        int new_socket = accept(server_socket_, (struct sockaddr *)&client_address, (socklen_t *)&client_address_length);
        if (new_socket < 0)
        {
            std::cerr << "Failed to accept connection." << std::endl;
            return;
        }

        send(new_socket, welcome_message_.c_str(), welcome_message_.length(), 0);
        client_sockets_.push_back(new_socket);

        std::string client_identifier = color::setColor(color::ForeGround::BRIGHT_BLUE) + "CLIENT #" + color::setColor(color::ForeGround::RESET) + std::to_string(new_socket);
        updateClientIdentifier(new_socket, client_identifier);

        std::thread client_thread(&ChatServer::handleClient, this, new_socket);
        client_thread.detach();
    }
}

void ChatServer::handleClient(int client_socket)
{
    std::array<char, 1024> buffer{};
    int receive_message;

    while (true)
    {
        buffer.fill('\0');

        receive_message = recv(client_socket, buffer.data(), buffer.size() - 1, 0);
        if (receive_message <= 0)
        {
            std::cerr << "The connection with the " << color::setColor(color::ForeGround::BRIGHT_BLUE) + "CLIENT #" + color::setColor(color::ForeGround::RESET)
                      << color::setColor(color::ForeGround::BRIGHT_RED) + std::to_string(client_socket) + color::setColor(color::ForeGround::RESET) << " has been terminated." << std::endl;

            stopServer();
            break;
        }

        std::string message(buffer.data());

        if (message == "exit")
        {
            std::cerr << "The connection with the " << color::setColor(color::ForeGround::BRIGHT_BLUE) + "CLIENT #" + color::setColor(color::ForeGround::RESET)
                      << color::setColor(color::ForeGround::BRIGHT_RED) + std::to_string(client_socket) + color::setColor(color::ForeGround::RESET) << " has been terminated." << std::endl;

            stopServer();
            break;
        }

        broadcastMessage(message, client_socket);
    }

    close(client_socket);
    auto remove_socket = std::find(client_sockets_.begin(), client_sockets_.end(), client_socket);
    if (remove_socket != client_sockets_.end())
    {
        client_sockets_.erase(remove_socket);
    }
}

void ChatServer::broadcastMessage(const std::string &message, int sender_socket)
{
    std::string sender_identifier = client_identifiers_[sender_socket];

    for (const auto &client : client_identifiers_)
    {
        int client_socket = client.first;
        const std::string &client_identifier = client.second;

        if (client_socket != sender_socket)
        {
            std::string formatted_message = "[" + sender_identifier + "]: " + message;
            send(client_socket, formatted_message.c_str(), formatted_message.length(), 0);
        }
    }

    std::cout << "Message from " << color::setColor(color::ForeGround::BRIGHT_BLUE) + sender_identifier + color::setColor(color::ForeGround::RESET) << ": " << message << std::endl;
}