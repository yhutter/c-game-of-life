build:
	cc -Wall -std=c99 src/*.c `pkg-config sdl3 sdl3-image --cflags --libs` -O3 -o game_of_life

run:
	./game_of_life
