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
    ExecutionResult connectTo(const std::string &server_ip_, int port_num_);
    ExecutionResult sendMsg(const char *msg_, size_t size_);

    void run();
    bool connectToServer();

private:
    int client_socket_;
    struct sockaddr_in server_address_;

    std::string server_ip_;
    int port_num_;

    std::atomic<bool> is_connected_;
    std::atomic<bool> is_closed_;

    void initializeServerAddress(struct sockaddr_in &server_address_, const std::string &server_ip_, int port_num_);

    // ExecutionResult close();
};