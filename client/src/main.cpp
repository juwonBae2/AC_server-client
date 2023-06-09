// #include <iostream>
// #include "spdlog/fmt/fmt.h"
// #include "consoleStyle.hpp"
// #include "color.hpp"

// int main()
// {
//     fmt::print("This is the client dir.\n");

//     std::string text = "Font test";

//     std::string boldText = console_stlye::makeBold(text);
//     std::string underlinedText = console_stlye::makeUnderlined(text);
//     std::string italicText = console_stlye::makeItalic(text);
//     std::string inverseText = console_stlye::makeInverse(text);

//     std::cout << "Bold: " << boldText << std::endl;
//     std::cout << "Underlined: " << underlinedText << std::endl;
//     std::cout << "Italic: " << italicText << std::endl;
//     std::cout << "Inverse: " << inverseText << std::endl;

//     return 0;
// }

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 7777

int main(int argc, char const *argv[])
{
    int sock = 0;
    int valread;
    struct sockaddr_in serv_addr;
    const char *hello = "Hello from client";
    char buffer[1024] = {0};

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    // Send message to the server
    send(sock, hello, strlen(hello), 0);
    std::cout << "Hello message sent" << std::endl;

    // Receive message from the server
    valread = read(sock, buffer, 1024);
    std::cout << buffer << std::endl;

    return 0;
}