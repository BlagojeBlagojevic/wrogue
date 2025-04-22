#include "app.h"


void SDL_ERR(int code) {

	if((code) < 0) {
		ERROR_BREAK("SDL_ERROR: %s\n", SDL_GetError());
		}
	return;
	}

void *P_SDL_ERR(void *ptr) {
	if(ptr == NULL) {
		ERROR_BREAK("SDL_ERROR: %s\n", SDL_GetError());
		}
	return ptr;
	}



void Text_Renderer_C(SDL_Renderer *renderer, TTF_Font *font, i32 startX, i32 startY, i32 w_c, i32 h_c, const char *c, SDL_Color textColor) {
	if(renderer == NULL) {
		ASSERT("renderer is null!!!");
		}
	if(font == NULL) {
		ASSERT("font is null!!!");
		}
	SDL_Surface *textSurface = P_SDL_ERR(TTF_RenderText_Solid(font, c, textColor));
	SDL_Texture *textTexture = P_SDL_ERR(SDL_CreateTextureFromSurface(renderer, textSurface));
	SDL_Rect textRect = {startX, startY, w_c, h_c}; // rectangle where the text is drawn
	SDL_ERR(SDL_RenderCopy(renderer, textTexture, NULL, &textRect));
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	}


void render_player(Entitiy *player) {
	i32 startX = player->pos.x * FONT_W;
	i32 startY = player->pos.y * FONT_H;
	
	SDL_Color color;
	if(player->health >= 3) {
		color = player->color;
		}
	else if(player->health == 2) {
		color = (SDL_Color) {
			255, 125, 125, 0
			};
		}
	else if(player->health == 1) {
		color = (SDL_Color) {
			255, 0, 0, 0
			};
		}
	else {
		color = player->color;
		}
	Text_Renderer_C(RENDERER, FONT, startX, startY, 10, 15, &player->ch, color);
	}



void player_input(SDL_Event *event, Entitiy* player, Entitiy_DA *entitis, Item_DA *items, Tile* map) {
	const u32 key = event->key.keysym.sym;
	MOVMENT = SDL_FALSE;  //NOT PROB
	if(player->isStunded != 0){
		MOVMENT = SDL_TRUE;
		player->isStunded--;
		CLAMP(player->isStunded, 0, 255);
	}
	else if(key == UP_ARROW || key == KEY_W) {
		if(player->pos.y > 0 && MAP_ISW(map, player->pos.x, player->pos.y-1) == SDL_TRUE) {
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.y--;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_FALSE;
			MOVMENT = SDL_TRUE;
			}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x,  player->pos.y-1, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster], items, map);
				MOVMENT = SDL_TRUE;
				}
			}
		}
	else if(key == DOWN_ARROW || key == KEY_S) {
		if(player->pos.y < MAP_Y && MAP_ISW(map, player->pos.x, player->pos.y+1) == SDL_TRUE) {
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.y++;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_FALSE;
			MOVMENT = SDL_TRUE;
			}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x,  player->pos.y + 1, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster], items, map);
				MOVMENT = SDL_TRUE;
				}
			}

		}
	else if(key == LEFT_ARROW || key == KEY_A) {
		if(player->pos.x > 0 && MAP_ISW(map, player->pos.x-1, player->pos.y) == SDL_TRUE) {
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.x--;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			MOVMENT = SDL_TRUE;
			}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x - 1,  player->pos.y, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster], items, map);
				MOVMENT = SDL_TRUE;
				}
			}

		}
	else if(key == RIGHT_ARROW || key == KEY_D) {
		if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x + 1, player->pos.y) == SDL_TRUE) {
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.x++;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			MOVMENT = SDL_TRUE;
			}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x + 1,  player->pos.y, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster], items, map);
				MOVMENT = SDL_TRUE;
				}
			}
		}
	else if(key == KEY_Q) {
			if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x - 1, player->pos.y - 1) == SDL_TRUE) {
				MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
				player->pos.x--;
				player->pos.y--;
				MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
				MOVMENT = SDL_TRUE;
				}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x - 1,  player->pos.y - 1, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster], items, map);
				MOVMENT = SDL_TRUE;
					}
			}
		}
	else if(key == KEY_E) {
			if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x + 1, player->pos.y - 1) == SDL_TRUE) {
				MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
				player->pos.x++;
				player->pos.y--;
				MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
				MOVMENT = SDL_TRUE;
				}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x + 1,  player->pos.y - 1, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster], items, map);
				MOVMENT = SDL_TRUE;
					}
			}
		}
	else if(key == KEY_Z || key == KEY_Y) {
			if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x - 1, player->pos.y + 1) == SDL_TRUE) {
				MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
				player->pos.x--;
				player->pos.y++;
				MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
				MOVMENT = SDL_TRUE;
				}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x - 1,  player->pos.y + 1, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster], items, map);
				MOVMENT = SDL_TRUE;
					}
			}
		}
	else if(key == KEY_C) {
			if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x + 1, player->pos.y + 1) == SDL_TRUE) {
				MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
				player->pos.x++;
				player->pos.y++;
				MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
				MOVMENT = SDL_TRUE;
				}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x + 1,  player->pos.y + 1, entitis);
			if(witchIsMonster != -1) {
				player_attack(player, &entitis->items[witchIsMonster], items, map);
				MOVMENT = SDL_TRUE;
					}
			}
		}		
		
	else if(key == SPACE) {
		//DO NOTHING
		MOVMENT = SDL_TRUE;
		}
	else if(key == KEY_I) {
		//DO NOTHING
		if(ITEMSREND == SDL_FALSE) {
			char* msg = "You decided to look a items on flor.";
			da_append(&MESSAGES, msg);
			}
		if(ITEMSREND == SDL_TRUE) {
			char* msg = "Items nah!!!";
			da_append(&MESSAGES, msg);
			}
		ITEMSREND = !ITEMSREND;
		MOVMENT = SDL_TRUE;
		}
	else if(key == KEY_P) {
		PICKITEM = SDL_TRUE;
		MOVMENT = SDL_TRUE;
		}
	else if(key == KEY_O){
		char* msg = "Casted spell of moving rock!!!";
		da_append(&MESSAGES, msg);
		OPENDOOR = SDL_TRUE;
		MOVMENT = SDL_TRUE;
		//LOG("O\n");
	}
	}


void render_stats(Entitiy *player) {
	char stats[1024];
	i32 startX = MAP_X * FONT_W + 10;
	i32 startY = HEIGHT - 300;
	SDL_Rect temp = {startX-3, startY, 600, 150};
	SDL_ERR(SDL_SetRenderDrawColor(RENDERER, 0X40, 0X20, 0X20, 0XFF));
	SDL_RenderFillRect(RENDERER, &temp);
	snprintf(stats, 1024, "STATS: Health %d", player->health);
	Text_Renderer_C(RENDERER, FONT,startX, startY, strlen(stats) * FONT_W, 20, stats, WHITE);
	for(Damage_Types i = 0; i < DAMAGE_NUM; i++) {
		stats[0] = '\0';
		snprintf(stats, 1024, "STATS: %s att: %d def: %d", damageStr[i], player->attack[i], player->defence[i]);
		Text_Renderer_C(RENDERER, FONT, startX, startY + FONT_H_MESSAGES*(i+1), strlen(stats) * FONT_W,
		                FONT_H_MESSAGES, stats, WHITE);
		}

	}

void render_monsters(Entitiy_DA *monsters, Entitiy *player, Tile *map) {
	DROP(damageStr);
	DROP(monsterName);
	DROP(monsters[BASIC_MONSTER]);
	i32 radius = player->radius;
	i32 startX = player->pos.x - radius;
	i32 startY = player->pos.y - radius;
	i32 stopX  = player->pos.x + radius;
	i32 stopY  = player->pos.y + radius;
	CLAMP(startX, 0, MAP_X-1);
	CLAMP(stopX,  0, MAP_X-1);
	CLAMP(startY, 0, MAP_Y-1);
	CLAMP(stopY,  0, MAP_Y-1);//*/
	for(u64 count = 0; count < monsters->count; count++) {
		//if(monsters->items[count].pos.x >= startX && monsters->items[count].pos.x <= stopX
		//    && monsters->items[count].pos.y >= startY && monsters->items[count].pos.y <= stopY) {
			if(MAP_ISV(map, monsters->items[count].pos.x, monsters->items[count].pos.y) == SDL_TRUE){
			render_player(&monsters->items[count]);
			}

		}

	}

void render_messages(i32 startX, i32 startY, char* message) {
	if(message != NULL) {
		u64 w_c = strlen(message) * FONT_W;
		Text_Renderer_C(RENDERER, FONT, startX, startY, (i32)w_c, 20, message, WHITE);
		}

	}

void render_item(Item* item, Tile* map) {
	
	i32 x = item->pos.x;
	i32 y = item->pos.y;
	i32 startX = x * FONT_W;
	i32 startY = y * FONT_H;
	char ch = item->ch;
	//LOG("\nCHAR %c\n", ch);
	SDL_Rect temp = {startX, startY, FONT_W, FONT_H};
	DROP(temp);
	if(MAP_ISW(map, x, y) == SDL_TRUE && MAP_ISV(map, x, y) == SDL_TRUE) {
		//SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
		//SDL_RenderFillRect(RENDERER, &temp);
		Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W+10, FONT_H+10, &ch, item->color);
		}
	}

void render_items(Item_DA *items, Tile* map, Entitiy* player) {
	i32 radius = player->radius;
	i32 startX = player->pos.x - radius;
	i32 startY = player->pos.y - radius;
	i32 stopX  = player->pos.x + radius;
	i32 stopY  = player->pos.y + radius;
	CLAMP(startX, 0, MAP_X-1);
	CLAMP(stopX,  0, MAP_X-1);
	CLAMP(startY, 0, MAP_Y-1);
	CLAMP(stopY,  0, MAP_Y-1);
	for(u64 i = 0; i < items->count; i++) {
		Item item = items->items[i];
		i32 x = item.pos.x;
		i32 y = item.pos.y;
		if(x >= startX && y >= startY && x <= stopX && y <= stopY ) {
			render_item(&items->items[i], map);
			//Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, &item.ch, item.color);
			}
		}
	}

void render_inventory(Item_DA *inventory) {
	i32 startX = 	MAP_X * FONT_H + 100;
	i32 startY = 30 + FONT_H_MESSAGES * (NUM_RENDER_MSG+3); //THIS DEPENS CUZZ HOW MUTCH MESSAGES
	i32 h = 580/2;
	i32 w = 500;
	
	SDL_Rect rec = {startX, startY, w, h};
	SDL_SetRenderDrawColor(RENDERER, 0, 30, 10, 0);
	SDL_RenderFillRect(RENDERER, &rec);
	for(u64 i = 0; i < inventory->count; i++) {
		;
		//LOG("NAME:%s\n", inventory->items[i].name);
		render_messages(startX, startY + (i*FONT_H_MESSAGES), inventory->items[i].name);
		//Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H_MESSAGES, inventory->items[i].name, WHITE);
		}
	}


void render_map_fov(Entitiy *player, Tile *map) {
	//field_of_vison(player, map);
	i32 radius = player->radius;
	i32 startX = player->pos.x - radius;
	i32 startY = player->pos.y - radius;
	i32 stopX  = player->pos.x + radius;
	i32 stopY  = player->pos.y + radius;
	CLAMP(startX, 0, MAP_X-1);
	CLAMP(stopX,  0, MAP_X-1);
	CLAMP(startY, 0, MAP_Y-1);
	CLAMP(stopY,  0, MAP_Y-1);
	for(i32 y  = 0; y < MAP_Y; y++) {
		for(i32 x = 0; x < MAP_X; x++) {
			if(MAP_ISV(map, x, y) == SDL_TRUE) {
				i32 startX = x * FONT_W;
				i32 startY = y * FONT_H;
				char ch = MAP_CH(map, x, y);
				if(ch == '#') {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					}
				else if(ch == '/' ){
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 80, 80, 80, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
				}
				else if(ch == ',') {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					} // if(ch != '.')
				else if(ch == '+'){
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "+", WHITE);
				}
				else if(ch == '-'){
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 0x40, 0x15, 0x15, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "-", WHITE);
				}
				else {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 10, 10, 10, 100);
					SDL_RenderDrawRect(RENDERER, &textRect);
					DROP(textRect);
					}

				}
			else if (MAP_VISITED(map, x, y) == SDL_TRUE){
				i32 startX = x * FONT_W;
				i32 startY = y * FONT_H;
				char ch = MAP_CH(map, x, y);
				if(ch == '#') {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 60, 60, 60, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					}
				else if(ch == '/' ){
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 40, 40, 40, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
				}
				else if(ch == '+'){
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "+", WHITE);
				}
				else if(ch == '-'){
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 0x40, 0x15, 0x15, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "-", WHITE);
				}
				else {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 10, 10, 10, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					DROP(textRect);
					}
			}	
			else {
				i32 startX = x * FONT_W;
				i32 startY = y * FONT_H;
				SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
				SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0);
				//SDL_SetRenderDrawColor(RENDERER, 255, 0, 0, 0);
				SDL_RenderFillRect(RENDERER, &textRect);
				DROP(textRect);
				}
			}
		}
	}

void render_map_dikstra(Entitiy *player, Tile *map) {
	i32 radius = player->radius;
	i32 startX = player->pos.x - radius;
	i32 startY = player->pos.y - radius;
	i32 stopX  = player->pos.x + radius;
	i32 stopY  = player->pos.y + radius;
	CLAMP(startX, 0, MAP_X-1);
	CLAMP(stopX,  0, MAP_X-1);
	CLAMP(startY, 0, MAP_Y-1);
	CLAMP(stopY,  0, MAP_Y-1);
	for(i32 y  = 0; y < MAP_Y; y++) {
		for(i32 x = 0; x < MAP_X; x++) {
				//DROP(textRect);
				i32 startX = x * FONT_W;
				i32 startY = y * FONT_H;
				SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
				u64 c = 0xFFFFFFF * MAP_DIJKSTRA(map, x, y);
				u8 r = (c & 0xFF000000) >> 24; 
				u8 g = (c & 0x00FF0000) >> 16; 
				u8 b = (c & 0x0000FF00) >> 8;
				u8 a = (c & 0x000000FF) >> 0;
				LOG("Color %lu\n", c);
				SDL_SetRenderDrawColor(RENDERER, r, r, r, a);
				SDL_RenderFillRect(RENDERER, &textRect);

		}
	}	
}	

void render_map(Tile *map, Entitiy *player) {
	Text_Renderer_C(RENDERER, FONT, WIDTH/2, 0, 10*10+10, 20, "ROUGE GAME()", WHITE);


	i32 radius = player->radius;
	i32 startX = player->pos.x - radius;
	i32 startY = player->pos.y - radius;
	i32 stopX  = player->pos.x + radius;
	i32 stopY  = player->pos.y + radius;
	CLAMP(startX, 0, MAP_X-1);
	CLAMP(stopX,  0, MAP_X-1);
	CLAMP(startY, 0, MAP_Y-1);
	CLAMP(stopY,  0, MAP_Y-1);

#ifdef PLAYER_VISION
	for(i32 y  = startY; y < stopY; y++) {
		for(i32 x = startX; x < stopX; x++) {//*/
#endif

#ifndef PLAYER_VISION
			for(i32 y  = 0; y < MAP_Y; y++) {
				for(i32 x = 0; x < MAP_X; x++) {
#endif
					//*/
					i32 startX = x * FONT_W;
					i32 startY = y * FONT_H;
					char ch = MAP_CH(map, x, y);
					if(ch == '#') {
						SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
						SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
						SDL_RenderFillRect(RENDERER, &textRect);
						}
					else if(ch == ',') {
						SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
						//DROP(textRect);
						SDL_SetRenderDrawColor(RENDERER, 100, 10, 10, 100);
						SDL_RenderFillRect(RENDERER, &textRect);
						} // if(ch != '.')
					else if(ch == '+') {
						SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
						//DROP(textRect);
						SDL_SetRenderDrawColor(RENDERER, 0, 10, 255, 100);
						SDL_RenderFillRect(RENDERER, &textRect);
						Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W + 3, FONT_H, &ch, WHITE);
						}
					else if(ch == '-') {
						SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
						//DROP(textRect);
						
						SDL_SetRenderDrawColor(RENDERER, 0, 10, 255, 100);
						SDL_RenderFillRect(RENDERER, &textRect);
						Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, &ch, WHITE);
							}
					else if(ch == '/'){
						SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
						SDL_SetRenderDrawColor(RENDERER, 10, 255, 10, 100);
						SDL_RenderFillRect(RENDERER, &textRect);
					}
					else {
						SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
						SDL_SetRenderDrawColor(RENDERER, 10, 10, 10, 100);
						SDL_RenderDrawRect(RENDERER, &textRect);
						DROP(textRect);
						}
					}
				}

			}

		void main_renderer(Entitiy* player, Entitiy_DA *monster, Item_DA *items, Tile *map) {
			SDL_ERR(SDL_RenderClear(RENDERER));
			//render_map(map, player);
			render_map_fov(player, map);
			//render_map_dikstra(player, map);
			render_player(player);
			//render_player(&monster->items[0]);
			if(ITEMSREND == SDL_TRUE) {
				render_items(items, map, player);
				}
			render_monsters(monster, player, map);
			
			render_inventory(&player->inventory);
			render_stats(player);
			i32 count = 1;
			SDL_Rect temp = {MAP_X*FONT_W + 10, 30, 600, 150};
			SDL_ERR(SDL_SetRenderDrawColor(RENDERER, 0X40, 0X20, 0X20, 0XFF));
			SDL_RenderFillRect(RENDERER, &temp);
			for(i32 i = (i32)MESSAGES.count-1; i >= ((i32)MESSAGES.count - NUM_RENDER_MSG); i--) {
				render_messages((MAP_X*FONT_W + 10), (30 + FONT_H_MESSAGES*(count++)), MESSAGES.items[i]);
				}
			SDL_ERR(SDL_SetRenderDrawColor(RENDERER, 0X20, 0X20, 0X20, 0XFF));
			SDL_RenderPresent(RENDERER);
			}

		void event_user(Entitiy *player, Entitiy_DA *entitis, Item_DA *items, Tile* map) {
			MOVMENT = SDL_FALSE;
			while(MOVMENT == SDL_FALSE) {
				if(SDL_WaitEvent(&EVENT)) {
					if(EVENT.type == SDL_QUIT) {
						MOVMENT = SDL_TRUE;
						QUIT = 1;
						}
					else if(EVENT.type == SDL_WINDOWEVENT) {  //JUST FOR NOW
						MOVMENT = SDL_TRUE;
						SDL_GetWindowSize(WINDOW, &WIDTH, &HEIGHT);
						//FONT_H = HEIGHT / MAP_Y - 4;
						//FONT_W = WIDTH  / MAP_X;
						FONT_W = 11;
						FONT_H = 10;
						//FONT_H = 6;
						//FONT_W = 10;
						if(WIDTH > 4096 || HEIGHT > 2048) {
							ASSERT("Oversized window\n");
							}
						//LOG("width %d, height %d\n", WIDTH, HEIGHT);
						}
					else if(EVENT.type == SDL_KEYDOWN) {
						player_input(&EVENT, player, entitis, items, map);

						SDL_Delay(1);
						}
					}
				EVENT.type = 0;
				}
			}
