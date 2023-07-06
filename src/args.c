#include <mtocptwm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

extern int guiMain();
extern bool mode;
void handleArgs(int argc, char *argv[]) {
	bool badArgs = false;
	if (argc > 2)  {badArgs = true;}
	if (argc == 2) {
		if (argv[1][0] == 'r') {mode = true;}
		else if (argv[1][0] == 't') {mode = false;}
		else {badArgs = true;}
	}
	if (badArgs) {
		puts("Please give argument `r` or `t` for recieve or transmit, or none to start GUI.");
		platformExit(true);
	}
	// no args, handle GUI stuff
	if (argc == 1) {
		pid_t pid = fork();
		if (pid == -1) {
			// error
			perror("fork");
			exit(1);
		}
		if (pid == 0) {
			guiMain();
			return;
		}
		if (pid == 1) {
			// TODO: parent process, find some way to communicate with the child, then wait for the child (SDL) to tell us the mode.  Some ideas, ranked in order of how good they are:
			// - redirect stdout to the child's stdin (would require minimal changes, like opening a pipe, then redirecting our stdout to the pipe's input, and redirecting our stdin to the pipe's output)
			// - IPC
			// - temporary files
		}
	}
}