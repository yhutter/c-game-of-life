#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL3/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;

bool init_window(void);

#endif // DISPLAY_H
