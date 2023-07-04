#include <stdio.h>
#include <stdbool.h>

extern void setupNet();
extern void broadcastLoop(const char *message, const bool mode);
extern void platformInit(int *argc, char *argv[]);
#include <mtocptwm.h>
#ifdef __SWITCH__
int shouldExit = 0;
#endif


int main(int argc, char *argv[]) {
	platformInit(&argc, argv);
	#ifdef __SWITCH__
		if (shouldExit) {
			return 0;
		}
	#endif
	bool badArgs = false;
	bool mode;
	if (argc != 2) {badArgs = true;}
	if (argv[1][0] == 'r') {mode = true;}
	else if (argv[1][0] == 't') {mode = false;}
	else {badArgs = true;}
	if (badArgs) {
		puts("Please give argument `r` or `t` for recieve or transmit.");
		return 1;
	}
	setupNet();

	char message[124];

	// TODO: Listen for connections on a TCP port (find an open port, or if not possible, ask the firewall to let us through (MS firewall, or UPnP if router)), then put the IP and port here
	sprintf(message, "__mtocptwm__/HELLO\nName: %s\nPort: %d", netInfo.name, netInfo.localListenPort);

	broadcastLoop(message, mode);
}