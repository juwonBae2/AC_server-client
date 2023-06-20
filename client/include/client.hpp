#pragma once

#include <string>
#include <arpa/inet.h>

class Client
{
public:
    Client(const std::string &server_IP, int port_num);
    void initializeServerAddress(struct sockaddr_in &server_address, const std::string &server_IP, int port_num);
    bool connectToServer(int client_socket, const struct sockaddr_in &server_address);
    void run();

private:
    int client_socket;
    struct sockaddr_in server_address;
};