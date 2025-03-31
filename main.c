#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include<SDL2/SDL_hints.h>
#include<SDL2/SDL_error.h>
#include<SDL2/SDL_log.h>
#include<SDL2/SDL_video.h>
#include<SDL2/SDL_render.h>
#include<SDL2/SDL_pixels.h>
#include<SDL2/SDL_rect.h>
#include<SDL2/SDL_surface.h>
#include<SDL2/SDL_clipboard.h>
#include<SDL2/SDL_events.h>
#include<SDL2/SDL_image.h>
//#include<SDL2/SDl_timer.h>
#include<SDL2/SDL_keyboard.h>
#include<SDL2/SDL_scancode.h>
#undef main

//COLORS
#define WHITE (SDL_Color){255, 255, 255, 0}

//TTF
#include<SDL2/SDL_ttf.h>
const char* fontLoc = "assets/fonts/f.ttf";

//ARROW KEYS
#define LEFT_ARROW 1073741904
#define RIGHT_ARROW 1073741903
#define UP_ARROW 1073741906
#define DOWN_ARROW 1073741905

//#include<pthread.h> TBD asychronus stuff

//TYPES
#include<stdint.h>
typedef  uint8_t  u8;
typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  uint64_t u64;
typedef  int16_t  i16;
typedef  int32_t  i32;
typedef  int64_t  i64;
typedef  float    f32;
typedef  double   f64;

#define rand_f64() (f64)rand()/(f64)RAND_MAX

//DYNAMIC ARRAY
#define DA_SIZE 256
#define da_append(da, item)                                                              \
	do {                                                                                 \
		if ((da)->count >= (da)->capacity) {                                             \
			(da)->capacity = (da)->capacity == 0 ? DA_SIZE : (da)->capacity*2;           \
			(da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items));     \
			assert((da)->items != NULL && "Realloc fail !!!");                           \
			}                                                                                \
		\
		(da)->items[(da)->count++] = (item);                                             \
		} while (0)




//ERROR AND LOG HANDLING

#define WARNING(...)        fprintf(stdout, __VA_ARGS__)
#define ERROR_BREAK(...)    fprintf(stderr, __VA_ARGS__); exit(-1)
#define LOG(...)     			  fprintf(stdout, __VA_ARGS__)
#define PAUSE()           {char a; fputs(a, stdin);}
#define CLAMP(X, LOW, HIGH) {if(X < LOW) X = LOW; if(X > HIGH) X = HIGH;}
#define ASSERT(msg) {fprintf(stderr, "aseert in:\n\tFILE %s\n\tLINE %d\n\tmsg: %s" , __FILE__, __LINE__, msg); exit(-1);}
#define DROP(var) {(void)var;}
void SDL_ERR(int code) {

	if((code) < 0) {
		ERROR_BREAK("SDL_ERROR: %s\n", SDL_GetError());
		}
	return;
	}

void *P_SDL_ERR(void *ptr) {
	if(ptr == NULL) {
		ERROR_BREAK("SDL_ERROR: %s\n", SDL_GetError());
		}
	return ptr;
	}

#define MAP_X 90
#define MAP_Y 90

typedef struct Graphics_State {
	SDL_Window   *window;
	SDL_Renderer *renderer;
	TTF_Font     *font;
	i32           fontW;
	i32           fontH;
	SDL_Event     event;   //TBD like a queue of elements
	i32           width;
	i32           height;
	u8            isQuit;
	} Graphics_State;

const char* title = "Ime kakvo";

Graphics_State mainGraphics;
#define WINDOW   mainGraphics.window
#define RENDERER mainGraphics.renderer
#define FONT     mainGraphics.font
#define FONT_W   mainGraphics.fontW
#define FONT_H   mainGraphics.fontH
#define QUIT     mainGraphics.isQuit
#define EVENT    mainGraphics.event
#define WIDTH    mainGraphics.width
#define HEIGHT   mainGraphics.height

void Text_Renderer_C(SDL_Renderer *renderer, TTF_Font *font, i32 startX, i32 startY, i32 w_c, i32 h_c, char *c, SDL_Color textColor) {
	if(renderer == NULL) {
		ASSERT("renderer is null!!!");
		}
	if(font == NULL) {
		ASSERT("font is null!!!");
		}
	SDL_Surface *textSurface = P_SDL_ERR(TTF_RenderText_Solid(font, c, textColor));
	SDL_Texture *textTexture = P_SDL_ERR(SDL_CreateTextureFromSurface(renderer, textSurface));
	SDL_Rect textRect = {startX, startY, w_c, h_c}; // rectangle where the text is drawn
	SDL_ERR(SDL_RenderCopy(renderer, textTexture, NULL, &textRect));
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	}



typedef struct {
	i32 x;
	i32 y;
	} Position;

typedef struct {
	char ch;
	SDL_Color color;
	u8   isWall;
	} Tile;
#define MAP(map, x, y)      map[((x) % MAP_X) + ((y) % MAP_Y) * MAP_X]
#define MAP_CH(map, x, y)   map[((x) % MAP_X) + ((y) % MAP_Y) * MAP_X].ch
#define MAP_ISW(map, x, y)  map[((x) % MAP_X) + ((y) % MAP_Y) * MAP_X].isWall


typedef struct {
	Position pos;
	SDL_Color color;
	char ch;
	} Entitiy;

typedef struct {
	Entitiy *items;
	u64 capacity;
	u64 count;
	} Entitiy_DA;

typedef struct {
	i32 width;
	i32 height;
	Position pos;
	Position center;
	} Room;

Entitiy* create_player(Position startPos) {
	Entitiy* player = calloc(1, sizeof(Entitiy));
	player->pos.x = startPos.x;
	player->pos.y = startPos.y;
	player->ch = '@';
	return player;
	}

Entitiy* create_monster(Position startPos) {
	Entitiy* monster = calloc(1, sizeof(Entitiy));
	monster->pos.x = startPos.x;
	monster->pos.y = startPos.y;
	monster->ch = 'M';
	return monster;
	}



void render_player(Entitiy *player) {
	i32 startX = player->pos.x * FONT_W;
	i32 startY = player->pos.y * FONT_H;
	Text_Renderer_C(RENDERER, FONT, startX, startY, 10, 15, &player->ch, WHITE);
	}

void player_input(SDL_Event *event, Entitiy* player, Tile* map) {
	const u32 key = event->key.keysym.sym;

	if(key == UP_ARROW) {
		if(player->pos.y > 0 && MAP_ISW(map, player->pos.x, player->pos.y-1) == SDL_TRUE) {
			player->pos.y--;
			}
		}
	else if(key == DOWN_ARROW) {
		if(player->pos.y < MAP_Y && MAP_ISW(map, player->pos.x, player->pos.y+1) == SDL_TRUE) {
			player->pos.y++;
			}
		}
	else if(key == LEFT_ARROW) {
		if(player->pos.x > 0 && MAP_ISW(map, player->pos.x-1, player->pos.y) == SDL_TRUE) {
			player->pos.x--;
			}
		}
	else if(key == RIGHT_ARROW) {
		if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x+1, player->pos.y) == SDL_TRUE) {
			player->pos.x++;
			}
		}
	}

Room create_room(i32 x, i32 y, i32 height, i32 width) {
	Room newRoom;
	if(height > MAP_Y) {
		ASSERT("height too large for room creation");
		}
	if(width > MAP_X) {
		ASSERT("width too large for room creation");
		}
	LOG("Created room (x, y, height, width ) (%d %d %d %d)\n", x, y, height, width);
	newRoom.height   = height;
	newRoom.width    = width;
	newRoom.pos.x    = x;
	newRoom.pos.y    = y;
	newRoom.center.x = x +  (i32)(width  / 2);
	newRoom.center.y = y +  (i32)(height / 2);
	if(newRoom.center.y > MAP_Y) {
		ASSERT("newRoom.center.x too large for room creation");
		}
	if(newRoom.center.x > MAP_X) {
		ASSERT("newRoom.center.x too large for room creation");
		}
	return newRoom;
	}

void add_room_to_map(Tile *map, Room room) {
	for(i32 y = room.pos.y; y < room.pos.y + room.height; y++) {
		for(i32 x = room.pos.x; x < room.pos.x + room.width; x++) {
			MAP_CH(map, x, y)  = '.';
			MAP_ISW(map, x, y) = SDL_TRUE;
			}
		}
	}

void add_room_wall_to_map(Tile *map, Room room) {
	for(i32 y = room.pos.y; y < room.pos.y + room.height; y++) {
		for(i32 x = room.pos.x; x < room.pos.x + room.width; x++) {
			if(y == room.pos.y && MAP_CH(map, x, y) != ','  && MAP_CH(map, x, y-1) != ',')  {
				MAP_CH(map, x, y)  = '#';
				MAP_ISW(map, x, y) = SDL_FALSE;
				}
			else if(x == room.pos.x && MAP_CH(map, x, y) != ',' && MAP_CH(map, x-1, y) != ',') {
				MAP_CH(map, x, y)  = '#';
				MAP_ISW(map, x, y) = SDL_FALSE;
				}
			else if(x == room.pos.x + room.width - 1 && MAP_CH(map, x-1, y) != ',') {
				MAP_CH(map, x, y)  = '#';
				MAP_ISW(map, x, y) = SDL_FALSE;
				}
			else if(y == room.pos.y + room.height - 1 && MAP_CH(map, x, y-1) != ',') {
				MAP_CH(map, x, y)  = '#';
				MAP_ISW(map, x, y) = SDL_FALSE;
				}
			}
		}
	}

void connect_room_centers(Position centerOne, Position centerTwo, Tile* map) {
	Position temp;
	temp.x = centerOne.x;
	temp.y = centerOne.y;

	while (1) {
		if (abs((temp.x - 1) - centerTwo.x) < abs(temp.x - centerTwo.x))
			temp.x--;
		else if (abs((temp.x + 1) - centerTwo.x) < abs(temp.x - centerTwo.x))
			temp.x++;
		else if (abs((temp.y + 1) - centerTwo.y) < abs(temp.y - centerTwo.y))
			temp.y++;
		else if (abs((temp.y - 1) - centerTwo.y) < abs(temp.y - centerTwo.y))
			temp.y--;
		else
			break;

		MAP_CH(map, temp.x, temp.y) = ',';
		MAP_ISW(map, temp.x, temp.y) = SDL_TRUE;

		//MAP_CH(map, temp.x+1, temp.y) = '.';
		//MAP_ISW(map, temp.x+1, temp.y) = SDL_FALSE;

		//MAP_CH(map, temp.x, temp.y+1) = '.';
		//MAP_ISW(map, temp.x, temp.y+1) = SDL_FALSE;

		//MAP_CH(map, temp.x, temp.y-1) = '.';
		//MAP_ISW(map, temp.x, temp.y-1) = SDL_FALSE;

		//MAP_CH(map, temp.x-1,  temp.y) = '.';
		//MAP_ISW(map, temp.x-1, temp.y) = SDL_FALSE;
		}
	}

void add_walls_around_roads(Tile* map) {
	for(i32 y = 1; y < MAP_Y-1; y++) {
		for(i32 x = 1; x < MAP_X-1; x++) {
			if(MAP_CH(map, x, y) == ',') {
				if(MAP_CH(map, x+1, y) != ',') {
					MAP_CH(map, x+1, y) = '#';
					}
				if(MAP_CH(map, x+1, y+1) != ',') {
					MAP_CH(map, x+1, y+1) = '#';
					}
				if(MAP_CH(map, x, y+1) != ',') {
					MAP_CH(map, x, y+1) = '#';
					}

				}
			}
		}
	}

void caved_part(Tile *map, i32 x, i32 y) {
	for(i32 carved = 0; carved < rand()%10 + 3; carved++) {
		i32 xr = rand()%3 - 1;
		i32 yr = rand()%3 - 1;
		MAP_CH(map, x + xr, y + yr) = '.';
		MAP_ISW(map, x + xr, y + yr) = SDL_TRUE;

		}
	}
void caved_map(Tile *map, f64 percantage) {

	for(i32 y = 1; y < MAP_Y - 1; y++) {
		for(i32 x = 1; x < MAP_X - 1; x++) {
			if(rand_f64() < percantage) {

				if(MAP_CH(map, x, y) == '.' && MAP_CH(map, x - 1, y) == '#') {
					MAP_CH(map, x - 1, y) = '.';
					MAP_ISW(map, x - 1, y) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			if(rand_f64() < percantage) {
				if(MAP_CH(map, x, y) == '.' && MAP_CH(map, x + 1, y) == '#') {
					MAP_CH(map, x + 1, y) = '.';
					MAP_ISW(map, x + 1, y) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			if(rand_f64() < percantage) {
				if(MAP_CH(map, x, y) == '.' && MAP_CH(map, x, y + 1) == '#') {
					MAP_CH(map, x, y + 1) = '.';
					MAP_ISW(map, x, y + 1) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			if(rand_f64() < percantage) {
				if(MAP_CH(map, x, y) == '.' && MAP_CH(map, x, y - 1) == '#') {
					MAP_CH(map, x, y - 1) = '.';
					MAP_ISW(map, x, y - 1) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			}
		}
	}



void generete_dungons(Tile *map, i32 minRooms, i32 maxRooms) {
	i32 width, height, x, y, nRooms;
	if(minRooms >= maxRooms) {
		ASSERT("We have a larger amount of minRooms >= maxRooms");
		}
	nRooms = (i32)(rand()%(u32)(maxRooms)) + minRooms;

	//nRooms = 5;
	Room *rooms = calloc(nRooms, sizeof(Room));
	rooms[0] = create_room(9, 9, 10, 10);
	add_room_to_map(map, rooms[0]);
	//add_room_wall_to_map(map, rooms[0]);
	for(i32 i = 1; i < nRooms; i++) {
		y = (rand() % (MAP_Y - 22));
		x = (rand() % (MAP_X - 22));
		height = (rand() % 15) + 7;
		width  = (rand() % 15) + 7;
		rooms[i] = create_room(x, y, height, width);

		add_room_to_map(map, rooms[i]);
		add_room_wall_to_map(map, rooms[i]);
		connect_room_centers(rooms[i-1].center, rooms[i].center, map);
		}
	/*
	for(i32 y = nRooms/2-1; y < nRooms; y++) {
		for(i32 x = 0; x < nRooms/2+1; x++) {
			if(x != y) {
				connect_room_centers(rooms[x].center, rooms[y].center, map);
				}

			}
		}
	//*/
	//add_walls_around_roads(map);
	f64 percantage = rand_f64() / 2.2f;
	LOG("percantage of caved map %f", percantage);
	caved_map(map, percantage);
	free(rooms);
	}


#define RAND_MAP()\
	for(i32 y = 0; y < MAP_Y; y++) {\
		for(i32 x = 0; x < MAP_X; x++) {\
			f64 r = rand_f64();\
			if(r < 1.0f/100.0f) {\
				MAP_CH(map, x, y) = '#';\
				MAP_ISW(map, x, y) = SDL_FALSE;\
				}\
			else {\
				MAP_CH(map, x, y)= '.';\
				MAP_ISW(map, x, y) = SDL_TRUE;\
				}\
			}

#define MAP_STDOUT()\
	for(int y = 0; y < MAP_Y; y++){\
		for(int x = 0; x < MAP_X; x++){\
			printf("%c", MAP(map, x, y));\
			}	\
		printf("\n");\
		}

Tile* init_map() {
	Tile *map;
	map = calloc(MAP_Y*MAP_Y, sizeof(Tile));
	for(i32 y = 0; y < MAP_Y*MAP_Y; y++) {
		map[y].ch = '#';
		}
	//memset(map. , '.', sizeof(Tile) * MAP_Y * MAP_Y);
	if(map == NULL) {
		ASSERT("alloc of map failed!!!");
		}
	//RAND_MAP();
	generete_dungons(map, 2, 30);
	return map;
	}




#define PLAYER_VISION
void render_map(Tile *map, Entitiy *player) {
	Text_Renderer_C(RENDERER, FONT, WIDTH/2, 0, 10*10, 20, "ROUGE GAME", WHITE);
	
	
	const i32 radius = 10;
	i32 startX = player->pos.x - radius;
	i32 startY = player->pos.y - radius;
	i32 stopX  = player->pos.x + radius;
	i32 stopY  = player->pos.y + radius;
	CLAMP(startX, 0, MAP_X-1);
	CLAMP(stopX,  0, MAP_X-1);
	CLAMP(startY, 0, MAP_Y-1);
	CLAMP(stopY,  0, MAP_Y-1);
	for(i32 y  = startY; y < stopY; y++) {
		for(i32 x = startX; x < stopX; x++) {//*/

	/*
	for(i32 y  = 0; y < MAP_Y; y++) {
		for(i32 x = 0; x < MAP_X; x++) {
			//*/
			i32 startX = x * FONT_W;
			i32 startY = y * FONT_H;
			char ch = MAP_CH(map, x, y);
			if(ch == '#') {
				SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
				SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
				SDL_RenderFillRect(RENDERER, &textRect);
				}
			else if(ch == ',') {
				SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
				SDL_SetRenderDrawColor(RENDERER, 10, 10, 10, 100);
				//SDL_RenderFillRect(RENDERER, &textRect);
				}
			else if(ch != '.') {
				Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, &ch, WHITE);
				}

			}
		}

	}

void render_monsters(Entitiy_DA *monsters, Entitiy *player) {
	
	const i32 radius = 10;
	i32 startX = player->pos.x - radius;
	i32 startY = player->pos.y - radius;
	i32 stopX  = player->pos.x + radius;
	i32 stopY  = player->pos.y + radius;
	CLAMP(startX, 0, MAP_X-1);
	CLAMP(stopX,  0, MAP_X-1);
	CLAMP(startY, 0, MAP_Y-1);
	CLAMP(stopY,  0, MAP_Y-1);//*/
	for(u64 count = 0; count < monsters->count; count++) {
		if(monsters->items[count].pos.x >= startX && monsters->items[count].pos.x <= stopX
		&& monsters->items[count].pos.y >= startY && monsters->items[count].pos.y <= stopY){
			render_player(&monsters->items[count]);	
		}
		
		}

	}

//static i32 asd = 0;
void main_renderer(Entitiy* player, Entitiy_DA *monster, Tile *map) {
	SDL_ERR(SDL_RenderClear(RENDERER));
	render_map(map, player);
	render_player(player);
	//render_player(&monster->items[0]);
	render_monsters(monster, player);
	SDL_ERR(SDL_SetRenderDrawColor(RENDERER, 0X10, 0X10, 0X10, 0XFF));
	SDL_RenderPresent(RENDERER);
	}

void event_user(Entitiy *player, Tile* map) {
	if(SDL_WaitEvent(&EVENT)) {
		if(EVENT.type == SDL_QUIT) {
			QUIT = 1;
			}
		else if(EVENT.type == SDL_WINDOWEVENT) {  //JUST FOR NOW
			SDL_GetWindowSize(WINDOW, &WIDTH, &HEIGHT);
			FONT_H = HEIGHT / MAP_Y + 1;
			FONT_W = WIDTH  / MAP_X;
			//FONT_H = 15;
			//FONT_W = 10;
			if(WIDTH > 4096 || HEIGHT > 2048) {
				ASSERT("Oversized window\n");
				}
			//LOG("width %d, height %d\n", WIDTH, HEIGHT);
			}
		else if(EVENT.type==SDL_KEYDOWN) {
			player_input(&EVENT, player, map);
			}
		}
	}


void genereate_monsters(Entitiy_DA *monsters, Tile *map) {
	for(i32 y = 0; y < MAP_Y; y++) {
		for(i32 x = 0; x < MAP_X; x++) {
			if(MAP_CH(map, x, y) != '#') {
				if(rand_f64() < 0.01f) {
					Entitiy *temp = create_monster((Position) {
						.x = x, .y = y
						});
					da_append(monsters, *temp);
					}
				}
			}
		}
	}



#define SEED 12344
#include<time.h>

int main() {


	SDL_ERR(SDL_Init(SDL_INIT_VIDEO));
	SDL_ERR(TTF_Init());
	srand(time(0));
	WINDOW   = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 800,  SDL_WINDOW_OPENGL);
	(void*)P_SDL_ERR(WINDOW);
	RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetWindowResizable(WINDOW, SDL_TRUE);
	FONT = TTF_OpenFont(fontLoc, 64);
	(void*)P_SDL_ERR(FONT);
	(void*)P_SDL_ERR(RENDERER);
	QUIT = 0;

	Entitiy* player = create_player((Position) {
		10, 10
		});
	Tile *map = init_map();
	Entitiy_DA monsters = {0};
	/*Entitiy *monster = create_player((Position) {
		15, 15
		});
	da_append(&monsters, *monster);
	//*/
	genereate_monsters(&monsters, map);
	//MAP_STDOUT();
	while(!QUIT) {
		main_renderer(player,  &monsters, map);
		event_user(player, map);
		//SDL_Delay(10);
		}

	return 0;
	ASSERT("UNREACHABLE");
	}
