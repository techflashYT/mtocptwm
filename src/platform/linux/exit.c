#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <mtocptwm.h>
void __attribute__ ((noreturn)) platformExit(bool err) {
	shutdown(netInfo.socket, SHUT_RDWR);
	close(netInfo.socket);
	exit(err);
}