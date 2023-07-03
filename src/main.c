#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

extern void setupNet();
extern void broadcastLoop(const char *message, const bool mode);
extern void platformInit(int *argc, char *argv[]);
#include <mtocptwm.h>

int main(int argc, char *argv[]) {
	platformInit(&argc, argv);
	if (argc != 2) {puts("bad args"); return 1;}
	setupNet();

	char message[124];

	// TODO: Listen for connections on a TCP port (find an open port, or if not possible, ask the firewall to let us through (MS firewall, or UPnP if router)), then put the IP and port here
	sprintf(message, "__mtocptwm__/HELLO\nName: %s\nPort: %d", netInfo.name, netInfo.localListenPort);


	bool mode;
	if (argv[1][0] == 'r') {mode = true;}
	else if (argv[1][0] == 't') {mode = false;}
	else {
		puts("bad args");
		exit(2);
	}
	broadcastLoop(message, mode);
}