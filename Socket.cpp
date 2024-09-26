//
// Created by wangzhehao on 9/25/24.
//

#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

Socket::Socket() : fd(-1) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "server: socket create error");
}

Socket::Socket(int _fd) : fd(_fd) {
    errif(fd == -1, "server: socket create error");
}

Socket::~Socket() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress *addr) {
    errif(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "server: socket bind error");
}

void Socket::listen() {
    errif(::listen(fd, SOMAXCONN) == -1, "server: socket listen error");
}

void Socket::set_nonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accepct(InetAddress *addr) {
    int clnt_sockfd = ::accept(fd, (sockaddr *)&addr->addr, &addr->addr_len);
    errif(clnt_sockfd == -1, "server: socket accpect error");
    return clnt_sockfd;
}

int Socket::getFd() {
    return fd;
}
