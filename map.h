#ifndef _MAP_H
#define _MAP_H

#include "utils.h"



typedef struct {
	char ch;
	SDL_Color color;
	u8   isW;
	u8   isV;
	} Tile;
	
typedef struct {
	i32 width;
	i32 height;
	Position pos;
	Position center;
	} Room;
	
#define MAP(map, x, y)      map[((x) % MAP_X) + ((y) % MAP_Y) * MAP_X]
#define MAP_CH(map, x, y)   map[((x) % MAP_X) + ((y) % MAP_Y) * MAP_X].ch
#define MAP_ISW(map, x, y)  map[((x) % MAP_X) + ((y) % MAP_Y) * MAP_X].isW
#define MAP_ISV(map, x, y)  map[((x) % MAP_X) + ((y) % MAP_Y) * MAP_X].isV

#define CLEAR_VISON_FIELD(map){for(u64 i = 0; i < MAP_X * MAP_Y; i++){map[i].isV = SDL_FALSE;}};




Room create_room(i32 x, i32 y, i32 height, i32 width);
void add_room_to_map(Tile *map, Room room);
void add_room_wall_to_map(Tile *map, Room room);
void connect_room_centers(Position centerOne, Position centerTwo, Tile* map);
void add_walls_around_roads(Tile* map);
void caved_part(Tile *map, i32 x, i32 y);
void caved_map(Tile *map, f64 percantage);
void generete_dungons(Tile *map, i32 minRooms, i32 maxRooms);

Tile* init_map();


#endif
