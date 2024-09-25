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

const int MAX_EVENTS = 1024;
const int READ_BUFFER = 1024;

void set_nonblocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main() {
    // 1. Create socket for listening
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "server: socket create error");

    // 2. Bind ip address & port
    sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    int ret = bind(sockfd, (sockaddr *) &serv_addr, sizeof(serv_addr));
    errif(ret == -1, "server: socket bind error");

    // 3. Set listening
    ret = listen(sockfd, SOMAXCONN);
    errif(ret == -1, "server: socket listen error");

    int epfd = epoll_create1(0);
    errif(epfd == -1, "server: epoll create error");

    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));

    bzero(&ev, sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN || EPOLLET;
    set_nonblocking(sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    while (true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "server: epoll wait error");
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sockfd) {
                // 4. Establish connection
                sockaddr_in clnt_addr;
                socklen_t clnt_addr_len = sizeof(clnt_addr);
                bzero(&clnt_addr, sizeof(clnt_addr));

                int clnt_sockfd = accept(sockfd, (sockaddr *) &clnt_addr, &clnt_addr_len);
                errif(ret == -1, "server: socket accept error");
                printf("server: new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr),
                       ntohs(clnt_addr.sin_port));

                bzero(&ev, sizeof(ev));
                ev.data.fd = clnt_sockfd;
                ev.events = EPOLLIN | EPOLLET;
                set_nonblocking(clnt_sockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);
            } else if (events[i].events & EPOLLIN) {
                char buf[READ_BUFFER];
                while (true) {
                    bzero(&buf, sizeof(buf));
                    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                    if (bytes_read > 0) {
                        printf("server: client %d says: %s\n", events[i].data.fd, buf);
                        write(events[i].data.fd, buf, sizeof(buf));
                    } else if (bytes_read == -1 && errno == EINTR) {
                        printf("server: continue reading");
                        continue;
                    } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
                        printf("server: finish reading once, errno: %d\n", errno);
                        break;
                    } else if (bytes_read == 0) {
                        printf("server: EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);
                        break;
                    }
                }
            } else {
                // TODO
                printf("server: TODO");
            }
        }
    }
    close(sockfd);
    return 0;
}
