#include "font.h"
#include "data.h"
#include "vars.h"
TTF_Font *sansFont_8;
TTF_Font *sansFont_12;
TTF_Font *sansFont_16;
SDL_RWops*sansFontRWOps;
void FONT_Init() {
	int ret = TTF_Init();
	if (ret != 0) {
		printf("error initializing SDL-TTF: %s\r\n", TTF_GetError());
		exit(1);
	}
	printf("TTF_Init(): %d\r\n", ret);

	sansFontRWOps = SDL_RWFromConstMem(DATA_FontSans, DATA_FontSans_Size);
	if (sansFontRWOps == NULL) {
		printf("error creating SDL RWops from mem: %s\r\n", SDL_GetError());
		exit(1);
	}
	printf("SDL_RWFromConstMem(): %p\r\n", (void *)sansFontRWOps);

	sansFont_8  = TTF_OpenFontRW(sansFontRWOps, false, 8);
	printf("sansFont_8: %p\r\n", (void *)sansFont_8);
	sansFont_12 = TTF_OpenFontRW(sansFontRWOps, false, 12);
	printf("sansFont_12: %p\r\n", (void *)sansFont_12);
	sansFont_16 = TTF_OpenFontRW(sansFontRWOps, false, 16);
	printf("sansFont_16: %p\r\n", (void *)sansFont_16);
	if (sansFont_8 == NULL || sansFont_12 == NULL || sansFont_16 == NULL) {
		printf("error creating SDL-TTF Font from RWops: %s\r\n", SDL_GetError());
		exit(1);
	}
	return;
}
SDL_Texture *FONT_Draw(char text[], TTF_Font *font, SDL_Color fg, SDL_Color bg, enum FONT_RendStyle renderStyle) {
	SDL_Surface *surf;

	if      (renderStyle == FONT_RendStyle_Solid)   surf = TTF_RenderText_Solid  (font, text, fg);
	else if (renderStyle == FONT_RendStyle_Shaded)  surf = TTF_RenderText_Shaded (font, text, fg, bg);
	else if (renderStyle == FONT_RendStyle_Blended) surf = TTF_RenderText_Blended(font, text, fg);

	if (surf == NULL) {
		printf("failed to render SDL text to surface: %s\r\n", SDL_GetError());
		exit(1);
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surf);
	if (texture == NULL) {
		printf("failed to create SDL Texture from SDL Surface: %s\r\n", SDL_GetError());
		exit(1);
	}
	SDL_FreeSurface(surf);
	return texture;
}