#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <stdint.h>
void PLAT_TxLoop(const uint_fast8_t delaySec) {
	Sleep(delaySec * 1000);
}