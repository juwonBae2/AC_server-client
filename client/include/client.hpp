#pragma once

#include <string>
#include <arpa/inet.h>

class Client
{
private:
    int client_socket;
    struct sockaddr_in server_address;

public:
    Client(const std::string &server_IP, int port_num);
    void run();
};