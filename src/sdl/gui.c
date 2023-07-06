#include <stdbool.h>
#include <stdio.h>
#include "font.h"
#include "button.h"
#include "vars.h"
#include <data.h>
#include <SDL2/SDL_log.h>
SDL_Window *win;
SDL_Surface *winSurface;
SDL_Renderer *rend;
extern SDL_RWops *sansFontRWOps;
void GUI_Main() {
	puts("initializing GUI");
	// returns zero on success else non-zero
	int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	if (ret != 0) {
		printf("error initializing SDL: %s\r\n", SDL_GetError());
		exit(1);
	}
	printf("SDL_Init(): %d\r\n", ret);

	#if __SWITCH__
	char *title = "sdl2_gles2";
	int winXpos = 0;
	int winYpos = 0;
	int winWidth = 1920;
	int winHeight = 1080;
	#else
	char *title = "sdl2_gles2";
	int winXpos = SDL_WINDOWPOS_CENTERED;
	int winYpos = SDL_WINDOWPOS_CENTERED;
	int winWidth = 640;
	int winHeight = 480;
	#endif

	win = SDL_CreateWindow(title, winXpos, winYpos, winWidth, winHeight, 0);

	if (win == NULL) {
		printf("error creating SDL Window: %s\r\n", SDL_GetError());
		exit(1);
	}
	printf("SDL_CreateWindow(): %p\r\n", (void *)win);

	FONT_Init();
	puts("returned from FONT_Init()");

	#if __SWITCH__
	int renderIndex = 0;
	#else
	int renderIndex = -1;
	#endif
	// uint32_t renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	uint32_t renderFlags = 0;
	// triggers the program that controls
	// your graphics hardware and sets flags
 
	// creates a renderer to render our images
	puts("creating SDL renderer");
	rend = SDL_CreateRenderer(win, renderIndex, renderFlags);
	puts("done");

	if (rend == NULL) {
		printf("error initializing SDL renderer: %s\r\n", SDL_GetError());
		exit(1);
	}
	printf("SDL_CreateRenderer(): %p\r\n", (void *)rend);
 
	// controls animation loop
	bool close = false;
 
	button_t startButton = {
		.color = { .r = 255, .g = 255, .b = 255, .a = 255, },
		.drawRect = { .x = 128, .y = 128, .w = 128, .h = 128 },
		.text = "bottom text"
	};



	// animation loop
	while (!close) {
		SDL_Event event;
 
		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					// handling of close button
					close = 1;
					break;
			}

			// pass event to button
			BUTTON_ProceessEvent(&startButton, &event);
		}
		SDL_SetRenderDrawColor(rend, 69, 69, 69, 255);

		// clears the screen
		SDL_RenderClear(rend);


		if (button(rend, &startButton)) {
			puts("button click");
		}

		// triggers the double buffers
		// for multiple rendering
		SDL_RenderPresent(rend);
 
		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}
	puts("cleaning up SDL thread and exiting.");
	// destroy renderer
	SDL_DestroyRenderer(rend);
 
	// destroy window
	SDL_DestroyWindow(win);
	
	// close font
	TTF_CloseFont(sansFont_8);
	TTF_CloseFont(sansFont_12);
	TTF_CloseFont(sansFont_16);

	// close rwops for font
	ret = SDL_RWclose(sansFontRWOps);
	if (ret != 0) {
		printf("error closing SDL RWops: %s", SDL_GetError());
	}

	// close SDL_TTF
	TTF_Quit();

	// close SDL
	SDL_Quit();

	return;
}
