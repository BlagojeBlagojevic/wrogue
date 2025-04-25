//#include"utils.h"
#include "map.h"
#include "entity.h"
#include "app.h"
#define SEED 12344
#include<time.h>

Graphics_State mainGraphics;
int main() {

	DROP(damageStr);
	DROP(monsterName);
	DROP(monsters);
	SDL_ERR(SDL_Init(SDL_INIT_VIDEO));
	SDL_ERR(TTF_Init());
	monster_definitions_export();
	u64 seed = (u64)time(0);
	srand(seed);

	WINDOW   = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800,  SDL_WINDOW_OPENGL);
	(void)P_SDL_ERR(WINDOW);
	RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetWindowResizable(WINDOW, SDL_TRUE);
	FONT = TTF_OpenFont(fontLoc, 128);
	(void)P_SDL_ERR(FONT);
	(void)P_SDL_ERR(RENDERER);
	QUIT = 0;
	MOVMENT = SDL_TRUE;
	ITEMSREND = SDL_FALSE;
	Entitiy* player = create_entity('@', "Some Name", 5, 10, (Position) {
		40, 40
		}, WHITE);
	player->attack[0]  = 2;
	player->defence[0] = 2;
	player->attack[1]  = 2;
	player->defence[1] = 2;
	player->attack[2]  = 2;
	player->defence[2] = 2;
	player->attack[3]  = 2;
	player->defence[3] = 2;

	//player->invertory = {0};
	Tile *map = init_map();
	Entitiy_DA monsters = {0};
	Item_DA items = {0};
	Item* sword = create_item(36, 36, SWORD_CREATE());
	da_append(&items, (*sword));
	Item* armor = create_item(38, 38, ARMOR_CREATE());
	da_append(&items, (*armor));
	Item* helmet = create_item(40, 38, HELMET_CREATE());
	da_append(&items, (*helmet));
	Item* shield = create_item(36, 40, SHIELD_CREATE());
	da_append(&items, (*shield));
	Item* shoes = create_item(36, 38, SHOES_CREATE());
	da_append(&items, (*shoes));
	//Item* dart = create_item(14, 14, DART_CREATE());
	//da_append(&items, (*dart));
	//Item* potion = create_item(16, 16, POTION_CREATE());
	//da_append(&items, (*potion));
	//da_append(&items, (*item));
	//da_append(&items, (*item));
	//da_append(&items, (*item));
	//da_append(&items, (*item));
	//da_append(&items, (*item));
	//LOG("char %c\n", items.items[0].ch);
	for(i32 i = 0; i < NUM_RENDER_MSG; i++) {
		da_append(&MESSAGES, "   ");
		}
	char* msg = calloc(30, sizeof(char));
	snprintf(msg, 30, "Your seed is %ld", seed);
	da_append(&MESSAGES, msg);
	genereate_monsters(&monsters, map);
	MOVMENT = 0;
	COUNTMOVES = 0;


	//MAP_STDOUT();
	while(!QUIT) {
		if(EQUITEM == SDL_FALSE) {
			main_renderer(player,  &monsters, &items, map);
			update_entity(player, &monsters, map, &items);
			}
		main_renderer(player,  &monsters, &items, map);
		event_user(player, &monsters, &items, map);
		if(EQUITEM == SDL_TRUE) {
			main_renderer(player,  &monsters, &items, map);
			LOG("Items:\n");
			for(u64 i = 0; i < player->inventory.count; i++) {
				Item item = player->inventory.items[i];
				if(item.isEquiped == SDL_TRUE) {
					LOG("\t%s\n", item.descripction);
					}
				}
			}
		//for(u64 count = 0; count < MESSAGES.count; count++) {
		//	LOG("%s", MESSAGES.items[count]);
		//	}

		//system("pause");
		//SDL_Delay(1000);
		}

	return 0;
	ASSERT("UNREACHABLE");
	}
