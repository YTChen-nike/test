#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <aepa/inet.h>
#include <pthread.h>
#include <stdlib.h>

#define PORT 8888
#define BACKLOG 5
#define MAXBUFFSIZE 1024

void process_cli(int connectfd, struct sockaddr_in client);
void *start_routine(void *arg);

typedef struct _cli_info{
	int connect_fd;
	struct sockaddr_in client_addr;
}cli_info;

int main()
{
	int listenfd, connectfd;
	pthread_t pid;
	cli_info *info;
	struct sockaddr_in ser_addr;
	struct sockaddr_in cli_addr;
	int sin_size;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1){
		peeror("craete socket failed!");
		goto EXIT;
	}

	int opt = SO_REUSEADDR;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	memset(ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(PORT);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listenfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) == -1){
		perror("bind failed");
		goto EXIT;
	}

	if(listen(listenfd, BACKLOG) == -1){
		perror("listen failed");
		goto EXIT;
	}

	sin_size = sizeof(struct sockaddr_in);

	while(1){
		connectfd = accept(listenfd, (struct sockaddr*)&cli_addr, sin_size);
		if(connectfd == -1){
			perror("accept failed");
			goto EXIT;
		}

		info = new cli_info;
		info->connect_fd = connectfd;
		strncpy(info->client_addr, cli_addr, sizeof(cli_addr));

		if(pthread_create(&thread, NULL, start_routine, (void *)info)){
			perror("pthread_create error");
			goto EXIT;
		}
	}
EXIT:
	if(listenfd)
		close(listenfd);
	return 0;
}

void process_cli(int connectfd, struct sockaddr_in client)
{
	int num;
	char recvbuf[MAXBUFFSIZE],sendbuf[MAXBUFFSIZE], cli_name[MAXBUFFSIZE];

	printf("You got a connection from %s.", inet_ntoa(client.sin_addr));
	num = recv(connectfd, cli_name, MAXBUFFSIZE, 0);
	if(num == 0){
		close(connectfd);
		printf("Client disconnected.\n");
		return ;
	}
	cli_name[num - 1] = '\0';
	printf("client's name is %s.\n", cli_name);

	while(num = recv(connectfd, recvbuf, MAXBUFFSIZE, 0)){
		recvbuf[num] = '\0';
		printf("recvived client(%s) messge: %s", cli_name, recvbuf);
		for(int i = 0; i < num -1; i++){
			sendbuf[i] = recvbuf[num - i - 2];
		}
		sendbuf[num - 1] = '\0';
		send(connectfd, sendbuf, strlen(sendbuf), 0);
	}
	close(connectfd);
}
void *start_routine(void *arg)
{
	cli_info* info = (cli_info*)arg;

	process_cli(info->connect_fd, info->client_addr);
	delete info;
	pthread_exit(NULL);
}

