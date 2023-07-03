#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#ifdef PLAT_LINUX
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#ifdef PLAT_WII
#include <network.h>
#include <platWii.h>
#include <gccore.h>
#define bind net_bind
#define sendto net_sendto
#define recvfrom net_recvfrom
#endif

#ifdef PLAT_WIN



extern uint32_t addrlen;
extern struct sockaddr_in addr;

#include <mtocptwm.h>

static void transmit(const char *message);
static void recieve();
void broadcastLoop(const char *message, const bool mode) {
	if (mode == false) {
		transmit(message);
	}
	else {
		recieve();
	}
}


static void transmit(const char *message) {
	addr.sin_addr.s_addr = inet_addr(netInfo.multicastIP);
	int i = 0;
	while (1) {
		char buf[128];

		uint32_t *tmp = (uint32_t *)&buf[0];
		memcpy(buf + 4, message, 124);

		*tmp = i;

		printf("sending #%d\n", i);
		i++;
		#ifdef PLAT_WII
		int size = addr.sin_len;
		#endif
		#ifdef PLAT_LINUX
		int size = sizeof(addr);
		#endif
		int result = sendto(netInfo.socket, buf, 128, 0, (struct sockaddr *) &addr, size);
		if (result < 0) {
			#ifdef PLAT_WII
			errno = result;
			#endif

			perror("sendto err");
			printf("return val: %d", result);
			exit(1);
		}
		#ifdef PLAT_WII
		for (uint_fast8_t i = 0; i != 5; i++) {
			for (uint_fast8_t i2 = 0; i2 != 59; i2++) {
				scanWiimotes();
				VIDEO_WaitVSync();
			}
		}
		#endif
		#ifdef PLAT_LINUX
		sleep(5);
		#endif
	}
}


static void recieve() {
	char buf[128];
	if (bind(netInfo.socket, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("bind");
		exit(1);
	}
#ifdef PLAT_LINUX
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(netInfo.multicastIP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(netInfo.socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
		perror("setsockopt mreq");
		exit(1);
	}
#endif
	while (1) {
		#ifdef PLAT_WII
		scanWiimotes();
		#endif
		int res = recvfrom(netInfo.socket, buf, 128, 0, (struct sockaddr *) &addr, &addrlen);
		if (res < 0) {
			#ifdef PLAT_WII
			errno = res;
			#endif
			
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