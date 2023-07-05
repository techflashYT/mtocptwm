#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mtocptwm.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
void __attribute__ ((noreturn)) platformExit(bool err) {
	close(netInfo.socket);
	WSACleanup();

	exit(err);
}