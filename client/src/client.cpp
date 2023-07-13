#include "client.hpp"

ExecutionResult Client::connectTo(const std::string &server_ip, int port_num)
{
    // 소켓 생성
    client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_ < 0)
    {
        std::cerr << "Socket creation failed." << std::endl;
        return ExecutionResult::failure(strerror(errno));
    }

    // TODO: ExecutionResult를 사용해 성공 실패 여부를 결정
    this->server_ip_ = server_ip;
    this->port_num_ = port_num;
    std::cerr << "Server connected successfully\n";
    // 채팅 시작

    // 미구현
    return ExecutionResult::success("success");
}

// 서버 주소 초기화
void Client::initializeServerAddress(struct sockaddr_in &server_address_, const std::string &server_ip_, int port_num_)
{
    memset(&server_address_, '\0', sizeof(server_address_));
    server_address_.sin_family = AF_INET;
    server_address_.sin_port = htons(port_num_);
    if (inet_pton(AF_INET, server_ip_.c_str(), &(server_address_.sin_addr)) <= 0)
    {
        std::cerr << "IP address setting failed." << std::endl;
        return;
    }
}

bool Client::connectToServer()
{
    struct sockaddr_in server_address;
    initializeServerAddress(server_address, server_ip_, port_num_);

    // 서버에 연결
    int result = connect(client_socket_, (struct sockaddr *)&server_address, sizeof(server_address));
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
    // if (!connectToServer())
    // {
    //     std::cerr << color::setColor(color::ForeGround::BRIGHT_RED) + "Server connection timed out." + color::setColor(color::ForeGround::RESET) << std::endl;
    //     close(client_socket);
    //     return;
    // }
    // else
    // {
    // 채팅 시작
    Message messageHandler(client_socket_);
    std::thread receive_message(&Message::receiveMessage, &messageHandler);
    std::thread send_message(&Message::sendMessage, &messageHandler);

    receive_message.join();
    send_message.join();
    // }

    // // 서버의 포트 번호 입력 받기
    // int portNum;
    // std::cerr << "서버의 포트 번호를 입력하세요: ";
    // std::cin >> portNum;
}
