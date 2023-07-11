#pragma once

#include "chatHandler.hpp"
#include "executionResult.hpp"
#include "consoleStyle.hpp"
#include "color.hpp"
#include "spdlog/fmt/fmt.h"

#include <chrono>
#include <string>
#include <atomic>
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <unistd.h>

class Client
{
public:
    Client(const std::string &server_IP, int port_num);
    ExecutionResult sendMsg(const char *msg, size_t size);

    void run();
    bool connectToServer(int client_socket, const struct sockaddr_in &server_address);

private:
    int client_socket;
    struct sockaddr_in server_address;

    std::string server_IP;
    int port_num;

    std::atomic<bool> isConnected;
    std::atomic<bool> isClosed;

    void initializeServerAddress(struct sockaddr_in &server_address, const std::string &server_IP, int port_num);

    // ExecutionResult close();
};