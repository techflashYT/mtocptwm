#include <unistd.h>
#include <stdint.h>
void PLAT_TxLoop(const uint_fast8_t delaySec) {
	sleep(delaySec);
}