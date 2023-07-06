#include <stdlib.h>
#include <nswitch.h>
#include "../common/menu.c"
extern int shouldExit;
PadState pad;
void PLAT_Init(int *argc, char *argv[]) {
	socketInitializeDefault();
	// nxlinkStdio();
	// initialize the text console
	consoleInit(NULL);

	// set supported input layout: 1 player, standard 2-stick controls (dual joycon, pro controller, usb)
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);

	// init default gamepad (read handheld mode input, as well as first controller)
	padInitializeDefault(&pad);

	uint64_t buttons;
	uint_fast8_t selected = 0;
	*argc = 2;
	TMENU_Init();

	while (appletMainLoop()) {
		// scan gamepad
		padUpdate(&pad);

		buttons = padGetButtons(&pad);
		if (buttons & HidNpadButton_Plus) {
			consoleExit(NULL);
			exit(1);
			break;
		}
		if (buttons & HidNpadButton_A) {
			if (selected == 0) {
				argv[1] = "t";
				break;
			}
			else if (selected == 1) {
				argv[1] = "r";
				break;
			}
			else {
				argv[1] = "WTF??????";
				break;
			}
		}
		if (buttons & HidNpadButton_AnyDown) {
			if (selected == 0) {
				selected++;
			}
		}
		if (buttons & HidNpadButton_AnyUp) {
			if (selected == 1) {
				selected--;
			}
		}
		
		TMENU_Run(selected);
		consoleUpdate(NULL);
	}
	puts("\x1b[1;1H\x1b[2J");
	consoleUpdate(NULL);
}
