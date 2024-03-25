main: main.c
	$(CC) -o main main.c `sdl2-config --cflags --libs` -lSDL2_ttf
