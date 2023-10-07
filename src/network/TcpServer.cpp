#include "TcpServer.h"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h>

void logError(const std::string &message)
{
    std::cerr << message << ": " << strerror(errno) << std::endl;
}

TcpServer::TcpServer(const std::string &ip, int port)
: ip(ip),
  port(port),
  sockaddrLen(sizeof(sockAddr))
{
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockAddr.sin_port = htons(this->port);

    if ((serverSockFd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        logError("Couldn't get server socket");
        exit(1);
    }

    if (bind(serverSockFd, reinterpret_cast<struct sockaddr*>(&sockAddr), sizeof(sockAddr)) < 0) 
    {
        logError("issue binding to server socket");
        exit(1);
    }

    if (listen(serverSockFd, 1) < 0) 
    {
        logError("issue listening");
        exit(1);
    }
}

TcpServer::~TcpServer()
{
    if(::close(serverSockFd) < 0)
    {
        logError("Couldn't close server socket");
    }
}

bool TcpServer::connect()
{
    if ((clientSockFd = accept(serverSockFd, 
                             reinterpret_cast<struct sockaddr*>(&sockAddr), 
                             reinterpret_cast<socklen_t*>(&sockaddrLen))) < 0)
    {
        logError("issue accepting client - ");
        return false;
    }
    return true;
}

int TcpServer::send(const std::string &message)
{
    int bytesSent = -1;
    if ((bytesSent = ::write(clientSockFd, message.c_str(), message.length())) < 0)
    {
        logError("Couldn't send message");
    }
    return bytesSent;
}

std::array<char, RECV_BUFFER_SIZE> TcpServer::receive()
{
    std::array<char, RECV_BUFFER_SIZE> recvBuffer{0};

    int receivedBytes = ::read(clientSockFd, recvBuffer.data(), RECV_BUFFER_SIZE);
    if (receivedBytes < 0)
    {
        logError("Could not receive data from client");
    }
    return recvBuffer;
}
