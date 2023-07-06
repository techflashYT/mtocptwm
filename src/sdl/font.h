#include <SDL2/SDL_ttf.h>
enum FONT_RendStyle {FONT_RendStyle_Solid, FONT_RendStyle_Shaded, FONT_RendStyle_Blended};
extern SDL_Texture *FONT_Draw(char text[], TTF_Font *font, SDL_Color fg, SDL_Color bg, enum FONT_RendStyle renderStyle);
extern void FONT_Init();
extern TTF_Font *sansFont;