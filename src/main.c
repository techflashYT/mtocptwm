#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef __SWITCH__
#include <nswitch.h>
#include <stdlib.h>
#endif

extern void NET_Init();
extern void PLAT_Init(int *argc, char *argv[]);
extern void ARG_Init(int argc, char *argv[]);
extern void GUI_Init(char *envp[]);
bool mode;
bool doGUI;
#include <mtocptwm.h>

int main(int argc, char *argv[], char *envp[]) {
	PLAT_Init(&argc, argv);
	ARG_Init(argc, argv);
	NET_Init();

	// TODO: Listen for connections on a TCP port (find an open port, or if not possible, ask the firewall to let us through (MS firewall, or UPnP if router)), then put the IP and port here

	if (doGUI) {
		GUI_Init(envp);
	}
	PLAT_Exit(false);
}