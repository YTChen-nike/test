#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFSIZE 1024
#define PORT 8888

int main(int argc, char* argv[])
{
	int client_fd = 0;
	int len = 0;
	struct sockaddr_in server_addr;
	char buf[BUFFSIZE] = {0};

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0){
		perror("socket error\n");
		goto EXIT;
	}

	if(connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("connet error\n");
		goto EXIT;
	}

	printf("connet success\n");

	len = recv(client_fd, buf, BUFFSIZE, 0);
	buf[len] = '\0';
	printf("client_buf = %s\n", buf);

	while(1){
		printf("Enter string to send:");
		scanf("%s", buf);
		if(!strcmp(buf, "quit")){
			break;
		}

		len = send(client_fd, buf, strlen(buf), 0);
		len = recv(client_fd, buf, BUFFSIZE, 0);
		buf[len] = '\0';
		printf("received: %s\n",buf);
	}

	close(client_fd);
	return 0;
EXIT:
	return -1;	
}
