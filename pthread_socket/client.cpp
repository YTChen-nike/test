#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
string m_strIP = "127.0.0.1";
int m_nPort = 8888;

int main()
{
	int fd = 0;
	int retcode = 0;
	int recv_count = 0;
	char *recvbuf = NULL;
	struct sockaddr_in addr;
	unsigned long flags;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPort);
	addr.sin_addr.s_addr = inet_addr(m_strIP.c_str());

	retcode = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
	std::cout << recv_count << std::endl;
	std::cout << recvbuf << std::endl;
	if(0 == retcode){
		if(fcntl(fd, F_SETFL, flags) < 0){
			std::cout << "Connect error! " << std::endl;
			return -1;
		}
		else {
			std::cout << "Connect error! " << std::endl;
			return fd;
		}
	}
}


