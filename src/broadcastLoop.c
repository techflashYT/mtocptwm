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

static void NET_Tx(const char *message);
static void NET_Rx();

/* TODO: complete redesign

	- only loop for specified amount of times
	- work when called with little/no boilerplate
	- timeout of specified seconds for NET_Rx
	- general cleanup
*/
void NET_Loop(const char *message, const bool mode) {
	if (mode == false) {
		NET_Tx(message);
	}
	else {
		NET_Rx();
	}
}


static void NET_Tx(const char *message) {
	addr.sin_addr.s_addr = inet_addr(netInfo.multicastIP);
	int i = 0;
	while (1) {
		char buf[128];

		uint32_t *tmp = (uint32_t *)&buf[0];
		memcpy(buf + 4, message, 124);

		*tmp = i;

		printf("sending #%d\n", i);
		i++;
		int result = sendto(netInfo.socket, buf, 128, 0, (struct sockaddr *) &addr, sizeof(addr));
		if (result < 0) {
			perror("sendto err");
			printf("\r\nreturn value: %d\r\n", result);

			PLAT_Exit(true);
		}
		PLAT_TxLoop();
	}
}


static void NET_Rx() {
	#if __SWITCH__
	puts("This option is broken on Switch!!!\r\nIt simply will not work correctly.");
	consoleUpdate(NULL);
	sleep(2);
	PLAT_Exit(true);
	#endif
	char buf[128];
	int ret = bind(netInfo.socket, (struct sockaddr *) &addr, addrlen);
	if (ret < 0) {
		puts("ccc");
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
	while (1) {
		int res = recvfrom(netInfo.socket, buf, 128, 0, (struct sockaddr *) &addr, &addrlen);
		if (res < 0) {
			puts("aaa");
			perror("recvfrom");
			printf("\r\nreturn value: %d\r\n", ret);

			PLAT_Exit(true);
		}
		else if (res == 0) {
			break;
		}
		int pktNum = *((uint32_t *)&buf[0]);
		printf("%s: message #%d = \"%s\"\n", inet_ntoa(addr.sin_addr), pktNum, &buf[4]);
		#if __SWITCH__
		consoleUpdate(NULL);
		#endif
	}
}
