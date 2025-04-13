
#include "map.h"

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
		height = (rand() % 10) + 7;
		width  = (rand() % 10) + 7;
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
	generete_dungons(map, 10, 20);
	return map;
	}






