#include <stdio.h>


#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "display.h"

bool running = false;

void check_events(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_EVENT_QUIT:
				running = false;
				break;
		}
	}
}

void clean_up(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(void) {
	running = init_window();
	while (running) {
		check_events();
	}
	clean_up();
	return 0;
}



