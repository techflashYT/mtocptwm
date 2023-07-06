#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef __SWITCH__
#include <nswitch.h>
#include <stdlib.h>
#endif

extern void setupNet();
extern void broadcastLoop(const char *message, const bool mode);
extern void platformInit(int *argc, char *argv[]);
extern void handleArgs(int argc, char *argv[]);
bool mode;
#include <mtocptwm.h>

int main(int argc, char *argv[]) {
	platformInit(&argc, argv);
	setupNet();
	handleArgs(argc, argv);

	char message[124];

	// TODO: Listen for connections on a TCP port (find an open port, or if not possible, ask the firewall to let us through (MS firewall, or UPnP if router)), then put the IP and port here
	sprintf(message, "__mtocptwm__/HELLO\nName: %s\nPort: %d", netInfo.name, netInfo.localListenPort);

	broadcastLoop(message, mode);
	platformExit(false);
}
