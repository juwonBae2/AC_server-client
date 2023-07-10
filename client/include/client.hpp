#pragma once

#include "chatHandler.hpp"
#include "executionResult.hpp"

#include <string>
#include <atomic>
#include <arpa/inet.h>
#include <iostream>

class Client
{
public:
    ExecutionResult connectTo(const std::string &server_IP, int port_num);
    ExecutionResult sendMsg(const char *msg, size_t size);

private:
    int client_socket;
    struct sockaddr_in server_address;

    std::string server_IP;
    int port_num;

    std::atomic<bool> isConnected;
    std::atomic<bool> isClosed;

    void initializeServerAddress(struct sockaddr_in &server_address, const std::string &server_IP, int port_num);
    bool connectToServer();

    void run();
    // ExecutionResult close();
};