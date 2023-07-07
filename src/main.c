#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef __SWITCH__
#include <nswitch.h>
#include <stdlib.h>
#endif

extern void NET_Init();
extern void NET_Loop(const char *message, const bool mode);
extern void PLAT_Init(int *argc, char *argv[]);
extern void ARG_Init(int argc, char *argv[], char *evnp[]);
bool mode;
#include <mtocptwm.h>

int main(int argc, char *argv[], char *envp[]) {
	PLAT_Init(&argc, argv);
	ARG_Init(argc, argv, envp);
	NET_Init();

	char message[124];

	// TODO: Listen for connections on a TCP port (find an open port, or if not possible, ask the firewall to let us through (MS firewall, or UPnP if router)), then put the IP and port here
	sprintf(message, "__mtocptwm__/HELLO\nName: %s\nPort: %d", netInfo.name, netInfo.localListenPort);

	NET_Loop(message, mode);
	PLAT_Exit(false);
}