//
// Created by wangzhehao on 9/25/24.
//

#ifndef MYCPPSERVER_EPOLL_H
#define MYCPPSERVER_EPOLL_H

#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll {
private:
    int epfd;
    struct epoll_event *events;

public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op);
    void updateChannel(Channel *);
    std::vector<Channel *> poll(int timeout = -1);
};


#endif //MYCPPSERVER_EPOLL_H
