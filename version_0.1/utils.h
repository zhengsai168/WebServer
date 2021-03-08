#ifndef UTILS_H
#define UTILS_H
#include <string>
void set_nonblocking( int fd );
int socket_listen( const char* ip, int port );
std::string respond_html();
#endif