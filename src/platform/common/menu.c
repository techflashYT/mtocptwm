/*
	FIXME: Replace me with the SDL menu when it is finished.
	Code for the terminal-based menu.
	Used (for now) in the Wii and Switch ports.
*/
#include <stdio.h>
char *baseStr = "\x1b[2J\x1b[0;0HMain Menu\r\n"
				"Please select a mode:\r\n"
				"\r\n"
				"  Transmit\r\n"
				"  Recieve";

static void TMENU_Init() {
	puts(baseStr);
	puts("\x1b[3;0H>");
}
static void TMENU_Run(int selected) {
	puts(baseStr);
	printf("\x1b[%d;0H>", 4 + selected);
}