#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

extern int portNum, sock, addrlen;
extern char *multicastIP;
extern struct sockaddr_in addr;
extern struct ip_mreq mreq;

static void transmit(const char *message);
static void recieve();
void broadcastLoop(const char *name, const char *message, const bool mode) {
	if (mode == false) {
		transmit(message);
	}
	else {
		recieve();
	}
}


static void transmit(const char *message) {
	addr.sin_addr.s_addr = inet_addr(multicastIP);
	int i = 0;
	while (1) {
		char buf[128];

		uint32_t *tmp = (uint32_t *)&buf[0];
		memcpy(buf + 4, message, 124);

		*tmp = i;

		printf("sending #%d\n", i);
		i++;
		int result = sendto(sock, buf, 128, 0, (struct sockaddr *) &addr, addrlen);
		if (result < 0) {
			perror("sendto");
			exit(1);
		}
		sleep(5);
	}
}

static void recieve() {
	char buf[128];
	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("bind");
		exit(1);
	}
	mreq.imr_multiaddr.s_addr = inet_addr(multicastIP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		&mreq, sizeof(mreq)) < 0) {
		perror("setsockopt mreq");
		exit(1);
	}
	while (1) {
		int res = recvfrom(sock, buf, 128, 0, (struct sockaddr *) &addr, &addrlen);
		if (res < 0) {
			perror("recvfrom");
			exit(1);
		}
		else if (res == 0) {
			break;
		}
		int pktNum = *((uint32_t *)&buf[0]);
		printf("%s: message #%d = \"%s\"\n", inet_ntoa(addr.sin_addr), pktNum, &buf[4]);
	}
}