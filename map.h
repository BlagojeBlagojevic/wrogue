#ifndef _MAP_H
#define _MAP_H

#include "utils.h"


typedef enum {
	TILE_BLOCKED    = '#',
	TILE_FLOOR      = '.',
	TILE_ROAD       = ',',
	TILE_WALL       = '/',
	TILE_RUINS      = '+',
	TILE_STAIRS     = '<',
	TILE_BLIGHT     = ':',
	TILE_TREE       = ';',
	TILE_GRASS      = 'G',
	TILE_POISION    = 'P',
	TILE_GARG_STAT  = 'S',
	TILE_REPEL      = 'E',

	
//DO NOT CHANGE ORDER
	TILE_STUN_TRAP  = 'T',
  TILE_SPIKE      = 'X',
	TILE_POI_TRAP   = 'p', 	
	TILE_RUINS_TRAP = 'R',
//DO NOT CHANGE ORDER
	
	TILE_NON,

	TILE_NUM
	} Tile_Type;



typedef struct {
	char ch;
	SDL_Color color;
	u8   isW;
	u8   isV;
	u8   visited;
	u8   counter;
	f64  distance;
	} Tile;

typedef struct {
	i32 width;
	i32 height;
	Position pos;
	Position center;
	} Room;

typedef struct {
	Room *items;
	u64 capacity;
	u64 count;
	} Room_DA;





#define MAP(map, x, y)      	  map[((x) % (MAP_X - 1)) + ((y) % (MAP_Y - 1)) * MAP_X]
#define MAP_CH(map, x, y)   	  map[((x) % (MAP_X - 1)) + ((y) % (MAP_Y - 1)) * MAP_X].ch
#define MAP_ISW(map, x, y)  	  map[((x) % (MAP_X - 1)) + ((y) % (MAP_Y - 1)) * MAP_X].isW
#define MAP_ISV(map, x, y)  	  map[((x) % (MAP_X - 1)) + ((y) % (MAP_Y - 1)) * MAP_X].isV
#define MAP_VISITED(map, x, y)  map[((x) % (MAP_X - 1)) + ((y) % (MAP_Y - 1)) * MAP_X].visited
#define MAP_DIJKSTRA(map, x, y) map[((x) % (MAP_X - 1)) + ((y) % (MAP_Y - 1)) * MAP_X].distance
#define MAP_COUNTER(map, y, x)	map[((x) % (MAP_X - 1)) + ((y) % (MAP_Y - 1)) * MAP_X].counter

#define CLEAR_VISON_FIELD(map){for(u64 i = 0; i < MAP_X * MAP_Y; i++){map[i].isV = SDL_FALSE;}};



SDL_bool checkCollision(i32 x1, i32 y1, i32 w1, i32 h1, i32 x2, i32 y2, i32 w2, i32 h2);
Room create_room(i32 x, i32 y, i32 height, i32 width);
void add_room_to_map(Tile *map, Room room);
void add_room_wall_rectangle(Tile *map, Room room);
void add_room_wall_circle(Tile *map, Room room);
void add_room_wall_blob(Tile *map, Room room);
void add_room_wall_to_map(Tile *map, Room room);

SDL_bool isDoor(Tile *map, Position pos);
void add_doors(Tile *map);
void connect_room_centers(Position centerOne, Position centerTwo, Tile* map, SDL_bool isDoorDis);
void add_walls_around_roads(Tile* map);
void caved_part(Tile *map, i32 x, i32 y);
void caved_map(Tile *map, f64 percantage);
void generete_dungons(Room_DA* room, Tile *map, i32 minRooms, i32 maxRooms);
void caved_part_generator(Tile_Type type, Tile *map, i32 maxDistanceD);

SDL_bool is_trap(Tile* map, i32 x, i32 y);

Tile* init_map(Room_DA* rooms);
Tile* init_map_RA(Room_DA* rooms);
Tile* init_map_BSP(Room_DA* rooms,  int splitDepth);
void  generate_traps(Tile* map);

#define MIN_LEAF_SIZE 10
#define MAX_LEAF_SIZE 40

typedef struct BSPNode {
	// region bounds
	i32 x, y, width, height;
	struct BSPNode *left;
	struct BSPNode *right;
	Room room;
	} BSPNode;



//CUSTOM ROOM TYPES


#endif
