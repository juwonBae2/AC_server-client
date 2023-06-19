#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int clientSocket, portNum, n;
    char buffer[1024];
    struct sockaddr_in serverAddr
    {
    };

    std::string serverIP = "127.0.0.1";
    // TODO: IP를 받게 할 지 안 할지는 미정
    // std::string serverIP;
    // std::cout << "서버의 IP 주소를 입력하세요: ";
    // std::cin >> serverIP;

    // TODO: 이거 왜 작동 안해
    // 서버의 포트 번호 입력 받기
    // std::cout << "서버의 포트 번호를 입력하세요: ";
    // std::cin >> portNum;

    // 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "소켓 생성에 실패했습니다." << std::endl;
        return 1;
    }

    // 서버 주소 초기화
    memset(&serverAddr, '\0', sizeof(serverAddr));
    portNum = 7777;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    if (inet_pton(AF_INET, serverIP.c_str(), &(serverAddr.sin_addr)) <= 0)
    {
        std::cerr << "IP 주소 설정에 실패했습니다." << std::endl;
        return 1;
    }

    // 서버에 연결
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "서버에 연결할 수 없습니다." << std::endl;
        return 1;
    }

    while (true)
    {
        // 서버로부터 메시지 수신
        memset(buffer, '\0', sizeof(buffer));
        n = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (n <= 0)
        {
            std::cerr << "메시지 수신에 실패했습니다." << std::endl;
            break;
        }

        // TODO: 서버를 통해서가 아닌 client 자체로 통신할지 안할지 여부
        // std::cout << "서버: " << buffer << std::endl;

        // 종료 메시지 수신 시 채팅 종료
        if (strcmp(buffer, "종료") == 0)
            break;

        // 사용자로부터 메시지 입력
        std::cout << "사용자: ";
        std::string message;
        std::getline(std::cin, message);

        // 메시지 서버로 전송
        send(clientSocket, message.c_str(), message.length(), 0);
    }

    // 소켓 종료
    close(clientSocket);

    return 0;
}
