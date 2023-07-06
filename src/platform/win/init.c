#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

WSADATA wsaData;


void PLAT_Init(__attribute__((unused)) int *argc, __attribute__((unused)) char *argv[]) {
	// initialize WinSock2
	int res = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (res != 0) {
		printf("platformInit_win: WSAStartup failed: %d\n", res);
		exit(1);
	}
}