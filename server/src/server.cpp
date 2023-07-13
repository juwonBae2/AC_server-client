#include "server.hpp"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <array>

ChatServer::ChatServer()
{
    server_socket = -1;
    welcome_message = R"(  ▶ Welcome to chatting server ◀)";
}

void ChatServer::start(int port_num)
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
                  << color::setColor(color::ForeGround::BRIGHT_WHITE) + welcome_message + color::setColor(color::ForeGround::RESET) << "\n"
                  << std::endl;

        acceptClients();
    }
}

bool ChatServer::createSocket()
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
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

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Binding failed." << std::endl;
        return false;
    }
    return true;
}

bool ChatServer::listenForConnection()
{
    if (listen(server_socket, 10) < 0)
    {
        std::cerr << "Failed to wait for connection." << std::endl;
        return false;
    }
    return true;
}

void ChatServer::acceptClients()
{
    struct sockaddr_in client_address
    {
    };
    socklen_t client_address_length;

    while (true)
    {
        client_address_length = sizeof(client_address);
        int new_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&client_address_length);
        if (new_socket < 0)
        {
            std::cerr << "Failed to accept connection." << std::endl;
            return;
        }

        send(new_socket, welcome_message.c_str(), welcome_message.length(), 0);
        client_sockets.push_back(new_socket);

        // 이거 때문에 서버 닫는 시간이 오래걸림
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
            break;
        }

        std::cout << color::setColor(color::ForeGround::BRIGHT_BLUE) + "CLIENT #" + color::setColor(color::ForeGround::RESET) << client_socket
                  << ": " << color::setColor(color::ForeGround::WHITE) + buffer.data() + color::setColor(color::ForeGround::RESET) << std::endl;

        if (strcmp(buffer.data(), "exit") == 0)
        {
            std::cerr << "The connection with the " << color::setColor(color::ForeGround::BRIGHT_BLUE) + "CLIENT #" + color::setColor(color::ForeGround::RESET)
                      << color::setColor(color::ForeGround::BRIGHT_RED) + std::to_string(client_socket) + color::setColor(color::ForeGround::RESET) << " has been terminated." << std::endl;
            break;
        }

        for (int client : client_sockets)
        {
            if (client != client_socket)
            {
                send(client, buffer.data(), receive_message, 0);
            }
        }
    }

    close(client_socket);
    auto remove_socket = std::find(client_sockets.begin(), client_sockets.end(), client_socket);
    if (remove_socket != client_sockets.end())
    {
        client_sockets.erase(remove_socket);
    }
}
