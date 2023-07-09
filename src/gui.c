#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ipc.h>

extern void NET_SetupIPCLocalhost();

static void GUI_Main(char *envp[]) {
	sleep(10000);
	char *argv[] = { "./gui", NULL };
	execve("./gui", argv, envp);
	perror("./gui");
	PLAT_Exit(true);
}
void GUI_Init(char *envp[]) {
	NET_SetupIPCLocalhost();

	pid_t pid = fork();
	if (pid == -1) {
		// error
		perror("fork");
		PLAT_Exit(true);
	}
	if (pid == 0) {
		childSetup();
		communicate();
	}
	if (pid >= 1) {
		// parent
		GUI_Main(envp);
		PLAT_Exit(false);
	}
}