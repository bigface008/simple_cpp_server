//
// Created by wangzhehao on 9/25/24.
//

#ifndef MYCPPSERVER_SOCKET_H
#define MYCPPSERVER_SOCKET_H


class InetAddress;
class Socket {
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    void set_nonblocking();

    int accepct(InetAddress *);

    int getFd();
};


#endif //MYCPPSERVER_SOCKET_H
