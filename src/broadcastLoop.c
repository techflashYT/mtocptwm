#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#if PLAT_LINUX || __SWITCH__
	#include <sys/socket.h>
	#include <arpa/inet.h>
#endif

#ifdef __SWITCH__
#include <nswitch.h>
#endif

#ifdef PLAT_WIN
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#endif
	#include <winsock2.h>
	#define sleep(sec) Sleep(sec * 1000)
	extern int addrlen;
#else // if anything but windows
	#include <unistd.h>
	#include <string.h>
	extern uint32_t addrlen;
#endif

extern struct sockaddr_in addr;

#include <mtocptwm.h>

extern void NET_Init();
static int NET_Tx(const char *message, const uint_fast8_t delaySec);
static int NET_Rx(char *msg, const uint_fast8_t attempt, const uint_fast8_t timeoutSec);

/* TODO: complete redesign

	- only loop for specified amount of times
	- work when called with little/no boilerplate
	- timeout of specified seconds for NET_Rx
	- general cleanup
*/
int NET_Go(char *message, const bool mode, const uint_fast8_t attempt, const uint_fast8_t delaySec) {
	// FIXME: reinit the socket.  This is really dumb and there has to be a better way to do this.
	close(netInfo.socket);
	NET_Init();
	if (mode == false) {
		return NET_Tx(message, delaySec);
	}
	else {
		return NET_Rx(message, attempt + 1, delaySec);
	}
}
static uint_fast64_t NET_NumPkt = 0;

static int NET_Tx(const char *message, const uint_fast8_t delaySec) {
	addr.sin_addr.s_addr = inet_addr(netInfo.multicastIP);
	char buf[128];

	uint32_t *tmp = (uint32_t *)&buf[0];
	memcpy(buf + 4, message, 124);

	*tmp = NET_NumPkt;
	NET_NumPkt++;

	printf("sending packet #%ld\n", NET_NumPkt);
	int result = sendto(netInfo.socket, buf, 128, 0, (struct sockaddr *) &addr, sizeof(addr));
	if (result < 0) {
		perror("sendto err");
		printf("\r\nreturn value: %d\r\n", result);

		PLAT_Exit(true);
	}
	#if __SWITCH__
	consoleUpdate(NULL);
	#endif
	return 0;
}

static int NET_Rx(char *msg, const uint_fast8_t attempt, const uint_fast8_t timeoutSec) {
	printf("waiting for data, attempt #%d, and %ds timeout\r\n", attempt, timeoutSec);
	#if __SWITCH__
	puts("This option is broken on Switch!!!\r\nIt simply will not work correctly.");
	consoleUpdate(NULL);
	sleep(2);
	PLAT_Exit(true);
	#endif

	char buf[128];
	int ret = bind(netInfo.socket, (struct sockaddr *) &addr, addrlen);
	if (ret < 0) {
		perror("bind");
		printf("\r\nreturn value: %d\r\n", ret);

		PLAT_Exit(true);
	}
	#if PLAT_LINUX || __SWITCH__
		struct ip_mreq mreq;
		mreq.imr_multiaddr.s_addr = inet_addr(netInfo.multicastIP);
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		ret = setsockopt(netInfo.socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
		if (ret < 0) {
			perror("setsockopt mreq");
			printf("\r\nreturn value: %d\r\n", ret);
			
			PLAT_Exit(true);
		}
	#endif
	fd_set readFds;
	struct timeval timeout;


	FD_ZERO(&readFds);
	FD_SET(netInfo.socket, &readFds);

	timeout.tv_sec = timeoutSec;
	timeout.tv_usec = 0;
	// Wait for activity on the socket with the specified timeout
	int activity = select(netInfo.socket + 1, &readFds, NULL, NULL, &timeout);
	if (activity == -1) {
		perror("select failed");
		PLAT_Exit(true);
	}
	else if (activity == 0) {
		// Timeout occurred
		puts("timeout");
		ret = 1;
		goto end;
	}
	int res = recvfrom(netInfo.socket, buf, 128, 0, (struct sockaddr *) &addr, &addrlen);
	if (res < 0) {
		perror("recvfrom");
		printf("\r\nreturn value: %d\r\n", ret);
		PLAT_Exit(true);
	}
	// int pktNum = *((uint32_t *)&buf[0]);
	strcpy(msg, &buf[4]);

	end:
	PLAT_RxLoop(timeoutSec);
	return ret;
}
