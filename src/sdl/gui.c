#include <stdbool.h>
#include <stdio.h>
#include "font.h"
#include "button.h"
#include "vars.h"
#include <data.h>
SDL_Window *win;
SDL_Surface *winSurface;
SDL_Renderer *rend;
extern SDL_RWops *sansFontRWOps;
int GUI_Main() {
	puts("initializing GUI");
	// returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		exit(1);
	}
	win = SDL_CreateWindow("MTOCPTWM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);

	FONT_Init();
	// triggers the program that controls
	// your graphics hardware and sets flags
	uint32_t renderFlags = SDL_RENDERER_ACCELERATED;
 
	// creates a renderer to render our images
	rend = SDL_CreateRenderer(win, -1, renderFlags);

	if (rend == NULL) {
		printf("error initializing SDL renderer: %s", SDL_GetError());
		exit(1);
	}
 
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

	// destroy renderer
	SDL_DestroyRenderer(rend);
 
	// destroy surface
	int ret = SDL_DestroyWindowSurface(win);
	if (ret != 0) {
		printf("error destroying SDL Window Surface: %s", SDL_GetError());
	}

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
 
	return 0;
}
