#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <nswitch.h>
#include <mtocptwm.h>
void __attribute__ ((noreturn)) platformExit(bool err) {
	consoleUpdate(NULL);
	shutdown(netInfo.socket, SHUT_RDWR);
	close(netInfo.socket);

	consoleExit(NULL);
	socketExit();

	if (err) {
		sleep(2);
	}
	exit(err);
}