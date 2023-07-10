#include <stdlib.h>
#include <nswitch.h>
extern int shouldExit;
PadState pad;
void PLAT_Init(int *argc, char *argv[]) {
void PLAT_Init(int *argc, char *argv[]) {
	socketInitializeDefault();
	nxlinkStdio();
	puts("\x1b[1;1H\x1b[2JNXLink test!!");

	// set supported input layout: 1 player, standard 2-stick controls (dual joycon, pro controller, usb)
	// padConfigureInput(1, HidNpadStyleSet_NpadStandard);

	// init default gamepad (read handheld mode input, as well as first controller)
	// padInitializeDefault(&pad);
}
