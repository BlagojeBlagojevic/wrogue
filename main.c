//#include"utils.h"

#include "map.h"
#include "entity.h"
#include "app.h"
#define SEED 12344
#include<time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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
SDL_Texture*      grassTextures;
SDL_Texture*      boulderTextures;
SDL_Texture*      treeTextures;
SDL_Texture*      stairTextures;
SDL_Texture*      rangeItemsTextures;
SDL_Texture*      glyphTextures;
SDL_Texture*      scrolTextures;
static Tile*      map;
static Entitiy_DA monster;
static Entitiy*   player;
static Item_DA    items;

void generate_level() {
	Room_DA rooms = {0};
	CHANCE_MONSTER_HUTING_WOUND = 0.3;
	LEVEL++;
	i32 whatMap = rand()%4;
	if(whatMap == 0) map = init_map_BSP(&rooms, 4);
	else map = init_map_RA(&rooms);
	monster.count = 0;
	items.count = 0;
	if(rooms.count <= 2) {
		Room room = {.pos = {0, 0}, .width = 20, .height = 20};
		for(i32 y = 0; y < MAP_Y - room.height; y+=room.height) {
			room.pos.y=y;
			for(i32 x = 0; x < MAP_X - room.width; x+=room.width) {
				room.pos.x=x;
			}
		}
	} else {
		for(u64 i = 0; i < rooms.count; i++) {
			genereate_monsters_generator(player, &monster, map, LEVEL, rooms.items[i], SDL_TRUE);
		}
	}
	calculate_diakstra_map(player, map, &monster, rooms.items[0].pos.x, rooms.items[0].pos.y);
	caved_part_generator(TILE_TREE, map, 2);
	calculate_diakstra_map(player, map, &monster,  rooms.items[0].pos.x, rooms.items[0].pos.y);
	caved_part_generator(TILE_GRASS, map, 100);
	MOVMENT = 0;
	COUNTMOVES = 0;
	player->pos.x = rooms.items[0].center.x;
	player->pos.y = rooms.items[0].center.y;
	if(rooms.items != NULL) {
		free(rooms.items);
	}
	genreate_scrol_items(&items, map);
}

void game_loop() {
	if(MAP_CH(map, player->pos.x, player->pos.y) == TILE_STAIRS) {
		player->attack[0]++;
		player->defence[0]++;
		player->attack[1]++;
		player->defence[1]++;
		player->attack[2]++;
		player->defence[2]++;
		player->attack[3]++;
		player->defence[3]++;
		player->maxHealth += 10;
		generate_level();
		DEPTH--;
		if(DEPTH == -1) {
			LOG("You win\n");
#ifdef __EMSCRIPTEN__
			emscripten_cancel_main_loop();
#else
			exit(0);
#endif
			QUIT = 1;
			return;
		}
	}
	if(EQUITEM == SDL_FALSE) {
		main_renderer(player, &monster, &items, map);
		update_entity(player, &monster, map, &items);
	}
	main_renderer(player, &monster, &items, map);
	event_user(player, &monster, &items, map);
	lingering_map_tile(map, player, &monster);
	if(monster.count < 12 || ((COUNTMOVES + 1) % 200 == 0)) {
		Room room = {.pos = {0, 0}, .width = MAP_Y - 1, .height = MAP_X - 1};
		genereate_monsters_generator(player, &monster, map, LEVEL + (COUNTMOVES % 200 == 0), room, SDL_FALSE);
	}
	if(player->health <= 0) {
		da_append(&MESSAGES, "You lose");
		player->health = 10;
		player->maxHealth = 10;
		LEVEL = 0;
		DEPTH = rand() % 7 + 1;
		generate_level();
	}
	if(EQUITEM == SDL_TRUE) {
		main_renderer(player, &monster, &items, map);
		LOG("Items:\n");
		for(u64 i = 0; i < player->inventory.count; i++) {
			Item item = player->inventory.items[i];
			if(item.isEquiped == SDL_TRUE) {
				LOG("\t%s\n", item.descripction);
				da_append(&MESSAGES, item.descripction);
			}
		}
	}
}

int main() {
	DROP(damageStr);
	DROP(monsterName);
	DROP(monsters);
	SDL_ERR(SDL_Init(SDL_INIT_VIDEO));
	SDL_ERR(TTF_Init());
	u64 seed = (u64)time(0);
	srand(seed);
	for(i32 i = 0; i < NUM_RENDER_MSG; i++) {
		da_append(&MESSAGES, "   ");
	}
	char* msg = calloc(30, sizeof(char));
	snprintf(msg, 30, "Your seed is %d", (i16)seed);
	da_append(&MESSAGES, msg);

	WINDOW   = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_OPENGL);
	(void)P_SDL_ERR(WINDOW);
	RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetWindowResizable(WINDOW, SDL_TRUE);
	FONT = TTF_OpenFont(fontLoc, 128);
	(void)P_SDL_ERR(FONT);
	(void)P_SDL_ERR(RENDERER);
	init_texture();
	QUIT = 0;
	MOVMENT = SDL_TRUE;
	ITEMSREND = SDL_TRUE;

	player = create_entity('@', "Some Name", 10, 20, (Position){40, 40}, WHITE);
	player->attack[0] = player->attack[1] = player->attack[2] = player->attack[3] = 2;
	player->defence[0] = player->defence[1] = player->defence[2] = player->defence[3] = 2;
	player->stamina = player->maxStamina = 15;
	player->hunger = 255;
	player->chanceToDecressStaminaMove = 0.1f;

	Item* sword = create_item(0, 0, DAGER_CREATE());
	sword->isEquiped = SDL_TRUE;
	da_append(&player->inventory, (*sword));

	Item* armor = create_item(0, 0, PLAYER_ARMOR_CREATE());
	armor->isEquiped = SDL_TRUE;
	da_append(&player->inventory, (*armor));

	armor = create_item(0, 0, MEAT_CREATE());
	da_append(&player->inventory, (*armor));

	Item* bow = create_item(player->pos.x, player->pos.y, BOW_CREATE());
	bow->isEquiped = SDL_TRUE;
	da_append(&player->inventory, (*bow));
	Item* arrow = create_item(player->pos.x, player->pos.y, ARROW_CREATE());
	da_append(&player->inventory, (*arrow));

	monster_definitions_export();
	export_generators();
	LEVEL = 0;
	DEPTH = 7;
	generate_level();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(game_loop, 0, 1);
#else
	while (!QUIT) {
		game_loop();
		SDL_Delay(10);
	}
#endif
	return 0;
}
