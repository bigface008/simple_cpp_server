//
// Created by wangzhehao on 1/27/24.
//

#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "src/EventLoop.h"
#include "src/Server.h"

const int MAX_EVENTS = 1024;
const int READ_BUFFER = 1024;

//void set_nonblocking(int fd) {
//    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
//}
void handleReadEvent(int);

int main() {
    auto *loop = new EventLoop();
    auto *server = new Server(loop);
    loop->loop();
}
