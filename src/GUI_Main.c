#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
void GUI_Main(char *envp[]) {
    char *argv[] = { "./gui", NULL };
    execve("./gui", argv, envp);
    perror("./gui");
    exit(1);
}