#pragma once
#include <arpa/inet.h>

class Message
{
public:
    Message(int socket) : client_socket(socket) {}

    void receiveMessage();
    void sendMessage();

private:
    int client_socket;
    struct sockaddr_in server_address;
};