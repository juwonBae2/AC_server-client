#include <sys/socket.h> // 소켓 관련 함수와 구조체를 정의하는 헤더
#include <netinet/in.h> // 인터넷 주소 구조체를 정의하는 헤더

class Server
{
private:
    struct sockaddr_in server_;
};