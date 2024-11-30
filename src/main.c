#include "display.h"
#include <SDL3/SDL.h>
#include <stdlib.h>

#define CELL_SIZE 16
#define NUM_CELLS_X  (WINDOW_WIDTH / CELL_SIZE)
#define NUM_CELLS_Y  (WINDOW_HEIGHT / CELL_SIZE)
#define NUM_CELLS_TOTAL  (NUM_CELLS_X * NUM_CELLS_Y)
#define CELL_ALIVE 1
#define CELL_DEAD  0

bool running = false;
uint32_t background_color = 0xff181818;
uint32_t cell_dead_color = 0xffe4e4ef;
uint32_t cell_alive_color = 0xff181818;
uint8_t *cells = NULL;

void init_cells(void) {
	cells = malloc(NUM_CELLS_TOTAL * sizeof(uint8_t));
	memset(cells, CELL_DEAD, NUM_CELLS_TOTAL * sizeof(uint8_t));
}

void render_cells(void) {
	for (int i = 0; i < NUM_CELLS_TOTAL; i++) {
		int x = i % NUM_CELLS_X;
		int y = i / NUM_CELLS_X;
		uint32_t color = cells[i] == CELL_DEAD ? cell_dead_color : cell_alive_color;
		draw_rectangle(
			x * CELL_SIZE,
			y * CELL_SIZE,
			CELL_SIZE,
			CELL_SIZE,
			color
		);
	}
}

void destroy_cells(void) {
	free(cells);
}

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
	render_cells();
	draw_grid(CELL_SIZE, background_color);
	SDL_RenderPresent(renderer);
}


int main(void) {
	running = init_display();
	init_cells();
	while (running) {
		check_events();
		render();
	}
	destroy_cells();
	destroy_display();
	return 0;
}



