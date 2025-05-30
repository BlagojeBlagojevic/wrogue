CFLAGS  = -Wall -Wextra -Wno-unused-variable -Wimplicit-fallthrough=0 -Werror -std=c11 -ggdb -lSDL2 -lm -lSDL2_ttf -lSDL2_image
CC = gcc


build: 
	$(CC) -o app main.c app.c entity.c item.c map.c $(CFLAGS) 

mem: 
	$(CC) -o app main.c app.c entity.c item.c map.c $(CFLAGS) -O2 -fsanitize=address -static-libasan


CCEMCC      = emcc
CFLAGSEMCC  = -Wall -Wextra -Wno-unused-variable -std=c11 -ggdb \
          -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2  -s ASYNCIFY -O3 
OUT     = index.html

buildemcc:
	$(CCEMCC) $(CFLAGSEMCC) main.c app.c entity.c item.c map.c \
	--preload-file assets@/assets --use-preload-plugins \
	-o $(OUT)