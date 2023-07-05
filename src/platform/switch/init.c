#include <stdlib.h>
#include <nswitch.h>
#include "../common/menu.c"
extern int shouldExit;
PadState pad;
ViDisplay display;
Event vsyncEvent;
void platformInit(int *argc, char *argv[]) {
	socketInitializeDefault();
	// nxlinkStdio();
	// initialize the text console
	consoleInit(NULL);

	// set supported input layout: 1 player, standard 2-stick controls (dual joycon, pro controller, usb)
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);

	// init default gamepad (read handheld mode input, as well as first controller)
	padInitializeDefault(&pad);

	uint64_t buttons = 0;
	uint64_t buttonsOld = 1;
	uint_fast8_t selected = 0;
	*argc = 2;
	menuInit();

	// setup to be able to wait for vsync
	Result rc = viOpenDefaultDisplay(&display);
	if (rc != 0) {
		uint16_t rc16[2] = &rc;
		printf("viOpenDefaultDisplay(): Error %d-%d", rc16[0], rc16[1]);
		exit(1);
	}

	while (appletMainLoop()) {
		
		// scan gamepade
		padUpdate(&pad);

		buttons = padGetButtons(&pad);
		if (buttons == buttonsOld) {
			continue;
		}
		buttonsOld = buttons;
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
		
		menu(selected);
		consoleUpdate(NULL);
	}
	puts("\x1b[1;1H\x1b[2J");
	consoleUpdate(NULL);
}