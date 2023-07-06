typedef struct {
	SDL_Rect drawRect;    // dimensions of button
	SDL_Color color;

    char *text;

	bool pressed;
} button_t;
extern void BUTTON_ProceessEvent(button_t *btn, const SDL_Event *ev);
extern bool button(SDL_Renderer *r, button_t *btn);
