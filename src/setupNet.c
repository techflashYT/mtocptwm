#ifdef PLAT_LINUX
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <net/if.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif

#ifdef PLAT_WII
	#include <network.h>
	#include <errno.h>
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
void setupNet() {
	setupMulticastSocket();
	getHostname();
}

static void setupMulticastSocket() {
	netInfo.multicastIP = "239.0.0.1";
	netInfo.multicastListenPort = 6000;
	netInfo.socket = 
	#ifdef PLAT_WII
		net_socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	#endif
	#if PLAT_LINUX || PLAT_WIN
		socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	#endif
	if (netInfo.socket < 0) {
		#ifdef PLAT_WII
		errno = netInfo.socket;
		#endif
			
		perror("socket err");
		printf("returned: %d", netInfo.socket);
		exit(1);
	}
	memset((char *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(netInfo.multicastListenPort);
	#ifdef PLAT_WII
	addr.sin_len = 8; // weird hack to fix libogc
	#endif
	addrlen = sizeof(addr);
}
static void getHostname() {
	#ifdef PLAT_WII
		strcpy(netInfo.name, "Nintendo_Wii");
	#endif

	#if PLAT_LINUX || PLAT_WIN
	gethostname(netInfo.name, 64);

	// POSIX standard says that if the name is truncated,
	// it might not include a null terminator.
	// Nobody sane would use a hostname > 64 characters,
	// but just in case, add one.
	netInfo.name[63] = '\0';
	#endif
}