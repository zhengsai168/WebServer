#include "all.h"
#include "utils.h"

using namespace std;

void set_nonblocking( int fd )
{
	// set a fd to nonblocking 
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
}

int socket_listen( const char* ip, int port ){
	// return a socket listen fd
	int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    int listenfd = socket( AF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );

    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret != -1 );

    ret = listen( listenfd, 5 );
    assert( ret != -1 );
    return listenfd;
}

string respond_html(){
    string respond_1 = "HTTP/1.1 200 OK\r\n"
    "Content-Type: ttext/html;charset=utf-8\r\n"
    "Date: Sun, 07 Feb 2021 14:00:32 GMT\r\n"
    "Expires: Sun, 07 Feb 2021 14:00:32 GMT\r\n"
    "Content-Length: ";
    int html_fd = open("index.html", O_RDONLY);
    char html_string[1024];
    int html_len = read(html_fd, html_string, 1024);
    html_string[html_len] = '\0';
    string respond_2 = to_string(strlen(html_string));
    string respond_3 = "\r\nConnection: close\r\n\r\n";
    string respond_4 = html_string;
    string res = respond_1 + respond_2 + respond_3 + respond_4;
    return res;
}