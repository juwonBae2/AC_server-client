#include "server.hpp"

int main()
{
    int port = 7777;

    ChatServer server;
    server.startServer(port);

    return 0;
}