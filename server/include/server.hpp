#pragma once

#include <vector>
#include <thread>
#include "color.hpp"

class ChatServer
{
private:
    int server_socket;
    std::vector<int> client_sockets;
    std::string welcome_message;

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