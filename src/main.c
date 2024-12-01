#include "display.h"
#include <SDL3/SDL.h>
#include <stdlib.h>

#define CELL_SIZE 16
#define NUM_CELLS_X  (WINDOW_WIDTH / CELL_SIZE)
#define NUM_CELLS_Y  (WINDOW_HEIGHT / CELL_SIZE)
#define NUM_CELLS_TOTAL  (NUM_CELLS_X * NUM_CELLS_Y)
#define CELL_ALIVE 1
#define CELL_DEAD  0
#define DRAWING 0
#define RUNNING 1 

bool running = false;
uint32_t background_color = 0xff11111b;
uint32_t cell_dead_color = 0xff11111b;
uint32_t cell_alive_color = 0xffbac2de;

typedef struct GameState {
	uint8_t *cells;
	uint8_t mode;
} GameState;

GameState state = { 
	.cells = NULL,
	.mode = DRAWING
};


void init_cells(void) {
	state.cells = malloc(NUM_CELLS_TOTAL * sizeof(uint8_t));
	memset(state.cells, CELL_DEAD, NUM_CELLS_TOTAL * sizeof(uint8_t));
}

uint32_t cell_index_from_mouse(float mouse_x, float mouse_y) {
	int cell_x = (int) (mouse_x / CELL_SIZE);
	int cell_y = (int) (mouse_y / CELL_SIZE);
	uint32_t cell_index = (cell_y * NUM_CELLS_X) + cell_x;
	return cell_index;
}

void update_cells(void) {
	// Keep track of mouse position when in drawing mode.
	if (state.mode == DRAWING) {
		float mouse_x;
		float mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		uint32_t index = cell_index_from_mouse(mouse_x, mouse_y);
		state.cells[index] = CELL_ALIVE;
	}
}

void render_cells(void) {
	for (int i = 0; i < NUM_CELLS_TOTAL; i++) {
		int x = i % NUM_CELLS_X;
		int y = i / NUM_CELLS_X;
		uint32_t color = state.cells[i] == CELL_DEAD ? cell_dead_color : cell_alive_color;
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
	free(state.cells);
}


void check_events(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_EVENT_QUIT: {
				running = false;
				break;
			}
		}
	}
}

void render(void) {
	render_color_buffer();
	clear_color_buffer(background_color);
	update_cells();
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



