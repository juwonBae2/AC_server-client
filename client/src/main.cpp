#include "client.hpp"

int main()
{
    std::string serverIP = "127.0.0.1"; // 서버 IP 주소
    int portNum = 7777;                 // 서버 포트 번호

    Client client;
    ExecutionResult result = client.connectTo(serverIP, portNum);
    // else

    // if (!connectToServer())
    // {
    //     auto start_time = std::chrono::steady_clock::now();
    //     auto end_time = start_time + std::chrono::seconds(10);

    //     while (std::chrono::steady_clock::now() < end_time)
    //     {
    //         std::cerr << color::setColor(color::ForeGround::BRIGHT_CYAN) + "Client failed to connect.\n" + color::setColor(color::ForeGround::RESET)
    //                   << "Make sure the server is open and listening\n";

    //         std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    //         std::cerr << color::setColor(color::ForeGround::GREEN) + "Retrying to connect...\n\n" + color::setColor(color::ForeGround::RESET);

    //         if (connectToServer())
    //         {
    //             std::cerr << "Server connected successfully\n";
    //             // 채팅 시작
    //             break;
    //         }
    //     }

    //     if (std::chrono::steady_clock::now() >= end_time)
    //     {
    //         std::cerr << color::setColor(color::ForeGround::BRIGHT_RED) + "Server connection timed out." + color::setColor(color::ForeGround::RESET) << std::endl;
    //         // close();
    //         return;
    //     }
    // }

    return 0;
}
