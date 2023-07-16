#pragma once

#include <vector>
#include <thread>
#include <map>
#include "color.hpp"

class ChatServer
{
public:
    ChatServer();
    void startServer(int port_num);
    void stopServer();

private:
    int server_socket_;
    std::vector<int> client_sockets_;
    std::string welcome_message_;
    std::atomic<bool> is_server_running_;

    std::map<int, std::string> client_identifiers_;

    bool createSocket();
    bool bindSocket(int port_num);
    bool listenForConnection();

    void acceptClients();
    void handleClient(int client_socket);
    void broadcastMessage(const std::string &message, int sender_socket);

    void updateClientIdentifier(int client_socket, const std::string &identifier);
};