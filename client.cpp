//
// Created by wangzhehao on 1/27/24.
//

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "src/util.h"

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "client: socket create error");

    sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    int ret = connect(sockfd, (sockaddr * ) & serv_addr, sizeof(serv_addr));
    errif(ret == -1, "client: socket connect error");

    while (true) {
        char buf[512];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if (write_bytes == -1) {
            printf("client: socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));

        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            printf("client: message from server: %s\n", buf);
        } else if (read_bytes == 0) {
            printf("client: server socket disconnected!\n");
            break;
        } else if (read_bytes == -1) {
            close(sockfd);
            errif(true, "client: socket read error");
        }
    }
    close(sockfd);
    return 0;
}
