#include <stdio.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <switch.h>
#pragma GCC diagnostic pop

#include <errno.h>
#define perror(x) printf(x ": %s\r\n", strerror(errno))

extern PadState pad;


/*
#define puts(x) \
	puts(x);\
	consoleUpdate(NULL);

#define perror(x) \
	perror(x); \
	consoleUpdate(NULL);

#define printf(...) \
	printf(__VA_ARGS__); \
	consoleUpdate(NULL);
*/

