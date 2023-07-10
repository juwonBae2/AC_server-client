#include "server.hpp"

int main()
{
    int port = 7777;

    ChatServer server;
    server.start(port);

    return 0;
}