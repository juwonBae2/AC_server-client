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

    // Send and receive messages
    while (true)
    {
        // Send message to the server
        std::string message;
        std::cout << "Enter a message: ";
        std::getline(std::cin, message);
        send(sock, message.c_str(), message.length(), 0);

        // Check if the user wants to exit
        if (message == "exit")
            break;

        // Receive message from the server
        valread = read(sock, buffer, 1024);
        std::cout << "Server: " << buffer << std::endl;

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
    }

    // Close the socket
    close(sock);

    return 0;
}
