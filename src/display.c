#include "display.h"

#include <stdio.h>
#include <stdbool.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int width = 1280;
int height = 720;

bool init_window(void) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Game of Life", width, height, 0);
	if (window == NULL) {
		fprintf(stderr, "Failed to create Window: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		fprintf(stderr, "Failed to create Renderer: %s\n", SDL_GetError());
		return false;
	}
	return true;
}
