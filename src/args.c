#include <mtocptwm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#if __SWITCH__
#include <nswitch.h>
#endif

#include <string.h>
#include <ipc.h>


extern void NET_SetupIPCLocalhost();

extern int GUI_Main(char *envp[]);
extern bool mode;


void ARG_Init(int argc, char *argv[], char *envp[]) {
	bool badArgs = false;
	if (argc > 2)  {badArgs = true;}
	if (argc == 2) {
		if (argv[1][0] == 'r') {mode = true;}
		else if (argv[1][0] == 't') {mode = false;}
		else {badArgs = true;}
	}
	if (badArgs) {
		puts("Please give argument `r` or `t` for recieve or transmit, or none to start GUI.");
		PLAT_Exit(true);
	}
	// no args, handle GUI stuff
	if (argc == 1) {
		NET_SetupIPCLocalhost();

		pid_t pid = fork();
		if (pid == -1) {
			// error
			perror("fork");
			exit(1);
		}
		if (pid == 0) {
			childSetup();
			communicate();
		}
		if (pid >= 1) {
			// parent
			parentSetup();

			GUI_Main(envp);
			exit(0);
		}
	}
}
