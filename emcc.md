```makefile
# Emscripten Makefile
CC      = emcc
CFLAGS  = -Wall -Wextra -Wno-unused-variable -std=c11 -ggdb \
          -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2  -s ASYNCIFY -O2
OUT     = index.html

build:
	$(CC) $(CFLAGS) main.c app.c entity.c item.c map.c \
	--preload-file assets@/assets --use-preload-plugins \
	-o $(OUT)
```
Animations are a problem cuzz not like normal game 
Assets in init textures and font to change /assets prob
Probobly optimaize somehow animations or webgl support
Also disable vSync maybe
https://gist.github.com/aaangeletakis/3187339a99f7786c25075d4d9c80fad5
@BlagojeBlagojevic ➜ /workspaces/wrogue (emcc) $ sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

Emscripten: 
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
