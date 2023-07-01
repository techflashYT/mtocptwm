#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

extern void setupNet(int argc, char *argv[]);
extern void broadcastLoop(const char *name, const char *message, const bool mode);
int main(int argc, char *argv[]) {
	if (argc != 2) {puts("bad args"); return 1;}
	setupNet(argc, argv);

	char message[124];
	char name[64];

	gethostname(name, 64);

	// POSIX standard says that if the name is truncated,
	// it might not include a null terminator.
	// Nobody sane would use a hostname > 64 characters,
	// but just in case, add one.
	name[64] = '\0';
	// TODO: Listen for connections on a TCP port (find an open port, or if not possible, ask the firewall to let us through (MS firewall, or UPnP if router)), then put the IP and port here
	sprintf(message, "__mtocptwm__/HELLO\nName: %s\nIP: %s\nPort: %s", name, "You.rMo.mAB.CDE", "6969");

	bool mode;
	if (argv[1][0] == 'r') {
		mode = true;
	}
	else if (argv[1][0] == 't') {
		mode = false;
	}
	else {
		puts("bad args");
		exit(2);
	}
	broadcastLoop(name, message, mode);
}