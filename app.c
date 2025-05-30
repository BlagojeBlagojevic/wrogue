#include "app.h"

void init_texture() {
	SDL_Surface *tempSur = IMG_Load("assets/monsters.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	monstersTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/ground.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	groundTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/wall.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	wallTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/acid.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	poisonTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/blight.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	blightTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/player.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	playerTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);


	tempSur = IMG_Load("assets/items.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	itemTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);


	tempSur = IMG_Load("assets/sword.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	swordTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/grass.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	grassTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/boulder.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	boulderTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/downst.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	stairTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/tree.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	treeTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/rangeitems.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	rangeItemsTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);


	tempSur = IMG_Load("assets/glyph.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	glyphTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);

	tempSur = IMG_Load("assets/scrol.png");
	if(tempSur == NULL) {
		ASSERT("We have no file");
		}
	scrolTextures = P_SDL_ERR(SDL_CreateTextureFromSurface(RENDERER, tempSur));
	SDL_FreeSurface(tempSur);



	}



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
	i32 startX = player->pos.x * FONT_W - CAMERA.x;
	i32 startY = player->pos.y * FONT_H - CAMERA.y;

	//Text_Renderer_C(RENDERER, FONT, startX, startY, 10, 15, &player->ch, color);
	SDL_Rect textRect = {startX, startY, FONT_H, FONT_W};

	SDL_ERR(SDL_RenderCopy(RENDERER, playerTextures, NULL, &textRect));

	//render_interpolated(player);
	}

void render_interpolated(Entitiy *ent) {
	i32 startX = ent->oldPos.x * FONT_W - CAMERA.x + 1;
	i32 startY = ent->oldPos.y * FONT_H - CAMERA.y + 1;
	i32 stopX  = ent->pos.x * FONT_W - CAMERA.x;
	i32 stopY  = ent->pos.y * FONT_H - CAMERA.y;
	i32 xSign  = abs(stopX - startX) / (stopX - startX);
	i32 ySign  = abs(stopY - startY) / (stopY - startY);
	i32 count = 0;
	while(startX != stopX && startY != stopY) {
		if(count++ == 300) {
			break;
			}
		if(abs(startX - stopX) > 1) {
			startX+=(xSign*STEP_INTERPOL);
			CLAMP(startX, stopX - 2*FONT_W,  stopX - 2*FONT_W);
			}
		if(abs(startY - stopY) > 1) {
			startY+=(ySign*STEP_INTERPOL);
			CLAMP(startY, stopY - 2*FONT_H,  stopY - 2 * FONT_H);
			}

		SDL_Rect textRect = {startX, startY, FONT_H, FONT_W};

		SDL_ERR(SDL_RenderCopy(RENDERER, playerTextures, NULL, &textRect));
		//SDL_RenderPresent(RENDERER);
		//SDL_Delay(1);
		}



	}

void render_player_texture(Entitiy *player) {
	i32 startX = player->pos.x * FONT_W - CAMERA.x;
	i32 startY = player->pos.y * FONT_H - CAMERA.y;

	SDL_Rect what;
	switch(player->ch) {
		case 'a': {
				what.x = 0;
				what.y = 0;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'Z': {
				what.x = 256;
				what.y = 0;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'G': {
				what.x = 512;
				what.y = 0;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'I': {
				what.x = 768;
				what.y = 0;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'w': {
				what.x = 0;
				what.y = 270;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'W': {
				what.x = 256;
				what.y = 280;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'N': {
				what.x = 256;
				what.y = 280;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'B': {
				what.x = 512;
				what.y = 280;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'F': {
				what.x = 768;
				what.y = 280;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'Y': {
				what.x = 512;
				what.y = 790;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'D': {
				what.x = 768;
				what.y = 790;
				what.h = 256;
				what.w = 256;
				break;
				}

		case 'E': {
				what.x = 768;
				what.y = 540;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'O': {
				what.x = 256;
				what.y = 540;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'V': {
				what.x = 0;
				what.y = 530;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'A': {
				what.x = 512;
				what.y = 535;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'R': {
				what.x = 0;
				what.y = 790;
				what.h = 256;
				what.w = 256;
				break;
				}
		case 'K': {
				what.x = 256;
				what.y = 790;
				what.h = 256;
				what.w = 256;
				break;
				}
		default: {
				what.x = 0;
				what.y = 0;
				what.h = 256;
				what.w = 256;
				break;
				}

		}
	SDL_Rect size = {startX, startY, FONT_H, FONT_W};
	SDL_RenderCopy(RENDERER, monstersTextures, &what, &size);

	//main_renderer(player,  &monster, &items, map);
	//Text_Renderer_C(RENDERER, FONT, startX, startY, 10, 15, &player->ch, color);
	}







void player_input(SDL_Event *event, Entitiy* player, Entitiy_DA *entitis, Item_DA *items, Tile* map) {
	const u32 key = event->key.keysym.sym;
	player->oldPos = player->pos;
	if(player->hunger > 0) {
		if(rand_f64() < CHANCE_HUNGER_DECRESE) {
			player->hunger--;
			CLAMP(player->hunger, 0, 255);
			}

		}
	else {
		player->health--;
		}

	MOVMENT = SDL_FALSE;  //NOT PROB
	if(EQUITEM == SDL_TRUE) {
		MOVMENT = SDL_TRUE;
		if(key >= '0' && key <= '9') {
			da_append(&BUFFER, key);
			}
		else if(key == KEY_BACKSPACE) {
			da_remove_last(&BUFFER);
			}
		else if(key == KEY_U) {
			EQUITEM = SDL_FALSE;
			MOVMENT = SDL_TRUE;
			i32 buffer = 0, p = 1;
			for(i32 i = BUFFER.count - 1; i >= 0; i--) {
				buffer += ((BUFFER.items[i] - '0') * p);
				p*=10;
				}
			//LOG("Buffer %d\n", buffer);
			equiped_item(&player->inventory, (u64)buffer);
			if(USEITEM == SDL_TRUE) {

				use_item(player, entitis, &player->inventory, map, (u64)buffer);
				da_append(&MESSAGES, "Use item:");
				USEITEM = SDL_FALSE;
				}
			BUFFER.count = 0;
			}
		}
	else if(player->isStunded > 0) {

		MOVMENT = SDL_TRUE;
		player->isStunded--;
		player->stamina++;
		CLAMP(player->stamina, 0, player->maxStamina);
		//CLAMP(player->isStunded, 0, 255);
		}
	else if(player->stamina <= 0) {
		da_append(&MESSAGES, "You are exhausted");
		player->isStunded = STAMINA_STUN_DURATION;
		player->stamina++;
		MOVMENT = SDL_TRUE;
		}
	else if(key == KEY_R) {
		i32 witch_monster = -1;
		MOVMENT = SDL_TRUE;
		//RANGE = SDL_TRUE;
		//LOG("Last %c", LASTKEY);
		switch(LASTKEY) {
			case UP_ARROW:
			case KEY_W: {
					witch_monster = is_monster_on_entity(player->pos.x, player->pos.y - 1, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x, player->pos.y - 2, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x, player->pos.y - 3, entitis);
					if(witch_monster != -1) {
						break;
						}
					break;
					}
			case DOWN_ARROW:
			case KEY_S: {
					witch_monster = is_monster_on_entity(player->pos.x, player->pos.y + 1, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x, player->pos.y + 2, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x, player->pos.y + 3, entitis);
					if(witch_monster != -1) {
						break;
						}
					break;
					}
			case RIGHT_ARROW:
			case KEY_D: {
					witch_monster = is_monster_on_entity(player->pos.x + 1, player->pos.y, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x + 2, player->pos.y, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x + 3, player->pos.y, entitis);
					if(witch_monster != -1) {
						break;
						}
					break;
					}
			case LEFT_ARROW:
			case KEY_A: {
					witch_monster = is_monster_on_entity(player->pos.x - 1, player->pos.y, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x - 2, player->pos.y, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x - 3, player->pos.y, entitis);
					if(witch_monster != -1) {
						break;
						}
					break;
					}
			case KEY_Q: {
					witch_monster = is_monster_on_entity(player->pos.x - 1, player->pos.y - 1, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x - 2, player->pos.y - 2, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x - 3, player->pos.y - 3, entitis);
					if(witch_monster != -1) {
						break;
						}
					break;
					}
			case KEY_E: {
					witch_monster = is_monster_on_entity(player->pos.x + 1, player->pos.y - 1, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x + 2, player->pos.y - 2, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x + 3, player->pos.y - 3, entitis);
					if(witch_monster != -1) {
						break;
						}
					break;
					}
			case KEY_Y:
			case KEY_Z: {
					witch_monster = is_monster_on_entity(player->pos.x - 1, player->pos.y + 1, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x - 2, player->pos.y + 2, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x - 3, player->pos.y + 3, entitis);
					if(witch_monster != -1) {
						break;
						}
					break;
					}
			case KEY_C: {
					witch_monster = is_monster_on_entity(player->pos.x + 1, player->pos.y + 1, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x + 2, player->pos.y + 2, entitis);
					if(witch_monster != -1) {
						break;
						}
					witch_monster = is_monster_on_entity(player->pos.x + 3, player->pos.y + 3, entitis);
					if(witch_monster != -1) {
						break;
						}
					break;
					}
			default: {
					break;
					}
			}
		//LOG("%d", witch_monster);
		if(witch_monster != -1) {
			//exit(-1);
			SDL_bool isF =  player_attack_range(player, &entitis->items[witch_monster], items, map);
			}
		else {
			//da_append(&MESSAGES, "Noting to attack i that direction");
			}

		}

	else if(key == UP_ARROW || key == KEY_W) {

		if(player->pos.y > 0 && MAP_ISW(map, player->pos.x, player->pos.y-1) == SDL_TRUE) {
			if(rand_f64() < player->chanceToDecressStaminaMove) {
				player->stamina--;
				CLAMP(player->stamina, 0, player->maxStamina);
				}
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.y--;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_FALSE;
			MOVMENT = SDL_TRUE;
			}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x,  player->pos.y-1, entitis);
			if(witchIsMonster != -1) {
				if(player->chanceToDecressStaminaMove < rand_f64())
					player->stamina-=rand()%2;
				CLAMP(player->stamina, 0, player->maxStamina);
				SDL_bool isF =  player_attack(player, &entitis->items[witchIsMonster], items, map);
				if(isF == SDL_TRUE) {
					MAP_ISW(map, entitis->items[witchIsMonster].pos.x, entitis->items[witchIsMonster].pos.y) = SDL_TRUE;
					free_entity(&entitis->items[witchIsMonster]);
					da_remove_unordered(entitis, witchIsMonster);
					}
				MOVMENT = SDL_TRUE;
				}
			}
		LASTKEY = key;
		}
	else if(key == DOWN_ARROW || key == KEY_S) {
		if(player->pos.y < MAP_Y && MAP_ISW(map, player->pos.x, player->pos.y+1) == SDL_TRUE) {
			if(rand_f64() < player->chanceToDecressStaminaMove)
				player->stamina--;
			CLAMP(player->stamina, 0, player->maxStamina);
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.y++;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_FALSE;
			MOVMENT = SDL_TRUE;
			}
		else {
			i32 witchIsMonster = is_monster_on_entity(player->pos.x,  player->pos.y + 1, entitis);
			if(witchIsMonster != -1) {
				player->stamina-=rand()%2;
				CLAMP(player->stamina, 0, player->maxStamina);
				SDL_bool isF =  player_attack(player, &entitis->items[witchIsMonster], items, map);
				if(isF == SDL_TRUE) {
					MAP_ISW(map, entitis->items[witchIsMonster].pos.x, entitis->items[witchIsMonster].pos.y) = SDL_TRUE;
					free_entity(&entitis->items[witchIsMonster]);
					da_remove_unordered(entitis, witchIsMonster);
					}
				MOVMENT = SDL_TRUE;
				}
			}
		LASTKEY = key;
		}
	else if(key == LEFT_ARROW || key == KEY_A) {
		if(rand_f64() < player->chanceToDecressStaminaMove)
			player->stamina--;
		CLAMP(player->stamina, 0, player->maxStamina);
		if(player->pos.x > 0 && MAP_ISW(map, player->pos.x-1, player->pos.y) == SDL_TRUE) {
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.x--;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			MOVMENT = SDL_TRUE;
			}
		else {
			player->stamina-=rand()%2;
			CLAMP(player->stamina, 0, player->maxStamina);
			i32 witchIsMonster = is_monster_on_entity(player->pos.x - 1,  player->pos.y, entitis);
			if(witchIsMonster != -1) {
				SDL_bool isF =  player_attack(player, &entitis->items[witchIsMonster], items, map);
				if(isF == SDL_TRUE) {
					MAP_ISW(map, entitis->items[witchIsMonster].pos.x, entitis->items[witchIsMonster].pos.y) = SDL_TRUE;
					free_entity(&entitis->items[witchIsMonster]);
					da_remove_unordered(entitis, witchIsMonster);
					}
				MOVMENT = SDL_TRUE;
				}
			}
		LASTKEY = key;
		}
	else if(key == RIGHT_ARROW || key == KEY_D) {

		if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x + 1, player->pos.y) == SDL_TRUE) {
			if(rand_f64() < player->chanceToDecressStaminaMove)
				player->stamina--;
			CLAMP(player->stamina, 0, player->maxStamina);
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.x++;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			MOVMENT = SDL_TRUE;
			}
		else {
			player->stamina-=rand()%2;
			CLAMP(player->stamina, 0, player->maxStamina);
			i32 witchIsMonster = is_monster_on_entity(player->pos.x + 1,  player->pos.y, entitis);
			if(witchIsMonster != -1) {
				SDL_bool isF =  player_attack(player, &entitis->items[witchIsMonster], items, map);
				if(isF == SDL_TRUE) {
					MAP_ISW(map, entitis->items[witchIsMonster].pos.x, entitis->items[witchIsMonster].pos.y) = SDL_TRUE;
					free_entity(&entitis->items[witchIsMonster]);
					da_remove_unordered(entitis, witchIsMonster);
					}
				MOVMENT = SDL_TRUE;
				}
			}
		LASTKEY = key;
		}
	else if(key == KEY_Q) {

		if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x - 1, player->pos.y - 1) == SDL_TRUE) {
			if(rand_f64() < player->chanceToDecressStaminaMove)
				player->stamina-=rand()%2;
			CLAMP(player->stamina, 0, player->maxStamina);
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.x--;
			player->pos.y--;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			MOVMENT = SDL_TRUE;
			}
		else {
			player->stamina-=rand()%2;
			CLAMP(player->stamina, 0, player->maxStamina);
			i32 witchIsMonster = is_monster_on_entity(player->pos.x - 1,  player->pos.y - 1, entitis);
			if(witchIsMonster != -1) {
				SDL_bool isF =  player_attack(player, &entitis->items[witchIsMonster], items, map);
				if(isF == SDL_TRUE) {
					MAP_ISW(map, entitis->items[witchIsMonster].pos.x, entitis->items[witchIsMonster].pos.y) = SDL_TRUE;
					free_entity(&entitis->items[witchIsMonster]);
					da_remove_unordered(entitis, witchIsMonster);
					}
				MOVMENT = SDL_TRUE;
				}
			}
		LASTKEY = key;
		}
	else if(key == KEY_E) {

		if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x + 1, player->pos.y - 1) == SDL_TRUE) {
			if(rand_f64() < player->chanceToDecressStaminaMove)
				player->stamina-=rand()%2;
			CLAMP(player->stamina, 0, player->maxStamina);
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.x++;
			player->pos.y--;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			MOVMENT = SDL_TRUE;
			}
		else {

			player->stamina-=rand()%2;
			CLAMP(player->stamina, 0, player->maxStamina);
			i32 witchIsMonster = is_monster_on_entity(player->pos.x + 1,  player->pos.y - 1, entitis);
			if(witchIsMonster != -1) {
				SDL_bool isF =  player_attack(player, &entitis->items[witchIsMonster], items, map);
				if(isF == SDL_TRUE) {
					MAP_ISW(map, entitis->items[witchIsMonster].pos.x, entitis->items[witchIsMonster].pos.y) = SDL_TRUE;
					free_entity(&entitis->items[witchIsMonster]);
					da_remove_unordered(entitis, witchIsMonster);
					}
				MOVMENT = SDL_TRUE;
				}
			}
		LASTKEY = key;
		}
	else if(key == KEY_Z || key == KEY_Y) {
		if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x - 1, player->pos.y + 1) == SDL_TRUE) {
			if(rand_f64() < player->chanceToDecressStaminaMove)
				player->stamina-=rand()%2;
			CLAMP(player->stamina, 0, player->maxStamina);
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.x--;
			player->pos.y++;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			MOVMENT = SDL_TRUE;
			}
		else {

			player->stamina-=rand()%2;
			CLAMP(player->stamina, 0, player->maxStamina);
			i32 witchIsMonster = is_monster_on_entity(player->pos.x - 1,  player->pos.y + 1, entitis);
			if(witchIsMonster != -1) {
				SDL_bool isF =  player_attack(player, &entitis->items[witchIsMonster], items, map);
				if(isF == SDL_TRUE) {
					MAP_ISW(map, entitis->items[witchIsMonster].pos.x, entitis->items[witchIsMonster].pos.y) = SDL_TRUE;
					free_entity(&entitis->items[witchIsMonster]);
					da_remove_unordered(entitis, witchIsMonster);
					}
				MOVMENT = SDL_TRUE;
				}
			}
		LASTKEY = key;
		}
	else if(key == KEY_C) {
		if(player->pos.x < MAP_X && MAP_ISW(map, player->pos.x + 1, player->pos.y + 1) == SDL_TRUE) {
			if(player->chanceToDecressStaminaMove < rand_f64())
				player->stamina-=rand()%2;
			CLAMP(player->stamina, 0, player->maxStamina);
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			player->pos.x++;
			player->pos.y++;
			MAP_ISW(map, player->pos.x, player->pos.y) = SDL_TRUE;
			MOVMENT = SDL_TRUE;
			}
		else {

			player->stamina-=rand()%2;
			CLAMP(player->stamina, 0, player->maxStamina);
			i32 witchIsMonster = is_monster_on_entity(player->pos.x + 1,  player->pos.y + 1, entitis);
			if(witchIsMonster != -1) {
				SDL_bool isF =  player_attack(player, &entitis->items[witchIsMonster], items, map);
				if(isF == SDL_TRUE) {
					MAP_ISW(map, entitis->items[witchIsMonster].pos.x, entitis->items[witchIsMonster].pos.y) = SDL_TRUE;
					free_entity(&entitis->items[witchIsMonster]);
					da_remove_unordered(entitis, witchIsMonster);
					}
				MOVMENT = SDL_TRUE;
				}
			}
		LASTKEY = key;
		}

	else if(key == SPACE) {
		//DO NOTHING
		player->stamina++;
		CLAMP(player->stamina, 0, player->maxStamina);
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
	else if(key == KEY_O) {
		char* msg = "Casted spell of moving rock!!!";
		da_append(&MESSAGES, msg);
		OPENDOOR = SDL_TRUE;
		MOVMENT = SDL_TRUE;
		BUFFER.count = 0;
		//LOG("O\n");
		}
	else if(key == KEY_U) {
		char* msg = "What item you want to equ ";
		da_append(&MESSAGES, msg);
		da_append(&MESSAGES, "Type what item you want to equip");
		MOVMENT = SDL_TRUE;
		EQUITEM = SDL_TRUE;
		}

	}


void render_stats(Entitiy *player) {
	char stats[1024];
	i32 startX = 0;
	i32 startY = HEIGHT - 300;
	SDL_Rect temp = {startX + 10, startY, 600, 150};
	//SDL_ERR(SDL_SetRenderDrawColor(RENDERER, 0X40, 0X20, 0X20, 0XFF));
	//SDL_RenderFillRect(RENDERER, &temp);
	snprintf(stats, 1024, "STATS: Health %d", player->health);
	Text_Renderer_C(RENDERER, FONT,startX - 3, startY, 200, FONT_H_MESSAGES, stats, WHITE);
	Damage_Types i = 0;
	snprintf(stats, 1024, "STATS: %s att: %d", damageStr[i], player->attack[i]);
	Text_Renderer_C(RENDERER, FONT, startX, startY + FONT_H_MESSAGES*(i+1), 200,
	                FONT_H_MESSAGES, stats, WHITE);
	i = 0;
	for(; i < DAMAGE_NUM; i++) {
		stats[0] = '\0';
		snprintf(stats, 1024, "STATS: %s def: %d", damageStr[i], player->defence[i]);
		Text_Renderer_C(RENDERER, FONT, startX, startY + FONT_H_MESSAGES*(i+2), 200,
		                FONT_H_MESSAGES, stats, WHITE);
		}
	stats[0] = '\0';
	snprintf(stats, 1024, "STATS: MAXSTM: %d STM: %d",player->maxStamina, player->stamina);
	Text_Renderer_C(RENDERER, FONT, startX, startY + FONT_H_MESSAGES*(i+2), 200,
	                FONT_H_MESSAGES, stats, WHITE);
	i++;
	stats[0] = '\0';
	if(player->hunger > 10) {
		snprintf(stats, 1024, "STATS: NOT HUNGRY        ");
		}
	else {
		snprintf(stats, 1024, "STATS: HUNGRY        ");
		}



	Text_Renderer_C(RENDERER, FONT, startX, startY + FONT_H_MESSAGES*(i+2), 200,
	                FONT_H_MESSAGES, stats, WHITE);

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
		//f64 distance = DISTANCE(player->pos.x, player->pos.y, monsters->items[count].pos.x, monsters->items[count].pos.y);
		if(MAP_ISV(map, monsters->items[count].pos.x, monsters->items[count].pos.y) == SDL_TRUE) {
			//render_player(&monsters->items[count]);
			render_player_texture(&monsters->items[count]);
			//SDL_RenderPresent(RENDERER);
			//SDL_Delay(10);
			}

		}

	}

void render_messages(i32 startX, i32 startY, char* message) {
	if(message != NULL) {
		u64 w_c = strlen(message) * 16;
		Text_Renderer_C(RENDERER, FONT, startX, startY, (i32)w_c, 20, message, RED);
		}

	}

void render_item(Item* item, Tile* map) {

	i32 x = item->pos.x;
	i32 y = item->pos.y;
	i32 startX = x * FONT_W - CAMERA.x;
	i32 startY = y * FONT_H - CAMERA.y;
	//char ch[2];
	//ch[0] = item->ch;
	//ch[1] = '\0';
	//LOG("\nCHAR %c\n", ch);

	if(MAP_ISW(map, x, y) == SDL_TRUE && MAP_ISV(map, x, y) == SDL_TRUE) {
		//SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
		//SDL_RenderFillRect(RENDERER, &temp);
		//Text_Renderer_C(RENDERER, FONT, startX, startY, 10, 15, ch, item->color);
		u8 isRange = SDL_FALSE;
		u8 isScrol = SDL_FALSE;
		SDL_Rect textSize = {startX, startY, FONT_H, FONT_W}, what = {0, 0, 0, 0};
		switch(item->type) {
			case SWORD_ITEM: {
					what.x = 0;
					what.y = 0;
					what.h = 256;
					what.w = 240;
					break;
					}
			case PLAYER_SWORD_ITEM: {
					what.x = 0;
					what.y = 0;
					what.h = 256;
					what.w = 240;
					break;
					}
			case AXE_ITEM: {
					what.x = 768;
					what.y = 0;
					what.h = 256;
					what.w = 240;
					break;
					}
			case DAGER_ITEM: {
					what.x = 580;
					what.y = 0;
					what.h = 256;
					what.w = 240;
					break;
					}
			case PIKE_ITEM: {
					what.x = 400;
					what.y = 256;
					what.h = 256;
					what.w = 240;
					break;
					}
			case SABER_ITEM: {
					what.x = 570;
					what.y = 0;
					what.h = 256;
					what.w = 240;
					break;
					}
			case BOW_ITEM: {
					what.x = 341;
					what.y = 0;
					what.h = 1100;
					what.w = 300;
					isRange = SDL_TRUE;
					break;
					}
			case ARROW_ITEM: {
					what.x = 641;
					what.y = 0;
					what.h = 1024;
					what.w = 341;
					isRange = SDL_TRUE;
					break;
					}

			case ARMOR_ITEM: {
					what.x = 160;
					what.y = 256;
					what.h = 256;
					what.w = 240;
					break;
					}
			case HELMET_ITEM: {
					what.x = 540;
					what.y = 256;
					what.h = 256;
					what.w = 240;
					break;
					}
			case SHIELD_ITEM: {
					what.x = 780;
					what.y = 256;
					what.h = 256;
					what.w = 240;
					break;
					}
			case SHOES_ITEM: {
					what.x = 780;
					what.y = 512;
					what.h = 256;
					what.w = 240;
					break;
					}

			case 		SCROL_TELEPORT_ITEM:  //FIRST
			case 		SCROL_AGREGATE_ITEM:
			case 		SCROL_ENCHANTING_ITEM:
			case 		SCROL_RECHARGING_ITEM:
			case 		SCROL_REPEL_ITEM:
			case 		SCROL_SUMMON_ITEM:
			case 		SCROL_SCARE_ITEM:
			case 		SCROL_ACQ_ITEM:
			case 		SCROL_IDENT_ITEM: {
					what.x = 0;
					what.y = 0;
					what.h = 1024;
					what.w = 1024;
					isScrol = SDL_TRUE;
					isRange = SDL_TRUE;
					break;
					}

			case HEALING_ITEM: {
					what.x = 0;
					what.y = 760;
					what.h = 256;
					what.w = 240;
					break;
					}

			case STRENGTH_POTION: {
					what.x = 0;
					what.y = 760;
					what.h = 256;
					what.w = 240;
					break;
					}
			case AGILITY_POTION: {
					what.x = 0;
					what.y = 760;
					what.h = 256;
					what.w = 240;
					break;
					}
			case DEFENCE_POTION: {
					what.x = 0;
					what.y = 760;
					what.h = 256;
					what.w = 240;
					break;
					}
			case VITALITY_POTION: {
					what.x = 0;
					what.y = 760;
					what.h = 256;
					what.w = 240;
					break;
					}
			case APPLE_ITEM: {
					what.x = 160;
					what.y = 500;
					what.h = 256;
					what.w = 240;
					break;
					}
			case MEAT_ITEM: {
					what.x = 400;
					what.y = 500;
					what.h = 256;
					what.w = 240;
					break;
					}
			case BERRY_ITEM: {
					what.x = 580;
					what.y = 700;
					what.h = 256;
					what.w = 240;
					break;
					}
			case GOLD_ITEM: {
					what.x = 760;
					what.y = 700;
					what.h = 256;
					what.w = 240;
					break;
					}
			default: {

					break;
					}
			}
		if(!isRange) {
			SDL_RenderCopy(RENDERER, itemTextures, &what, &textSize);
			}
		else if(isScrol) {
			SDL_RenderCopy(RENDERER, scrolTextures, &what, &textSize);
			//exit(-1);
			}
		else {
			SDL_RenderCopy(RENDERER, rangeItemsTextures, &what, &textSize);
			}

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
	i32 startX = 	700;
	i32 startY = 300; //THIS DEPENS CUZZ HOW MUTCH MESSAGES
	//i32 h = 580/2;
	//i32 w = 600;

	//SDL_Rect rec = {startX, startY, w, h};
	//SDL_SetRenderDrawColor(RENDERER, 0, 30, 10, 0);
	//SDL_RenderFillRect(RENDERER, &rec);
	i32 buffer = 0, p = 1, count  = 0;
	for(i32 i = BUFFER.count - 1; i >= 0; i--) {
		buffer += ((BUFFER.items[i] - '0') * p);
		p*=10;
		}
	for(u64 i = (u64)buffer; i < inventory->count && EQUITEM; i++) {
		//LOG("NAME:%s\n", inventory->items[i].name);
		char msg[256];
		snprintf(msg, 256, "%d. %s", (i16)i, inventory->items[i].descripction);
		render_messages(startX, startY + ((count++)*FONT_H_MESSAGES), msg);
		//Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H_MESSAGES, inventory->items[i].name, WHITE);
		}
	}


void render_map_fov_vision(Entitiy *player, Tile *map) {
	//field_of_vison(player, map);
	i32 radius = RADIUS;
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
			f64 distance = DISTANCE(x, y, player->pos.x, player->pos.y);
			//distance*=distance;
			if(MAP_ISV(map, x, y) == SDL_TRUE) {
				i32 startX = x * FONT_W;
				i32 startY = y * FONT_H;
				char ch = MAP_CH(map, x, y);
				if(ch == TILE_BLOCKED) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					}
				else if(ch == TILE_WALL ) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 80, 80, 80, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					}
				else if(ch == TILE_ROAD) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					} // if(ch != TILE_FLOOR)
				else if(ch == TILE_RUINS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "+", WHITE);
					}
				else if(ch == TILE_BLIGHT) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0);
					SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, ".", BLACK);
					}
				else if(ch == TILE_TREE) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, ":", GREEN);
					}
				else if(ch == TILE_GRASS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "\"", GREEN);
					}
				else if(ch == TILE_POISION) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, ".", GREEN);
					}
				else if(ch == '-') {
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
			else if (MAP_VISITED(map, x, y) == SDL_TRUE&&0) {
				i32 startX = x * FONT_W;
				i32 startY = y * FONT_H;
				char ch = MAP_CH(map, x, y);
				if(ch == TILE_BLOCKED) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 60, 60, 60, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					}
				else if(ch == TILE_WALL ) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 40, 40, 40, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					}
				else if(ch == TILE_RUINS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "+", WHITE);
					}
				else if(ch == '-') {
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
				SDL_SetRenderDrawColor(RENDERER, 0X20, 0X20, 0X20, 0XFF);
				//SDL_SetRenderDrawColor(RENDERER, 255, 0, 0, 0);
				SDL_RenderFillRect(RENDERER, &textRect);
				DROP(textRect);
				}
			}
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
				else if(ch == '/' ) {
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
				else if(ch == '+') {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "+", WHITE);
					}


				else if(ch == '-') {
					SDL_Rect textRect = {.x=startX-1, .y = startY-1, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 0x40, 0x15, 0x15, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "-", WHITE);
					}
				else if(ch == TILE_STAIRS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "<", WHITE);
					}
				else if(ch == TILE_BLIGHT) {
					//SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0);
					//SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, ".", BLACK);
					}
				else if(ch == TILE_TREE) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, ":", GREEN);
					}
				else if(ch == TILE_GRASS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "\"", YELLOW);
					}
				else if(ch == TILE_POISION) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "-", GREEN);
					}
				else if(ch == TILE_GARG_STAT) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "S", GREEN);
					}

				else {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 10, 10, 10, 100);
					SDL_RenderDrawRect(RENDERER, &textRect);
					DROP(textRect);
					}

				}
			else if (MAP_VISITED(map, x, y) == SDL_TRUE) {
				i32 startX = x * FONT_W;
				i32 startY = y * FONT_H;
				char ch = MAP_CH(map, x, y);
				if(ch == '#') {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 60, 60, 60, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					}
				else if(ch == '/' ) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 40, 40, 40, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					}
				else if(ch == '+') {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "+", WHITE);
					}
				else if(ch == '-') {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 0x40, 0x15, 0x15, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "-", WHITE);
					}
				else if(ch == TILE_STAIRS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "<", WHITE);
					}
				else if(ch == TILE_BLIGHT) {
					//SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0);
					//SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, ".", BLACK);
					}
				else if(ch == TILE_TREE) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, ":", GREEN);
					}
				else if(ch == TILE_GRASS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
					//DROP(textRect);
					SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					SDL_RenderDrawRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "\"", YELLOW);
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

static void update_camera(Entitiy* player, i32 map_width, i32 map_height) {
	// Center camera on player
	i32 desired_x = player->pos.x * FONT_W - CAMERA.w/2;
	i32 desired_y = player->pos.y * FONT_H - CAMERA.h/2;
	//CAMERA.w = 1000;
	//CAMERA.h = 600;
	// Clamp to map boundaries
	CAMERA.x = desired_x;
	CAMERA.y = desired_y;
	CLAMP(CAMERA.x, 0, map_width * FONT_W - CAMERA.w);
	CLAMP(CAMERA.y, 0, map_height * FONT_H - CAMERA.h);
	}



void render_map_graphical(Entitiy *player, Tile *map) {
//field_of_vison(player, map);
	update_camera(player, MAP_X, MAP_Y);
	i32 radius = player->radius;
	i32 startX = (player->pos.x) - radius;
	i32 startY = ( player->pos.y) - radius;
	i32 stopX  = ( player->pos.x) + radius;
	i32 stopY  = ( player->pos.y) + radius;
	CLAMP(startX, 0, MAP_X-1);
	CLAMP(stopX,  0, MAP_X-1);
	CLAMP(startY, 0, MAP_Y-1);
	CLAMP(stopY,  0, MAP_Y-1);
	i32 sW  = FONT_W;
	i32 sH  = FONT_H;
	i32 ofsetX = player->pos.x;
	i32 ofsetY = player->pos.y;
	for(i32 y  = 0; y < MAP_Y; y++) {
		for(i32 x = 0; x < MAP_X; x++) {

			f64 distance = DISTANCE(x, y, player->pos.x, player->pos.y);
			//distance*=distance;
			if(MAP_ISV(map, x, y) == SDL_TRUE) {
				i32 startX = x * sW - CAMERA.x;
				i32 startY = y * sH - CAMERA.y;
				if(startX + sW < 0 || startX > CAMERA.w ||
				    startY + sH < 0 || startY > CAMERA.h) continue;

				char ch = MAP_CH(map, x, y);
				if(ch == TILE_BLOCKED) {
					SDL_Rect textRect = {.x = startX, .y = startY, .w = sW, .h = sH};
					//SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
					//SDL_RenderFillRect(RENDERER, &textRect);
					SDL_RenderCopy(RENDERER, wallTextures, NULL, &textRect);

					}
				else if(ch == TILE_WALL ) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					//SDL_SetRenderDrawColor(RENDERER, 80, 80, 80, 100);
					//SDL_RenderFillRect(RENDERER, &textRect);
					SDL_RenderCopy(RENDERER, wallTextures, NULL, &textRect);
					}
				else if(ch == TILE_ROAD || ch == TILE_FLOOR) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					//DROP(textRect);
					//SDL_SetRenderDrawColor(RENDERER, 20, 10, 10, 255);
					//SDL_SetRenderDrawColor(RENDERER, 40, 40, 40, 100);
					//SDL_RenderDrawRect(RENDERER, &textRect);
					SDL_RenderCopy(RENDERER, groundTextures, NULL, &textRect);
					} // if(ch != TILE_FLOOR)
				else if(ch == TILE_RUINS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_RenderCopy(RENDERER, wallTextures, NULL, &textRect);
					textRect.w = sW+10;
					textRect.h = sH+10;
					SDL_RenderCopy(RENDERER, boulderTextures, NULL, &textRect);
					}
				else if(ch == TILE_BLIGHT) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_RenderCopy(RENDERER, blightTextures, NULL, &textRect);
					}
				else if(ch == TILE_TREE) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_RenderCopy(RENDERER, groundTextures, NULL, &textRect);
					SDL_RenderCopy(RENDERER, treeTextures, NULL, &textRect);
					}
				else if(ch == TILE_GRASS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					//DROP(textRect);
					SDL_RenderCopy(RENDERER, groundTextures, NULL, &textRect);
					SDL_RenderCopy(RENDERER, grassTextures, NULL, &textRect);
					}
				else if(ch == TILE_POISION) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};

					SDL_RenderCopy(RENDERER, poisonTextures, NULL, &textRect);
					}
				else if(ch == TILE_STAIRS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					//DROP(textRect);
					SDL_RenderCopy(RENDERER, groundTextures, NULL, &textRect);
					SDL_RenderCopy(RENDERER, stairTextures, NULL, &textRect);
					}
				else if(ch == TILE_REPEL) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_RenderCopy(RENDERER, glyphTextures, NULL, &textRect);
					}
				else if(ch == '-') {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_SetRenderDrawColor(RENDERER, 0x40, 0x15, 0x15, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, sW, sH, "-", WHITE);
					}

				else {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_SetRenderDrawColor(RENDERER, 10, 10, 10, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					DROP(textRect);
					}

				}
			else if (MAP_VISITED(map, x, y) == SDL_TRUE) {
				i32 startX = x * sW  - CAMERA.x;
				i32 startY = y * sH  - CAMERA.y;
				char ch = MAP_CH(map, x, y);
				if(ch == TILE_BLOCKED) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					//SDL_SetRenderDrawColor(RENDERER, 60, 60, 60, 100);
					//SDL_RenderFillRect(RENDERER, &textRect);
					SDL_RenderCopy(RENDERER, wallTextures, NULL, &textRect);
					}
				else if(ch == TILE_WALL ) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					//SDL_SetRenderDrawColor(RENDERER, 40, 40, 40, 100);
					//SDL_RenderFillRect(RENDERER, &textRect);
					SDL_RenderCopy(RENDERER, wallTextures, NULL, &textRect);
					}
				else if(ch == TILE_BLIGHT) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_RenderCopy(RENDERER, blightTextures, NULL, &textRect);
					}
				else if(ch == TILE_RUINS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_RenderCopy(RENDERER, wallTextures, NULL, &textRect);
					textRect.w = sW+10;
					textRect.h = sH+10;
					SDL_RenderCopy(RENDERER, boulderTextures, NULL, &textRect);
					}
				else if(ch == TILE_STAIRS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					//DROP(textRect);
					SDL_RenderCopy(RENDERER, groundTextures, NULL, &textRect);
					SDL_RenderCopy(RENDERER, stairTextures, NULL, &textRect);
					}
				else if(ch == TILE_REPEL) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_RenderCopy(RENDERER, glyphTextures, NULL, &textRect);
					}
				else if(ch == TILE_GRASS) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					//DROP(textRect);
					SDL_RenderCopy(RENDERER, groundTextures, NULL, &textRect);
					SDL_RenderCopy(RENDERER, grassTextures, NULL, &textRect);
					}
				else if(ch == TILE_TREE) {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_RenderCopy(RENDERER, groundTextures, NULL, &textRect);
					SDL_RenderCopy(RENDERER, treeTextures, NULL, &textRect);
					}
				else if(ch == '-') {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					SDL_SetRenderDrawColor(RENDERER, 0x40, 0x15, 0x15, 100);
					SDL_RenderFillRect(RENDERER, &textRect);
					Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, "-", WHITE);
					}
				else {
					SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
					//SDL_SetRenderDrawColor(RENDERER, 10, 10, 10, 100);
					//SDL_RenderFillRect(RENDERER, &textRect);
					SDL_RenderCopy(RENDERER, groundTextures, NULL, &textRect);
					DROP(textRect);
					}
				}
			else {
				i32 startX = x * sW  - CAMERA.x;
				i32 startY = y * sH  - CAMERA.y;
				SDL_Rect textRect = {.x=startX, .y = startY, .w = sW, .h = sH};
				SDL_SetRenderDrawColor(RENDERER, 0X0, 0X0, 0X0, 0XFF);
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
			//LOG("Color %lu\n", c);
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
					if(ch == TILE_BLOCKED) {
						SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
						SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 100);
						SDL_RenderFillRect(RENDERER, &textRect);
						}
					else if(ch == TILE_BLIGHT) {
						SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
						SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0);
						SDL_RenderFillRect(RENDERER, &textRect);
						//Text_Renderer_C(RENDERER, FONT, startX, startY, FONT_W, FONT_H, ".", BLIGHT_COL);
						}
					else if(ch == TILE_ROAD) {
						SDL_Rect textRect = {.x=startX, .y = startY, .w = FONT_W, .h = FONT_H};
						//DROP(textRect);
						SDL_SetRenderDrawColor(RENDERER, 100, 10, 10, 100);
						SDL_RenderFillRect(RENDERER, &textRect);
						} // if(ch != TILE_FLOOR)
					else if(ch == TILE_RUINS) {
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
					else if(ch == TILE_WALL) {
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
			//render_map_fov(player, map);
			//render_map_dikstra(player, map);
			render_map_graphical(player, map);
			render_player(player);
			//render_player(&monster->items[0]);
			if(ITEMSREND == SDL_TRUE) {
				render_items(items, map, player);
				}

			render_inventory(&player->inventory);
			render_stats(player);
			i32 count = 1;

			if(EQUITEM == SDL_FALSE) {
				for(i32 i = (i32)MESSAGES.count-1; i >= ((i32)MESSAGES.count - NUM_RENDER_MSG); i--) {
					render_messages(400, (0 + FONT_H_MESSAGES*(count++)), MESSAGES.items[i]);
					}
				}
			else {
				i32 count = 1;
				for(i32 i = (i32)MESSAGES.count-1; i >= ((i32)MESSAGES.count - NUM_RENDER_MSG); i--) {
					render_messages(400, (0 + FONT_H_MESSAGES*(count++)), MESSAGES.items[i]);
					}
				i32 buffer = 0, p = 1;
				for(i32 i = BUFFER.count - 1; i >= 0; i--) {
					buffer += ((BUFFER.items[i] - '0') * p);
					p*=10;
					}
				char* msg = calloc(MAX_NAME, sizeof(char));
				snprintf(msg, MAX_NAME, "You typed %d", buffer);
				da_append(&MESSAGES, msg);
				}
			//SDL_ERR(SDL_SetRenderDrawColor(RENDERER, 0X20, 0X20, 0X20, 0XFF));
			render_monsters(monster, player, map);

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
						FONT_W = 70;
						FONT_H = 70;
						CAMERA.w = WIDTH;
						CAMERA.h = HEIGHT;
						//FONT_H = 6;
						//FONT_W = 10;
						if(WIDTH > 4096 || HEIGHT > 2048) {
							ASSERT("Oversized window\n");
							}
						//LOG("width %d, height %d\n", WIDTH, HEIGHT);
						}
					else if(EVENT.type == SDL_KEYDOWN) {
						player_input(&EVENT, player, entitis, items, map);
						//
						player_negative_effect(player, map);
						//SDL_Delay(1);
						}
					}
				EVENT.type = 0;
				}
			}



