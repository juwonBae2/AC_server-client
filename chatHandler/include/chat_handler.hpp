#pragma once

#include <arpa/inet.h>
#include <array>

class Message
{
public:
    Message(int socket) : client_socket_(socket) {}

    void receiveMessage();
    void sendMessage();

    std::string getLineWithPrompt();

private:
    int client_socket_;
    struct sockaddr_in server_address_;

    int receive_message_;
    std::array<char, 1024> buffer_;
};