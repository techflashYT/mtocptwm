// mainly taken from devkitPro/examples/wii/template/source/template.c
// fix intelisense issue
#undef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <platWii.h>
#include <network.h>
#include <unistd.h>
#include <mtocptwm.h>
static void *xfb;
static GXRModeObj *rmode;
void platformInit(int *argc, char *argv[]) {
	VIDEO_Init();

	// Init wiimotes
	WPAD_Init();

	// get preferred video mode from system settings
	rmode = VIDEO_GetPreferredMode(NULL);

	// allocate memory for framebuffer in uncached area
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialize terminal
	CON_Init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

	// set up video registers with this mdoe
	VIDEO_Configure(rmode);

	// Tell GPU where the framebuffer is
	VIDEO_SetNextFramebuffer(xfb);

	// set visibility to true
	VIDEO_SetBlack(false);

	// flush changes to config
	VIDEO_Flush();

	// Wait 1 frame
	VIDEO_WaitVSync();
	if (rmode->viTVMode & VI_NON_INTERLACE) {
		// interlaced video, wait 1 more frame for the rest of the screen to update
		VIDEO_WaitVSync();
	}
	// set the cursor to row 0, col 0
	puts("\x1b[0;0H");

	// set argc and argv to make the rest of the logic easier
	*argc = 2;
	int selected = 0;
	int ret;

	char * baseStr ="\x1b[2J\x1b[0;0HMain Menu\r\n"
					"Please select a mode:\r\n"
					"\r\n"
					"  Transmit\r\n"
					"  Recieve";
	puts(baseStr);
	puts("\x1b[3;0H>");
	while (true) {
		ret = scanWiimotes();
		if (ret == 0x69) {
			VIDEO_WaitVSync();
			continue;
		}
		if (ret == ACT_SELECT) {
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
		else if (ret == ACT_DOWN) {
			if (selected != 1) {
				selected++;
			}
		}
		else if (ret == ACT_UP) {
			if (selected != 0) {
				selected--;
			}
		}

		puts(baseStr);
		printf("\x1b[%d;0H>", 5 + selected);

		VIDEO_WaitVSync();
	}
	puts("\x1b[1;1H\x1b[2J");
	puts("Initializing network...");
	char localIP[16] = {0};

	// Configure the network interface
	ret = net_init();
	uint32_t ipTmp = net_gethostip();
	uint8_t *ip = (uint8_t *)&ipTmp;
	char tmp[4];
	memset(localIP, 0, sizeof(localIP));

	itoa(ip[0], tmp, 10);
	int len = strlen(localIP);
	strcat(localIP + len, tmp);

	itoa(ip[1], tmp, 10);
	len = strlen(localIP);
	localIP[len] = '.';
	strcat(localIP + len + 1, tmp);

	itoa(ip[2], tmp, 10);
	len = strlen(localIP);
	localIP[len] = '.';
	strcat(localIP + len + 1, tmp);

	itoa(ip[3], tmp, 10);
	len = strlen(localIP);
	localIP[len] = '.';
	strcat(localIP + len + 1, tmp);

	if (ret >= 0) {
		printf("network configured, ip: %s\n", localIP);
	}
	 else {
		printf("network configuration failed!\n");
	}
	return;
}