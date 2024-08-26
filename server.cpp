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
#include "util.h"

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

    // 4. Establish connection
    sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (sockaddr *) &clnt_addr, &clnt_addr_len);
    errif(ret == -1, "server: socket accept error");

    char ip[64] = {0};
    printf("server: new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr),
           ntohs(clnt_addr.sin_port));

    // 5. Communicate
    char buf[512];
    while (1) {
        bzero(buf, sizeof(buf));
        int len = read(clnt_sockfd, buf, sizeof(buf));
        if (len > 0) {
            printf("server: client %d says: %s\n", clnt_sockfd, buf);
            write(clnt_sockfd, buf, len);
        } else if (len == 0) {
            printf("server: client %d is disconnected...\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        } else {
            close(clnt_sockfd);
            errif(true, "server: socket read error");
            break;
        }
    }

    close(sockfd);
    return 0;
}
