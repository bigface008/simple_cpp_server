//
// Created by wangzhehao on 9/25/24.
//

#ifndef MYCPPSERVER_ACCEPTOR_H
#define MYCPPSERVER_ACCEPTOR_H

#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor {
private:
    EventLoop *loop;
    Socket *sock;
    InetAddress *addr;
    Channel *acceptChannel;
public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    void acceptConnection();
    std::function<void(Socket*)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
};


#endif //MYCPPSERVER_ACCEPTOR_H
