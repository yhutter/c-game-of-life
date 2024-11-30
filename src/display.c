#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;
uint32_t *color_buffer = NULL;


void destroy_display(void) {
	free(color_buffer);
	SDL_DestroyTexture(color_buffer_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool init_display(void) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Game of Life", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (window == NULL) {
		fprintf(stderr, "Failed to create Window: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) {
		fprintf(stderr, "Failed to create Renderer: %s\n", SDL_GetError());
		return false;
	}

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH,
		WINDOW_HEIGHT	
	);

	if (color_buffer_texture == NULL) {
		fprintf(stderr, "Failed to create Color Buffer Texture: %s\n", SDL_GetError());
		return false;
	}

	color_buffer = (uint32_t*) malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));
	if (color_buffer == NULL) {
		fprintf(stderr, "Failed to create color buffer\n");
		return false;
	}
	return true;
}

void clear_color_buffer(uint32_t color) {
	for (int y = 0; y < WINDOW_HEIGHT; y++) {
		for (int x = 0; x < WINDOW_WIDTH; x++) {
			draw_pixel(x, y, color);
		}
	}
}

void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int) WINDOW_WIDTH * sizeof(uint32_t));
	SDL_RenderTexture(
		renderer,
		color_buffer_texture,
		NULL,
		NULL
	);
}

void draw_pixel(int x, int y, uint32_t color) {
	if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
		return;
	}
	color_buffer[(y * WINDOW_WIDTH) + x] = color;
}

void draw_rectangle(int x, int y, int w, int h, uint32_t color) {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			int curr_x = i + x;
			int curr_y = j + y;
			draw_pixel(curr_x, curr_y, color);
		}
	}
}

void draw_grid(int grid_size, uint32_t color) {
	for (int y = 0; y < WINDOW_HEIGHT; y++) {
		for (int x = 0; x < WINDOW_WIDTH; x++) {
			if (x % grid_size == 0 || y % grid_size == 0) {
				draw_pixel(x, y, color);
			}
		}
	}
}
