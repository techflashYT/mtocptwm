#include <stdio.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <switch.h>
#pragma GCC diagnostic pop
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

#define puts(x) \
	puts(x);\
	fflush(stdout);

#define perror(x) \
	perror(x); \
	fflush(stdout);

#define printf(...) \
	printf(__VA_ARGS__); \
	fflush(stdout);
