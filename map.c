
#include "map.h"
SDL_bool checkCollision(i32 x1, i32 y1, i32 w1, i32 h1, i32 x2, i32 y2, i32 w2, i32 h2) {
	SDL_Rect A = {.h = h1, .w = w1, .x = x1, .y = y1 };
	SDL_Rect B = {.h = h2, .w = w2, .x = x2, .y = y2 };
	i16 leftA, leftB;
	i16 rightA, rightB;
	i16 topA, topB;
	i16 bottomA, bottomB;

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
	i32 stopY = room.pos.y + room.height;
//	CLAMP(stopY, 2, MAP_Y - 2);
	i32 stopX = room.pos.x + room.width;
//	CLAMP(stopX, 2, MAP_X - 2);
	for(i32 y = room.pos.y; y < stopY; y++) {
		for(i32 x = room.pos.x; x < stopX; x++) {
			MAP_CH(map, x, y)  = '.';
			MAP_ISW(map, x, y) = SDL_TRUE;
			}
		}
	}


void add_room_wall_rectangle(Tile *map, Room room) {
	i32 stopY = room.pos.y + room.height;
	CLAMP(stopY, 3, MAP_Y - 2);
	i32 stopX = room.pos.x + room.width;
	CLAMP(stopX, 3, MAP_X - 2);
	i32 startX = room.pos.x;
	CLAMP(startX, 3, MAP_Y - 2);
	i32 startY = room.pos.y - 1;
	CLAMP(startY, 3, MAP_Y - 2);
	for(i32 y = startY; y < stopY; y++) {
		for(i32 x = startX; x < stopX; x++) {
			if(y == room.pos.y && MAP_CH(map, x, y) != ','  &&  MAP_CH(map, x, y-1) != ',')  {
				if(MAP_CH(map, x, y) != ',') {
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
					}
				else {
					break;
					}
				}
			else if(x == room.pos.x && MAP_CH(map, x, y) != ',' && MAP_CH(map, x-1, y) != ',') {
				if(MAP_CH(map, x, y) != ',' && MAP_CH(map, x, y) != '+') {
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
					}
				else {
					break;
					}
				}
			else if(x == room.pos.x + room.width - 1 && MAP_CH(map, x+1, y) != ',') {
				if(MAP_CH(map, x, y) != ','  && MAP_CH(map, x, y) != '+') {
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
					}
				else {
					break;
					}
				}
			else if(y == room.pos.y + room.height - 1 && MAP_CH(map, x, y+1) != ',') {
				if(MAP_CH(map, x, y) != ','  && MAP_CH(map, x, y) != '+') {
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
					}
				else {
					break;
					}
				}
			}
		}
	}






void add_room_wall_circle(Tile *map, Room room) {
	i32 stopY = room.pos.y + room.height;
	CLAMP(stopY, 3, MAP_Y - 2);
	i32 stopX = room.pos.x + room.width;
	CLAMP(stopX, 3, MAP_X - 2);
	i32 startX = room.pos.x;
	CLAMP(startX, 3, MAP_Y - 2);
	i32 startY = room.pos.y - 1;
	CLAMP(startY, 3, MAP_Y - 2);
	f64 radius = DISTANCE(room.pos.x, room.pos.y, room.center.x, room.center.y)- 5;
	CLAMP(radius, 5.0f, INF);
	//LOG("STOP X %d STOP Y %d\n", stopX, stopY);
	for(i32 y = startY; y < stopY; y++) {
		for(i32 x = startX; x < stopX; x++) {
			f64 distance = (x - room.center.x)*(x - room.center.x) + (y - room.center.y)*(y - room.center.y);

			//LOG("Distance %f Radius %f\n", distance, radius);
			if(distance > (radius*radius)) {
				if(MAP_CH(map, x, y) != ','  && ((y-1) != 0) &&  MAP_CH(map, x, y-1) != ',')  {
					if(MAP_CH(map, x, y) != ',') {
						MAP_CH(map, x, y)  = '/';
						MAP_ISW(map, x, y) = SDL_FALSE;
						}
					else {
						break;
						}
					}
				else if(MAP_CH(map, x, y) != ',' && MAP_CH(map, x-1, y) != ',') {
					if(MAP_CH(map, x, y) != ',' && MAP_CH(map, x, y) != '+') {
						MAP_CH(map, x, y)  = '/';
						MAP_ISW(map, x, y) = SDL_FALSE;
						}
					else {
						break;
						}
					}
				else if(MAP_CH(map, x+1, y) != ',') {
					if(MAP_CH(map, x, y) != ','  && MAP_CH(map, x, y) != '+') {
						MAP_CH(map, x, y)  = '/';
						MAP_ISW(map, x,  y) = SDL_FALSE;
						}
					else {
						break;
						}
					}
				else if(MAP_CH(map, x, y+1) != ',') {
					if(MAP_CH(map, x, y) != ','  && MAP_CH(map, x, y) != '+') {
						MAP_CH(map, x, y)  = '/';
						MAP_ISW(map, x, y) = SDL_FALSE;
						}
					else {
						break;
						}
					}
				}

			}
		}
	}


void add_room_wall_blob(Tile *map, Room room) {
	i32 stopY = room.pos.y + room.height;
	CLAMP(stopY, 3, MAP_Y - 2);
	i32 stopX = room.pos.x + room.width;
	CLAMP(stopX, 3, MAP_X - 2);
	i32 startX = room.pos.x;
	CLAMP(startX, 3, MAP_Y - 2);
	i32 startY = room.pos.y - 1;
	CLAMP(startY, 3, MAP_Y - 2);
	for(i32 y = startY; y < stopY; y++) {
		for(i32 x = startX; x < stopX; x++) {
			if(MAP_CH(map, x, y) != ','  && ((y-1) != 0) &&   MAP_CH(map, x, y-1) != ',')  {
				if(MAP_CH(map, x, y) != ',') {
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
					}
				else {
					break;
					}
				}
			else if(MAP_CH(map, x, y) != ',' && MAP_CH(map, x-1, y) != ',') {
				if(MAP_CH(map, x, y) != ',' && MAP_CH(map, x, y) != '+') {
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
					}
				else {
					break;
					}
				}
			else if(MAP_CH(map, x+1, y) != ',') {
				if(MAP_CH(map, x, y) != ','  && MAP_CH(map, x, y) != '+') {
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
					}
				else {
					break;
					}
				}
			else if(MAP_CH(map, x, y+1) != ',') {
				if(MAP_CH(map, x, y) != ','  && MAP_CH(map, x, y) != '+') {
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
					}
				else {
					break;
					}
				}
			}
		}
	for(i32 y = room.pos.y; y < stopY; y++) {
		for(i32 x = room.pos.x; x < stopX; x++) {
			if(rand_f64() < 0.3f)
				caved_part(map, x, y);
			}
		}

	}

void add_room_drunkard_walk(Tile *map, Room room, i32 maxSteps) {
	i32 stopY = room.pos.y + room.height;
	CLAMP(stopY, 1, MAP_Y - 1);
	i32 stopX = room.pos.x + room.width;
	CLAMP(stopX, 1, MAP_X - 1);
	i32 startX = room.pos.x;
	CLAMP(startX, 1, MAP_Y - 1);
	i32 startY = room.pos.y - 1;
	CLAMP(startY, 1, MAP_Y - 1);
	SDL_bool isActive[4] = {SDL_TRUE, SDL_TRUE, SDL_TRUE, SDL_TRUE};
	Position direction[4] = {room.center, room.center, room.center, room.center};
	u8 countActive = 4;
	for(i32 y = startY; y < stopY; y++) {
		for(i32 x = startX; x < stopX; x++) {
			if(MAP_CH(map, x, y) != TILE_ROAD && MAP_CH(map, x, y) != TILE_RUINS) {
				MAP_CH(map, x, y) = TILE_WALL;
				MAP_ISW(map, x, y)  = SDL_FALSE;
				}

			}
		}

	for(i32 x = 0; x < maxSteps; x++) {
		for(u8 i = 0; i < 4; i++) {
			if(isActive[i] == SDL_TRUE) {
				u8 dir = rand()%4;
				switch(dir) {
					case 0: {
							direction[i].x+=1;
							break;
							}
					case 1: {
							direction[i].x+=-1;
							break;
							}
					case 2: {
							direction[i].y+=1;
							break;
							}
					case 3: {
							direction[i].y-=1;
							break;
							}

					}
				if(direction[i].x > startX && direction[i].x < stopX && direction[i].y > startY && direction[i].y < stopY) {
					MAP_CH(map, direction[i].x, direction[i].y) = '.';
					MAP_ISW(map, direction[i].x, direction[i].y) = SDL_TRUE;
					}
				else {
					countActive--;
					isActive[i] = SDL_FALSE;
					if(isActive == 0) {
						break;
						}
					}
				}
			}
		}

	}






void add_room_wall_to_map(Tile *map, Room room) {
	///if(rand()%5 == 0) add_room_wall_arbitray_shape(map, room);


	i32 chance = rand()%4;
	if(chance == 0) add_room_wall_rectangle(map, room);
	else if(chance == 1) add_room_wall_circle(map, room);
	//else if(chance == 2) add_room_drunkard_walk(map, room, 600);
	else {
		chance = rand()%3;
		if(chance == 0) {
			add_room_wall_rectangle(map, room);
			i32 c = rand()%2;
			if(c == 0) {
				MAP_CH(map, room.center.x-1, room.center.y-1) = '/';
				MAP_CH(map, room.center.x-1, room.center.y)   = '/';
				MAP_CH(map, room.center.x+1, room.center.y)   = '/';
				MAP_CH(map, room.center.x+1, room.center.y+1) = '/';
				}
			}
		else  add_room_wall_circle(map, room);
		//else                 add_room_wall_blob(map, room);
		}
	}

SDL_bool isDoor(Tile *map, Position pos) {
	i32 count = 0;
	i32 startX = pos.x - 1;
	i32 startY = pos.y - 1;
	i32 stopX = pos.x + 1;
	i32 stopY = pos.y + 1;
	CLAMP(startX, 0, MAP_X - 1);
	CLAMP(startY, 0, MAP_Y - 1);
	CLAMP(stopX, 0, MAP_X - 1);
	CLAMP(stopY, 0, MAP_Y - 1);
	for (i32 y = startY; y <= stopY; y++) {
		for (i32 x = startX; x <= stopX; x++) {
			if(MAP_CH(map, x, y) == '.') {
				count++;
				}
			else if(MAP_CH(map, x, y) == '1') {
				count = 0;
				break;
				}

			}
		}

	if(count == 4) {
		//LOG("ISdoor\n");
		return SDL_TRUE;
		}
	else
		return SDL_FALSE;
	}
void add_doors(Tile *map) {
	for (u64 y = 0; y < MAP_Y; y++) {
		for (u64 x = 0; x < MAP_X; x++) {
			Position temp = {.x = x, .y = y};
			if(isDoor(map, temp) == SDL_TRUE && rand_f64() < CHANCE_SPAWN_DOOR) {
				MAP_CH(map, x, y) = '1';
				MAP_ISW(map, x, y) = SDL_FALSE;
				}
			}
		}

	}

void connec_two_point_wall(Position centerOne, Position centerTwo, Tile* map) {
	Position temp;
	temp.x = centerOne.x;
	temp.y = centerOne.y;
	u8 countDoors = 0;
	while (1) {
		if (abs((temp.x - 1) - centerTwo.x) < abs(temp.x - centerTwo.x))
			temp.x--;
		else if (abs((temp.x + 1) - centerTwo.x) < abs(temp.x - centerTwo.x))
			temp.x++;
		else if (abs((temp.y + 1) - centerTwo.y) < abs(temp.y - centerTwo.y))
			temp.y++;
		else if (abs((temp.y - 1) - centerTwo.y) < abs(temp.y - centerTwo.y))
			temp.y--;
		else {
			break;
			}
		if(temp.x == 1 || temp.x == MAP_X-1 || temp.y == 1 || temp.y == MAP_Y - 1 ) {
			break;
			}
		MAP_CH(map, temp.x, temp.y) = '#';
		MAP_ISW(map, temp.x, temp.y) = SDL_FALSE;
		}

	}

void connect_room_centers(Position centerOne, Position centerTwo, Tile* map, SDL_bool isDoorDis) {
	Position temp;
	temp.x = centerOne.x;
	temp.y = centerOne.y;
	u8 countDoors = 0;
	while (1) {
		if (abs((temp.x - 1) - centerTwo.x) < abs(temp.x - centerTwo.x))
			temp.x--;
		else if (abs((temp.x + 1) - centerTwo.x) < abs(temp.x - centerTwo.x))
			temp.x++;
		else if (abs((temp.y + 1) - centerTwo.y) < abs(temp.y - centerTwo.y))
			temp.y++;
		else if (abs((temp.y - 1) - centerTwo.y) < abs(temp.y - centerTwo.y))
			temp.y--;
		else {
			break;
			}
		if(temp.x == 1 || temp.x == MAP_X-1 || temp.y == 1 || temp.y == MAP_Y - 1 ) {
			break;
			}
		if(MAP_CH(map, temp.x, temp.y) == '/' && isDoorDis == SDL_FALSE) {
			MAP_CH(map, temp.x, temp.y) = '+';
			MAP_ISW(map, temp.x, temp.y) = SDL_FALSE;
			//temp.x--;
			//temp.y--;
			countDoors++;
			if(countDoors == 10) {
				break;
				}
			}
		else {
			MAP_CH(map, temp.x, temp.y) = ',';
			MAP_ISW(map, temp.x, temp.y) = SDL_TRUE;
			}



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
		i32 xr = rand()%3 - 1 + x;
		i32 yr = rand()%3 - 1 + y;
		CLAMP(xr, 1, MAP_X - 1);
		CLAMP(yr, 1, MAP_Y - 1);
		MAP_CH(map, xr, yr) = ',';
		MAP_ISW(map, xr, yr) = SDL_TRUE;
		}
	}
void caved_map(Tile *map, f64 percantage) {

	for(i32 y = 2; y < MAP_Y - 2; y++) {
		for(i32 x = 2; x < MAP_X - 2; x++) {
			if(rand_f64() < percantage) {
				if((MAP_CH(map, x, y) == '.'|| (MAP_CH(map, x, y) == '/'))  && MAP_CH(map, x - 1, y) == '#') {
					MAP_CH(map, x - 1, y) = '.';
					MAP_ISW(map, x - 1, y) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			if(rand_f64() < percantage) {
				if((MAP_CH(map, x, y) == '.'|| (MAP_CH(map, x, y) == '/'))  && MAP_CH(map, x + 1, y) == '#') {
					MAP_CH(map, x + 1, y) = '.';
					MAP_ISW(map, x + 1, y) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			if(rand_f64() < percantage) {
				if((MAP_CH(map, x, y) == '.'|| (MAP_CH(map, x, y) == '/'))  && MAP_CH(map, x, y + 1) == '#') {
					MAP_CH(map, x, y + 1) = '.';
					MAP_ISW(map, x, y + 1) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			if(rand_f64() < percantage) {
				if((MAP_CH(map, x, y) == '.'|| (MAP_CH(map, x, y) == '/'))  && MAP_CH(map, x, y - 1) == '#') {
					MAP_CH(map, x, y - 1) = '.';
					MAP_ISW(map, x, y - 1) = SDL_TRUE;
					caved_part(map, x, y);
					}
				}
			//CAVED ROAD
			if(rand_f64() < CHANCE_CAVE_ROAD) {
				if(MAP_CH(map, x, y) == ',') {
					caved_part(map, x, y);
					}
				}
			}
		}
	}



void caved_part_generator(Tile_Type type, Tile *map, i32 maxDistanceD) {
	f64 chance = 0.0f;
	if(type == TILE_NON || maxDistanceD == 0) {
		return;
		}
	for(u64 y = 1; y < MAP_Y - 1; y++) {
		for(u64 x = 1; x < MAP_X - 1; x++) {
			if(MAP_DIJKSTRA(map, x, y) < (f64)maxDistanceD) {
				if(type == TILE_RUINS) {
					chance = 0.01f;
					//LOG("chance %f\n", chance);
					}
				else {

					chance = 1.0f / (f64)(MAP_DIJKSTRA(map, x, y) + 1.0f);
					}

				if(rand_f64() < chance && MAP_CH(map, x, y) != TILE_STAIRS) {
					//LOG("chance %f\n", chance);
					MAP_CH(map, x, y) = (char)type;
					if(type == TILE_RUINS) {
						MAP_ISW(map, x, y) = SDL_FALSE;
						}
					}
				}
			}
		}
	}




void generete_dungons(Room_DA* room, Tile *map, i32 minRooms, i32 maxRooms) {
	i32 width, height, x, y, nRooms;
	u8 isColided = SDL_FALSE;
	if(minRooms >= maxRooms) {
		ASSERT("We have a larger amount of minRooms >= maxRooms");
		}
	nRooms = (i32)(rand()%(u32)(maxRooms - minRooms)) + minRooms;
	//nRooms = 100;
	//nRooms = 5;
	Room *rooms = calloc(nRooms+200, sizeof(Room));
	rooms[0] = create_room(35, 35, 7, 7);
	add_room_to_map(map, rooms[0]);

	//add_room_wall_to_map(map, rooms[0]);
	i32 count = 0;
	for(i32 i = 1; i < nRooms; i++) {
		while(1) {
			isColided = SDL_FALSE;
			y = (rand() % (MAP_Y - 13));
			x = (rand() % (MAP_X - 13));
			height = (rand() % 15) + 5;
			width  = (rand() % 15) + 5;
			for(i32 j = 0; j < i; j++) {
				if(checkCollision(x, y, width, height,
				                  rooms[j].pos.x, rooms[j].pos.y, rooms[j].width, rooms[j].height)) {
					isColided = SDL_TRUE;
					break;
					}
				}
			if(isColided == SDL_TRUE && rand_f64() < 0.1f) {
				break;
				}
			if(count == 1000) {
				break;
				}
			count++;
			}
		if(count == 1000) {
			nRooms = i;
			break;
			}
		//rooms[i].center.x  = 0;
		rooms[i] = create_room(x, y, height, width);
		da_append(room, rooms[i]);
		add_room_to_map(map, rooms[i]);
		add_room_wall_to_map(map, rooms[i]);
		count = 0;
		//connect_room_centers(rooms[i-1].center, rooms[i].center, map, SDL_FALSE);
		}
	///*

	//*/
	//add_walls_around_roads(map);
	f64 percantage = 0.01f + rand_f64();
	LOG("percantage of caved map %f", percantage);

	for(i32 i = 0; i < nRooms; i++) {
		i32 minDistance = INF;
		i32 minIndex = i;
		for(i32 j = i + 1; j < nRooms-1; j++) {
			i32 distance = DISTANCE(rooms[j].center.x, rooms[j].center.y, rooms[i].center.x, rooms[i].center.y);
			if(distance < minDistance) {
				minDistance = distance;
				minIndex = j;
				}
			}
		connect_room_centers(rooms[i].center, rooms[minIndex].center, map, SDL_FALSE);
		}
	connect_room_centers(rooms[nRooms-1].center, rooms[0].center, map, SDL_FALSE);
	//if(rand()%2)
	//caved_map(map, percantage);
	//add_doors(map);

	for(i32 y = 0; y < MAP_Y; y++) {
		for(i32 x = 0; x < MAP_X; x++) {
			if(y == 0 || x == 0 || y == MAP_Y-1 || x == MAP_X-1 ) {
				MAP_CH(map, x, y) = TILE_WALL;
				MAP_ISW(map, x, y) = SDL_FALSE;

				}
			}
		}
	//free(rooms);

	}




#define RAND_MAP()\
	for(i32 y = 0; y < MAP_Y; y++) {\
		for(i32 x = 0; x < MAP_X; x++) {\
			f64 r = rand_f64();\
			if(r < 1.0f/100.0f) {\
				MAP_CH(map, x, y) = TILE_BLOCKED;\
				MAP_ISW(map, x, y) = SDL_FALSE;\
				}\
			else {\
				MAP_CH(map, x, y)= TILE_FLOOR;\
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

SDL_bool is_trap(Tile* map, i32 x, i32 y) {
	if(MAP_CH(map, x, y) == TILE_STUN_TRAP || MAP_CH(map, x, y) == TILE_RUINS_TRAP || MAP_CH(map, x, y) == TILE_SPIKE ||
	    MAP_CH(map, x, y) == TILE_POI_TRAP) {
		return SDL_TRUE;
		}
	return SDL_FALSE;
	}


Tile* init_map(Room_DA* rooms) {
	Tile *map;
	map = calloc(MAP_Y*MAP_Y + 1, sizeof(Tile));
	//RAND_MAP();
	for(i32 y = 0; y < MAP_Y * MAP_Y; y++) {
		map[y].ch = TILE_BLOCKED;
		}
	//memset(map. , TILE_FLOOR, sizeof(Tile) * MAP_Y * MAP_Y);
	if(map == NULL) {
		ASSERT("alloc of map failed!!!");
		}
	//RAND_MAP();
	i32 maxRoom = 12, minRoom = 10;
	generete_dungons(rooms, map, 200, 600);


	SDL_bool isGenerateDown  = SDL_FALSE;
	while(!isGenerateDown) {
		i32 x = rand()%(MAP_X - 4) + 2;
		i32 y = rand()%(MAP_X - 4) + 2;
		CLAMP(x, 4, MAP_X - 4);
		CLAMP(y, 4, MAP_Y - 4);
		if(MAP_CH(map, x, y) == TILE_FLOOR) {
			MAP_CH(map, x, y) = TILE_STAIRS;
			isGenerateDown = SDL_TRUE;
			break;
			}
		}
	//LEVEL 1

	return map;
	}


//BRAHEMSTHAN ALGOS

static void plot_4_points (i32 cx, i32 cy, i32 x, i32 y, Tile* map, char ch, SDL_bool isW) {
	MAP_CH(map, cx + x, cy + y) = ch;
	if (x != 0) {
		MAP_CH(map, cx - x, cy + y) = ch;
		MAP_ISW(map, cx - x, cy + y) = isW;
		}
	if (y != 0) {
		MAP_CH(map,cx + x, cy - y) = ch;
		MAP_ISW(map,cx + x, cy - y) = isW;
		}
	if (x != 0 && y != 0) {
		MAP_CH(map, cx - x, cy - y) = ch;
		MAP_ISW(map, cx - x, cy - y) = isW;
		}
	}
void plot_8_points (i32 cx, i32 cy, i32 x, i32 y, Tile* map, char ch) {
	plot_4_points (cx, cy, x, y, map,ch, 1);
	if (x != y) plot_4_points (cx, cy, y, x, map, ch,1);
	}

static void plot_circle (i32 xm, i32 ym, i32 r, Tile* map, char ch, SDL_bool isW) {
	i32 x = -r, y = 0, err = 2-2*r; /* II. Quadrant */
	do {
		MAP_CH(map, xm-x, ym+y) = ch; /*   I. Quadrant */
		MAP_CH(map, xm-y, ym-x) = ch; /*  II. Quadrant */
		MAP_CH(map, xm+x, ym-y) = ch; /* III. Quadrant */
		MAP_CH(map, xm+y, ym+x) = ch; /*  IV. Quadrant */

		MAP_ISW(map, xm-x, ym+y) = isW; /*   I. Quadrant */
		MAP_ISW(map, xm-y, ym-x) = isW; /*  II. Quadrant */
		MAP_ISW(map, xm+x, ym-y) = isW; /* III. Quadrant */
		MAP_ISW(map, xm+y, ym+x) = isW; /*  IV. Quadrant */
		r = err;
		if (r >=  x) err += ++x*2+1; /* e_xy+e_x > 0 */
		if (r <= y) err += ++y*2+1; /* e_xy+e_y < 0 */
		}
	while (x < 0);
	}

static void plot_fill_circle(i32 xm, i32 ym, i32 r, Tile* map, char ch, SDL_bool isW) {
	for(i32 i = 0; i < r; i++) {
		plot_circle(xm, ym, i, map, ch, isW);
		}
	}
static void plot_line (i32 x0, i32 y0, i32 x1, i32 y1,Tile* map,  char ch, SDL_bool isW) {
	i32 dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
	i32 dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1;
	i32 err = dx + dy, e2; /* error value e_xy */

	for (;;) { /* loop */
		MAP_CH(map, x0, y0) = ch;
		MAP_ISW(map, x0, y0) = isW;
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;  /* e_xy+e_x > 0 */
			x0 += sx;
			}
		if (e2 <= dx) {
			err += dx;  /* e_xy+e_y < 0 */
			y0 += sy;
			}
		}
	}

void plot_ellipse_rect (i32 x0, i32 y0, i32 x1, i32 y1, Tile* map, char ch) {
	i32 a = abs (x1 - x0), b = abs (y1 - y0), b1 = b & 1; /* values of diameter */
	i64 dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
	i64 err = dx + dy + b1 * a * a, e2; /* error of 1.step */

	if (x0 > x1) {
		x0 = x1;  /* if called with swapped points */
		x1 += a;
		}
	if (y0 > y1) y0 = y1; /* .. exchange them */
	y0 += (b + 1) / 2;
	y1 = y0-b1;   /* starting pixel */
	a *= 8 * a;
	b1 = 8 * b * b;
	do {
		MAP_CH(map, x1, y0) = ch; /*   I. Quadrant */
		MAP_CH(map, x0, y0) = ch; /*  II. Quadrant */
		MAP_CH(map, x0, y1) = ch; /* III. Quadrant */
		MAP_CH(map, x1, y1) = ch; /*  IV. Quadrant */
		e2 = 2 * err;
		if (e2 >= dx) {
			x0++;
			x1--;
			err += dx += b1;
			} /* x step */
		if (e2 <= dy) {
			y0++;
			y1--;
			err += dy += a;
			}  /* y step */
		}
	while (x0 <= x1);
	while (y0-y1 < b) {
		/* too early stop of flat ellipses a=1 */
		MAP_CH(map, x0-1, y0) = ch; /* -> finish tip of ellipse */
		MAP_CH(map, x1+1, y0++) = ch;
		MAP_CH(map, x0-1, y1) = ch;
		MAP_CH(map, x1+1, y1--) = ch;
		}
	}

void plot_basic_bezier (i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, Tile* map, char ch) {
	int sx = x0 < x2 ? 1 : -1;
	int sy = y0 < y2 ? 1 : -1; /* step direction */
	int cur = sx * sy *((x0 - x1) * (y2 - y1) - (x2 - x1) * (y0 - y1)); /* curvature */
	int x = x0 - 2 * x1 + x2, y = y0 - 2 * y1 +y2, xy = 2 * x * y * sx * sy;
	/* compute error increments of P0 */
	long dx = (1 - 2 * abs (x0 - x1)) * y * y + abs (y0 - y1) * xy - 2 * cur * abs (y0 - y2);
	long dy = (1 - 2 * abs (y0 - y1)) * x * x + abs (x0 - x1) * xy + 2 * cur * abs (x0 - x2);
	/* compute error increments of P2 */
	long ex = (1 - 2 * abs (x2 - x1)) * y * y + abs (y2 - y1) * xy + 2 * cur * abs (y0 - y2);
	long ey = (1 - 2 * abs (y2 - y1)) * x * x + abs (x2 - x1) * xy - 2 * cur * abs (x0 - x2);
	/* sign of gradient must not change */
	//assert ((x0 - x1) * (x2 - x1) <= 0 && (y0 - y1) * (y2 - y1) <= 0);
	if (cur == 0) {
		/* straight line */
		plot_line(x0, y0, x2, y2, map, ch, 0);
		return;
		}
	x *= 2 * x;
	y *= 2 * y;
	if (cur < 0) {
		/* negated curvature */
		x = -x;
		dx = -dx;
		ex = -ex;
		xy = -xy;
		y = -y;
		dy = -dy;
		ey = -ey;
		}
	/* algorithm fails for almost straight line, check error values */
	if (dx >= -y || dy <= -x || ex <= -y || ey >= -x) {
		plot_line(x0, y0, x1, y1, map, ch, 1); /* simple approximation */
		plot_line(x1, y1, x2, y2, map, ch, 1);
		return;
		}
	dx -= xy;
	ex = dx + dy;
	dy -= xy; /* error of 1.step */
	for (;;) {
		/* plot curve */

		MAP_CH(map, x0, y0) = ch;
		ey = 2 * ex - dy; /* save value for test of y step */
		if (2 * ex >= dx) {
			/* x step */
			if (x0 == x2) break;
			x0 += sx;
			dy -= xy;
			ex += dx += y;
			}
		if (ey <= 0) {
			/* y step */
			if (y0 == y2) break;
			y0 += sy;
			dx -= xy;
			ex += dy += x;
			}
		}
	}




Tile* init_map_RA(Room_DA* room) {
	Tile *map;
	map = calloc(MAP_Y*MAP_Y + 1, sizeof(Tile));
	//RAND_MAP();
	for(i32 y = 0; y < MAP_Y * MAP_Y; y++) {
		map[y].ch = TILE_BLOCKED;
		}
	//memset(map. , TILE_FLOOR, sizeof(Tile) * MAP_Y * MAP_Y);
	if(map == NULL) {
		ASSERT("alloc of map failed!!!");
		}
	i32 nRooms = 10;
	//nRooms = 100;
	//nRooms = 5;
	Room *rooms = calloc(nRooms+200, sizeof(Room));
	rooms[0] = create_room(35, 35, 7, 7);
	add_room_to_map(map, rooms[0]);
	da_append(room, rooms[0]);

	//plot_fill_circle(rooms[0].center.x, rooms[0].center.y, 10, map, '.');
	//add_room_wall_to_map(map, rooms[0]);
	//SDL_bool isColided = SDL_FALSE;
	i32 count = 0;
	u8 isDrunc = rand()%2;
	if(isDrunc) {
		u8 howStart = rand()%3 + 1;
		Room allMap = create_room(0, 0, MAP_Y - 1, MAP_X - 1);
		add_room_drunkard_walk(map, allMap, 20000);
		da_append(room, allMap);

		}


	for(i32 i = 1; i < nRooms && !isDrunc; i++) {

		i32 y = (rand() % (MAP_Y - 25));
		i32 x = (rand() % (MAP_X - 25));
		i32 height = (rand() % 15) + 5;
		i32 width  = (rand() % 15) + 5;

		i32 r = rand()%7 + 5;
		rooms[i] = create_room(x, y, width, height);

		i32 chance = rand()%3;
		//CIRCLE
		if(chance == 0) {
			plot_fill_circle(rooms[i].center.x, rooms[i].center.y, r, map, '.', SDL_TRUE);

			//plot_circle(rooms[i].center.x, rooms[i].center.y, r, map, TILE_WALL);
			//plot_circle(rooms[i].center.x, rooms[i].center.y, r+1, map, TILE_WALL);
			}
		//REC
		if(chance == 1) {
			add_room_to_map(map, rooms[i]);
			//add_room_wall_rectangle(map, rooms[i]);
			}
		//4 FILL
		if(chance == 2) {
			i32 move = rand()%5;
			plot_fill_circle(rooms[i].center.x + move, rooms[i].center.y + move, 5, map, '.', SDL_TRUE);
			plot_fill_circle(rooms[i].center.x - move, rooms[i].center.y + move, 5, map, '.', SDL_TRUE);
			plot_fill_circle(rooms[i].center.x + move, rooms[i].center.y - move, 5, map, '.', SDL_TRUE);
			plot_fill_circle(rooms[i].center.x - move, rooms[i].center.y - move, 5, map, '.', SDL_TRUE);
			}
		//CIRCLE
		if(chance == 3) {
			add_room_to_map(map, rooms[i]);
			add_room_wall_circle(map, rooms[i]);
			}
		//ROAD
		//plot_line(rooms[i].center.x, rooms[i].center.y, rooms[i-1].center.x, rooms[i-1].center.y, map, ',', SDL_TRUE);
		//plot_line(rooms[i-1].center.x-1, rooms[i-1].center.y, rooms[i].center.x, rooms[i].center.y, map, ',', SDL_TRUE);
		da_append(room, rooms[i]);
		}

	for(i32 i = 0; i < nRooms; i++) {
		i32 minDistance = INF;
		i32 minIndex = i;
		for(i32 j = i + 1; j < nRooms-1; j++) {
			i32 distance = DISTANCE(rooms[j].center.x, rooms[j].center.y, rooms[i].center.x, rooms[i].center.y);
			if(distance < minDistance) {
				minDistance = distance;
				minIndex = j;
				}
			}
		if(rand()%2) {
			plot_line(rooms[i].center.x, rooms[i].center.y, rooms[minIndex].center.x, rooms[minIndex].center.y, map, ',', SDL_TRUE);
			plot_line(rooms[minIndex].center.x, rooms[minIndex].center.y, rooms[i].center.x, rooms[i].center.y, map, ',', SDL_TRUE);
			}
		else {
			connect_room_centers(rooms[i].center, rooms[minIndex].center, map, SDL_FALSE);
			}

		}
	connect_room_centers(rooms[0].center, rooms[nRooms - 1].center, map, SDL_FALSE);

	for(i32 y = 0; y < MAP_Y; y++) {
		for(i32 x = 0; x < MAP_X; x++) {
			if(y == 0 || x == 0 || y == MAP_Y-1 || x == MAP_X-1 ) {
				MAP_CH(map, x, y) = TILE_WALL;
				MAP_ISW(map, x, y) = SDL_FALSE;

				}
			}
		}
	//DOWN
	SDL_bool isGenerateDown  = SDL_FALSE;
	while(!isGenerateDown) {
		i32 x = rand()%(MAP_X - 4) + 2;
		i32 y = rand()%(MAP_X - 4) + 2;
		CLAMP(x, 4, MAP_X - 4);
		CLAMP(y, 4, MAP_Y - 4);
		if(MAP_CH(map, x, y) == TILE_FLOOR) {
			MAP_CH(map, x, y) = TILE_STAIRS;
			isGenerateDown = SDL_TRUE;
			break;
			}
		}
	//LOG("Ma");
	return map;

	}

/* bsp_generator.c
 */



// Create a new BSP node
static BSPNode* bsp_create_node(i32 x, i32 y, i32 width, i32 height) {
	BSPNode* node = (BSPNode*)malloc(sizeof(BSPNode));
	node->x = x;
	node->y = y;
	node->width = width;
	node->height = height;
	node->left = node->right = NULL;
	node->room = create_room(x, y, height, width);
	return node;
	}

// Recursively split a node
static SDL_bool bsp_split(BSPNode* node) {
	if (!node) return SDL_FALSE;
	if (node->left || node->right) return SDL_FALSE; // already split

	// Decide split orientation
	SDL_bool splitH = rand() & 1;
	if (node->width > node->height && node->width / node->height >= 1.25) splitH = SDL_FALSE;
	else if (node->height > node->width && node->height / node->width >= 1.25) splitH = SDL_TRUE;

	i32 max = (splitH ? node->height : node->width) - MIN_LEAF_SIZE;
	if (max <= MIN_LEAF_SIZE) return SDL_FALSE;

	i32 split = (rand() % (max - MIN_LEAF_SIZE)) + MIN_LEAF_SIZE;

	if (splitH) {
		node->left = bsp_create_node(node->x, node->y, node->width, split);
		node->right = bsp_create_node(node->x, node->y + split, node->width, node->height - split);
		}
	else {
		node->left = bsp_create_node(node->x, node->y, split, node->height);
		node->right = bsp_create_node(node->x + split, node->y, node->width - split, node->height);
		}
	return SDL_TRUE;
	}

// Traverse leaves, splitting until desired depth/count
static void bsp_generate(BSPNode* root, int depth) {
	if (depth <= 0) return;
	if (bsp_split(root)) {
		bsp_generate(root->left, depth - 1);
		bsp_generate(root->right, depth - 1);
		}
	}

// Add rooms from leaf nodes to map and connect them
static void bsp_carve(Room_DA* rooms, BSPNode* node, Tile* map) {
	if (!node) return;

	// Leaf: create and carve a room
	if (!node->left && !node->right) {
		i32 rw = (rand() % (node->width - 6)) + 6;
		i32 rh = (rand() % (node->height - 6)) + 6;
		i32 rx = node->x + rand() % (node->width - rw);
		i32 ry = node->y + rand() % (node->height - rh);
		node->room = create_room(rx, ry, rh, rw);
		//node->hasRoom = SDL_TRUE;
		add_room_to_map(map, node->room);
		add_room_wall_rectangle(map, node->room);
		da_append(rooms, node->room);
		}
	else {
		// Internal: carve children and then connect their rooms
		bsp_carve(rooms, node->left, map);
		bsp_carve(rooms, node->right, map);

		}
	}

// Free BSP tree
static void bsp_free(BSPNode* node) {
	if (!node) return;
	bsp_free(node->left);
	bsp_free(node->right);
	free(node);
	}

// Public API: generate BSP dungeon
Tile* init_map_BSP(Room_DA* rooms, int splitDepth) {
	//Room_DA rooms;
	//da_init(&rooms);
	Tile*	map = calloc(MAP_Y*MAP_Y + 1, sizeof(Tile));
	for(i32 y = 0; y < MAP_Y * MAP_Y; y++) {
		map[y].ch = TILE_BLOCKED;
		}

	BSPNode* root = bsp_create_node(1, 1, MAP_X - 2, MAP_Y - 2);

	bsp_generate(root, splitDepth);
	bsp_carve(rooms, root, map);
	//add_room_wall_rectangle(map, root->room);
	//add_walls_around_roads(map);
	//add_doors(map);
	//ROADS
	for(u64 i = 0; i < rooms->count; i++) {
		i32 minDistance = INF;
		i32 minIndex = i;
		for(u64 j = i + 1; j < rooms->count; j++) {
			i32 distance = DISTANCE(rooms->items[j].center.x, rooms->items[j].center.y, rooms->items[i].center.x, rooms->items[i].center.y);
			if(distance < minDistance) {
				minDistance = distance;
				minIndex = j;
				}
			}
		if(rand()%2) {
			plot_line(rooms->items[i].center.x, rooms->items[i].center.y, rooms->items[minIndex].center.x, rooms->items[minIndex].center.y, map, ',', SDL_TRUE);
			plot_line(rooms->items[minIndex].center.x, rooms->items[minIndex].center.y, rooms->items[i].center.x, rooms->items[i].center.y, map, ',', SDL_TRUE);
			}
		else {
			connect_room_centers(rooms->items[i].center, rooms->items[minIndex].center, map, SDL_FALSE);
			}

		}
	//connect_room_centers(rooms->items[0].center, rooms->items[nRooms - 1].center, map, SDL_FALSE);

	for(i32 y = 0; y < MAP_Y; y++) {
		for(i32 x = 0; x < MAP_X; x++) {
			if(y == 0 || x == 0 || y == MAP_Y-1 || x == MAP_X-1 ) {
				MAP_CH(map, x, y) = TILE_WALL;
				MAP_ISW(map, x, y) = SDL_FALSE;

				}
			}
		}
	//DOWN
	SDL_bool isGenerateDown  = SDL_FALSE;
	while(!isGenerateDown) {
		i32 x = rand()%(MAP_X - 4) + 2;
		i32 y = rand()%(MAP_X - 4) + 2;
		CLAMP(x, 4, MAP_X - 4);
		CLAMP(y, 4, MAP_Y - 4);
		if(MAP_CH(map, x, y) == TILE_FLOOR) {
			MAP_CH(map, x, y) = TILE_STAIRS;
			isGenerateDown = SDL_TRUE;
			break;
			}
		}




	bsp_free(root);

	return map;
	}

void  generate_traps(Tile* map) {

	i32 whatTile = rand()%(TILE_RUINS_TRAP - TILE_STUN_TRAP);
	//LOG("what tile %d\n", whatTile);
	//CLAMP(whatTile, 0, TILE_RUINS_TRAP - TILE_STUN_TRAP());
	u8 isGen = 0;
	while(!isGen) {
		i32 x = rand()%(MAP_X - 1);
		i32 y = rand()%(MAP_Y - 1);
		if(MAP_CH(map, x, y) == TILE_FLOOR) {
			isGen = 1;
			switch(whatTile) {
				case 0: {
						MAP_CH(map, x, y) = TILE_STUN_TRAP;
						break;
						}
				case 1: {
						MAP_CH(map, x, y) = TILE_RUINS_TRAP;
						break;
						}
				case 2: {
						MAP_CH(map, x, y) = TILE_SPIKE;
						break;
						}
				case 3:{
					MAP_CH(map, x, y)   = TILE_POI_TRAP; 
					break;
				}		
				default: {
						ASSERT("Not defined trap");
						break;
						}
				}
			}
		}

	}
