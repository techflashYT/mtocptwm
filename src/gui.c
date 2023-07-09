#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ipc.h>

extern void NET_SetupIPCLocalhost();

static void GUI_Main(char *envp[]) {
	char *argv[] = { "./gui", NULL };
	execve("./gui", argv, envp);
	perror("./gui");
	exit(1);
}
void GUI_Init(char *envp[]) {
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
		GUI_Main(envp);
		exit(0);
	}
}