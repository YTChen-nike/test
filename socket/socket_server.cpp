/*socket_server*/
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFSIZE 1024
#define PORT 8888

int main(int argc, char *argv[])
{
	int server_fd, client_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int len = 0;
	socklen_t sin_size = 0;
	char buf[BUFFSIZE];

	bzero(&server_addr, sizeof(server_addr));
	bzero(&client_addr, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0){
		perror("socket error\n");
		goto EXIT;
	}

	if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("bind error\n");
		goto EXIT;
	}

	if(listen(server_fd, 5) < 0){
		perror("lisent error\n");
		goto EXIT;
	}

	sin_size = sizeof(client_addr);
	client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &sin_size);
	if(client_fd < 0){
		perror("accept error\n");
		goto EXIT;
	}

	printf("accept client %s\n", inet_ntoa(client_addr.sin_addr));
	len = send(client_fd, "Hello client\n", 30, 0);

	while((len = recv(client_fd, buf, BUFFSIZE, 0)) > 0){
		//ceshi
		printf("567\n");
		buf[len] = '\0';
		printf("buf = %s\n", buf);
		if(send(client_fd, buf, len, 0) < 0){
			perror("send error\n");
			goto EXIT;
		}
	}
	//ceshi
	printf("123\n");
	close(client_fd);
	close(server_fd);

	return 0;
EXIT:
	return -1;
}
