#include <stdio.h>


#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


int width = 1280;
int height = 720;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;


int main(void) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow("Game of Life", width, height, 0);
	if (window == NULL) {
		fprintf(stderr, "Failed to create Window: %s\n", SDL_GetError());
		return -1;
	}
	
	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		fprintf(stderr, "Failed to create Renderer: %s\n", SDL_GetError());
		return -1;
	}

	bool running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT:
					running = false;
					break;
			}
		}
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}



