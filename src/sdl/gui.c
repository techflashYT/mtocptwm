#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include "button.h"
#include <data.h>
TTF_Font *font;
SDL_RWops*fontRWOps;
int GUI_Main() {
	puts("initializing GUI");
	// returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}
	if (TTF_Init() != 0) {
		printf("error initializing SDL-TTF: %s", TTF_GetError());
		return 1;
	}
	SDL_Window *win = SDL_CreateWindow("MTOCPTWM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);

	fontRWOps = SDL_RWFromConstMem(DATA_FontSans, DATA_FontSans_Size);
	if (fontRWOps == NULL) {
		printf("error creating SDL RWops from mem: %s", SDL_GetError());
		return 1;
	}
	font = TTF_OpenFontRW(fontRWOps, false, 12);
	if (font == NULL) {
		printf("error creating SDL-TTF Font from RWops: %s", SDL_GetError());
		return 1;
	}
	// triggers the program that controls
	// your graphics hardware and sets flags
	uint32_t renderFlags = SDL_RENDERER_ACCELERATED;
 
	// creates a renderer to render our images
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, renderFlags);
 
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
			button_process_event(&startButton, &event);
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
 
	// destroy window
	SDL_DestroyWindow(win);
	 
	// close SDL
	SDL_Quit();
 
	return 0;
}
