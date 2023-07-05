#include <gccore.h>
#include <network.h>
#include <wiiuse/wpad.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <mtocptwm.h>
void __attribute__ ((noreturn)) platformExit(bool err) {
	net_deinit();
	WPAD_Shutdown();
	net_shutdown(netInfo.socket, 2);
	close(netInfo.socket);
	if (err) {
		sleep(2);
	}
	VIDEO_SetBlack(true);
	exit(err);
}