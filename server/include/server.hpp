#pragma once

#include <vector>
#include <thread>
#include "color.hpp"

class ChatServer
{
private:
    int server_socket_;
    std::vector<int> client_sockets_;
    std::string welcome_message_;

public:
    ChatServer();
    void start(int port_num);

private:
    bool createSocket();
    bool bindSocket(int port_num);
    bool listenForConnection();
    void acceptClients();
    void handleClient(int client_socket);
};