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

#ifdef PLAT_WII
	#include <errno.h>
	#include <network.h>
	#include <platWii.h>
	#include <gccore.h>
	#define bind net_bind
	#define sendto net_sendto
	#define recvfrom net_recvfrom
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
		#ifdef PLAT_WII
		int size = addr.sin_len;
		#else
		int size = sizeof(addr);
		#endif
		int result = sendto(netInfo.socket, buf, 128, 0, (struct sockaddr *) &addr, size);
		if (result < 0) {
			#ifdef PLAT_WII
			errno = result;
			#endif
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
	int ret = bind(netInfo.socket, (struct sockaddr *) &addr, sizeof(addr));
	if (ret < 0) {
		#ifdef PLAT_WII
		errno = ret;
		#endif
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
			#ifdef PLAT_WII
			errno = ret;
			#endif
			perror("setsockopt mreq");
			printf("\r\nreturn value: %d\r\n", ret);
			
			PLAT_Exit(true);
		}
	#endif
	while (1) {
		#ifdef PLAT_WII
		WII_HandleRemotes();
		#endif
		int res = recvfrom(netInfo.socket, buf, 128, 0, (struct sockaddr *) &addr, &addrlen);
		if (res < 0) {
			#ifdef PLAT_WII
			errno = res;
			#endif
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
