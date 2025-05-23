CFLAGS  = -Wall -Wextra -Wno-unused-variable -Wimplicit-fallthrough=0 -Werror -std=c11 -ggdb -lSDL2 -lm -lSDL2_ttf -lSDL2_image
CC = gcc


build: 
	$(CC) -o app main.c app.c entity.c item.c map.c $(CFLAGS) 

mem: 
	$(CC) -o app main.c app.c entity.c item.c map.c $(CFLAGS) -O2 -fsanitize=address -static-libasan
