#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "spdlog/fmt/fmt.h"
#include "client.hpp"
#include "consoleStyle.hpp"
#include "color.hpp"
#include <thread>

class pipe_ret_t
{
private:
    bool success_flag_ = false;
    std::string msg_ = "";

public:
    bool isSuccessful() const { return success_flag_; }
    std::string message() const { return msg_; }
};

Client::Client(const std::string &server_IP, int port_num)
{
    // 소켓 생성
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    initializeServerAddress(server_address, server_IP, port_num);

    bool connected = false;
    auto start_time = std::chrono::steady_clock::now();
    int loop_count = 0;

    pipe_ret_t success_flag;
    connected = success_flag.isSuccessful();

    if (!this->connectToServer(client_socket, server_address))
    {
        while (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        {
            loop_count++;
            std::cerr << color::setColor(color::ForeGround::BRIGHT_CYAN) + "Client failed to connect.\n" + color::setColor(color::ForeGround::RESET)
                      << "Make sure the server is open and listening\n";

            // TODO: 추가 방어 코드 작성 예정
            // std::cerr << "Client failed to connect: " << success_flag.message() << "\n"
            //           << "Make sure the server is open and listening\n\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(2000));

            std::cerr << color::setColor(color::ForeGround::GREEN) + "Retrying to connect...\n\n" + color::setColor(color::ForeGround::RESET);

            auto current_time = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);

            if (elapsed_time.count() >= 10)
            {
                std::cerr << color::setColor(color::ForeGround::BRIGHT_RED) + "Server connection timed out." + color::setColor(color::ForeGround::RESET) << std::endl;
                close(client_socket);
                break;
            }
            else if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == 0)
            {
                std::cerr << "Server connected successfully\n";
                // 채팅 시작
                run();
            }
        }

        // 서버 연결 실패 시 처리
        close(client_socket);
        return;
    }
    std::cerr << "Server connected successfully\n";
    // 채팅 시작
    run();
}

// 서버 주소 초기화
void Client::initializeServerAddress(struct sockaddr_in &server_address, const std::string &server_IP, int port_num)
{
    memset(&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_num);
    if (inet_pton(AF_INET, server_IP.c_str(), &(server_address.sin_addr)) <= 0)
    {
        std::cerr << "IP address setting failed." << std::endl;
        return;
    }
}

// TODO:아래 함수 전체 수정할 예정
bool Client::connectToServer(int client_socket, const struct sockaddr_in &server_address)
{
    // 서버에 연결
    int result = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    std::string server_connection_failure = console_stlye::makeInverse(color::setColor(color::ForeGround::GREEN) + "Unable to connect to server." + color::setColor(color::ForeGround::RESET));

    if (result < 0)
    {
        std::cerr << server_connection_failure << std::endl;
        return false;
    }

    return true;
}

void Client::run()
{
    int receive_message;
    std::array<char, 1024> buffer;

    // TODO: IP를 받게 할 지 안 할지는 미정
    // std::string serverIP;
    // std::cerr << "서버의 IP 주소를 입력하세요: ";
    // std::cin >> serverIP;

    // TODO: 이거 왜 작동 안해
    // 서버의 포트 번호 입력 받기
    // std::cerr << "서버의 포트 번호를 입력하세요: ";
    // std::cin >> portNum;

    while (true)
    {
        buffer.fill('\0');

        // 서버로부터 메시지 수신
        receive_message = recv(client_socket, buffer.data(), buffer.size() - 1, 0);
        if (receive_message <= 0)
        {
            fmt::print(color::setColor(color::ForeGround::RED) + "Failed to receive message. Please check server status.\n" + color::setColor(color::ForeGround::RESET));
            break;
        }

        // TODO: 서버를 통해서가 아닌 client 자체로 통신할지 안할지 여부
        // std::cerr << "Server: " << buffer << std::endl;

        while (true)
        {
            // 메시지 입력
            std::cerr << (color::setColor(color::ForeGround::BRIGHT_GREEN) + "User: " + color::setColor(color::ForeGround::RESET));
            std::string message;
            std::getline(std::cin, message);

            // 메시지 서버로 전송
            send(client_socket, message.c_str(), message.length(), 0);

            // 종료 메시지 수신 시 채팅 종료 (TODO: 작동안함)
            if (strcmp(buffer.data(), "exit") == 0)
            {
                fmt::print(color::setColor(color::ForeGround::BRIGHT_CYAN) + "Left the chat server.\n" + color::setColor(color::ForeGround::RESET));
                // close(client_socket);
                break;
            }
        }
    }

    // 소켓 종료
    close(client_socket);
}
