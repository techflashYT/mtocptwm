#include <unistd.h>
#include <stdint.h>
void PLAT_RxLoop(const uint_fast8_t delaySec) {
	sleep(delaySec);
}