#include <SDL2/SDL.h>
#include "button.h"

void button_process_event(button_t *btn, const SDL_Event *ev) {
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
	SDL_SetRenderDrawColor(r, btn->color.r, btn->color.g, btn->color.b, btn->color.a);
	SDL_RenderFillRect(r, &btn->drawRect);
	

	// if button press detected - reset it so it wouldn't trigger twice
	if (btn->pressed) {
		btn->pressed = false;
		return true;
	}
	return false;
}
