#include "client.hpp"

// TODO: 함수로 뺄 예정.. 전체적으로 리펙토링 해야됨
ExecutionResult Client::connectTo(const std::string &serverIP, int portNum)
{
    // 소켓 생성
    client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_ < 0)
    {
        std::cerr << "Socket creation failed." << std::endl;
        return ExecutionResult::failure(strerror(errno));
    }

    initializeServerAddress(server_address_, serverIP, portNum);
    if (inet_pton(AF_INET, serverIP.c_str(), &(server_address_.sin_addr)) <= 0)
    {
        std::cerr << color::setColor(color::ForeGround::BRIGHT_RED) + "IP address setting failed." + color::setColor(color::ForeGround::RESET) << std::endl;

        return ExecutionResult::failure(strerror(errno));
    }

    // 서버에 연결
    if (connect(client_socket_, (struct sockaddr *)&server_address_, sizeof(server_address_)) < 0)
    {
        std::cerr << color::setColor(color::ForeGround::BRIGHT_RED) + "Failed to connect to server. Please check server status." + color::setColor(color::ForeGround::RESET) << std::endl;

        return ExecutionResult::failure(strerror(errno));
    }

    return ExecutionResult::success("Server connected successfully");
}

// 서버 주소 초기화
void Client::initializeServerAddress(struct sockaddr_in &serverAddress, const std::string &serverIP, int portNum)
{
    memset(&serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portNum);
}

void Client::run()
{
    // 채팅 시작
    Message messageHandler(client_socket_);
    std::thread receive_message(&Message::receiveMessage, &messageHandler);
    std::thread send_message(&Message::sendMessage, &messageHandler);

    receive_message.join();
    send_message.join();

    // // 서버의 포트 번호 입력 받기
    // int portNum;
    // std::cerr << "서버의 포트 번호를 입력하세요: ";
    // std::cin >> portNum;
}
