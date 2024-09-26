//
// Created by wangzhehao on 9/25/24.
//

#ifndef MYCPPSERVER_SERVER_H
#define MYCPPSERVER_SERVER_H


class EventLoop;
class Socket;
class Server {
private:
    EventLoop *loop;
public:
    Server(EventLoop *);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
};


#endif //MYCPPSERVER_SERVER_H
