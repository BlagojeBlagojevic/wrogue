#ifndef _UTILS_H
#define _UTILS_H
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
#define WHITE      (SDL_Color){255, 255, 255, 0}
#define RED        (SDL_Color){255, 0, 0, 0}
#define GREEN      (SDL_Color){0, 255, 0, 0}
#define BLUE       (SDL_Color){30, 0, 255, 0}
#define YELLOW     (SDL_Color){255, 255, 0, 0}
#define BLACK      (SDL_Color){0, 0, 0, 0}
#define UNDE_COL   (SDL_Color){252, 3, 177, 0}
#define BLIGHT_COL (SDL_Color){32, 43, 34, 0}
//TTF
#include<SDL2/SDL_ttf.h>
//static const char* fontLoc = "assets/fonts/f.ttf";
//#define fontLoc "assets/fonts/f.ttf"
#define fontLoc "assets/fonts/w.ttf"
//ARROW KEYS
#define KEY_Q 113
#define KEY_W 119
#define KEY_E 101

#define KEY_A 97
#define KEY_S 115
#define KEY_D 100

#define KEY_Z 122
#define KEY_Y 121
#define KEY_X 120
#define KEY_C 99

#define LEFT_ARROW 1073741904
#define RIGHT_ARROW 1073741903
#define UP_ARROW 1073741906
#define DOWN_ARROW 1073741905
#define SPACE 32
#define KEY_I 105
#define KEY_P 112
#define KEY_O 111

#define KEY_U 117

#define KEY_BACKSPACE 8
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




//ERROR AND LOG HANDLING

#define WARNING(...)        fprintf(stdout, __VA_ARGS__)
#define ERROR_BREAK(...)    fprintf(stderr, __VA_ARGS__); exit(-1)
#define LOG(...)     			  fprintf(stdout, __VA_ARGS__)
#define PAUSE()           {char a; fputs(a, stdin);}
#define CLAMP(X, LOW, HIGH) {if((X) < (LOW)) (X) = (LOW); if((X) > (HIGH)) (X) = (HIGH);}
#define ASSERT(msg) {fprintf(stderr, "aseert in:\n\tFILE %s\n\tLINE %d\n\tmsg: %s" , __FILE__, __LINE__, msg); exit(-1);}
#define DROP(var) {(void)var;}

#define DA_SIZE 16
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


#define da_remove_unordered(da, i)               					   \
do {                                             					   \
	size_t j = (i);                              					   \
	if(j > (da)->count){ASSERT("Not that amount of elements in da");}  \
	(da)->items[j] = (da)->items[--(da)->count]; 					   \
} while(0)		

#define da_remove_last(da) do{ if((da)->count > 0){ (da)->count--; } }while(0)






typedef struct {
	u64 	count;
	u64 	capacity;
	char** items;
	} Str;

typedef struct{
	u64 	count;
	u64 	capacity;
	i32* items;	
}Num;

#define MAP_X 80
#define MAP_Y 80

typedef struct {
	i32 x;
	i32 y;
	} Position;


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
	SDL_Rect      camera;
	Str           messages;
	Num           inputBuffer;
	SDL_bool      useItem;
	SDL_bool      isRenderItemsOnMap;
	SDL_bool      isPickingItem;
	SDL_bool      isOpeningDoor;
	SDL_bool      isEquItem;
	u64           countMoves;
	u8            depth;
	u8            levelPlayer;
	u8            isTimeToGenerateMap;
	} Graphics_State;

//static const char* title = "Ime kakvo";
#define title "Ime kakvo"

#define FONT_H_MESSAGES 20
#define MAX_NAME 30
#define MAX_DESCRIPTION 200

extern Graphics_State mainGraphics;
#define WINDOW     mainGraphics.window
#define RENDERER   mainGraphics.renderer
#define FONT       mainGraphics.font
#define FONT_W     mainGraphics.fontW
#define FONT_H     mainGraphics.fontH
#define QUIT       mainGraphics.isQuit
#define EVENT      mainGraphics.event
#define WIDTH      mainGraphics.width
#define HEIGHT     mainGraphics.height
#define MOVMENT    mainGraphics.isMovmentEvent
#define CAMERA     mainGraphics.camera
#define MESSAGES   mainGraphics.messages
#define USEITEM    mainGraphics.useItem
#define BUFFER     mainGraphics.inputBuffer
#define ITEMSREND  mainGraphics.isRenderItemsOnMap
#define PICKITEM   mainGraphics.isPickingItem
#define OPENDOOR   mainGraphics.isOpeningDoor
#define COUNTMOVES mainGraphics.countMoves
#define EQUITEM    mainGraphics.isEquItem
#define DEPTH      mainGraphics.depth
#define LEVEL      mainGraphics.levelPlayer
#define GENMAP     mainGraphics.isTimeToGenerateMap



#define INF (f64)100000.0f
#define ZERO 0.0f
#define DISTANCE(x1, y1, x2, y2)    sqrt((f64)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)))
#define DISTANCE_DIKSTRA(x1, y1, x2, y2) sqrt((f64)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)))
#define DISTANCE_RANGE_ATTACK_MAX 3.0f
#define DISTANCE_RANGE_ATTACK_MIN 1.9f

#define PERCANTAGE_RUN_CHANCE 0.3f
#define PERCANTAGE_CROW_RUN_CHANCE 0.5f
#define PERCENTAGE_MONSTER_GENERATED 0.005f
#define MAX_STOP_RUN_DISTANCE 10.0f
#define MAX_CURAGE_CHANCE 0.2f

#define MAX_HEALTH_VALUE 255
#define NUM_RENDER_MSG 5
#define RADIUS 10
#define CHANCE_INCREMENT_HEALTH 0.05f
#define CHANCE_SPAWN_DOOR 0.1f
#define CHANCE_NON_IN_ROOM 0.1f
#define PERCANTAGE_DISABLE_DOOR 0.50f
#define CHANCE_NON_CLEAR_RUINS 0.5f
#define CHANCE_DMG_CLEAR_RUINS 0.1f
#define CHANCE_CHANGE_DIRECTION 0.05f
#define CHANCE_CAVE_ROAD 0.05f
#define CHANCE_SPIRIT_ATTACK 0.01f
#define CHANCE_MONSTER_HUTING_WOUND 0.3

#define CHANCE_LIFESTEAL 0.1f
#define CHANCE_CRIT      0.1f
#define CHANCE_DROP_ITEM 0.1f
#define CHANCE_DROP_ITEM_CONS 0.7
#define CHANCE_ITEM_CURSED 0.06f
#define CHANCE_ITEM_USED_IN_COMBAT 0.80f
#define CHANCE_USE_DEF 0.8f
#define CHANCE_NEGATIVE_DAMAGE 0.05f
#define CHANCE_ITEM_PER_LEVEL 0.02f
#define CHANCE_DROP_SELING_CAUSE_DAMAGE 0.8f
#define CHANCE_MONSTER_HAVE_ITEM 0.05f
#define CHANCE_CRITICAL_PLAYER 0.05f
#define CHANCE_PLAYER_LEVEL  0.05f
#define CHANCE_DISIPATE_POISON 0.1f
#define CHANCE_LEVEL_AD   0.05f

//BSP STUFF
#define MIN_ROOM_SIZE 6
#define MAX_ROOM_SIZE 16
#define MAX_SPLIT_RATIO 0.9f
#define CORRIDOR_WIDTH 2

#endif
