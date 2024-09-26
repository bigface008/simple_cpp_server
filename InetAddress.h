//
// Created by wangzhehao on 9/25/24.
//

#ifndef MYCPPSERVER_INETADDRESS_H
#define MYCPPSERVER_INETADDRESS_H

#include <arpa/inet.h>

class InetAddress {
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();
};


#endif //MYCPPSERVER_INETADDRESS_H
