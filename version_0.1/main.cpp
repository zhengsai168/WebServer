#include "all.h"
#include "epoll.h"
#include "utils.h"

using namespace std;

extern struct epoll_event* events;

int main(int argc, char* argv[]){
	if(argc < 3){
		cout<<"Need ip and port!"<<endl;
		exit(1);
	}
	const char* ip = argv[1];
	int port = atoi(argv[2]);
	
	int listen_fd = socket_listen(ip, port);
	int epoll_fd = epoll_init();
	epoll_add_fd(epoll_fd, listen_fd);
	while(1){
		int ret_num = my_epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		cout<<"ret num"<<ret_num<<endl;
		et(events, ret_num, epoll_fd, listen_fd);
	}
	
	return 0;
}