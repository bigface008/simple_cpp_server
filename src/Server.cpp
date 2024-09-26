//
// Created by wangzhehao on 9/25/24.
//

#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <errno.h>

const int READ_BUFFER = 1024;

Server::Server(EventLoop *_loop) : loop(_loop) {
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->set_nonblocking();

    Channel *servChannel = new Channel(loop, serv_sock->getFd());
    std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
    servChannel->setCallback(cb);
    servChannel->enableReading();
}

Server::~Server() {}

void Server::handleReadEvent(int sockfd) {
    char buf[READ_BUFFER];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            printf("server: client %d says: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if (bytes_read == -1 && errno == EINTR) {
            printf("server: continue reading");
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("server: finish reading once, errno: %d\n", errno);
            break;
        } else if (bytes_read == 0) {
            printf("server: EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
    }
}

void Server::newConnection(Socket *serv_sock) {
    InetAddress *clnt_addr = new InetAddress(); // TODO: memory leak
    Socket *clnt_sock = new Socket(serv_sock->accepct(clnt_addr)); // TODO: memory leak
    printf("server: new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr),
           ntohs(clnt_addr->addr.sin_port));
    clnt_sock->set_nonblocking();
    Channel *clntChannel = new Channel(loop, clnt_sock->getFd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getFd());
    clntChannel->setCallback(cb);
    clntChannel->enableReading();
}