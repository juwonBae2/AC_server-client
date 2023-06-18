#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <vector>

#define PORT 7777

int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *hello = "Hello from server";
    std::vector<int> clientSockets; // Stores connected client sockets

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    // Attaching socket to the port
    int status = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    std::cout << status << std::endl;
    if (status)
    {
        std::cerr << "Setsockopt error" << std::endl;
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen error" << std::endl;
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true)
    {
        // Accept incoming connections
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            std::cerr << "Accept error" << std::endl;
            return -1;
        }

        // Add the new client socket to the vector
        clientSockets.push_back(new_socket);

        // Receive message from the client
        valread = read(new_socket, buffer, 1024);
        std::cout << "Client: " << buffer << std::endl;

        // Send message to all connected clients
        for (int clientSocket : clientSockets)
        {
            send(clientSocket, buffer, strlen(buffer), 0);
        }

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
    }

    // Close all client sockets
    for (int clientSocket : clientSockets)
    {
        close(clientSocket);
    }

    // Close the server socket
    close(server_fd);

    return 0;
}
