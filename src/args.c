#include <mtocptwm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#if __SWITCH__
#include <nswitch.h>
#endif


extern int GUI_Main();
extern bool mode;
void ARG_Init(int argc, char *argv[]) {
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
		#ifdef __SWITCH__
			Thread t;
			Result res = threadCreate(&t, (ThreadFunc)GUI_Main, NULL, NULL, 10240, 0x3B, 2);
			if (res != 0) {
				fatalThrow(res);
			}
			threadStart(&t);
			while (true) { sleep(5); }
		#else
			pid_t pid = fork();
			if (pid == -1) {
				// error
				perror("fork");
				exit(1);
			}
			if (pid == 0) {
				// TODO: child process, find some way to communicate with the parent, then wait for the parent (SDL) to tell us the mode.  Some ideas, ranked in order of how good they are:
				// - redirect stdout to the parent's stdin (would require minimal changes, like opening a pipe, then redirecting our stdout to the pipe's input, and redirecting our stdin to the pipe's output)
				// - IPC
				// - temporary files
				while (true) { sleep(5); }
			}
			if (pid >= 1) {
				GUI_Main();
				exit(0);
			}
		#endif
	}
}