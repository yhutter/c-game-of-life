#include "display.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>

#define CELL_SIZE 3 
#define NUM_CELLS_X  (WINDOW_WIDTH / CELL_SIZE)
#define NUM_CELLS_Y  (WINDOW_HEIGHT / CELL_SIZE)
#define NUM_CELLS_TOTAL  (NUM_CELLS_X * NUM_CELLS_Y)
#define CELL_ALIVE 1
#define CELL_DEAD  0
#define DRAWING 0
#define RUNNING 1 
#define FPS 60
#define WAIT_TIME (1000/FPS)

bool running = false;
uint32_t background_color = 0xffe4e4ef;
uint32_t foreground_color = 0xff181818;

typedef struct game_state_t {
	uint8_t *current_cells;
	uint8_t *old_cells;
	uint8_t mode;
	bool show_grid;
} game_state_t;

game_state_t state = { 
	.current_cells = NULL,
	.old_cells = NULL,
	.mode = DRAWING,
	.show_grid = true
};

void init_cells(char* image_path) {
	state.current_cells = malloc(NUM_CELLS_TOTAL * sizeof(uint8_t));
	state.old_cells = malloc(NUM_CELLS_TOTAL * sizeof(uint8_t));
	memset(state.current_cells, CELL_DEAD, NUM_CELLS_TOTAL * sizeof(uint8_t));
	if (image_path != NULL) {
		SDL_Surface *image_surface = IMG_Load(image_path);
		if (image_surface == NULL) {
			fprintf(stderr, "Could not load image %s\n", SDL_GetError());
		} else {
			uint8_t *pixels = image_surface->pixels;
			int width = image_surface->w;
			int height = image_surface->h;
			int bpp = 4;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					size_t pixel_index = (y * image_surface->pitch) + x * bpp;
					size_t cell_index = (y * NUM_CELLS_X) + x;
					uint8_t pixel = pixels[pixel_index];
					state.current_cells[cell_index] = pixel == 0 ? CELL_ALIVE : CELL_DEAD;
				}
			}
			SDL_DestroySurface(image_surface);
		}
	}
	memcpy(state.old_cells, state.current_cells, NUM_CELLS_TOTAL * sizeof(uint8_t));
}

uint32_t cell_index_from_mouse(float mouse_x, float mouse_y) {
	int cell_x = (int) (mouse_x / CELL_SIZE);
	int cell_y = (int) (mouse_y / CELL_SIZE);
	uint32_t cell_index = (cell_y * NUM_CELLS_X) + cell_x;
	return cell_index;
}

uint32_t count_living_neighbours(int index, uint8_t *cells) {
	// [ ] [ ] [ ]
	// ^
	// top_row_start
	// [ ] [x] [ ]
	// ^
	// middle_row_start
	// [ ] [ ] [ ]
	// ^
	// bottom_row_start

	size_t top_row_start= index - NUM_CELLS_X - 1;
	size_t middle_row_start = index - 1;
	size_t bottom_row_start = index + NUM_CELLS_X - 1;
	uint32_t living_neighbours = 0;

	// Check top row
	for (int i = 0; i < 3; i++) {
		size_t index_to_check = top_row_start + i;
		if (index_to_check < 0 || index_to_check > NUM_CELLS_TOTAL) {
			continue;
		}
		if (cells[index_to_check] == CELL_ALIVE) {
			living_neighbours++;
		}
	}
	// Check middle row
	for (int i = 0; i < 3; i++) {
		size_t index_to_check = middle_row_start + i;
		if (index_to_check < 0 || index_to_check > NUM_CELLS_TOTAL || index_to_check == index) {
			continue;
		}
		if (cells[index_to_check] == CELL_ALIVE) {
			living_neighbours++;
		}
	}
	// Check bottom row
	for (int i = 0; i < 3; i++) {
		size_t index_to_check = bottom_row_start + i;
		if (index_to_check < 0 || index_to_check > NUM_CELLS_TOTAL) {
			continue;
		}
		if (cells[index_to_check] == CELL_ALIVE) {
			living_neighbours++;
		}
	}
	return living_neighbours;
}

void update_cells() {
	// Copy current into old
	memcpy(state.old_cells, state.current_cells, NUM_CELLS_TOTAL * sizeof(uint8_t));
	for (int i = 0; i < NUM_CELLS_TOTAL; i++) {
		uint32_t neighbours = count_living_neighbours(i, state.old_cells);
		uint8_t cell_state = state.old_cells[i];
		if (cell_state == CELL_ALIVE) {
			// 1. Any life cell with fewer then 2 neighbours dies.
			if (neighbours < 2) {
				cell_state = CELL_DEAD;
			}
			// 2. Any life cell with two or three neighbours lives.
			else if (neighbours == 2 || neighbours == 3) {
				cell_state = CELL_ALIVE;
			}
			// 3. Any life cell with more then three neighbour dies.
			else {
				cell_state = CELL_DEAD;
			}

		} else {
			// 4. Any dead cell with exactly three live neighbours becomes alive again.
			if (neighbours == 3) {
				cell_state = CELL_ALIVE;
			}
		}
		state.current_cells[i] = cell_state;
	}
}

void update(void) {
	// Keep track of mouse position when in drawing mode.
	if (state.mode == DRAWING) {
		float mouse_x;
		float mouse_y;
		SDL_MouseButtonFlags flags = SDL_GetMouseState(&mouse_x, &mouse_y);
		if (flags & SDL_BUTTON_LMASK) {
			uint32_t index = cell_index_from_mouse(mouse_x, mouse_y);
			state.current_cells[index] = CELL_ALIVE;
		} else if (flags & SDL_BUTTON_RMASK) {
			uint32_t index = cell_index_from_mouse(mouse_x, mouse_y);
			state.current_cells[index] = CELL_DEAD;
		}
	} else {
		// Running mode
		update_cells();
	}
}

void render_cells(uint8_t *cells) {
	for (int i = 0; i < NUM_CELLS_TOTAL; i++) {
		int x = i % NUM_CELLS_X;
		int y = i / NUM_CELLS_X;
		uint32_t color = cells[i] == CELL_DEAD ? background_color : foreground_color;
		draw_rectangle(
			x * CELL_SIZE,
			y * CELL_SIZE,
			CELL_SIZE,
			CELL_SIZE,
			color
		);
	}
	// Display grid to indicate that we are in drawing mode
	if (state.mode == DRAWING && state.show_grid) {
		draw_grid(CELL_SIZE, foreground_color);
	}
}

void destroy_cells(void) {
	free(state.current_cells);
	free(state.old_cells);
}


void check_events(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_EVENT_QUIT: {
				running = false;
				break;
			}
			case SDL_EVENT_KEY_DOWN: {
				switch (event.key.key) {
					case SDLK_D: {
						// Toggle between running and drawing mode
						if (state.mode == DRAWING) {
							state.mode = RUNNING;
						} else {
							state.mode = DRAWING;
						}
						break;
					}
					case SDLK_G: {
						state.show_grid = !state.show_grid;
						break;
					}
				}
				break;
			}
		}
	}
}

void render(void) {
	render_color_buffer();
	clear_color_buffer(background_color);
	render_cells(state.current_cells);
	SDL_RenderPresent(renderer);
}


int main(void) {
	running = init_display();
	init_cells("src/sako.png");
	while (running) {
		check_events();
		update();
		render();
		SDL_Delay(WAIT_TIME);
	}
	destroy_cells();
	destroy_display();
	return 0;
}



