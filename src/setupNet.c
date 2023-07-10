#if PLAT_LINUX || __SWITCH__
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <net/if.h>
	#include <unistd.h>
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
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
struct sockaddr_in addr;
#ifdef PLAT_WIN
	int addrlen;
#else
	uint32_t addrlen;
#endif


#include <mtocptwm.h>

netInfo_t netInfo;

static void setupMulticastSocket();
static void getHostname();
void NET_Init() {
	setupMulticastSocket();
	getHostname();
}

static void setupMulticastSocket() {
	netInfo.multicastIP = "239.0.0.1";
	netInfo.multicastListenPort = 6000;
	netInfo.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (netInfo.socket < 0) {
		perror("socket err");
		printf("returned: %d", netInfo.socket);
		PLAT_Exit(true);
	}
	memset((char *)&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(netInfo.multicastListenPort);
	addrlen = sizeof(addr);
}
static void getHostname() {
	gethostname(netInfo.name, 64);

	// POSIX standard says that if the name is truncated,
	// it might not include a null terminator.
	// Nobody sane would use a hostname > 64 characters,
	// but just in case, add one.
	netInfo.name[63] = '\0';
}
