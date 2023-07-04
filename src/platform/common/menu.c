#include <stdio.h>
char *baseStr = "\x1b[2J\x1b[0;0HMain Menu\r\n"
				"Please select a mode:\r\n"
				"\r\n"
				"  Transmit\r\n"
				"  Recieve";

static void menuInit() {
	puts(baseStr);
	puts("\x1b[3;0H>");
}
static void menu(int selected) {
	puts(baseStr);
	printf("\x1b[%d;0H>", 4 + selected);
}