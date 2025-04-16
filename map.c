
#include "map.h"
SDL_bool checkCollision(i32 x1, i32 y1, i32 w1, i32 h1, i32 x2, i32 y2, i32 w2, i32 h2){
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

    if( bottomA <= topB )
    {
        return SDL_FALSE;
    }

    if( topA >= bottomB )
    {
        return SDL_FALSE;
    }

    if( rightA <= leftB )
    {
        return SDL_FALSE;
    }

    if( leftA >= rightB )
    {
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
	CLAMP(stopY, 2, MAP_Y - 3);
	i32 stopX = room.pos.x + room.width;
	CLAMP(stopX, 2, MAP_X - 3);
	for(i32 y = room.pos.y; y < stopY; y++) {
		for(i32 x = room.pos.x; x < stopX; x++) {
			MAP_CH(map, x, y)  = '.';
			MAP_ISW(map, x, y) = SDL_TRUE;
			}
		}
	}

void add_room_wall_to_map(Tile *map, Room room) {
	i32 stopY = room.pos.y + room.height;
	CLAMP(stopY, 1, MAP_Y - 7);
	i32 stopX = room.pos.x + room.width;
	CLAMP(stopX, 1, MAP_X - 7);
	//LOG("STOP X %d STOP Y %d\n", stopX, stopY);
	for(i32 y = room.pos.y; y < stopY; y++) {
		for(i32 x = room.pos.x; x < stopX; x++) {
			if(y == room.pos.y && MAP_CH(map, x, y) != ','  &&  MAP_CH(map, x, y-1) != ',')  {
				if(MAP_CH(map, x, y) != ','){
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
				}
				else{
					break;
				}
				}
			else if(x == room.pos.x && MAP_CH(map, x, y) != ',' && MAP_CH(map, x-1, y) != ',') {
				if(MAP_CH(map, x, y) != ',' && MAP_CH(map, x, y) != '+'){
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
				}
				else{
					break;
				}
				}
			else if(x == room.pos.x + room.width - 1 && MAP_CH(map, x+1, y) != ',') {
				if(MAP_CH(map, x, y) != ','  && MAP_CH(map, x, y) != '+'){
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
				}
				else{
					break;
				}
				}
			else if(y == room.pos.y + room.height - 1 && MAP_CH(map, x, y+1) != ',') {
				if(MAP_CH(map, x, y) != ','  && MAP_CH(map, x, y) != '+'){
					MAP_CH(map, x, y)  = '/';
					MAP_ISW(map, x, y) = SDL_FALSE;
				}
				else{
					break;
				}
				}
			}
		}
	}

SDL_bool isDoor(Tile *map, Position pos){
	i32 count = 0;
	i32 startX = pos.x - 1;
	i32 startY = pos.y - 1;
	i32 stopX = pos.x + 1;
	i32 stopY = pos.y + 1;
	CLAMP(startX, 0, MAP_X - 1);
	CLAMP(startY, 0, MAP_Y - 1);
	CLAMP(stopX, 0, MAP_X - 1);
	CLAMP(stopY, 0, MAP_Y - 1);
	for (i32 y = startY; y <= stopY; y++){
		for (i32 x = startX; x <= stopX; x++){
			if(MAP_CH(map, x, y) == '.'){
				count++;
			}
			else if(MAP_CH(map, x, y) == '1'){
				count = 0;
				break;
			}
			
		}	
	}
	
	if(count == 4){
		//LOG("ISdoor\n");
		return SDL_TRUE;
	}	
	else
		return SDL_FALSE;
}
void add_doors(Tile *map){
	for (u64 y = 0; y < MAP_Y; y++){
		for (u64 x = 0; x < MAP_X; x++){
			Position temp = {.x = x, .y = y};
			if(isDoor(map, temp) == SDL_TRUE && rand_f64() < CHANCE_SPAWN_DOOR){
				MAP_CH(map, x, y) = '1';
				MAP_ISW(map, x, y) = SDL_FALSE;
			}		
		}
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
		else{
			break;
		}
		if(temp.x == 1 || temp.x == MAP_X-1 || temp.y == 1 || temp.y == MAP_Y - 1 ){
			break;
		}	
		if(MAP_CH(map, temp.x, temp.y) == '/' && isDoorDis == SDL_FALSE){
			MAP_CH(map, temp.x, temp.y) = '+';
			MAP_ISW(map, temp.x, temp.y) = SDL_FALSE;
			//temp.x--;
			//temp.y--;
			countDoors++;
			if(countDoors == 10){
				break;
			}	
		}
		else{
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
		MAP_CH(map, xr, yr) = '.';
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



void generete_dungons(Tile *map, i32 minRooms, i32 maxRooms) {
	i32 width, height, x, y, nRooms;
	u8 isColided = SDL_FALSE;
	if(minRooms >= maxRooms) {
		ASSERT("We have a larger amount of minRooms >= maxRooms");
		}
	//nRooms = (i32)(rand()%(u32)(maxRooms - minRooms)) + minRooms;
	 nRooms = 100;
	//nRooms = 5;
	Room *rooms = calloc(nRooms, sizeof(Room));
	rooms[0] = create_room(9, 9, 10, 10);
	add_room_to_map(map, rooms[0]);

	add_room_wall_to_map(map, rooms[0]);
	i32 count = 0;
	for(i32 i = 1; i < nRooms; i++) {
		while(1){
			isColided = SDL_FALSE;
			y = (rand() % (MAP_Y - 13));
			x = (rand() % (MAP_X - 13));
			height = (rand() % 5) + 7;
			width  = (rand() % 5) + 7;
			for(i32 j = 0; j < i; j++){
				if(checkCollision(x, y, width, height,
					 rooms[j].pos.x, rooms[j].pos.y, rooms[j].width, rooms[j].height)){
						isColided = SDL_TRUE; 
						break;
					 }
			}
			if(isColided == SDL_FALSE){
				break;
			}
			if(count == 100){
				break;
			}
			count++;
		}
		if(count == 100){
			nRooms = i;
			break;
		}
		rooms[i] = create_room(x, y, height, width);
		
		add_room_to_map(map, rooms[i]);
		add_room_wall_to_map(map, rooms[i]);
		count = 0;
		//connect_room_centers(rooms[i-1].center, rooms[i].center, map, SDL_FALSE);
		}
	///*
	
	//*/
	//add_walls_around_roads(map);
	f64 percantage = 0.01f + rand_f64()/2.0f;
	LOG("percantage of caved map %f", percantage);
	
	for(i32 i = 0; i < nRooms; i++){
		i32 minDistance = INF;
		i32 minIndex = i + 1;
		for(i32 j = i + 1; j < nRooms; j++){
		i32 distance = DISTANCE(rooms[j].center.x, rooms[j].center.y, rooms[i].center.x, rooms[i].center.y);
		if(distance < minDistance){
			minDistance = distance;
			minIndex = j;
		}	
	}
	if(rand_f64() < 0.05f){
		connect_room_centers(rooms[i].center, rooms[0].center, map, SDL_FALSE);
	}
	else{
		connect_room_centers(rooms[i].center, rooms[minIndex].center, map, SDL_FALSE);
	}
	
}
	caved_map(map, percantage);
	//add_doors(map);
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
	generete_dungons(map, 15, 30);
	return map;
	}





