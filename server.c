#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, n;
	unsigned int clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2)
	{
		fprintf(stderr, "ERROR, no port provided\n");
		exit(2);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR in binding");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	while(strcmp(buffer, "exit") != 0)
	{
		memset(buffer, 0, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0)
			error("ERROR reading socket");
		printf("Here is the message: %s\n", buffer);
		n = write(newsockfd, "I got your message", 18);
		if (n < 0)
			error("ERROR writing to socket");
	}
	return 0;
}


