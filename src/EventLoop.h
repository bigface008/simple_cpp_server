//
// Created by wangzhehao on 9/25/24.
//

#ifndef MYCPPSERVER_EVENTLOOP_H
#define MYCPPSERVER_EVENTLOOP_H


class Epoll;
class Channel;
class EventLoop {
private:
    Epoll *ep;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *);
};


#endif //MYCPPSERVER_EVENTLOOP_H
