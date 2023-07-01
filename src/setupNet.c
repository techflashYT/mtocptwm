#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct sockaddr_in addr;
int addrlen, sock;
struct ip_mreq mreq;


int portNum = 6000;
char *multicastIP = "239.0.0.1";

char localIP[17];

static void getLocalIP();
static void setupMulticastSocket();

void setupNet() {
	setupMulticastSocket();
	getLocalIP();
}

static void setupMulticastSocket() {
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

static void getLocalIP() {
	int fd;
	struct ifreq ifr;
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	// only IPv4
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);

	// save the local IP address
	strcpy(localIP, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
}
