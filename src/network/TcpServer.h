#include <array>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

constexpr int RECV_BUFFER_SIZE = 30720;

class TcpServer
{
public:
    /* constructor / destructors */
    TcpServer(const std::string &ip, int port);
    ~TcpServer();

    /* member functions */
    bool connect();
    int send(const std::string &message);
    std::array<char, RECV_BUFFER_SIZE> receive();

private:
    std::string ip;
    int port;
    int serverSockFd;
    int clientSockFd;
    struct sockaddr_in sockAddr;
    unsigned short sockaddrLen;
};