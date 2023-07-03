#undef __linux__
#include <stdio.h>
#include <wiiuse/wpad.h>
#include <wiiuse/wiiuse.h>
#include <unistd.h>
#include <stdlib.h>
#include <platWii.h>
int scanWiimotes() {
	// scan for controller updates
	WPAD_ScanPads();

	// get pressed buttons
	uint32_t pressed = WPAD_ButtonsDown(0);
	if (pressed & WPAD_BUTTON_HOME) {
		for (uint_fast8_t i = 0; i != 50; i++) {
			puts("");
			usleep(10000);
		}
		puts("Bye!");
		sleep(1);
		exit(0);
	}
	else if (pressed & WPAD_BUTTON_A) {
		return ACT_SELECT;
	}
	else if (pressed & WPAD_BUTTON_DOWN) {
		return ACT_DOWN;
	}
	else if (pressed & WPAD_BUTTON_UP) {
		return ACT_UP;
	}
	else {
		return 0x69;
	}
}