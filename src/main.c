#include <SDL3/SDL.h>
#include "display.h"

bool running = false;
uint32_t background_color = 0xff181818;

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

void render(void) {
	render_color_buffer();
	clear_color_buffer(background_color);
	SDL_RenderPresent(renderer);
}


int main(void) {
	running = init_display();
	while (running) {
		check_events();
		render();
	}
	destroy_display();
	return 0;
}



