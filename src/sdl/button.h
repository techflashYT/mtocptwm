typedef struct {
	SDL_Rect drawRect;    // dimensions of button
	struct {
		uint8_t r, g, b, a;
	} color;

    char *text;

	bool pressed;
} button_t;
extern void button_process_event(button_t *btn, const SDL_Event *ev);
extern bool button(SDL_Renderer *r, button_t *btn);
