#include "client.hpp"

int main()
{
    std::string server_ip = "127.0.0.1"; // 서버 IP 주소
    int port_num = 7777;                 // 서버 포트 번호

    Client client;
    ExecutionResult result = client.connectTo(server_ip, port_num);

    if (result.success())
    {
        client.run();
    }
    else
    {
        std::cerr << "Failed to connect to the server: " << result.message() << std::endl;
        return 1;
    }

    return 0;
}
