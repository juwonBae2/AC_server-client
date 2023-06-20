#include "client.hpp"

int main()
{
    std::string server_IP = "127.0.0.1";
    int port_num = 7777;

    Client client(server_IP, port_num);

    return 0;
}