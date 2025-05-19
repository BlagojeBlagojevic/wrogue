//#include"utils.h"

#include "map.h"
#include "entity.h"
#include "app.h"
#define SEED 12344
#include<time.h>


//TBD CAMERA

Graphics_State    mainGraphics;
SDL_Texture*      monstersTextures;
SDL_Texture*      groundTextures;
SDL_Texture*      wallTextures;
SDL_Texture*      poisonTextures;
SDL_Texture*      blightTextures;
SDL_Texture*      playerTextures;
SDL_Texture*      cloudTextures;
SDL_Texture*      itemTextures;
SDL_Texture*      swordTextures;
static Tile*      map;
static Entitiy_DA monster;
static Entitiy*   player;
static Item_DA    items;


void generate_level() {

	Room_DA rooms = {0};

	LEVEL++;
	i32 whatMap = rand()%4;
	if(whatMap == 0) map = init_map_BSP(&rooms, 4);
	//else if(whatMap == 2) map = init_map(&rooms);
	else map = init_map_RA(&rooms);
	monster.count = 0;
	items.count = 0;
	if(rooms.count <= 2) {
		Room room;
		room.pos.x  = 0;
		room.pos.y  = 0;
		room.width  = 20;
		room.height = 20;
		//exit(-1);
		for(i32 y = 0; y < MAP_Y - room.height; y+=room.height) {
			room.pos.y=y;
			for(i32 x = 0; x < MAP_X - room.width; x+=room.width) {
				room.pos.x=x;
				genereate_monsters_generator(player, &monster, map, LEVEL, room);
				}
			}
		}
	else {
		for(u64 i = 1; i < rooms.count; i++) {
			genereate_monsters_generator(player, &monster, map, LEVEL, rooms.items[i]);
			}
		}



	//genereate_monsters(&monster, map);
	//calculate_diakstra_map(player, map, &monster, rooms.items[0].pos.x, rooms.items[0].pos.y);
	//caved_part_generator(TILE_TREE, map, 5);
	//calculate_diakstra_map(player, map, &monster,  rooms.items[0].pos.x, rooms.items[0].pos.y);
	//caved_part_generator(TILE_GRASS, map, 100);
	MOVMENT = 0;
	COUNTMOVES = 0;
	player->pos.x = rooms.items[0].center.x;
	player->pos.y = rooms.items[0].center.y;
	if(rooms.items != NULL) {
		free(rooms.items);
		}
	//Item* helmet = create_item(player->pos.x, player->pos.y, GOLD_CREATE());
	//da_append(&items, (*helmet));

	}


int main() {

	DROP(damageStr);
	DROP(monsterName);
	DROP(monsters);

	//INIT
	SDL_ERR(SDL_Init(SDL_INIT_VIDEO));
	SDL_ERR(TTF_Init());

	u64 seed = (u64)time(0);
	srand(seed);
	for(i32 i = 0; i < NUM_RENDER_MSG; i++) {
		da_append(&MESSAGES, "   ");
		}
	char* msg = calloc(30, sizeof(char));
	snprintf(msg, 30, "Your seed is %ld", seed);
	da_append(&MESSAGES, msg);

	WINDOW   = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800,  SDL_WINDOW_OPENGL);
	(void)P_SDL_ERR(WINDOW);
	RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetWindowResizable(WINDOW, SDL_TRUE);
	FONT = TTF_OpenFont(fontLoc, 128);
	(void)P_SDL_ERR(FONT);
	(void)P_SDL_ERR(RENDERER);
	init_texture();
	QUIT = 0;
	MOVMENT = SDL_TRUE;
	ITEMSREND = SDL_FALSE;
	player = create_entity('@', "Some Name", 10, 20, (Position) {
		40, 40
		}, WHITE);

	player->attack[0]  = 2;
	player->defence[0] = 4;
	player->attack[1]  = 2;
	player->defence[1] = 4;
	player->attack[2]  = 2;
	player->defence[2] = 4;
	player->attack[3]  = 2;
	player->defence[3] = 4;
	player->stamina    = 10;
	player->maxStamina = 10;
	player->chanceToDecressStaminaMove = 0.1f;
	Item* sword = create_item(0, 0, PLAYER_SWORD_CREATE());
	sword->isEquiped = SDL_TRUE;
	da_append(&player->inventory, (*sword));
	Item* armor = create_item(0, 0, PLAYER_ARMOR_CREATE());
	armor->defence[DAMAGE_BASIC] = 1;
	armor->isEquiped = SDL_TRUE;
	da_append(&player->inventory, (*armor));
	//LOG("Plateer");
	//exit(-1);
	/*

	Item* helmet = create_item(40, 38, HELMET_CREATE());
	helmet->isEquiped = SDL_TRUE;
	da_append(&player->inventory, (*helmet));
	Item* shield = create_item(36, 40, SHIELD_CREATE());
	shield->isEquiped = SDL_TRUE;
	da_append(&player->inventory, (*shield));
	Item* shoes = create_item(36, 38, SHOES_CREATE());
	shoes->isEquiped = SDL_TRUE;
	da_append(&player->inventory, (*shoes));
	//*/
	Item* healing = create_item(37, 38, HEALING_CREATE());

	//player->invertory = {0};
	monster_definitions_export();
	export_generators();
	//Entitiy_DA monsters = {0};

	LEVEL = 0;
	DEPTH = 25;
	generate_level();
	//MAP_STDOUT();
	while(!QUIT) {
		//monster.count < 1 &&
		if( MAP_CH(map, player->pos.x, player->pos.y) == TILE_STAIRS) {
			player->attack[0]  += 1;
			player->defence[0] += 1;
			player->attack[1]  += 1;
			player->defence[1] += 1;
			player->attack[2]  += 1;
			player->defence[2] += 1;
			player->attack[3]  += 1;
			player->defence[3] += 1;
			player->maxHealth +=10;
			//player->health = player->maxHealth;
			//LEVEL +=1;
			generate_level();
			DEPTH--;
			if(DEPTH == -1) {
				LOG("You win\n");
				system("cls");
				exit(-1);
				}
			//INCREMENT PLAYER

			}
		if(EQUITEM == SDL_FALSE) {
			main_renderer(player,  &monster, &items, map);
			update_entity(player, &monster, map, &items);
			}
		main_renderer(player,  &monster, &items, map);
		SDL_Delay(10);
		event_user(player, &monster, &items, map);
		lingering_map_tile(map, player, &monster);
		if(monster.count < 5) {
			Room room;
			room.pos.x  = 0;
			room.pos.y  = 0;
			room.width  = MAP_Y - 1;
			room.height = MAP_X - 1;
			genereate_monsters_generator(player, &monster, map, LEVEL, room);
			}
		if((COUNTMOVES+1) % 200 == 0) {
			Room room;
			room.pos.x  = 0;
			room.pos.y  = 0;
			room.width  = MAP_Y - 1;
			room.height = MAP_X - 1;
			genereate_monsters_generator(player, &monster, map, LEVEL + 1, room);
			}
		if(player->health <= 0) {
			;
			da_append(&MESSAGES, "You loose");
			exit(-1);
			player->health = 10;
			player->maxHealth = 10;
			LEVEL = 0;
			DEPTH = rand()%7 + 1;
			generate_level();
			}
		if(EQUITEM == SDL_TRUE) {
			main_renderer(player,  &monster, &items, map);
			LOG("Items:\n");
			for(u64 i = 0; i < player->inventory.count; i++) {
				Item item = player->inventory.items[i];
				if(item.isEquiped == SDL_TRUE) {
					LOG("\t%s\n", item.descripction);
					da_append(&MESSAGES, item.descripction);
					}
				}
			}
		//for(u64 count = 0; count < MESSAGES.count; count++) {
		//	LOG("%s", MESSAGES.items[count]);
		//	}

		//system("pause");
		//SDL_Delay(1000);
		//Sleep(1);
		}

	return 0;
	ASSERT("UNREACHABLE");
	}
