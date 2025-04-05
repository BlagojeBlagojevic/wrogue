//#include"utils.h"
#include "map.h"
#include "entity.h"
#include "app.h"
#define SEED 12344
#include<time.h>

int main() {

	DROP(damageStr);
	DROP(monsterName);
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
