#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <thread>

std::vector<int> clientSockets;

void handleClient(int clientSocket)
{
    char buffer[1024];
    int n;

    while (true)
    {
        // 클라이언트로부터 메시지 수신
        memset(buffer, '\0', sizeof(buffer));
        n = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (n <= 0)
        {
            std::cerr << "클라이언트와의 연결이 종료되었습니다." << std::endl;
            break;
        }

        std::cout << "클라이언트 #" << clientSocket << ": " << buffer << std::endl;

        // 종료 메시지 수신 시 클라이언트 연결 종료
        if (strcmp(buffer, "종료") == 0)
            break;

        // 다른 클라이언트들에게 메시지 전달
        for (int client : clientSockets)
        {
            if (client != clientSocket)
            {
                send(client, buffer, n, 0);
            }
        }
    }

    // 클라이언트 소켓 종료 및 제거
    close(clientSocket);
    auto it = std::find(clientSockets.begin(), clientSockets.end(), clientSocket);
    if (it != clientSockets.end())
    {
        clientSockets.erase(it);
    }
}

int main()
{
    int serverSocket, newSocket, portNum, clientLen;
    struct sockaddr_in serverAddr
    {
    }, clientAddr{};
    std::string welcomeMessage = "채팅에 오신 것을 환영합니다!";

    // 소켓 생성
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        std::cerr << "소켓 생성에 실패했습니다." << std::endl;
        return 1;
    }

    // 서버 주소 초기화
    memset(&serverAddr, '\0', sizeof(serverAddr));
    portNum = 12345;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // 소켓 바인딩
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "바인딩에 실패했습니다." << std::endl;
        return 1;
    }

    // 연결 대기
    if (listen(serverSocket, 10) < 0)
    {
        std::cerr << "연결 대기에 실패했습니다." << std::endl;
        return 1;
    }

    std::cout << "채팅 서버가 실행 중입니다." << std::endl;

    clientLen = sizeof(clientAddr);

    while (true)
    {
        // 클라이언트로부터 연결 요청 대기
        newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&clientLen);
        if (newSocket < 0)
        {
            std::cerr << "연결 수락에 실패했습니다." << std::endl;
            return 1;
        }

        // 클라이언트에게 환영 메시지 전송
        send(newSocket, welcomeMessage.c_str(), welcomeMessage.length(), 0);

        // 연결된 클라이언트 소켓을 저장
        clientSockets.push_back(newSocket);

        // 클라이언트를 처리하기 위한 스레드 생성
        std::thread clientThread(handleClient, newSocket);
        clientThread.detach();
    }

    // 소켓 종료
    close(serverSocket);

    return 0;
}
