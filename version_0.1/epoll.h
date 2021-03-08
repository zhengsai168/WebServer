#ifndef EPOLL_H
#define EPOLL_H
const int MAX_LISTEN = 3000;
const int MAX_EVENTS = 5000;
int epoll_init();
void epoll_add_fd( int epollfd, int fd);
int my_epoll_wait(int epoll_fd, struct epoll_event* events, int max_events, int timeout);
void et( epoll_event* events, int number, int epollfd, int listenfd );

#endif