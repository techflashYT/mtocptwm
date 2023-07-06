#include "font.h"
#include "data.h"
#include "vars.h"
TTF_Font *sansFont;
SDL_RWops*sansFontRWOps;
void FONT_Init() {
    if (TTF_Init() != 0) {
		printf("error initializing SDL-TTF: %s", TTF_GetError());
		exit(1);
	}

	sansFontRWOps = SDL_RWFromConstMem(DATA_FontSans, DATA_FontSans_Size);
	if (sansFontRWOps == NULL) {
		printf("error creating SDL RWops from mem: %s", SDL_GetError());
		exit(1);
	}
	sansFont = TTF_OpenFontRW(sansFontRWOps, false, 12);
	if (sansFont == NULL) {
		printf("error creating SDL-TTF Font from RWops: %s", SDL_GetError());
		exit(1);
	}
}
SDL_Texture *FONT_Draw(char text[], TTF_Font *font, SDL_Color fg, SDL_Color bg, enum FONT_RendStyle renderStyle) {
    SDL_Surface *surf;

    if      (renderStyle == FONT_RendStyle_Solid)   surf = TTF_RenderText_Solid  (font, text, fg);
    else if (renderStyle == FONT_RendStyle_Shaded)  surf = TTF_RenderText_Shaded (font, text, fg, bg);
    else if (renderStyle == FONT_RendStyle_Blended) surf = TTF_RenderText_Blended(font, text, fg);

	if (surf == NULL) {
		printf("failed to render SDL text to surface: %s", SDL_GetError());
		exit(1);
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surf);
	if (texture == NULL) {
		printf("failed to create SDL Texture from SDL Surface: %s", SDL_GetError());
		exit(1);
	}
	SDL_FreeSurface(surf);
    return texture;
}