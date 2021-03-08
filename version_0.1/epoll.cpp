#include "all.h"
#include "epoll.h"
#include "utils.h"

using namespace std;

struct epoll_event* events;
const int BUFFER_SIZE = 1024;

int epoll_init()
{
    int epoll_fd = epoll_create( MAX_LISTEN + 1 );
    assert(epoll_fd!=-1);
    events = new epoll_event[MAX_EVENTS];
    return epoll_fd;
}

void epoll_add_fd( int epollfd, int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    set_nonblocking( fd );
}

// 返回活跃事件数
int my_epoll_wait(int epoll_fd, struct epoll_event* events, int max_events, int timeout)
{
    int ret_count = epoll_wait(epoll_fd, events, max_events, timeout);
    if (ret_count < 0)
    {
        perror("epoll wait error");
    }
    return ret_count;
}

void et( epoll_event* events, int number, int epollfd, int listenfd )
{
    char buf[ BUFFER_SIZE ];
    for ( int i = 0; i < number; i++ )
    {
        int sockfd = events[i].data.fd;
        if ( sockfd == listenfd )
        {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof( client_address );
            int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
            epoll_add_fd( epollfd, connfd);
        }
        else if ( events[i].events & EPOLLIN )
        {
            printf( "event trigger once\n" );
            while( 1 )
            {
                memset( buf, '\0', BUFFER_SIZE );
                int ret = recv( sockfd, buf, BUFFER_SIZE-1, 0 );
                if( ret < 0 ) // 出错，或者是 需要再次尝试
                {
                    if( ( errno == EAGAIN ) || ( errno == EWOULDBLOCK ) )
                    {
                        printf( "read later\n" );
                        break;
                    }
                    close( sockfd );// error
                    break;
                }
                else if( ret == 0 )  // 读完了
                {
                	cout<<"ret = 0"<<endl;
                    close( sockfd );
                    break;
                }
                else
                {
                    printf( "get %d bytes of content: %s\n", ret, buf );
                    std::string respond_data = respond_html();
                    cout<<"start write index html"<<endl;
                    send(sockfd, respond_data.c_str(), respond_data.size(), 0);
                	cout<<"end write index html"<<endl;
                	close( sockfd );
                	break;
                }
            }
        }
        else
        {
            printf( "something else happened \n" );
        }
    }
}