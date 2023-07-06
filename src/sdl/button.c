#include "font.h"
#include "vars.h"
#include "button.h"

void BUTTON_ProceessEvent(button_t *btn, const SDL_Event *ev) {
	// react on mouse click within button rectangle by setting 'pressed'
	if (ev->type == SDL_MOUSEBUTTONDOWN) {
		if (ev->button.button == SDL_BUTTON_LEFT &&
				ev->button.x >= btn->drawRect.x &&
				ev->button.x <= (btn->drawRect.x + btn->drawRect.w) &&
				ev->button.y >= btn->drawRect.y &&
				ev->button.y <= (btn->drawRect.y + btn->drawRect.h)) {
			btn->pressed = true;
		}
	}
}

bool button(SDL_Renderer *r, button_t *btn) {
	// draw button
	int ret = SDL_SetRenderDrawColor(r, btn->color.r, btn->color.g, btn->color.b, btn->color.a);
	if (ret != 0) {
		printf("failed to set SDL Draw Color: %s", SDL_GetError());
		exit(1);
	}
	ret = SDL_RenderFillRect(r, &btn->drawRect);
	if (ret != 0) {
		printf("failed to render SDL Rect: %s", SDL_GetError());
		exit(1);
	}
	SDL_Color bg = { .r = 0, .g = 0, .b = 0, .a = 0 };
	SDL_Texture *texture = FONT_Draw(btn->text, sansFont_12, btn->color, bg, FONT_RendStyle_Shaded);
	ret = SDL_RenderCopy(r, texture, NULL, &btn->drawRect);
	if (ret != 0) {
		printf("failed to copy SDL texture to rect: %s", SDL_GetError());
		exit(1);
	}
	SDL_DestroyTexture(texture);

	// if button press detected - reset it so it wouldn't trigger twice
	if (btn->pressed) {
		btn->pressed = false;
		return true;
	}

	return false;
}
