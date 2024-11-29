#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL3/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *color_buffer_texture;
extern uint32_t *color_buffer;
extern int window_width;
extern int window_height;

bool init_display(void);
void destroy_display(void);
void clear_color_buffer(uint32_t color);
void render_color_buffer(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_rectangle(int x, int y, int w, int h, uint32_t color);
#endif // DISPLAY_H
