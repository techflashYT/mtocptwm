#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct sockaddr_in addr;
int addrlen, sock;
struct ip_mreq mreq;


int portNum = 6000;
char *ipAddr = "239.0.0.1";


void setupNet(int argc, char *argv[]) {
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}
	bzero((char *)&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(portNum);
	addrlen = sizeof(addr);
}