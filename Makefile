build:
	cc -Wall -std=c99 src/*.c `pkg-config sdl3 --cflags --libs` -o game_of_life

run:
	./game_of_life
