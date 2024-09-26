//
// Created by wangzhehao on 9/25/24.
//

#ifndef MYCPPSERVER_SERVER_H
#define MYCPPSERVER_SERVER_H

#include <map>

class Acceptor;
class Connection;
class EventLoop;
class Socket;
class Server {
private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int, Connection *> connections;
public:
    Server(EventLoop *);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
};


#endif //MYCPPSERVER_SERVER_H
