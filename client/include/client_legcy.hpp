#pragma once

#include "chat_handler.hpp"
#include "execution_result.hpp"
#include "console_style.hpp"
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
    Client(const std::string &server_IP_, int port_num_);
    ExecutionResult sendMsg(const char *msg, size_t size);

    void run();
    bool connectToServer(int client_socket_, const struct sockaddr_in &server_address_);

private:
    int client_socket_;
    struct sockaddr_in server_address_;

    std::string server_IP_;
    int port_num_;

    std::atomic<bool> is_connected_;
    std::atomic<bool> is_closed_;

    void initializeServerAddress(struct sockaddr_in &server_address_, const std::string &server_IP_, int port_num_);

    // ExecutionResult close();
};