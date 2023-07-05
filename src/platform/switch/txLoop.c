#include <stdint.h>
#include <nswitch.h>
#include <mtocptwm.h>
void platformTxLoop() {
	for (uint_fast8_t i = 0; i != 5; i++) {
		for (uint_fast8_t i2 = 0; i2 != 59; i2++) {
			if (!appletMainLoop()) {
				platformExit(false);
			}
			padUpdate(&pad);
			uint64_t buttons = padGetButtons(&pad);
			if (buttons & HidNpadButton_Plus) {
				platformExit(false);
			}
			// consoleUpdate waits for vsync.  So no, it isn't a waste to call it repeatedly.
			consoleUpdate(NULL);
		}
	}
}