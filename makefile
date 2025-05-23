CC      = emcc
CFLAGS  = -Wall -Wextra -Wno-unused-variable -std=c11 -ggdb \
          -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2  -s ASYNCIFY -O2
OUT     = index.html

build:
	$(CC) $(CFLAGS) main.c app.c entity.c item.c map.c \
	--preload-file assets@/assets --use-preload-plugins \
	-o $(OUT)