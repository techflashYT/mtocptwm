#include <platWii.h>
#include <stdint.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
void platformTxLoop() {
	for (uint_fast8_t i = 0; i != 5; i++) {
		for (uint_fast8_t i2 = 0; i2 != 59; i2++) {
			scanWiimotes();
			VIDEO_WaitVSync();
		}
	}
}