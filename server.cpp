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
#include "util.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "Socket.h"
#include "Channel.h"

const int MAX_EVENTS = 1024;
const int READ_BUFFER = 1024;

//void set_nonblocking(int fd) {
//    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
//}
void handleReadEvent(int);

int main() {
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();

    Epoll *ep = new Epoll();
    serv_sock->set_nonblocking();

    Channel *servChannel = new Channel(ep, serv_sock->getFd());
    servChannel->enableReading();
//    ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);

    while (true) {
        std::vector<Channel *> activeChannels = ep->poll();
        int nfds = activeChannels.size();
        for (int i = 0; i < nfds; ++i) {
            int chfd = activeChannels[i]->getFd();
            if (chfd == serv_sock->getFd()) {
                InetAddress *clnt_addr = new InetAddress(); // TODO: delete
                Socket *clnt_sock = new Socket(serv_sock->accepct(clnt_addr)); // TODO: delete
                printf("server: new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr),
                       ntohs(clnt_addr->addr.sin_port));
                clnt_sock->set_nonblocking();
                Channel *clntChannel = new Channel(ep, clnt_sock->getFd());
                clntChannel->enableReading();
            } else if (activeChannels[i]->getRevents() & EPOLLIN) {
                handleReadEvent(activeChannels[i]->getFd());
            } else {
                // TODO
                printf("server: TODO");
            }
        }
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}

void handleReadEvent(int sockfd) {
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
