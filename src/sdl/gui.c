#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

int guiMain() {
	// returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	SDL_Window *win = SDL_CreateWindow("MTOCPTWM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);

	// triggers the program that controls
	// your graphics hardware and sets flags
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
	// creates a renderer to render our images
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
 
	// controls animation loop
	bool close = false;
 
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
		}
 
		// clears the screen
		SDL_RenderClear(rend);

		SDL_SetRenderDrawColor(rend, 69, 69, 69, 255);
 
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
