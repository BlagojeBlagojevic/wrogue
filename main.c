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
#define SPACE 32
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
#define da_append(da, item)                                                            \
	do {                                                                                 \
		if ((da)->count >= (da)->capacity) {                                               \
			(da)->capacity = (da)->capacity == 0 ? DA_SIZE : (da)->capacity*2;               \
			(da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items));         \
			assert((da)->items != NULL && "Realloc fail !!!");                               \
			}                                                                                \
		\
		(da)->items[(da)->count++] = (item);                                               \
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

typedef struct {
	u64 	count;
	u64 	capacity;
	char** items;
	} Str;



#define MAP_X 100
#define MAP_Y 100

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
	SDL_bool      isMovmentEvent;
	Str           messages;
	} Graphics_State;

const char* title = "Ime kakvo";

#define FONT_W_MESSAGES 20
Graphics_State mainGraphics;
#define WINDOW    mainGraphics.window
#define RENDERER  mainGraphics.renderer
#define FONT      mainGraphics.font
#define FONT_W    mainGraphics.fontW
#define FONT_H    mainGraphics.fontH
#define QUIT      mainGraphics.isQuit
#define EVENT     mainGraphics.event
#define WIDTH     mainGraphics.width
#define HEIGHT    mainGraphics.height
#define MOVMENT   mainGraphics.isMovmentEvent
#define MESSAGES  mainGraphics.messages

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


typedef enum {
	DAMAGE_BASIC,
	DAMAGE_FIRE,
	DAMAGE_POISON,
	DAMAGE_NUM,

	} Damage_Types;

const char* damageStr[] = {
	"BASIC",
	"FIRE",
	"POISON",
	"NUM",
	};

typedef struct {
	Position pos;
	SDL_Color color;
	i32 radius;
	i32 health;
	char ch;
	i32 attack[DAMAGE_NUM];
	i32 defence[DAMAGE_NUM];
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

Entitiy* create_entity(char ch, i32 radius, i32 health, Position startPos) {
	Entitiy* entity = calloc(1, sizeof(Entitiy));
	entity->pos.x = startPos.x;
	entity->pos.y = startPos.y;
	entity->ch = ch;
	entity->radius = radius;
	entity->health = health;
	for(Damage_Types i = 0; i < DAMAGE_NUM; i++) {
		//TBD other types; for now just rand
		i32 random = rand()%5 + 1; //LIKE 6 DICES
		//LOG("%d", random);
		//system("pause");
		entity->attack[i] = random; //DAMAGE MAX
		entity->defence[i] = random + 1; //DAMAGE REDUCTION
		}
	return entity;
	}


#define INF (f64)100000.0f
#define ZERO 0.0f
#define DISTANCE(x1, y1, x2, y2) sqrt((f64)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)))

i32 roll_the_dice(i32 attack, i32 defence) {
	f64 maxAttack = 0.0f, maxDefence = 0.0f, num;
	//ATTACK
	for(i32 i = 0; i < attack; i++) {
		num = rand_f64();

		if(num > maxAttack) {
			maxAttack = num;
			}
		}
	//DEFENCE
	for(i32 i = 0; i < defence; i++) {
		num = rand_f64();

		if(num > maxDefence) {
			maxDefence = num;
			}
		}

	if(maxDefence > maxAttack) {
		return 0;
		}

	i32 diff = (i32)((maxAttack - maxDefence) * attack);
	CLAMP(diff, 1, INF);
	return diff;
	}

///TBD reusable
void message_attacked_by_monster(Entitiy* player, Entitiy* entity, i32 damage) {
	DROP(player);
	u64 len = strlen("Player attacked by monster %c damage %d") + 10;
	char* attackText = malloc(len*sizeof(char*));
	memset(attackText, '\0', len);
	if(attackText == NULL) {
		ASSERT("CALLOC FAILED\n");
		}
	i32 err = snprintf(attackText, len, "Player attacked by monster %c damage %d", entity->ch, damage);
	if(err < -1) {
		ASSERT("snprintf failed");
		}
	//LOG("%s\n", attackText);
	da_append(&MESSAGES, attackText);
	//add_to_str(attackText, &MESSAGES);
	//Text_Renderer_C(RENDERER, FONT, WIDTH - 100, HEIGHT - 100, 100, 20, "Da li ovo radi", WHITE);
	}

void message_attacked_by_player(Entitiy* player, Entitiy* entity, i32 damage) {
	DROP(player);
	u64 len = strlen("Monster attacked by player %c damage %d") + 10;
	char* attackText = malloc(len*sizeof(char*));
	memset(attackText, '\0', len);
	if(attackText == NULL) {
		ASSERT("CALLOC FAILED\n");
		}
	i32 err = snprintf(attackText, len, "Monster attacked by player %c damage %d", entity->ch, damage);
	if(err < -1) {
		ASSERT("snprintf failed");
		}
	//LOG("%s\n", attackText);
	da_append(&MESSAGES, attackText);
	//add_to_str(attackText, &MESSAGES);
	//Text_Renderer_C(RENDERER, FONT, WIDTH - 100, HEIGHT - 100, 100, 20, "Da li ovo radi", WHITE);
	}


void player_attack(Entitiy *player, Entitiy* entity) {
	i32 damage = roll_the_dice(player->attack[0], entity->defence[0]);
	entity->health-=damage;
	CLAMP(entity->health, 0, 100);
	message_attacked_by_player(player, entity, damage);
	//DROP(player);
	//i32 x1 = player->pos.x;
	//i32 y1 = player->pos.y;

	//i32 x2 = entity->pos.x;
	//i32 y2 = entity->pos.y;
	//f64 distance = DISTANCE(x1, y1, x2, y2);
	//if(distance == 0.0f) {
	//	}
	if(entity->health == 0) {
		entity->ch = 'S';
		}
	}



void monster_attack(Entitiy *player, Entitiy* entity) {
	//DROP(entity);
	i32 damage = roll_the_dice(entity->attack[0], player->defence[0]);

	player->health-=damage;
	CLAMP(player->health, 0, INF);
	//i32 startX =  player->pos.x;
	message_attacked_by_monster(player, entity, damage);
	//system("pause");
	//Text_Renderer_C(RENDERER, FONT, )
	if(player->health == 0) {

		system("cls");
		LOG("You loose");
		exit(-1);
		}
	}



void render_player(Entitiy *player) {
	i32 startX = player->pos.x * FONT_W;
	i32 startY = player->pos.y * FONT_H;
	SDL_Color color;
	if(player->health >= 3) {
		color = (SDL_Color) {
			255, 255, 255, 0
			};
		}
	else if(player->health == 2) {
		color = (SDL_Color) {
			255, 125, 125, 0
			};
		}
	else if(player->health == 1) {
		color = (SDL_Color) {
			255, 0, 0, 0
			};
		}
	else {
		color = (SDL_Color) {
			0, 255, 0, 0
			};
		}
	Text_Renderer_C(RENDERER, FONT, startX, startY, 10, 15, &player->ch, color);
	}

i32 is_monster_on_entity(i32 x, i32 y, Entitiy_DA* entities) {
	CLAMP(x, 0, (MAP_X - 1));
	CLAMP(y, 0, (MAP_Y - 1));
	for(u64 count = 0; count < entities->count; count++) {
		Entitiy entity = entities->items[count];
		if(entity.ch == 'M' && entity.pos.x == x && entity.pos.y == y) {

			entities->items[count] = entity;
			//LOG("health %d, count %d", entity.health, count);
			return count;
			}
		}
	return -1;
	}


void player_input(SDL_Event *event, Entitiy* player, Entitiy_DA *entitis, Tile* map) {
	const u32 key = event->key.keysym.sym;
	MOVMENT = SDL_FALSE;  //NOT PROB
	if(key == UP_ARROW) {
		if(player->pos.y > 0 && MAP_ISW(map, player->pos.x, player->pos.y-1) == SDL_TRUE) {
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.y--;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_FALSE;
			MOVMENT = SDL_TRUE;
			}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x,  player->pos.y-1, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster]);
				MOVMENT = SDL_TRUE;
				}
			}
		}
	else if(key == DOWN_ARROW) {
		if(player->pos.y < MAP_Y && MAP_ISW(map, player->pos.x, player->pos.y+1) == SDL_TRUE) {
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.y++;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_FALSE;
			MOVMENT = SDL_TRUE;
			}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x,  player->pos.y + 1, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster]);
				MOVMENT = SDL_TRUE;
				}
			}

		}
	else if(key == LEFT_ARROW) {
		if(player->pos.x > 0 && MAP_ISW(map, player->pos.x-1, player->pos.y) == SDL_TRUE) {
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.x--;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			MOVMENT = SDL_TRUE;
			}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x - 1,  player->pos.y, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster]);
				MOVMENT = SDL_TRUE;
				}
			}

		}
	else if(key == RIGHT_ARROW) {
		if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x + 1, player->pos.y) == SDL_TRUE) {
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.x++;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			MOVMENT = SDL_TRUE;
			}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x + 1,  player->pos.y, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster]);
				MOVMENT = SDL_TRUE;
				}
			}
		}
	else if(key == SPACE) {
		//DO NOTHING
		MOVMENT = SDL_TRUE;
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

				if(MAP_CH(map, x, y) == '.'  && MAP_CH(map, x - 1, y) == '#') {
					MAP_CH(map, x - 1, y) = '.';
					MAP_ISW(map, x - 1, y) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			if(rand_f64() < percantage) {
				if(MAP_CH(map, x, y) == '.'  && MAP_CH(map, x + 1, y) == '#') {
					MAP_CH(map, x + 1, y) = '.';
					MAP_ISW(map, x + 1, y) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			if(rand_f64() < percantage) {
				if(MAP_CH(map, x, y) == '.'  && MAP_CH(map, x, y + 1) == '#') {
					MAP_CH(map, x, y + 1) = '.';
					MAP_ISW(map, x, y + 1) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			if(rand_f64() < percantage) {
				if(MAP_CH(map, x, y) == '.'  && MAP_CH(map, x, y - 1) == '#') {
					MAP_CH(map, x, y - 1) = '.';
					MAP_ISW(map, x, y - 1) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			//CAVED ROAD
			if(rand_f64() < 0.1f) {
				if(MAP_CH(map, x, y) == ',') {
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
	nRooms = (i32)(rand()%(u32)(maxRooms - minRooms)) + minRooms;

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
		//add_room_wall_to_map(map, rooms[i]);
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
	//RAND_MAP();
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


	i32 radius = player->radius;
	i32 startX = player->pos.x - radius;
	i32 startY = player->pos.y - radius;
	i32 stopX  = player->pos.x + radius;
	i32 stopY  = player->pos.y + radius;
	CLAMP(startX, 0, MAP_X-1);
	CLAMP(stopX,  0, MAP_X-1);
	CLAMP(startY, 0, MAP_Y-1);
	CLAMP(stopY,  0, MAP_Y-1);
	///*
#ifdef PLAYER_VISION
	for(i32 y  = startY; y < stopY; y++) {
		for(i32 x = startX; x < stopX; x++) {//*/
#endif
			///*
#ifndef PLAYER_VISION
			for(i32 y  = 0; y < MAP_Y; y++) {
				for(i32 x = 0; x < MAP_X; x++) {
#endif
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
						DROP(textRect);
						//SDL_RenderFillRect(RENDERER, &textRect);
						}
					else if(ch != '.') {
						Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, &ch, WHITE);
						}

					}
				}

			}

		void render_stats(Entitiy *player) {
			char stats[1024];
			snprintf(stats, 1024, "STATS: Health %d", player->health);
			Text_Renderer_C(RENDERER, FONT, 15, HEIGHT - 100, strlen(stats) * FONT_W, 20, stats, WHITE);
			snprintf(stats, 1024, "STATS: Health %d", player->health);
			Text_Renderer_C(RENDERER, FONT, 15, HEIGHT - 100, strlen(stats) * FONT_W, FONT_W_MESSAGES, stats, WHITE);
			for(Damage_Types i = 0; i < DAMAGE_NUM; i++) {
				stats[0] = '\0';
				snprintf(stats, 1024, "STATS: %s att: %d def: %d", damageStr[i], player->attack[i], player->defence[i]);
				Text_Renderer_C(RENDERER, FONT, 15, HEIGHT - 100 + FONT_W_MESSAGES*(i+1), strlen(stats) * FONT_W,
				                FONT_W_MESSAGES, stats, WHITE);
				}

			}

		void render_monsters(Entitiy_DA *monsters, Entitiy *player) {

			i32 radius = player->radius;
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
				    && monsters->items[count].pos.y >= startY && monsters->items[count].pos.y <= stopY) {
					render_player(&monsters->items[count]);
					}

				}

			}

		void render_messages(i32 startX, i32 startY, char* message) {
			if(message != NULL) {
				u64 w_c = strlen(message) * FONT_W;
				Text_Renderer_C(RENDERER, FONT, startX, startY, (i32)w_c, 20, message, WHITE);
				}

			}


//static i32 asd = 0;
		void main_renderer(Entitiy* player, Entitiy_DA *monster, Tile *map) {
			SDL_ERR(SDL_RenderClear(RENDERER));
			render_map(map, player);
			render_player(player);
			//render_player(&monster->items[0]);
			render_monsters(monster, player);
			render_stats(player);
			i32 count = 1;
			for(i32 i = (i32)MESSAGES.count-1; i >= ((i32)MESSAGES.count - 5); i--) {
				render_messages((WIDTH - 600), (HEIGHT - 120 + FONT_W_MESSAGES*(count++)), MESSAGES.items[i]);
				}

			SDL_ERR(SDL_SetRenderDrawColor(RENDERER, 0X10, 0X10, 0X10, 0XFF));
			SDL_RenderPresent(RENDERER);
			}

		void event_user(Entitiy *player, Entitiy_DA *entitis, Tile* map) {
			MOVMENT = SDL_FALSE;
			while(MOVMENT == SDL_FALSE) {
				if(SDL_WaitEvent(&EVENT)) {
					if(EVENT.type == SDL_QUIT) {
						MOVMENT = SDL_TRUE;
						QUIT = 1;
						}
					else if(EVENT.type == SDL_WINDOWEVENT) {  //JUST FOR NOW
						MOVMENT = SDL_TRUE;
						SDL_GetWindowSize(WINDOW, &WIDTH, &HEIGHT);
						FONT_H = HEIGHT / MAP_Y - 1;
						FONT_W = WIDTH  / MAP_X;
						//FONT_H = 15;
						//FONT_W = 10;
						if(WIDTH > 4096 || HEIGHT > 2048) {
							ASSERT("Oversized window\n");
							}
						//LOG("width %d, height %d\n", WIDTH, HEIGHT);
						}
					else if(EVENT.type==SDL_KEYDOWN) {
						player_input(&EVENT, player, entitis, map);
						}
					}
				}
			}

		void genereate_monsters(Entitiy_DA *monsters, Tile *map) {
			for(i32 y = 0; y < MAP_Y; y++) {
				for(i32 x = 0; x < MAP_X; x++) {
					if(MAP_CH(map, x, y) != '#') {
						if(rand_f64() < 0.03f) {
							Entitiy *temp = create_entity('M', 5, 3, (Position) {
								.x = x, .y = y
								});
							da_append(monsters, *temp);
							}
						}
					}
				}
			LOG("\nGenerated monsters %d\n", (i32)monsters->count);
			}

//BLOCK MOVMENT DEPENDING ON TYPE OF MONSTERS
		void block_movement(Entitiy_DA *entitys, Tile *map) {
			for(u64 count = 0; count < entitys->count; count++) {
				if(entitys->items[count].ch == 'M') {
					i32 x = entitys->items[count].pos.x;
					i32 y = entitys->items[count].pos.y;
					MAP_ISW(map, x, y) = SDL_FALSE;
					}
				else if(entitys->items[count].ch == 'S') {
					i32 x = entitys->items[count].pos.x;
					i32 y = entitys->items[count].pos.y;
					MAP_ISW(map, x, y) = SDL_TRUE;
					}
				}
			}
//check if 2 fieled of vison colide
		SDL_bool check_colison_entitiy(Entitiy* player, Entitiy*  ent) {
			SDL_Rect A = {.h = player->radius, .w = player->radius, .x = player->pos.x, .y = player->pos.y };
			SDL_Rect B = {.h = ent->radius, .w = ent->radius, .x = ent->pos.x, .y = ent->pos.y };

			i32 leftA, leftB;
			i32 rightA, rightB;
			i32 topA, topB;
			i32 bottomA, bottomB;

			leftA = A.x;
			rightA = A.x + A.w;
			topA = A.y;
			bottomA = A.y + A.h;

			leftB = B.x;
			rightB = B.x + B.w;
			topB = B.y;
			bottomB = B.y + B.h;

			if( bottomA <= topB ) {
				return SDL_FALSE;
				}

			if( topA >= bottomB ) {
				return SDL_FALSE;
				}

			if( rightA <= leftB ) {
				return SDL_FALSE;
				}

			if( leftA >= rightB ) {
				return SDL_FALSE;
				}

			return SDL_TRUE;
			}


		f64 distnace_move(i32 x1, i32 y1, i32 x2, i32 y2, Tile *map) {
			CLAMP(x2, 0, MAP_X-1);
			CLAMP(y2, 0, MAP_Y-1);
			if(MAP_ISW(map, x2, y2) == SDL_TRUE) {
				f64 distance = DISTANCE(x1, y1, x2, y2);
				//if(distance == 0.0f){
				//	return INF;
				//}
				return distance;
				}
			//LOG("dist INF\n");
			//system("pause");
			return INF;
			}
//WE WILL SEE IF A* or Diakstra or This CRAP
		void make_best_move(Entitiy* player, Entitiy*  ent, Tile *map) {
			i32 x1 = player->pos.x;
			i32 y1 = player->pos.y;
			i32 x2 = ent->pos.x;
			i32 y2 = ent->pos.y;
			f64 distance  = DISTANCE(x1, y1, x2, y2);

			//MOVES WILL DEPEND OF WHAT MONSTER IS!!!

			//+1x
			f64 distancesMin = distnace_move(x1, y1, (x2 + 1), y2, map);
			i32 index = 0;
			//-1x
			distance = distnace_move(x1, y1, (x2 - 1), y2, map);
			if(distance < distancesMin) {
				distancesMin = distance;
				index = 1;
				}
			//+1y
			distance = distnace_move(x1, y1, x2, (y2 + 1), map);
			if(distance < distancesMin) {
				distancesMin = distance;
				index = 2;
				}
			//-1y
			distance = distnace_move(x1, y1, x2, (y2  - 1), map);
			if(distance < distancesMin) {
				distancesMin = distance;
				index = 3;
				}
			if(distancesMin == 0) {
				monster_attack(player, ent);
				return;
				}
			switch(index) {
				case 0: {
						if(distancesMin < INF && distancesMin != 0.0f) {
							//LOG("X++\n");
							ent->pos.x = ent->pos.x + 1;
							}
						break;
						}
				case 1: {
						if(distancesMin < INF && distancesMin != 0.0f) {
							//LOG("X--\n");
							ent->pos.x--;
							}
						break;
						}
				case 2: {
						if(distancesMin < INF && distancesMin != 0.0f) {
							//LOG("Y++\n");
							ent->pos.y++;
							}
						break;
						}
				case 3: {
						if(distancesMin < INF && distancesMin != 0.0f) {
							//LOG("Y--\n");
							ent->pos.y--;
							}
						break;
						}
				default: {
						ASSERT("Unreachable");
						break;
						}
				}




			//system("pause");
			}

//IF IN VISON FIELD MOVE TOWARDS PLAYER
//IF NOT RAND MOV DEPENDING ON TYPE OR PROB
		void move_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map) {
			i32 co = 0;
			DROP(co);
			for(u64 count = 0; count < entitys->count; count++) {
				Entitiy entity = entitys->items[count];
				if(entity.ch == 'M' && rand_f64() < 1.5f) {
					if(check_colison_entitiy(player, &entity) == SDL_TRUE) {
						//co++;
						MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_TRUE;
						make_best_move(player, &entity, map);
						MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_FALSE;
						entitys->items[count] = entity;
						}
					}
				}
			//LOG("Colided entitys %d\n", co);
			}

		void update_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map) {

			move_entity(player, entitys, map);
			block_movement(entitys, map);
			}


#define SEED 12344
#include<time.h>

		int main() {


			SDL_ERR(SDL_Init(SDL_INIT_VIDEO));
			SDL_ERR(TTF_Init());
			srand(time(0));
			WINDOW   = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1400, 800,  SDL_WINDOW_OPENGL);
			(void*)P_SDL_ERR(WINDOW);
			RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
			SDL_SetWindowResizable(WINDOW, SDL_TRUE);
			FONT = TTF_OpenFont(fontLoc, 128);
			(void*)P_SDL_ERR(FONT);
			(void*)P_SDL_ERR(RENDERER);
			QUIT = 0;
			MOVMENT = SDL_TRUE;
			Entitiy* player = create_entity('@', 5, 100, (Position) {
				10, 10
				});
				player->attack[0] = 9;
			Tile *map = init_map();
			Entitiy_DA monsters = {0};
			for(i32 i = 0; i < 5; i++) {
				da_append(&MESSAGES, "   ");
				}
			/*Entitiy *monster = create_player((Position) {
				15, 15
				});
			da_append(&monsters, *monster);
			//*/
			genereate_monsters(&monsters, map);
			MOVMENT = 0;
			main_renderer(player,  &monsters, map);
			//MAP_STDOUT();
			while(!QUIT) {
				main_renderer(player,  &monsters, map);
				event_user(player, &monsters, map);
				update_entity(player, &monsters, map);
				//for(u64 count = 0; count < MESSAGES.count; count++) {
				//	LOG("%s", MESSAGES.items[count]);
				//	}

				//system("pause");
				//SDL_Delay(1000);
				}

			return 0;
			ASSERT("UNREACHABLE");
			}
