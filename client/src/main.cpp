#include "client.hpp"
#include <thread>

int main()
{
    std::string serverIP = "127.0.0.1"; // 서버 IP 주소
    int portNum = 7777;                 // 서버 포트 번호

    Client client;
    ExecutionResult result = client.connectTo(serverIP, portNum);

    if (!client.connectToServer())
    {
        auto start_time = std::chrono::steady_clock::now();
        auto end_time = start_time + std::chrono::seconds(100);

        while (std::chrono::steady_clock::now() < end_time)
        {
            if (client.connectToServer())
            {
                client.run();
                break;
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));

            std::cerr << color::setColor(color::ForeGround::BRIGHT_RED) + "Server connection timed out." + color::setColor(color::ForeGround::RESET) << std::endl;
        }
    }
    else
    {
        client.run();
    }

    return 0;
}
