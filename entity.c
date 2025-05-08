#include "entity.h"


Entitiy* create_entity(char ch, const char* name, i32 radius, i32 health, Position startPos, SDL_Color color) {
	Entitiy* entity = calloc(1, sizeof(Entitiy));
	entity->health = health;
	entity->maxHealth = health;

	if(ch != '@') {
		for(Monster_Types m = 0; m < NUM_MONSTER; m++) {
			if(ch == monsterChar[m]) {
				memcpy(entity, &monsters[m], sizeof(Entitiy));
				}
			}
		}
	else {
		entity->ch = ch;
		entity->radius = radius;
		}
	if(entity->color.r == 0 && entity->color.g == 0 && entity->color.b == 0) {
		entity->color = color;
		}
	entity->pos.x = startPos.x;
	entity->pos.y = startPos.y;
	entity->isAlive = SDL_TRUE;
	memset(&entity->inventory, 0, sizeof(Item_DA));
	if(name != NULL) {
		i32 len = strlen(name);
		CLAMP(len, 1, (MAX_NAME-1));
		entity->name = calloc(NUM_MONSTER, sizeof(char*));
		memcpy(entity->name, name, len);
		}
	if(ch != '@') {

		Item *GOLD;

		i32 chance = rand()%NUM_ITEM;
		CLAMP(chance, HEALING_ITEM, GOLD_ITEM);
		switch(chance) {
			case HEALING_ITEM: {
					GOLD = create_item(37, 38, HEALING_CREATE());
					break;
					}
			case GOLD_ITEM: {
					GOLD = create_item(0, 0, GOLD_CREATE());
					break;
					}
			default: {
					ASSERT("UNRECHABLE");
					break;
					}
			}
		Item i = *GOLD;
		da_append(&entity->inventory, (i));
		LOG("Gold\n");
		if(rand_f64() < CHANCE_MONSTER_HAVE_ITEM * LEVEL) {

			Item *item  = create_item(0, 0, SWORD_CREATE());
			item->isEquiped = SDL_TRUE;
			i = *item;
			da_append(&entity->inventory, (i));
			}
		if(rand_f64() < CHANCE_MONSTER_HAVE_ITEM * LEVEL) {
			Item *item1  = create_item(0, 0, ARMOR_CREATE());
			item1->isEquiped = SDL_TRUE;
			i = *item1;
			da_append(&entity->inventory, (i));
			}
		if(rand_f64() < CHANCE_MONSTER_HAVE_ITEM * LEVEL) {
			Item* helmet = create_item(40, 38, HELMET_CREATE());
			helmet->isEquiped = SDL_TRUE;
			i = *helmet;
			da_append(&entity->inventory, (i));
			}
		if(rand_f64() < CHANCE_MONSTER_HAVE_ITEM * LEVEL) {
			Item* shield = create_item(36, 40, SHIELD_CREATE());
			shield->isEquiped = SDL_TRUE;
			i = *shield;
			da_append(&entity->inventory, (i));

			}
		if(rand_f64() < CHANCE_MONSTER_HAVE_ITEM * LEVEL) {
			Item* shoes = create_item(36, 38, SHOES_CREATE());
			shoes->isEquiped = SDL_TRUE;
			i = *shoes;
			da_append(&entity->inventory, (i));
			da_append(&entity->inventory, (*shoes));
			}
		//player
		//if(ch == '@') {

		//	}
		}
	return entity;
	}
void free_entity(Entitiy* ent) {
	free(ent->name);
	//TBD FREE INVENTORY;
	//free(ent);
	}


i32 roll_the_dice(i32 attack, i32 defence) {
	f64 maxAttack = 0.0f, maxDefence = 0.0f, num;
	//ATTACK
	for(i32 i = 0; i < attack; i++) {
		num = rand_f64();

		if(num > maxAttack) {
			maxAttack = num;
			}
		}
	//DEFENCE
	for(i32 i = 0; i < defence; i++) {
		num = rand_f64();
		if(num > maxDefence) {
			maxDefence = num;
			}
		}

	if(maxDefence > maxAttack) {
		if(rand_f64() < CHANCE_NEGATIVE_DAMAGE) {
			return (i32)((maxAttack - maxDefence) * defence);
			}
		return 0;
		}
	i32 diff;
	if(rand_f64() < CHANCE_USE_DEF) {
		diff = (i32)((maxAttack - maxDefence) * attack);
		}
	else {
		diff = (i32)((maxAttack) * attack);
		}
	CLAMP(diff, 1, INF);
	return diff;
	}

///TBD reusable
void message_attacked_by_monster(Entitiy* player, Entitiy* entity, i32 damage, Damage_Types type) {
	DROP(player);
	u64 len = 100;
	char* attackText = malloc(len*sizeof(char*));
	memset(attackText, '\0', len);
	if(attackText == NULL) {
		ASSERT("CALLOC FAILED\n");
		}
	i32 err = snprintf(attackText, len, "You are attacked by %s %s (-%d)", entity->name, damageStr[type], damage);
	if(err < -1) {
		ASSERT("snprintf failed");
		}
	//LOG("%s\n", attackText);
	da_append(&MESSAGES, attackText);
	//add_to_str(attackText, &MESSAGES);
	//Text_Renderer_C(RENDERER, FONT, WIDTH - 100, HEIGHT - 100, 100, 20, "Da li ovo radi", WHITE);
	}

void message_attacked_by_player(Entitiy* player, Entitiy* entity, i32 damage) {
	DROP(player);
	DROP(damageStr);
	u64 len = strlen("x attacked by you damage (xxxx)") + 10;
	char* attackText = malloc(len*sizeof(char*));
	memset(attackText, '\0', len);
	if(attackText == NULL) {
		ASSERT("CALLOC FAILED\n");
		}
	i32 err = snprintf(attackText, len, "%c attacked by you damage (%d)", entity->ch, damage);
	if(err < -1) {
		ASSERT("snprintf failed");
		}
	//LOG("%s\n", attackText);
	da_append(&MESSAGES, attackText);
	//add_to_str(attackText, &MESSAGES);
	//Text_Renderer_C(RENDERER, FONT, WIDTH - 100, HEIGHT - 100, 100, 20, "Da li ovo radi", WHITE);
	}

//TBD CURSED ITEMS
//TBD attack type
SDL_bool player_attack(Entitiy *player, Entitiy* entity, Item_DA *items, Tile* map) {
	DROP(map);
	i32 iPl = 0, iEnt = 0;
	for(u64 i = 0; i < player->inventory.count; i++) {
		Item item = player->inventory.items[i];
		if(item.isEquiped == SDL_TRUE) {
			//MAYBE CHANCE TO DO SOMTHING
			//TBD CHANGE THIS STUFF TO GET ALL TYPES WHEN ADDED
			if(rand_f64() < CHANCE_ITEM_USED_IN_COMBAT) {
				iPl += item.attack[0];
				}
			}
		}
	for(u64 i = 0; i < entity->inventory.count; i++) {
		Item item = entity->inventory.items[i];
		if(item.isEquiped == SDL_TRUE) {
			//MAYBE CHANCE TO DO SOMTHING
			//TBD CHANGE THIS STUFF TO GET ALL TYPES WHEN ADDED
			if(rand_f64() < CHANCE_ITEM_USED_IN_COMBAT) {
				iEnt += item.defence[0];
				}
			}
		}
	//LOG("player att %d decOrInc %d\n", player->attack[0] ,iPl );
	//LOG("entity def %d decOrInc %d\n", entity->attack[0] ,iEnt );
	i32 damage;
	if(rand_f64() < CHANCE_PLAYER_LEVEL) {
		damage = roll_the_dice(player->attack[0] + iPl, entity->defence[0] + iEnt + rand()%LEVEL);
		}
	else {
		damage = roll_the_dice(player->attack[0] + iPl, entity->defence[0] + iEnt);
		}

	if(damage < 0) {
		player->health+=damage;
		}
	else {
		entity->health-=damage;
		}

	CLAMP(entity->health, 0, 100);
	message_attacked_by_player(player, entity, damage);
	//DROP(player);
	//i32 x1 = player->pos.x;
	//i32 y1 = player->pos.y;

	//i32 x2 = entity->pos.x;
	//i32 y2 = entity->pos.y;
	//f64 distance = DISTANCE(x1, y1, x2, y2);
	//if(distance == 0.0f) {
	//	}
	if(entity->health == 0) {
		entity->isAlive = SDL_FALSE;
		//entity->ch = 'S';
		//DROP ITEMS
		for(u64 i = 0; i < entity->inventory.count; i++) {
			SDL_bool drop = SDL_FALSE;
			i32 mod = 3;
			i32 neg = -1;
			while(drop == SDL_FALSE) {

				drop = SDL_TRUE;
				i32 x = rand()%mod + neg;
				i32 y = rand()%mod + neg;
				//KINDA WE DO NOT SEE THEM IF IT DROP IN A WALL LIKE IF IT IS LOST
				//ITS STUPID
				Item item = entity->inventory.items[i];
				x+=entity->pos.x;
				y+=entity->pos.y;
				for(u64 j = 0; j < items->count; j++) {
					if(x == items->items[j].pos.x && y == items->items[j].pos.y) {

						drop = SDL_FALSE;
						mod++;
						neg--;
						break;
						}
					}
				if(drop == SDL_TRUE) {
					item.pos.x = x;
					item.pos.y = y;
					//LOG("Droped(%d %d)\n\n", x, y);
					//FOR DROPING WHAT
					if(item.type <= SHOES_ITEM) {
						if(rand_f64() < CHANCE_DROP_ITEM) {
							da_append(items, item);
							}
						}
					else {
						if(rand_f64() < CHANCE_DROP_ITEM_CONS) {
							da_append(items, item);
							}
						}
					}
				}
			}
		return SDL_TRUE;
		}
	else {
		return SDL_FALSE;
		}

	}


//TBD CURSED ITEMS
void monster_attack(Entitiy *player, Entitiy* entity, f64 distance) {
	//DROP(entity);
	i32 iD[DAMAGE_NUM] = {0}, iA[DAMAGE_NUM] = {0};
	//LEVEL SCALING
	if(rand_f64() < CHANCE_PLAYER_LEVEL) {
		for(i32 i = 0; i < DAMAGE_NUM; i++) {
			iA[i] += rand()%LEVEL;
			}
		}
	i32 lf = 0;
	f64 lfC = 0.0f;
	for(u64 i = 0; i < player->inventory.count; i++) {
		Item item = player->inventory.items[i];
		if(item.isEquiped == SDL_TRUE) {
			//MAYBE CHANCE TO DO SOMTHING
			//TBD CHANGE THIS STUFF TO GET ALL TYPES WHEN ADDED
			if(rand_f64() < CHANCE_ITEM_USED_IN_COMBAT) {
				for(i32 j = 0; j < DAMAGE_NUM; j++) {
					iD[j] += item.defence[j];
					}
				}
			}
		}
	for(u64 i = 0; i < entity->inventory.count; i++) {
		Item item = entity->inventory.items[i];
		//da_append(&MESSAGES, "MonsterItems");
		//da_append(&MESSAGES, item.descripction);
		if(item.isEquiped == SDL_TRUE) {
			//MAYBE CHANCE TO DO SOMTHING
			//TBD CHANGE THIS STUFF TO GET ALL TYPES WHEN ADDED
			if(rand_f64() < CHANCE_ITEM_USED_IN_COMBAT) {
				for(i32 j = 0; j < DAMAGE_NUM; j++) {
					iA[j] += item.attack[j];
					//LOG("ia %d\n", iA[j]);
					lfC += item.lifeStealChance;
					lf  += item.lifeSteal;
					}
				}
			}
		}

	i32 damage = 0;
	if(distance >= DISTANCE_RANGE_ATTACK_MIN && distance <= DISTANCE_RANGE_ATTACK_MAX) {
		if(entity->attack[DAMAGE_RANGE] > entity->attack[DAMAGE_SPELL]) {
			damage = roll_the_dice(entity->attack[DAMAGE_RANGE] + iA[DAMAGE_RANGE], player->defence[DAMAGE_RANGE]  + iD[DAMAGE_RANGE]);
			if(entity->attack[DAMAGE_RANGE] != 0) {
				message_attacked_by_monster(player, entity, damage, DAMAGE_RANGE);
				}
			}
		else {
			damage = roll_the_dice(entity->attack[DAMAGE_SPELL] + iA[DAMAGE_SPELL], player->defence[DAMAGE_SPELL] + iD[DAMAGE_SPELL]);
			if(entity->attack[DAMAGE_SPELL] != 0) {
				message_attacked_by_monster(player, entity, damage, DAMAGE_SPELL);
				}
			}
		}

	else {
		if(entity->attack[DAMAGE_BASIC]  > entity->attack[DAMAGE_POISON]) {
			damage = roll_the_dice(entity->attack[DAMAGE_BASIC] + iA[DAMAGE_BASIC], player->defence[DAMAGE_BASIC] + iD[DAMAGE_BASIC]);
			message_attacked_by_monster(player, entity, damage, DAMAGE_BASIC);
			}
		else {
			damage = roll_the_dice(entity->attack[DAMAGE_POISON] + iA[DAMAGE_POISON], player->defence[DAMAGE_POISON] + iD[DAMAGE_POISON]);
			message_attacked_by_monster(player, entity, damage, DAMAGE_POISON);
			}
		}
	if(damage > 0) {
		if(rand_f64() < CHANCE_CRITICAL_PLAYER) {
			da_append(&MESSAGES, "You are criticly injured");
			player->health-=player->maxHealth / 2;
			}
		else {
			player->health-=damage;
			}

		}
	else {
		entity->health+=damage;
		//TBD TEXT STORY
		}

	CLAMP(player->health, 0, INF);
	//APPLAY LIFE STEAL TO ENEMY
	if(rand_f64() < (entity->lifeStealChance - lfC)  && (entity->lifeStealValue + lf) != 0 && entity->health < entity->maxHealth) {
		entity->health+=(i32)entity->lifeStealValue + lf;
		CLAMP(entity->health, 0, entity->maxHealth);
		char* msg = calloc(50, sizeof(char));
		//snprintf(msg, 50, "%s lifestealed %u", entity->name, entity->lifeStealValue);
		//da_append(&MESSAGES, msg);
		}
	//i32 startX =  player->pos.x;

	//system("pause");
	//Text_Renderer_C(RENDERER, FONT, )

	}
/*


	//PROB THRU FILE OR SOMTHING
	//FOR NOW LET BE IN CODE
	//BASIC MONSTER
	//ATT
	monsters[BASIC_MONSTER].radius = 10;
	monsters[BASIC_MONSTER].ch = 'M';
	monsters[BASIC_MONSTER].attack[DAMAGE_BASIC]  = 3;
	monsters[BASIC_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[BASIC_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[BASIC_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF
	monsters[BASIC_MONSTER].defence[DAMAGE_BASIC]  = 3;
	monsters[BASIC_MONSTER].defence[DAMAGE_POISON] = 1;
	monsters[BASIC_MONSTER].defence[DAMAGE_RANGE]  = 2;
	monsters[BASIC_MONSTER].defence[DAMAGE_SPELL]  = 1;

	monsters[BASIC_MONSTER].isRunning = SDL_FALSE;
	monsters[BASIC_MONSTER].runWoundedPercent = 0.6f;
	monsters[BASIC_MONSTER].state = STATE_WANDERING;

	monsters[BASIC_MONSTER].stateChance[STATE_RUNING] = 0.6f;
	monsters[BASIC_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.01f;
	monsters[BASIC_MONSTER].stateChance[STATE_HUNTING] = 0.05f;
	monsters[BASIC_MONSTER].stateChance[STATE_WANDERING] = 0.5f;
	monsters[BASIC_MONSTER].stateChance[STATE_RESTING] = 0.3f;
	monsters[BASIC_MONSTER].stateChance[STATE_BESERK] = 0.01f;
	//ZOMBIE MONSTER
	//ATT

	monsters[ZOMBIE_MONSTER].radius = 50;
	monsters[ZOMBIE_MONSTER].ch = 'Z';
	monsters[ZOMBIE_MONSTER].attack[DAMAGE_BASIC]  = 1;
	monsters[ZOMBIE_MONSTER].attack[DAMAGE_POISON] = 4;
	monsters[ZOMBIE_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[ZOMBIE_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF
	monsters[ZOMBIE_MONSTER].defence[DAMAGE_BASIC]  = 3;
	monsters[ZOMBIE_MONSTER].defence[DAMAGE_POISON] = 3;
	monsters[ZOMBIE_MONSTER].defence[DAMAGE_RANGE]  = 3;
	monsters[ZOMBIE_MONSTER].defence[DAMAGE_SPELL]  = 3;

	monsters[ZOMBIE_MONSTER].isRunning = SDL_FALSE;
	monsters[ZOMBIE_MONSTER].runWoundedPercent = 0.0f;
	monsters[ZOMBIE_MONSTER].state = STATE_WANDERING;
	monsters[ZOMBIE_MONSTER].stateChance[STATE_RUNING] = 0.01f;
	monsters[ZOMBIE_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.01f;
	monsters[ZOMBIE_MONSTER].stateChance[STATE_HUNTING] = 0.6f;
	monsters[ZOMBIE_MONSTER].stateChance[STATE_WANDERING] = 0.05f;
	monsters[ZOMBIE_MONSTER].stateChance[STATE_RESTING] = 0.05f;
	monsters[ZOMBIE_MONSTER].stateChance[STATE_BESERK] = 0.6f;
	//WIZARD MONSTER
	//ATT
	monsters[WIZARD_MONSTER].radius = 20;
	monsters[WIZARD_MONSTER].ch = 'W';
	monsters[WIZARD_MONSTER].attack[DAMAGE_BASIC]  = 2;
	monsters[WIZARD_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[WIZARD_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[WIZARD_MONSTER].attack[DAMAGE_SPELL]  = 4;
	//DEF
	monsters[WIZARD_MONSTER].defence[DAMAGE_BASIC]  = 1;
	monsters[WIZARD_MONSTER].defence[DAMAGE_POISON] = 3;
	monsters[WIZARD_MONSTER].defence[DAMAGE_RANGE]  = 3;
	monsters[WIZARD_MONSTER].defence[DAMAGE_SPELL]  = 5;

	monsters[WIZARD_MONSTER].isRunning = SDL_FALSE;
	monsters[WIZARD_MONSTER].runWoundedPercent = 0.40f;
	monsters[WIZARD_MONSTER].state = STATE_WANDERING;

	monsters[WIZARD_MONSTER].stateChance[STATE_RUNING] = 0.05f;
	monsters[WIZARD_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.3f;
	monsters[WIZARD_MONSTER].stateChance[STATE_HUNTING] = 0.5f;
	monsters[WIZARD_MONSTER].stateChance[STATE_WANDERING] = 0.5f;
	monsters[WIZARD_MONSTER].stateChance[STATE_RESTING] = 0.1f;
	monsters[WIZARD_MONSTER].stateChance[STATE_BESERK] = 0.05f;
	//BEAR MONSTER
	//ATT

	monsters[BEAR_MONSTER].radius = 20;
	monsters[BEAR_MONSTER].ch = 'B';
	monsters[BEAR_MONSTER].attack[DAMAGE_BASIC]  = 4;
	monsters[BEAR_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[BEAR_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[BEAR_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF
	monsters[BEAR_MONSTER].defence[DAMAGE_BASIC]  = 4;
	monsters[BEAR_MONSTER].defence[DAMAGE_POISON] = 1;
	monsters[BEAR_MONSTER].defence[DAMAGE_RANGE]  = 3;
	monsters[BEAR_MONSTER].defence[DAMAGE_SPELL]  = 1;

	monsters[BEAR_MONSTER].isRunning = SDL_FALSE;
	monsters[BEAR_MONSTER].runWoundedPercent = 0.20f;
	monsters[BEAR_MONSTER].state = STATE_WANDERING;
	monsters[BEAR_MONSTER].stateChance[STATE_RUNING] = 0.5f;
	monsters[BEAR_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.03f;
	monsters[BEAR_MONSTER].stateChance[STATE_HUNTING] = 0.5f;
	monsters[BEAR_MONSTER].stateChance[STATE_WANDERING] = 0.5f;
	monsters[BEAR_MONSTER].stateChance[STATE_RESTING] = 0.7f;
	monsters[BEAR_MONSTER].stateChance[STATE_BESERK] = 0.7f;
	//CROW MONSTER
	//ATT
	monsters[CROW_MONSTER].radius = 20;
	monsters[CROW_MONSTER].ch = 'C';
	monsters[CROW_MONSTER].attack[DAMAGE_BASIC]  = 1;
	monsters[CROW_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[CROW_MONSTER].attack[DAMAGE_RANGE]  = 4;
	monsters[CROW_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF
	monsters[CROW_MONSTER].defence[DAMAGE_BASIC]  = 6;
	monsters[CROW_MONSTER].defence[DAMAGE_POISON] = 1;
	monsters[CROW_MONSTER].defence[DAMAGE_RANGE]  = 3;
	monsters[CROW_MONSTER].defence[DAMAGE_SPELL]  = 1;

	monsters[CROW_MONSTER].isRunning = SDL_FALSE;
	monsters[CROW_MONSTER].runWoundedPercent = 0.30f;
	monsters[CROW_MONSTER].state = STATE_WANDERING;

	monsters[CROW_MONSTER].stateChance[STATE_RUNING] = 0.05f;
	monsters[CROW_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.5f;
	monsters[CROW_MONSTER].stateChance[STATE_HUNTING] = 0.5f;
	monsters[CROW_MONSTER].stateChance[STATE_WANDERING] = 0.5f;
	monsters[CROW_MONSTER].stateChance[STATE_RESTING] = 0.1f;
	monsters[CROW_MONSTER].stateChance[STATE_BESERK] = 0.05f;
	//DEMON MONSTER
	//ATT
	monsters[DEMON_MONSTER].radius = 40;
	monsters[DEMON_MONSTER].ch = 'D';
	monsters[DEMON_MONSTER].attack[DAMAGE_BASIC]  = 3;
	monsters[DEMON_MONSTER].attack[DAMAGE_POISON] = 3;
	monsters[DEMON_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[DEMON_MONSTER].attack[DAMAGE_SPELL]  = 5;
	//DEF
	monsters[DEMON_MONSTER].defence[DAMAGE_BASIC]  = 4;
	monsters[DEMON_MONSTER].defence[DAMAGE_POISON] = 6;
	monsters[DEMON_MONSTER].defence[DAMAGE_RANGE]  = 0;
	monsters[DEMON_MONSTER].defence[DAMAGE_SPELL]  = 3;

	monsters[DEMON_MONSTER].isRunning = SDL_FALSE;
	monsters[DEMON_MONSTER].runWoundedPercent = 0.0f;
	monsters[DEMON_MONSTER].state = STATE_WANDERING;

	monsters[DEMON_MONSTER].stateChance[STATE_RUNING] = 0.05f;
	monsters[DEMON_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.05f;
	monsters[DEMON_MONSTER].stateChance[STATE_HUNTING] = 0.9f;
	monsters[DEMON_MONSTER].stateChance[STATE_WANDERING] = 0.1f;
	monsters[DEMON_MONSTER].stateChance[STATE_RESTING] = 0.01f;
	monsters[DEMON_MONSTER].stateChance[STATE_BESERK] = 0.8f;
	//GHOST MONSTER
	//ATT
	monsters[GHOST_MONSTER].radius = 20;
	monsters[GHOST_MONSTER].ch = 'G';
	monsters[GHOST_MONSTER].attack[DAMAGE_BASIC]  = 3;
	monsters[GHOST_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[GHOST_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[GHOST_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF
	monsters[GHOST_MONSTER].defence[DAMAGE_BASIC]  = 4;
	monsters[GHOST_MONSTER].defence[DAMAGE_POISON] = 6;
	monsters[GHOST_MONSTER].defence[DAMAGE_RANGE]  = 3;
	monsters[GHOST_MONSTER].defence[DAMAGE_SPELL]  = 3;

	monsters[GHOST_MONSTER].isRunning = SDL_FALSE;
	monsters[GHOST_MONSTER].runWoundedPercent = 0.0f;
	monsters[GHOST_MONSTER].state = STATE_WANDERING;
	for(i32 i = 0; i < (i32)STATE_NUM; i++){
		monsters[GHOST_MONSTER].stateChance[i] = rand_f64();
	}
*/
void monster_definitions_export() {


	//PROB THRU FILE OR SOMTHING
	//FOR NOW LET BE IN CODE
	//BASIC MONSTER
	//ATT
	monsters[BASIC_MONSTER].radius = 10;
	monsters[BASIC_MONSTER].ch = 'M';
	monsters[BASIC_MONSTER].attack[DAMAGE_BASIC]  = 3;
	monsters[BASIC_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[BASIC_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[BASIC_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF
	monsters[BASIC_MONSTER].defence[DAMAGE_BASIC]  = 3;
	monsters[BASIC_MONSTER].defence[DAMAGE_POISON] = 1;
	monsters[BASIC_MONSTER].defence[DAMAGE_RANGE]  = 2;
	monsters[BASIC_MONSTER].defence[DAMAGE_SPELL]  = 1;
	monsters[BASIC_MONSTER].health = 3;

	monsters[BASIC_MONSTER].isRunning = SDL_FALSE;
	monsters[BASIC_MONSTER].runWoundedPercent = 0.6f;
	monsters[BASIC_MONSTER].state = STATE_WANDERING;

	monsters[BASIC_MONSTER].stateChance[STATE_RUNING] = 0.6f;
	monsters[BASIC_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.01f;
	monsters[BASIC_MONSTER].stateChance[STATE_HUNTING] = 0.05f;
	monsters[BASIC_MONSTER].stateChance[STATE_WANDERING] = 0.5f;
	monsters[BASIC_MONSTER].stateChance[STATE_RESTING] = 0.3f;
	monsters[BASIC_MONSTER].stateChance[STATE_BESERK] = 0.01f;

	//ACOLAYT
	monsters[ACOLAYT_MONSTER].radius = 5;
	monsters[ACOLAYT_MONSTER].ch = 'A';
	monsters[ACOLAYT_MONSTER].attack[DAMAGE_BASIC]  = 1;
	monsters[ACOLAYT_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[ACOLAYT_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[ACOLAYT_MONSTER].attack[DAMAGE_SPELL]  = 1;
	//DEF  unarmord
	monsters[ACOLAYT_MONSTER].defence[DAMAGE_BASIC]  = 2;
	monsters[ACOLAYT_MONSTER].defence[DAMAGE_POISON] = 2;
	monsters[ACOLAYT_MONSTER].defence[DAMAGE_RANGE]  = 2;
	monsters[ACOLAYT_MONSTER].defence[DAMAGE_SPELL]  = 2;
	monsters[ACOLAYT_MONSTER].health = 5;
	monsters[ACOLAYT_MONSTER].maxHealth = 5;

	monsters[ACOLAYT_MONSTER].isRunning = SDL_FALSE;
	monsters[ACOLAYT_MONSTER].runWoundedPercent = 0.1f;
	monsters[ACOLAYT_MONSTER].state = STATE_WANDERING;

	monsters[ACOLAYT_MONSTER].stateChance[STATE_RUNING] = 0.05f;
	monsters[ACOLAYT_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.05f;
	monsters[ACOLAYT_MONSTER].stateChance[STATE_HUNTING] = 0.6f;
	monsters[ACOLAYT_MONSTER].stateChance[STATE_WANDERING] = 0.5f;
	monsters[ACOLAYT_MONSTER].stateChance[STATE_RESTING] = 0.3f;
	monsters[ACOLAYT_MONSTER].stateChance[STATE_BESERK] = 0.01f;
	monsters[ACOLAYT_MONSTER].stateChance[STATE_RESURECT] = 0.30f;
	monsters[ACOLAYT_MONSTER].stateChance[STATE_SUMMON] = 0.0f;


	monsters[ACOLAYT_MONSTER].lifeStealChance = 0.0f;
	monsters[ACOLAYT_MONSTER].lifeStealValue = 0.0f;
	monsters[ACOLAYT_MONSTER].color = UNDE_COL;

	monsters[ACOLAYT_MONSTER].maxStamina = 8;
	monsters[ACOLAYT_MONSTER].stamina    = 8;
	monsters[ACOLAYT_MONSTER].chanceToDecressStaminaMove = 0.1;
	//GHOUL
	monsters[GHOUL_MONSTER].radius = 14;
	monsters[GHOUL_MONSTER].ch = 'G';
	monsters[GHOUL_MONSTER].attack[DAMAGE_BASIC]  = 2;
	monsters[GHOUL_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[GHOUL_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[GHOUL_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF  unarmord
	monsters[GHOUL_MONSTER].defence[DAMAGE_BASIC]  = 2;
	monsters[GHOUL_MONSTER].defence[DAMAGE_POISON] = 2;
	monsters[GHOUL_MONSTER].defence[DAMAGE_RANGE]  = 2;
	monsters[GHOUL_MONSTER].defence[DAMAGE_SPELL]  = 2;
	monsters[GHOUL_MONSTER].health = 7;
	monsters[GHOUL_MONSTER].maxHealth = 7;

	monsters[GHOUL_MONSTER].isRunning = SDL_FALSE;
	monsters[GHOUL_MONSTER].runWoundedPercent = 0.6f;
	monsters[GHOUL_MONSTER].state = STATE_HUNTING;

	monsters[GHOUL_MONSTER].stateChance[STATE_RUNING] = 0.01f;
	monsters[GHOUL_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.01f;
	monsters[GHOUL_MONSTER].stateChance[STATE_HUNTING] = 0.7f;
	monsters[GHOUL_MONSTER].stateChance[STATE_WANDERING] = 0.03f;
	monsters[GHOUL_MONSTER].stateChance[STATE_RESTING] = 0.01f;
	monsters[GHOUL_MONSTER].stateChance[STATE_BESERK] = 0.2f;
	monsters[GHOUL_MONSTER].stateChance[STATE_RESURECT] = 0.00f;
	monsters[GHOUL_MONSTER].stateChance[STATE_SUMMON] = 0.0f;

	monsters[GHOUL_MONSTER].maxStamina = 8;
	monsters[GHOUL_MONSTER].stamina    = 8;
	monsters[GHOUL_MONSTER].chanceToDecressStaminaMove = 0.2;

	monsters[GHOUL_MONSTER].lifeStealChance = 0.1f;
	monsters[GHOUL_MONSTER].lifeStealValue  = 1.0f;
	monsters[GHOUL_MONSTER].color = UNDE_COL;

	//NECROMANCER_MONSTER
	monsters[NECROMANCER_MONSTER].radius = 10;
	monsters[NECROMANCER_MONSTER].ch = 'N';
	monsters[NECROMANCER_MONSTER].attack[DAMAGE_BASIC]  = 0;
	monsters[NECROMANCER_MONSTER].attack[DAMAGE_POISON] = 1;
	monsters[NECROMANCER_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[NECROMANCER_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF  light
	monsters[NECROMANCER_MONSTER].defence[DAMAGE_BASIC]  = 3;
	monsters[NECROMANCER_MONSTER].defence[DAMAGE_POISON] = 1;
	monsters[NECROMANCER_MONSTER].defence[DAMAGE_RANGE]  = 1;
	monsters[NECROMANCER_MONSTER].defence[DAMAGE_SPELL]  = 3;
	monsters[NECROMANCER_MONSTER].health = 2;
	monsters[NECROMANCER_MONSTER].maxHealth = 2;

	monsters[NECROMANCER_MONSTER].isRunning = SDL_FALSE;
	monsters[NECROMANCER_MONSTER].runWoundedPercent = 0.9f;
	monsters[NECROMANCER_MONSTER].state = STATE_WANDERING;

	monsters[NECROMANCER_MONSTER].stateChance[STATE_RUNING] = 0.3f;
	monsters[NECROMANCER_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.6f;
	monsters[NECROMANCER_MONSTER].stateChance[STATE_HUNTING] = 0.01f;
	monsters[NECROMANCER_MONSTER].stateChance[STATE_WANDERING] = 0.4f;
	monsters[NECROMANCER_MONSTER].stateChance[STATE_RESTING] = 0.2f;
	monsters[NECROMANCER_MONSTER].stateChance[STATE_BESERK] = 0.00f;
	monsters[NECROMANCER_MONSTER].stateChance[STATE_RESURECT] = 0.00f;
	monsters[NECROMANCER_MONSTER].stateChance[STATE_SUMMON] = 0.6f;

	monsters[NECROMANCER_MONSTER].maxStamina = 8;
	monsters[NECROMANCER_MONSTER].stamina    = 8;
	monsters[NECROMANCER_MONSTER].chanceToDecressStaminaMove = 0.1;

	monsters[NECROMANCER_MONSTER].lifeStealChance = 0.0f;
	monsters[NECROMANCER_MONSTER].lifeStealValue  = 0.0f;

	monsters[NECROMANCER_MONSTER].color = UNDE_COL;
	SPELL_SUMONM_GHOUL_EXPORT(monsters[NECROMANCER_MONSTER]);


	//BANSHIE_MONSTER
	monsters[BANSHIE_MONSTER].radius = 10;
	monsters[BANSHIE_MONSTER].ch = 'B';
	monsters[BANSHIE_MONSTER].attack[DAMAGE_BASIC]  = 0;
	monsters[BANSHIE_MONSTER].attack[DAMAGE_POISON] = 1;
	monsters[BANSHIE_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[BANSHIE_MONSTER].attack[DAMAGE_SPELL]  = 3;
	//DEF  light
	monsters[BANSHIE_MONSTER].defence[DAMAGE_BASIC]  = 1;
	monsters[BANSHIE_MONSTER].defence[DAMAGE_POISON] = 3;
	monsters[BANSHIE_MONSTER].defence[DAMAGE_RANGE]  = 3;
	monsters[BANSHIE_MONSTER].defence[DAMAGE_SPELL]  = 1;
	monsters[BANSHIE_MONSTER].health = 3;
	monsters[BANSHIE_MONSTER].maxHealth = 3;

	monsters[BANSHIE_MONSTER].isRunning = SDL_FALSE;
	monsters[BANSHIE_MONSTER].runWoundedPercent = 0.9f;
	monsters[BANSHIE_MONSTER].state = STATE_WANDERING;

	monsters[BANSHIE_MONSTER].stateChance[STATE_RUNING] = 0.03f;
	monsters[BANSHIE_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.5f;
	monsters[BANSHIE_MONSTER].stateChance[STATE_HUNTING] = 0.6f;
	monsters[BANSHIE_MONSTER].stateChance[STATE_WANDERING] = 0.4f;
	monsters[BANSHIE_MONSTER].stateChance[STATE_RESTING] = 0.05f;
	monsters[BANSHIE_MONSTER].stateChance[STATE_BESERK] = 0.01f;
	monsters[BANSHIE_MONSTER].stateChance[STATE_RESURECT] = 0.00f;
	monsters[BANSHIE_MONSTER].stateChance[STATE_SUMMON] = 0.00f;
	monsters[BANSHIE_MONSTER].stateChance[STATE_SPELL]  = 0.8f;


	monsters[BANSHIE_MONSTER].maxStamina = 8;
	monsters[BANSHIE_MONSTER].stamina    = 8;
	monsters[BANSHIE_MONSTER].chanceToDecressStaminaMove = 0.1;


	monsters[BANSHIE_MONSTER].lifeStealChance = 0.0f;
	monsters[BANSHIE_MONSTER].lifeStealValue  = 0.0f;

	monsters[BANSHIE_MONSTER].color  = UNDE_COL;
	SPELL_DECRESE_MAX_HEALTH_EXPORT(monsters[BANSHIE_MONSTER]);

	//SPIDER_MONSTER
	monsters[SPIDER_MONSTER].radius = 10;
	monsters[SPIDER_MONSTER].ch = 'F';
	monsters[SPIDER_MONSTER].attack[DAMAGE_BASIC]  = 0;
	monsters[SPIDER_MONSTER].attack[DAMAGE_POISON] = 3;
	monsters[SPIDER_MONSTER].attack[DAMAGE_RANGE]  = 3;
	monsters[SPIDER_MONSTER].attack[DAMAGE_SPELL] = 0;
	//DEF  light
	monsters[SPIDER_MONSTER].defence[DAMAGE_BASIC]  = 2;
	monsters[SPIDER_MONSTER].defence[DAMAGE_POISON] = 2;
	monsters[SPIDER_MONSTER].defence[DAMAGE_RANGE]  = 2;
	monsters[SPIDER_MONSTER].defence[DAMAGE_SPELL]  = 2;
	monsters[SPIDER_MONSTER].health = 5;
	monsters[SPIDER_MONSTER].maxHealth = 5;

	monsters[SPIDER_MONSTER].isRunning = SDL_FALSE;
	monsters[SPIDER_MONSTER].runWoundedPercent = 0.9f;
	monsters[SPIDER_MONSTER].state = STATE_WANDERING;

	monsters[SPIDER_MONSTER].stateChance[STATE_RUNING] = 0.05f;
	monsters[SPIDER_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.05f;
	monsters[SPIDER_MONSTER].stateChance[STATE_HUNTING] = 0.9f;
	monsters[SPIDER_MONSTER].stateChance[STATE_WANDERING] = 0.4f;
	monsters[SPIDER_MONSTER].stateChance[STATE_RESTING] = 0.2f;
	monsters[SPIDER_MONSTER].stateChance[STATE_BESERK] = 0.01f;
	monsters[SPIDER_MONSTER].stateChance[STATE_RESURECT] = 0.00f;
	monsters[SPIDER_MONSTER].stateChance[STATE_SUMMON] = 0.00f;
	monsters[SPIDER_MONSTER].stateChance[STATE_SPELL]  = 0.8f;

	monsters[SPIDER_MONSTER].maxStamina = 8;
	monsters[SPIDER_MONSTER].stamina    = 8;
	monsters[SPIDER_MONSTER].chanceToDecressStaminaMove = 0.1;




	monsters[SPIDER_MONSTER].lifeStealChance = 0.01f;
	monsters[SPIDER_MONSTER].lifeStealValue  = 1.0f;

	monsters[SPIDER_MONSTER].color = UNDE_COL;
	SPELL_SPIDER_EXPORT(monsters[SPIDER_MONSTER]);


	//DRAGON_MONSTER
	monsters[DRAGON_MONSTER].radius = 35;
	monsters[DRAGON_MONSTER].ch = 'D';
	monsters[DRAGON_MONSTER].attack[DAMAGE_BASIC]  = 5;
	monsters[DRAGON_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[DRAGON_MONSTER].attack[DAMAGE_RANGE]  = 5;
	monsters[DRAGON_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF  light
	monsters[DRAGON_MONSTER].defence[DAMAGE_BASIC]  = 3;
	monsters[DRAGON_MONSTER].defence[DAMAGE_POISON] = 3;
	monsters[DRAGON_MONSTER].defence[DAMAGE_RANGE]  = 3;
	monsters[DRAGON_MONSTER].defence[DAMAGE_SPELL]  = 3;
	monsters[DRAGON_MONSTER].health = 20;
	monsters[DRAGON_MONSTER].maxHealth = 20;

	monsters[DRAGON_MONSTER].isRunning = SDL_FALSE;
	monsters[DRAGON_MONSTER].runWoundedPercent = 0.6f;
	monsters[DRAGON_MONSTER].state = STATE_WANDERING;

	monsters[DRAGON_MONSTER].stateChance[STATE_RUNING] = 0.01f;
	monsters[DRAGON_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.05f;
	monsters[DRAGON_MONSTER].stateChance[STATE_HUNTING] = 0.99f;
	monsters[DRAGON_MONSTER].stateChance[STATE_WANDERING] = 0.05f;
	monsters[DRAGON_MONSTER].stateChance[STATE_RESTING] = 0.05f;
	monsters[DRAGON_MONSTER].stateChance[STATE_BESERK] = 0.1f;
	monsters[DRAGON_MONSTER].stateChance[STATE_RESURECT] = 0.00f;
	monsters[DRAGON_MONSTER].stateChance[STATE_SUMMON] = 0.00f;
	monsters[DRAGON_MONSTER].stateChance[STATE_SPELL]  = 0.99f;
	monsters[DRAGON_MONSTER].color = BLUE;

	monsters[DRAGON_MONSTER].maxStamina = 10;
	monsters[DRAGON_MONSTER].stamina    = 8;
	monsters[DRAGON_MONSTER].chanceToDecressStaminaMove = 0.3;


	monsters[DRAGON_MONSTER].lifeStealChance = 0.1f;
	monsters[DRAGON_MONSTER].lifeStealValue  = 1.0f;

	SPELL_DRAGON_EXPORT(monsters[DRAGON_MONSTER]);

//ORCS
	//GRUNT
	monsters[GRUNT_MONSTER].radius = 15;
	monsters[GRUNT_MONSTER].ch = 'O';
	monsters[GRUNT_MONSTER].attack[DAMAGE_BASIC]  = 3;
	monsters[GRUNT_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[GRUNT_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[GRUNT_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF  unarmord
	monsters[GRUNT_MONSTER].defence[DAMAGE_BASIC]  = 3;
	monsters[GRUNT_MONSTER].defence[DAMAGE_POISON] = 1;
	monsters[GRUNT_MONSTER].defence[DAMAGE_RANGE]  = 3;
	monsters[GRUNT_MONSTER].defence[DAMAGE_SPELL]  = 1;
	monsters[GRUNT_MONSTER].health = 10;
	monsters[GRUNT_MONSTER].maxHealth = 10;

	monsters[GRUNT_MONSTER].isRunning = SDL_FALSE;
	monsters[GRUNT_MONSTER].runWoundedPercent = 0.6f;
	monsters[GRUNT_MONSTER].state = STATE_WANDERING;

	monsters[GRUNT_MONSTER].stateChance[STATE_RUNING] = 0.01f;
	monsters[GRUNT_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.01f;
	monsters[GRUNT_MONSTER].stateChance[STATE_HUNTING] = 0.8f;
	monsters[GRUNT_MONSTER].stateChance[STATE_WANDERING] = 0.3f;
	monsters[GRUNT_MONSTER].stateChance[STATE_RESTING] = 0.1f;
	monsters[GRUNT_MONSTER].stateChance[STATE_BESERK] = 0.2f;
	monsters[GRUNT_MONSTER].stateChance[STATE_RESURECT] = 0.00f;
	monsters[GRUNT_MONSTER].stateChance[STATE_SUMMON] = 0.0f;

	monsters[GRUNT_MONSTER].maxStamina = 10;
	monsters[GRUNT_MONSTER].stamina    = 8;
	monsters[GRUNT_MONSTER].chanceToDecressStaminaMove = 0.3;



	monsters[GRUNT_MONSTER].lifeStealChance = 0.3f;
	monsters[GRUNT_MONSTER].lifeStealValue  = 1.0f;
	monsters[GRUNT_MONSTER].color = GREEN;

//BERSERKER
	monsters[BERSERKER_MONSTER].radius = 15;
	monsters[BERSERKER_MONSTER].ch = 'V';
	monsters[BERSERKER_MONSTER].attack[DAMAGE_BASIC]  = 4;
	monsters[BERSERKER_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[BERSERKER_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[BERSERKER_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF  unarmord
	monsters[BERSERKER_MONSTER].defence[DAMAGE_BASIC]  = 2;
	monsters[BERSERKER_MONSTER].defence[DAMAGE_POISON] = 2;
	monsters[BERSERKER_MONSTER].defence[DAMAGE_RANGE]  = 2;
	monsters[BERSERKER_MONSTER].defence[DAMAGE_SPELL]  = 2;
	monsters[BERSERKER_MONSTER].health = 4;
	monsters[BERSERKER_MONSTER].maxHealth = 4;

	monsters[BERSERKER_MONSTER].isRunning = SDL_FALSE;
	monsters[BERSERKER_MONSTER].runWoundedPercent = 0.6f;
	monsters[BERSERKER_MONSTER].state = STATE_WANDERING;

	monsters[BERSERKER_MONSTER].stateChance[STATE_RUNING] = 0.01f;
	monsters[BERSERKER_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.01f;
	monsters[BERSERKER_MONSTER].stateChance[STATE_HUNTING] = 0.9f;
	monsters[BERSERKER_MONSTER].stateChance[STATE_WANDERING] = 0.2f;
	monsters[BERSERKER_MONSTER].stateChance[STATE_RESTING] = 0.05f;
	monsters[BERSERKER_MONSTER].stateChance[STATE_BESERK] = 0.99f;
	monsters[BERSERKER_MONSTER].stateChance[STATE_RESURECT] = 0.00f;
	monsters[BERSERKER_MONSTER].stateChance[STATE_SUMMON] = 0.0f;

	monsters[BERSERKER_MONSTER].maxStamina = 10;
	monsters[BERSERKER_MONSTER].stamina    = 8;
	monsters[BERSERKER_MONSTER].chanceToDecressStaminaMove = 0.1;


	monsters[BERSERKER_MONSTER].lifeStealChance = 0.01f;
	monsters[BERSERKER_MONSTER].lifeStealValue  = 1.0f;
	monsters[BERSERKER_MONSTER].color = GREEN;

	monsters[BERSERKER_MONSTER].stateChance[STATE_SPELL] = 0.8f;
	SPELL_BESERKER_EXPORT(monsters[BERSERKER_MONSTER]);

//ARCHER
	monsters[ARCHER_MONSTER].radius = 10;
	monsters[ARCHER_MONSTER].ch = 'A';
	monsters[ARCHER_MONSTER].attack[DAMAGE_BASIC]  = 1;
	monsters[ARCHER_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[ARCHER_MONSTER].attack[DAMAGE_RANGE]  = 3;
	monsters[ARCHER_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF  light
	monsters[ARCHER_MONSTER].defence[DAMAGE_BASIC]  = 2;
	monsters[ARCHER_MONSTER].defence[DAMAGE_POISON] = 2;
	monsters[ARCHER_MONSTER].defence[DAMAGE_RANGE]  = 2;
	monsters[ARCHER_MONSTER].defence[DAMAGE_SPELL]  = 2;
	monsters[ARCHER_MONSTER].health = 4;
	monsters[ARCHER_MONSTER].maxHealth = 4;

	monsters[ARCHER_MONSTER].isRunning = SDL_FALSE;
	monsters[ARCHER_MONSTER].runWoundedPercent = 0.9f;
	monsters[ARCHER_MONSTER].state = STATE_WANDERING;

	monsters[ARCHER_MONSTER].stateChance[STATE_RUNING] = 0.5f;
	monsters[ARCHER_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.8f;
	monsters[ARCHER_MONSTER].stateChance[STATE_HUNTING] = 0.1f;
	monsters[ARCHER_MONSTER].stateChance[STATE_WANDERING] = 0.4f;
	monsters[ARCHER_MONSTER].stateChance[STATE_RESTING] = 0.2f;
	monsters[ARCHER_MONSTER].stateChance[STATE_BESERK] = 0.01f;
	monsters[ARCHER_MONSTER].stateChance[STATE_RESURECT] = 0.00f;
	monsters[ARCHER_MONSTER].stateChance[STATE_SUMMON] = 0.00f;
	monsters[ARCHER_MONSTER].stateChance[STATE_SPELL]  = 0.0f;

	monsters[ARCHER_MONSTER].maxStamina = 10;
	monsters[ARCHER_MONSTER].stamina    = 4;
	monsters[ARCHER_MONSTER].chanceToDecressStaminaMove = 0.1;


	monsters[ARCHER_MONSTER].lifeStealChance = 0.01f;
	monsters[ARCHER_MONSTER].lifeStealValue  = 1.0f;

	monsters[ARCHER_MONSTER].color = GREEN;

//WITCH DOC
	monsters[WITCH_MONSTER].radius = 10;
	monsters[WITCH_MONSTER].ch = 'W';
	monsters[WITCH_MONSTER].attack[DAMAGE_BASIC]  = 0;
	monsters[WITCH_MONSTER].attack[DAMAGE_POISON] = 1;
	monsters[WITCH_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[WITCH_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF  light
	monsters[WITCH_MONSTER].defence[DAMAGE_BASIC]  = 2;
	monsters[WITCH_MONSTER].defence[DAMAGE_POISON] = 1;
	monsters[WITCH_MONSTER].defence[DAMAGE_RANGE]  = 1;
	monsters[WITCH_MONSTER].defence[DAMAGE_SPELL]  = 3;
	monsters[WITCH_MONSTER].health = 3;
	monsters[WITCH_MONSTER].maxHealth = 3;

	monsters[WITCH_MONSTER].isRunning = SDL_FALSE;
	monsters[WITCH_MONSTER].runWoundedPercent = 0.9f;
	monsters[WITCH_MONSTER].state = STATE_WANDERING;

	monsters[WITCH_MONSTER].stateChance[STATE_RUNING] = 0.3f;
	monsters[WITCH_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.6f;
	monsters[WITCH_MONSTER].stateChance[STATE_HUNTING] = 0.01f;
	monsters[WITCH_MONSTER].stateChance[STATE_WANDERING] = 0.4f;
	monsters[WITCH_MONSTER].stateChance[STATE_RESTING] = 0.2f;
	monsters[WITCH_MONSTER].stateChance[STATE_BESERK] = 0.00f;
	monsters[WITCH_MONSTER].stateChance[STATE_RESURECT] = 0.00f;
	monsters[WITCH_MONSTER].stateChance[STATE_SPELL] = 0.8f;


	monsters[WITCH_MONSTER].maxStamina = 10;
	monsters[WITCH_MONSTER].stamina    = 4;
	monsters[WITCH_MONSTER].chanceToDecressStaminaMove = 0.1;


	monsters[WITCH_MONSTER].lifeStealChance = 0.0f;
	monsters[WITCH_MONSTER].lifeStealValue  = 0.0f;

	monsters[WITCH_MONSTER].color = GREEN;
	SPELL_WITCH_EXPORT(monsters[WITCH_MONSTER]);

//NEUTRAL ANIMALS
	monsters[RAT_MONSTER].radius = 5;
	monsters[RAT_MONSTER].ch = 'R';
	monsters[RAT_MONSTER].attack[DAMAGE_BASIC]  = 1;
	monsters[RAT_MONSTER].attack[DAMAGE_POISON] = 0;
	monsters[RAT_MONSTER].attack[DAMAGE_RANGE]  = 0;
	monsters[RAT_MONSTER].attack[DAMAGE_SPELL]  = 0;
	//DEF
	monsters[RAT_MONSTER].defence[DAMAGE_BASIC]  = 0;
	monsters[RAT_MONSTER].defence[DAMAGE_POISON] = 0;
	monsters[RAT_MONSTER].defence[DAMAGE_RANGE]  = 0;
	monsters[RAT_MONSTER].defence[DAMAGE_SPELL]  = 0;
	monsters[RAT_MONSTER].health = 1;
	monsters[RAT_MONSTER].maxHealth = 1;

	monsters[RAT_MONSTER].isRunning = SDL_FALSE;
	monsters[RAT_MONSTER].runWoundedPercent = 0.6f;
	monsters[RAT_MONSTER].state = STATE_WANDERING;

	monsters[RAT_MONSTER].stateChance[STATE_RUNING] = 0.6f;
	monsters[RAT_MONSTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.01f;
	monsters[RAT_MONSTER].stateChance[STATE_HUNTING] = 0.6f;
	monsters[RAT_MONSTER].stateChance[STATE_WANDERING] = 0.5f;
	monsters[RAT_MONSTER].stateChance[STATE_RESTING] = 0.3f;
	monsters[RAT_MONSTER].stateChance[STATE_BESERK] = 0.01f;
	monsters[RAT_MONSTER].stateChance[STATE_SUMMON] = 0.8f;

	monsters[RAT_MONSTER].maxStamina = 10;
	monsters[RAT_MONSTER].stamina    = 4;
	monsters[RAT_MONSTER].chanceToDecressStaminaMove = 0.1;


	SPELL_SUMONM_RAT_EXPORT(monsters[RAT_MONSTER]);

//GOBLIN
	monsters[GOBLIN_MOSNTER].radius = 20;
	monsters[GOBLIN_MOSNTER].ch = 'K';
	monsters[GOBLIN_MOSNTER].attack[DAMAGE_BASIC]  = 1;
	monsters[GOBLIN_MOSNTER].attack[DAMAGE_POISON] = 0;
	monsters[GOBLIN_MOSNTER].attack[DAMAGE_RANGE]  = 0;
	monsters[GOBLIN_MOSNTER].attack[DAMAGE_SPELL]  = 0;
	//DEF
	monsters[GOBLIN_MOSNTER].defence[DAMAGE_BASIC]  = 0;
	monsters[GOBLIN_MOSNTER].defence[DAMAGE_POISON] = 0;
	monsters[GOBLIN_MOSNTER].defence[DAMAGE_RANGE]  = 0;
	monsters[GOBLIN_MOSNTER].defence[DAMAGE_SPELL]  = 0;
	monsters[GOBLIN_MOSNTER].health = 3;
	monsters[GOBLIN_MOSNTER].maxHealth = 3;

	monsters[GOBLIN_MOSNTER].isRunning = SDL_FALSE;
	monsters[GOBLIN_MOSNTER].runWoundedPercent = 0.6f;
	monsters[GOBLIN_MOSNTER].state = STATE_WANDERING;

	monsters[GOBLIN_MOSNTER].stateChance[STATE_RUNING] = 0.6f;
	monsters[GOBLIN_MOSNTER].stateChance[STATE_MOVING_AWAY_RANGE] = 0.01f;
	monsters[GOBLIN_MOSNTER].stateChance[STATE_HUNTING] = 0.05f;
	monsters[GOBLIN_MOSNTER].stateChance[STATE_WANDERING] = 0.5f;
	monsters[GOBLIN_MOSNTER].stateChance[STATE_RESTING] = 0.3f;
	monsters[GOBLIN_MOSNTER].stateChance[STATE_BESERK] = 0.01f;
	monsters[GOBLIN_MOSNTER].stateChance[STATE_SPELL] = 0.3f;

	monsters[GOBLIN_MOSNTER].maxStamina = 10;
	monsters[GOBLIN_MOSNTER].stamina    = 4;
	monsters[GOBLIN_MOSNTER].chanceToDecressStaminaMove = 0.1;


	SPELL_GOBLIN_EXPORT(monsters[GOBLIN_MOSNTER]);





	//return monsters;
	}



void genereate_monsters(Entitiy_DA *monsters, Tile *map) {
	for(i32 y = 0; y < MAP_Y; y++) {
		for(i32 x = 0; x < MAP_X; x++) {
			if(MAP_CH(map, x, y) != TILE_BLOCKED) {
				if(rand_f64() < PERCENTAGE_MONSTER_GENERATED) {
					i32 type = rand()%(NUM_MONSTER - 1) + 1;
					type = WITCH_MONSTER;
					i32 vison = rand()%40+1;
					//i32 health = monsters->items[type].health;
					Entitiy *temp = create_entity(monsterChar[type], monsterName[type], vison, 3, (Position) {
						.x = x, .y = y
						}, WHITE);
					if(rand_f64() < 1.5f) {
						u64 count = monsters->count;
						if(count > 0) {
							Item *item  = create_item(0, 0, SWORD_CREATE());
							item->isEquiped = SDL_TRUE;
							da_append(&monsters->items[count-1].inventory, (*item));
							Item *item1  = create_item(0, 0, ARMOR_CREATE());
							item1->isEquiped = SDL_TRUE;
							da_append(&monsters->items[count-1].inventory, (*item1));
							}
						}
					da_append(monsters, *temp);
					}
				}
			}
		}
	LOG("\nGenerated monsters %d\n", (i32)monsters->count);
	}


SDL_bool Is_Monster(char c) {
	for(Monster_Types t = BASIC_MONSTER; t < NUM_MONSTER; t++) {
		if(monsterChar[t] == c) {
			//LOG("True\n\n");
			return SDL_TRUE;
			}
		}
	return SDL_FALSE;
	}


i32 is_monster_on_entity(i32 x, i32 y, Entitiy_DA* entities) {
	CLAMP(x, 0, (MAP_X - 1));
	CLAMP(y, 0, (MAP_Y - 1));
	for(u64 count = 0; count < entities->count; count++) {
		Entitiy entity = entities->items[count];
		if(Is_Monster(entity.ch) == SDL_TRUE && entity.pos.x == x && entity.pos.y == y) {

			entities->items[count] = entity;
			//LOG("health %d, count %d", entity.health, count);
			return count;
			}
		}


	return -1;
	}



//BLOCK MOVMENT DEPENDING ON TYPE OF MONSTERS
void block_movement(Entitiy_DA *entitys, Tile *map) {
	for(u64 count = 0; count < entitys->count; count++) {
		if(Is_Monster(entitys->items[count].ch) == SDL_TRUE) {
			i32 x = entitys->items[count].pos.x;
			i32 y = entitys->items[count].pos.y;
			MAP_ISW(map, x, y) = SDL_FALSE;
			}
		else if(entitys->items[count].ch == 'S') {
			i32 x = entitys->items[count].pos.x;
			i32 y = entitys->items[count].pos.y;
			MAP_ISW(map, x, y) = SDL_TRUE;
			free_entity(&entitys->items[count]);
			da_remove_unordered(entitys, count);
			}
		}
	}
//check if 2 rec colide or if fieled of vison colide
SDL_bool check_colison_entitiy(Entitiy* player, Entitiy* ent, Tile* map) {

	if(MAP_ISV(map, ent->pos.x, ent->pos.y) == SDL_FALSE) {
		return SDL_FALSE;
		}

	SDL_Rect A = {.h = player->radius, .w = player->radius, .x = player->pos.x, .y = player->pos.y };
	SDL_Rect B = {.h = ent->radius, .w = ent->radius, .x = ent->pos.x, .y = ent->pos.y };

	i32 leftA, leftB;
	i32 rightA, rightB;
	i32 topA, topB;
	i32 bottomA, bottomB;

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




void cast_ray(Entitiy *entity, Tile* map, f64 x, f64 y) {

	f64 ox,oy;
	ox = (f64)entity->pos.x;
	oy = (f64)entity->pos.y;

	for(i32 i = 0; i < RADIUS; i++) {
		CLAMP(ox, 0.00f, (f64)(MAP_X - 1));
		CLAMP(ox, 0.00f, (f64)(MAP_Y - 1));
		//LOG("%d %d\n", (u32)ox, (u32)oy);
		MAP_VISITED(map, (i32)ox, (i32)oy) = SDL_TRUE;
		MAP_ISV(map, (i32)ox, (i32)oy) = SDL_TRUE;//MAP_ISW(map, (i32)ox, (i32)oy) == SDL_FALSE ||
		if(MAP_ISW(map, (u32)ox, (u32)oy) == SDL_FALSE) {
			return;
			}
		ox+=x;
		oy+=y;
		};
	}
void field_of_vison(Entitiy *entity, Tile* map) {
	f64 x,y;
	CLEAR_VISON_FIELD(map);
	i32 xE = entity->pos.x;
	i32 yE = entity->pos.y;
	CLAMP(xE, 1, (MAP_X-2));
	CLAMP(yE, 1, (MAP_X-2));
	MAP_ISW(map, xE, yE) = SDL_TRUE;
	for(u64 i = 1; i < 359; i++) {
		x = cos((f64)i*0.01745329251f); //to rad
		y = sin((f64)i*0.01745329251f);
		cast_ray(entity, map, x, y);
		};
	MAP_ISW(map, xE, yE)   = SDL_FALSE;

	//MAP_ISW(map, xE+1, yE) = SDL_FALSE;
	//MAP_ISW(map, xE-1, yE) = SDL_FALSE;
	//MAP_ISW(map, xE, yE+1) = SDL_FALSE;
	//MAP_ISW(map, xE, yE-1) = SDL_FALSE;
	}


f64 distnace_move(i32 x1, i32 y1, i32 x2, i32 y2, Tile *map) {
	CLAMP(x2, 0, MAP_X-1);
	CLAMP(y2, 0, MAP_Y-1);
	if(MAP_ISW(map, x2, y2) == SDL_TRUE) {
		f64 distance = DISTANCE(x1, y1, x2, y2);
		//if(distance == 0.0f){
		//	return INF;
		//}
		return distance;
		}
	//LOG("dist INF\n");
	//system("pause");
	return INF;
	}
void make_run_move(Entitiy* player, Entitiy*  ent, Tile *map) {
	i32 x1 = player->pos.x;
	i32 y1 = player->pos.y;
	i32 x2 = ent->pos.x;
	i32 y2 = ent->pos.y;
	CLAMP(x2, 1, MAP_X - 2);
	CLAMP(y2, 1, MAP_Y - 2);
	f64 distance  =  MAP_DIJKSTRA(map,x2, y2);

	//MOVES WILL DEPEND OF WHAT MONSTER IS!!!

	//+1x
	f64 distancesMax = MAP_DIJKSTRA(map, (x2 + 1), y2);
	if(distancesMax == INF) {
		distancesMax = 0.0f;
		}
	i32 index = 0;
	//-1x
	distance = MAP_DIJKSTRA(map, (x2 - 1), y2);
	if(distance > distancesMax  && distance != INF) {
		distancesMax = distance;
		index = 1;
		}
	//+1y
	distance = MAP_DIJKSTRA(map, x2, (y2 + 1));
	if(distance > distancesMax && distance != INF) {
		distancesMax = distance;
		index = 2;
		}
	//-1y
	distance = MAP_DIJKSTRA(map, x2, (y2  - 1));
	if(distance > distancesMax  && distance != INF) {
		distancesMax = distance;
		index = 3;
		}
	//-1y -1x KEY_Q
	distance = MAP_DIJKSTRA(map, (x2 - 1), (y2  - 1));
	if(distance > distancesMax  && distance != INF) {
		distancesMax = distance;
		index = 4;
		}
	//-1y +1x KEY_E_
	distance = MAP_DIJKSTRA(map, (x2 + 1), (y2  - 1));
	if(distance > distancesMax  && distance != INF) {
		distancesMax = distance;
		index = 5;
		}
	//1y -1x KEY_Y
	distance = MAP_DIJKSTRA(map, (x2 - 1), (y2  + 1));
	if(distance > distancesMax  && distance != INF) {
		distancesMax = distance;
		index = 6;
		}
	//1y 1x KEY_C
	distance = MAP_DIJKSTRA(map, (x2 + 1), (y2  + 1));
	if(distance > distancesMax  && distance != INF) {
		distancesMax = distance;
		index = 7;
		}
	u8 isMoved = 0;
	switch(index) {
		case 0: {
				if(MAP_ISW(map, ent->pos.x + 1, ent->pos.y)) {
					//LOG("X++\n");
					ent->pos.x = ent->pos.x + 1;
					isMoved = 1;
					}

				break;
				}


		case 1: {
				if(MAP_ISW(map, ent->pos.x - 1, ent->pos.y)) {
					//LOG("X--\n");
					ent->pos.x--;
					isMoved = 1;
					}
				break;
				}
		case 2: {
				if(MAP_ISW(map, ent->pos.x, ent->pos.y + 1)) {
					//LOG("Y++\n");
					ent->pos.y++;
					isMoved = 1;
					}
				break;
				}
		case 3: {
				if(MAP_ISW(map, ent->pos.x, ent->pos.y - 1)) {
					//LOG("Y--\n");
					ent->pos.y--;
					isMoved = 1;
					}
				break;
				}
		case 4: {
				if(MAP_ISW(map, ent->pos.x - 1, ent->pos.y - 1)) {
					//LOG("Y--\n");
					ent->pos.x--;
					ent->pos.y--;
					isMoved = 1;
					}
				break;
				}

		case 5: {
				if(MAP_ISW(map, ent->pos.x + 1, ent->pos.y - 1)) {
					//LOG("Y--\n");
					ent->pos.x++;
					ent->pos.y--;
					isMoved = 1;
					}
				break;
				}
		case 6: {
				if(MAP_ISW(map, ent->pos.x - 1, ent->pos.y + 1)) {
					//LOG("Y--\n");
					ent->pos.x--;
					ent->pos.y++;
					isMoved = 1;
					}
				break;
				}
		case 7: {
				if(MAP_ISW(map, ent->pos.x + 1, ent->pos.y + 1)) {
					//LOG("Y--\n");
					ent->pos.x++;
					ent->pos.y++;
					isMoved = 1;
					}
				break;
				}
		default: {
				ASSERT("Unreachable");
				break;
				}
		}
	if(isMoved == 0) {
		make_best_move(player,ent, map);
		}
	CLAMP(ent->pos.x, 2, MAP_X - 2);
	CLAMP(ent->pos.y, 2, MAP_Y - 2);

	}
void make_best_move(Entitiy* player, Entitiy*  ent, Tile *map) {
	SDL_bool isMonsterVisible = is_monster_visible(map, ent);
	i32 x1 = player->pos.x;
	i32 y1 = player->pos.y;
	i32 x2 = ent->pos.x;
	i32 y2 = ent->pos.y;
	f64 distance  = DISTANCE(x1, y1, x2, y2);

	//MOVES WILL DEPEND OF WHAT MONSTER IS!!!

	//+1x
	f64 distancesMin = distnace_move(x1, y1, (x2 + 1), y2, map);
	i32 index = 0;
	//-1x
	distance = distnace_move(x1, y1, (x2 - 1), y2, map);
	if(distance < distancesMin) {
		distancesMin = distance;
		index = 1;
		}
	//+1y
	distance = distnace_move(x1, y1, x2, (y2 + 1), map);
	if(distance < distancesMin) {
		distancesMin = distance;
		index = 2;
		}
	//-1y
	distance = distnace_move(x1, y1, x2, (y2  - 1), map);
	if(distance < distancesMin) {
		distancesMin = distance;
		index = 3;
		}
	//-1y -1x KEY_Q
	distance = distnace_move(x1, y1, (x2 - 1), (y2  - 1), map);
	if(distance < distancesMin) {
		distancesMin = distance;
		index = 4;
		}
	//-1y +1x KEY_E_
	distance = distnace_move(x1, y1, (x2 + 1), (y2  - 1), map);
	if(distance < distancesMin) {
		distancesMin = distance;
		index = 5;
		}
	//1y -1x KEY_Y
	distance = distnace_move(x1, y1, (x2 - 1), (y2  + 1), map);
	if(distance < distancesMin) {
		distancesMin = distance;
		index = 6;
		}
	//1y 1x KEY_C
	distance = distnace_move(x1, y1, (x2 + 1), (y2  + 1), map);
	if(distance < distancesMin) {
		distancesMin = distance;
		index = 7;
		}
	u8 isRangeAttack = SDL_FALSE;
	switch(ent->ch) {
		case 'A': {
				if(distancesMin >= DISTANCE_RANGE_ATTACK_MIN && distancesMin <= DISTANCE_RANGE_ATTACK_MAX ) {
					isRangeAttack = SDL_TRUE;
					monster_attack(player, ent, distancesMin);
					break;
					}
				}
		case 'B': {
				if(distancesMin >= DISTANCE_RANGE_ATTACK_MIN && distancesMin <= DISTANCE_RANGE_ATTACK_MAX ) {
					isRangeAttack = SDL_TRUE;
					monster_attack(player, ent, distancesMin);
					break;
					}
				}
		case 'F': {
				if(distancesMin >= DISTANCE_RANGE_ATTACK_MIN && distancesMin <= DISTANCE_RANGE_ATTACK_MAX ) {
					isRangeAttack = SDL_TRUE;
					monster_attack(player, ent, distancesMin);
					break;
					}
				}

		case 'D': {
				if(distancesMin >= DISTANCE_RANGE_ATTACK_MIN &&distancesMin <= DISTANCE_RANGE_ATTACK_MAX ) {
					isRangeAttack = SDL_TRUE;
					monster_attack(player, ent, distancesMin);
					break;
					}
				}
		case 'W': {
				if(distancesMin >= DISTANCE_RANGE_ATTACK_MIN &&distancesMin <= DISTANCE_RANGE_ATTACK_MAX ) {
					isRangeAttack = SDL_TRUE;
					monster_attack(player, ent, distancesMin);
					break;
					}
				}
		}
	if (isRangeAttack == SDL_TRUE) {
		return;
		}

	switch(index) {
		case 0: {
				if(distancesMin < INF && distancesMin != 0.0f) {
					//LOG("X++\n");
					ent->pos.x = ent->pos.x + 1;
					}
				break;
				}

		case 1: {
				if(distancesMin < INF && distancesMin != 0.0f) {
					//LOG("X--\n");
					ent->pos.x--;
					}
				break;
				}
		case 2: {
				if(distancesMin < INF && distancesMin != 0.0f) {
					//LOG("Y++\n");
					ent->pos.y++;
					}
				break;
				}
		case 3: {
				if(distancesMin < INF && distancesMin != 0.0f) {
					//LOG("Y--\n");
					ent->pos.y--;
					}
				break;
				}
		case 4: {
				if(distancesMin < INF && distancesMin != 0.0f) {
					//LOG("Y--\n");
					ent->pos.x--;
					ent->pos.y--;
					}
				break;
				}

		case 5: {
				if(distancesMin < INF && distancesMin != 0.0f) {
					//LOG("Y--\n");
					ent->pos.x++;
					ent->pos.y--;
					}
				break;
				}
		case 6: {
				if(distancesMin < INF && distancesMin != 0.0f) {
					//LOG("Y--\n");
					ent->pos.x--;
					ent->pos.y++;
					}
				break;
				}
		case 7: {
				if(distancesMin < INF && distancesMin != 0.0f) {
					//LOG("Y--\n");
					ent->pos.x++;
					ent->pos.y++;
					}
				break;
				}
		default: {
				ASSERT("Unreachable");
				break;
				}
		}
	CLAMP(ent->pos.x, 1, MAP_X - 1);
	CLAMP(ent->pos.y, 1, MAP_Y - 1);
	}
//WE WILL SEE IF A* or Diakstra or This CRAP
void make_move_diakstra(Entitiy* player, Entitiy*  ent, Tile *map) {
	SDL_bool isMonsterVisible = is_monster_visible(map, ent);
	i32 x1 = player->pos.x;
	i32 y1 = player->pos.y;
	i32 x2 = ent->pos.x;
	i32 y2 = ent->pos.y;
	CLAMP(x1, 1, MAP_X - 2);
	CLAMP(x2, 1, MAP_X - 2);
	CLAMP(y1, 1, MAP_Y - 2);
	CLAMP(y2, 1, MAP_Y - 2);
	f64 distance  = DISTANCE(x1, y1, x2, y2);
	if(distance <= DISTANCE_RANGE_ATTACK_MIN) {
		monster_attack(player, ent, distance);
		//LOG("Range\n");
		//da_append(&MESSAGES, "Mele");
		return;
		}
	//MOVES WILL DEPEND OF WHAT MONSTER IS!!!

	//+1x
	f64 distancesMin = INF;
	f64 trueDistance = INF;
	i32 index = 0;
	//-1x
	distance = MAP_DIJKSTRA(map, (x2 + 1), y2) ;

	if(distance < distancesMin) {
		distancesMin = distance;
		trueDistance = distnace_move(x1, y1, (x2 + 1), y2, map);
		index = 0;
		}

	//+1x
	distance = MAP_DIJKSTRA(map, (x2 - 1), y2) ;

	if(distance < distancesMin) {
		distancesMin = distance;
		trueDistance = distnace_move(x1, y1, (x2 - 1), y2, map);
		index = 1;
		}
	//+1y
	distance = MAP_DIJKSTRA(map, (x2), (y2 + 1));
	if(distance < distancesMin) {
		distancesMin = distance;
		trueDistance = distnace_move(x1, y1, (x2), (y2 + 1), map);
		index = 2;
		}
	//-1y
	distance = MAP_DIJKSTRA(map, (x2), (y2 - 1));
	if(distance < distancesMin) {
		distancesMin = distance;
		trueDistance = distnace_move(x1, y1, (x2), (y2 - 1), map);
		index = 3;
		}
	//-1y -1x KEY_Q
	distance = MAP_DIJKSTRA(map, (x2 - 1), (y2 - 1));
	if(distance < distancesMin) {
		distancesMin = distance;
		trueDistance = distnace_move(x1, y1, (x2 - 1), (y2 - 1), map);
		index = 4;
		}
	//-1y +1x KEY_E_
	distance = MAP_DIJKSTRA(map, (x2 + 1), (y2 - 1));
	if(distance < distancesMin) {
		distancesMin = distance;
		trueDistance = distnace_move(x1, y1, (x2 + 1), (y2 - 1), map);
		index = 5;
		}
	//1y -1x KEY_Y
	distance = MAP_DIJKSTRA(map, (x2 - 1), (y2 + 1));
	if(distance < distancesMin) {
		distancesMin = distance;
		trueDistance = distnace_move(x1, y1, (x2 - 1), (y2 + 1), map);
		index = 6;
		}
	//1y 1x KEY_C
	distance = MAP_DIJKSTRA(map, (x2 + 1), (y2 + 1));
	if(distance < distancesMin) {
		distancesMin = distance;
		trueDistance = distnace_move(x1, y1, (x2 + 1), (y2 + 1), map);
		index = 7;
		}

	//RANGE ATTACK
	u8 isRangeAttack = SDL_FALSE;
	switch(ent->ch) {
		case 'A': {
				if(trueDistance >= DISTANCE_RANGE_ATTACK_MIN && trueDistance <= DISTANCE_RANGE_ATTACK_MAX ) {
					isRangeAttack = SDL_TRUE;
					monster_attack(player, ent, trueDistance);
					break;
					}
				}
		case 'B': {
				if(trueDistance >= DISTANCE_RANGE_ATTACK_MIN && trueDistance <= DISTANCE_RANGE_ATTACK_MAX ) {
					isRangeAttack = SDL_TRUE;
					monster_attack(player, ent, trueDistance);
					break;
					}
				}
		case 'F': {
				if(trueDistance >= DISTANCE_RANGE_ATTACK_MIN && trueDistance <= DISTANCE_RANGE_ATTACK_MAX ) {
					isRangeAttack = SDL_TRUE;
					monster_attack(player, ent, trueDistance);
					break;
					}
				}

		case 'D': {
				if(trueDistance >= DISTANCE_RANGE_ATTACK_MIN && trueDistance <= DISTANCE_RANGE_ATTACK_MAX ) {
					isRangeAttack = SDL_TRUE;
					monster_attack(player, ent, trueDistance);
					break;
					}
				}
		case 'W': {
				if(trueDistance >= DISTANCE_RANGE_ATTACK_MIN && trueDistance <= DISTANCE_RANGE_ATTACK_MAX ) {
					isRangeAttack = SDL_TRUE;
					monster_attack(player, ent, trueDistance);
					break;
					}
				}

		}
	if (isRangeAttack == SDL_TRUE) {
		return;
		}
	u8 isMoved = 0;
	switch(index) {
		case 0: {
				if(MAP_ISW(map, ent->pos.x + 1, ent->pos.y)) {
					//LOG("X++\n");
					ent->pos.x = ent->pos.x + 1;
					isMoved = 1;
					}

				break;
				}


		case 1: {
				if(MAP_ISW(map, ent->pos.x - 1, ent->pos.y)) {
					//LOG("X--\n");
					ent->pos.x--;
					isMoved = 1;
					}
				break;
				}
		case 2: {
				if(MAP_ISW(map, ent->pos.x, ent->pos.y + 1)) {
					//LOG("Y++\n");
					ent->pos.y++;
					isMoved = 1;
					}
				break;
				}
		case 3: {
				if(MAP_ISW(map, ent->pos.x, ent->pos.y - 1)) {
					//LOG("Y--\n");
					ent->pos.y--;
					isMoved = 1;
					}
				break;
				}
		case 4: {
				if(MAP_ISW(map, ent->pos.x - 1, ent->pos.y - 1)) {
					//LOG("Y--\n");
					ent->pos.x--;
					ent->pos.y--;
					isMoved = 1;
					}
				break;
				}

		case 5: {
				if(MAP_ISW(map, ent->pos.x + 1, ent->pos.y - 1)) {
					//LOG("Y--\n");
					ent->pos.x++;
					ent->pos.y--;
					isMoved = 1;
					}
				break;
				}
		case 6: {
				if(MAP_ISW(map, ent->pos.x - 1, ent->pos.y + 1)) {
					//LOG("Y--\n");
					ent->pos.x--;
					ent->pos.y++;
					isMoved = 1;
					}
				break;
				}
		case 7: {
				if(MAP_ISW(map, ent->pos.x + 1, ent->pos.y + 1)) {
					//LOG("Y--\n");
					ent->pos.x++;
					ent->pos.y++;
					isMoved = 1;
					}
				break;
				}
		default: {
				ASSERT("Unreachable");
				break;
				}
		}
	if(isMoved == 0) {
		make_best_move(player,ent, map);
		}
	CLAMP(ent->pos.x, 1, MAP_X - 1);
	CLAMP(ent->pos.y, 1, MAP_Y - 1);

	}

SDL_bool is_monster_visible(Tile* map, Entitiy* ent) {
	if(MAP_ISV(map, ent->pos.x, ent->pos.y) == SDL_FALSE) {
		return SDL_FALSE;
		}
	return SDL_TRUE;
	}

//#define LOG_AL
void state_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map) {
	DROP(player);
	for(u64 i = 0; i < entitys->count; i++) {
		Entitiy entity = entitys->items[i];

		//IF HEALTH IS A LOW RUNING MONSTER
		//ELSE GO BESERK
		if(Is_Monster(entity.ch)) {
			entity.spell.passedTurns++;
			if(rand_f64() < entity.chanceToDecressStaminaMove) {
				entity.stamina--;
				CLAMP(entity.stamina, 0, entity.maxStamina);
				}
			if(entity.health == 0) {
				if(rand_f64() < entity.stateChance[STATE_RESURECT]) {
					i32 health = rand()%2 + 1;
					entity.attack[DAMAGE_BASIC]++;
					entity.color = RED;
					char* msg = calloc(50, sizeof(char));
					snprintf(msg, 50, "%s is resurected", entity.name);
					da_append(&MESSAGES, msg);

					}
				else {
					char* msg = calloc(50, sizeof(char));
					snprintf(msg, 50, "You slay the %s", entity.name);
					da_append(&MESSAGES, msg);
					char* msg1 = "His spirit is wandering in this world";
					da_append(&MESSAGES, msg1);
					entity.ch = 'S';
					entity.color = GREEN;
					}
				}
			else if(entity.health == 1) {
				if(rand_f64() < entity.stateChance[STATE_RUNING]) {
					entity.state = STATE_RUNING;
					}
				else if(rand_f64() < CHANCE_INCREMENT_HEALTH) {
					entity.health++;
					entity.state = STATE_HUNTING;
					}
				else {
					entity.state = STATE_BESERK;
					}
				}
			else if(entity.stamina <= 0) {
				entity.state = STATE_RESTING;
				}
			//else if(player->health <= 3 || rand_f64() < ((player->maxHealth - player->health) / 100)) {
			else if(player->health <= 0.3*player->maxHealth) {
				entity.state = STATE_HUNTING;

				}
			else if(rand_f64() < entity.stateChance[STATE_SUMMON]  && entity.spell.cooldown < entity.spell.passedTurns) {
				entity.state = STATE_SUMMON;
				entity.spell.passedTurns = 0; //RESET

				}
			else if	(is_monster_visible(map, &entity) == SDL_TRUE && rand_f64() < entity.stateChance[STATE_SPELL] && entity.spell.passedTurns > entity.spell.cooldown) {
				i32 distance = DISTANCE(player->pos.x, player->pos.y, entity.pos.x, entity.pos.y);
				if(distance <= DISTANCE_RANGE_ATTACK_MAX + 2) {
					entity.state = STATE_SPELL;
					entity.spell.passedTurns = 0;
					}
				else {
					entity.state = STATE_MOVING_AWAY_RANGE;
					}
				}
			else if(entity.state == STATE_WANDERING) {
				if(is_monster_visible(map, &entity) == SDL_TRUE) {
					if(rand_f64() < entity.stateChance[STATE_HUNTING]) {
						entity.state  = STATE_HUNTING;
						}
					}
				else if(rand_f64() < entity.stateChance[STATE_RESTING]) {
					entity.state  = STATE_RESTING;

					}
				}
			else if(entity.state == STATE_HUNTING && rand_f64() < entity.stateChance[STATE_RESTING]) {
					{
					entity.state  = STATE_RESTING;
					if(rand_f64() < 0.10f) {
						da_append(&MESSAGES, "You stop hearing noices");
						}
					}
				}

			else if(entity.state == STATE_RESTING && rand_f64() < entity.stateChance[STATE_WANDERING]) {
					{
					entity.state  = STATE_WANDERING;

					}
				}
			else if(is_monster_visible(map, &entity) == SDL_TRUE && rand_f64() < entity.stateChance[STATE_HUNTING]) {
				entity.state = STATE_HUNTING;
				}
			else if(rand_f64() < entity.stateChance[STATE_MOVING_AWAY_RANGE]) {
				entity.state = STATE_MOVING_AWAY_RANGE;
				}
			else if(rand_f64() < entity.stateChance[STATE_RUNING]) {
				entity.state = STATE_RUNING;
				}
			else {
				entity.state = STATE_RESTING;
				}

			}
		else if(entity.ch == 'S') {
			entity.state = STATE_WANDERING;
			}
		entitys->items[i] = entity;
		}
	}


//IF IN VISON FIELD MOVE TOWARDS PLAYER
//IF NOT RAND MOV DEPENDING ON TYPE OR PROB
void move_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map) {
	i32 co = 0;
	DROP(co);
	for(u64 count = 0; count < entitys->count; count++) {

		Entitiy entity = entitys->items[count];

		if(Is_Monster(entity.ch)) {
			calculate_diakstra_map(player, map, entitys, player->pos.x, player->pos.y);
			entity.spell.passedTurns++;
			if(entity.state == STATE_RESTING) {
				//NOTHING
				entity.stamina++;
				}
			else if(entity.state == STATE_WANDERING) {
				MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_TRUE;
				i32 chance = rand()%5;
				if(chance) {
					make_run_move(player, &entity, map);
					}
				else {
					make_move_diakstra(player, &entity, map);
					}
				MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_FALSE;
				}
			else if(entity.state == STATE_HUNTING) {
				MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_TRUE;
				//make_run_move(player, &entity, map);
				make_move_diakstra(player, &entity, map);
				MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_FALSE;
				}
			else if(entity.state == STATE_BESERK) {
				MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_TRUE;
				make_move_diakstra(player, &entity, map);
				MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_FALSE;
				}
			else if(entity.state == STATE_RUNING) {
				MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_TRUE;
				make_run_move(player, &entity, map);
				MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_FALSE;
				}
			else if(entity.state == STATE_MOVING_AWAY_RANGE) {
				MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_TRUE;
				make_run_move(player, &entity, map);
				MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_FALSE;
				entity.state = STATE_HUNTING;
				}
			else if(entity.state == STATE_SUMMON) {
				Monster_Types type = entity.spell.value;
				//LOG("TYPE %d", entity.spell.value);
				Entitiy* summon = create_entity(monsterChar[type], monsterName[type], 30, 3, (Position) {
					.x = entity.pos.x, .y = entity.pos.y
					}, WHITE);
				summon->health = 2;
				summon->spell.passedTurns = 0;
				summon->spell.cooldown = 255;
				summon->stateChance[STATE_SUMMON] = 0.00f;
				summon->stateChance[STATE_SPELL] = 0.00f;
				summon->state = STATE_HUNTING;
				make_run_move(player, &entity, map);
				entity.state = STATE_RUNING;
				entity.spell.cooldown *=4;
				da_append(entitys, *summon);
				}
			else if(entity.state == STATE_SPELL) {
				switch (entity.spell.type) {

					case SPELL_DECRESE_MAX_HEALTH: {
							entity.spell.passedTurns = 0;
							char* msg = "You are cursed -1 max health";
							da_append(&MESSAGES, msg);
							player->maxHealth--;
							player->health--;
							entity.state = STATE_WANDERING;
							CLAMP(player->maxHealth, 0, INF);
							CLAMP(player->health, 0, INF);
							break;
							}

					case SPELL_STUN: {
							entity.spell.passedTurns = 0;
							char* msg = "You are stunded";
							player->isStunded = entity.spell.value;
							entity.state = STATE_HUNTING;
							da_append(&MESSAGES, msg);
							break;
							}

					case SPELL_DOUBLE_MOVE: {
							entity.spell.passedTurns = 0;
							for(i32 i = 0; i < entity.spell.value; i++) {
								MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_TRUE;
								make_best_move(player, &entity, map);
								MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_FALSE;
								}
							break;
							}
					case SPELL_KABUM: {
							i32 damage = player->health * entity.spell.value / 100;
							CLAMP(damage, 1, INF);
							char* msg = "Kabooom";
							da_append(&MESSAGES, msg);
							player->health-=damage;
							CLAMP(player->health, 0, INF);
							MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_TRUE;
							//MAYBE CASAM
							//MAP_CH(map, entity.pos.x, entity.pos.y) = TILE_RUINS;
							entity.health = 0;
							break;
							}

					case SPELL_STATIS_TRAP: {
							entity.spell.passedTurns = 0;
							i32 tileX = rand()%entity.spell.value - entity.spell.value / 2;
							i32 tileY = rand()%entity.spell.value - entity.spell.value / 2;
							tileX += entity.pos.x;
							tileY += entity.pos.y;
							CLAMP(tileX, 1, MAP_X - 1);
							CLAMP(tileY, 1, MAP_Y - 1);
							if(MAP_ISW(map, tileX, tileY) == SDL_TRUE) {
								MAP_CH(map, tileX, tileY) = TILE_STUN_TRAP;
								//LOG("SetTrap\n");
								}


							break;
							}

					default:
						ASSERT("Unrechable\n");
						break;
					}
				}


			}
		/*else{
			MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_TRUE;
			f64 chance = CHANCE_SPIRIT_ATTACK * (f64)COUNTMOVES;
			CLAMP(chance, 0.0f, 0.10f);
			if(rand_f64() < chance){
				make_best_move(player, &entity, map);
			}
			else{
				make_run_move(player, &entity, map);
			}

			MAP_ISW(map, entity.pos.x, entity.pos.y) = SDL_FALSE;
		}  */

		entitys->items[count] = entity;
		//LOG("Colided entitys %d\n", co);
		}
	}
void increment_player_health(Entitiy* player) {
	if(player->maxHealth != player->health) {
		//LOG("Difrent\n");
		f64 chance = rand_f64();
		if(chance < CHANCE_INCREMENT_HEALTH) {
			player->health++;
			da_append(&MESSAGES, "Good luck strike you health +1\n");
			}
		}
	}
void player_destroy_boolder(Entitiy* player, Tile* map) {
	//SDL_bool isDoor = SDL_FALSE;
	i32 startX = player->pos.x - 1;
	i32 stopX  = player->pos.x + 1;
	i32 startY = player->pos.y - 1;
	i32 stopY  = player->pos.y + 1;
	CLAMP(startX, 0, MAP_X-1);
	CLAMP(stopX, 0, MAP_X-1);
	CLAMP(startY, 0, MAP_Y-1);
	CLAMP(stopY, 0, MAP_Y-1);
	for (i32 y = startY; y <= stopY; y++) {
		for (i32 x = startX; x <= stopX; x++) {
			if(MAP_CH(map, x, y) == TILE_RUINS) {
				if(rand_f64() < CHANCE_NON_CLEAR_RUINS) {
					char* msg = "This ruins are tuff";
					da_append(&MESSAGES, msg);

					}
				else {
					char* msg = "You clear the ruins";
					da_append(&MESSAGES, msg);
					MAP_CH(map, x, y) = TILE_FLOOR;
					MAP_ISW(map, x, y) = SDL_TRUE;
					if(rand_f64() < CHANCE_DMG_CLEAR_RUINS) {
						i32 ran = rand()%4;
						player->health-=ran;
						char* msg = "This ruin is tuff";
						da_append(&MESSAGES, msg);
						char* msg1 = "Your health decresed";
						da_append(&MESSAGES, msg1);
						}
					}

				}

			}
		}
	OPENDOOR = SDL_FALSE;

	}

void update_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map, Item_DA *items) {


	state_entity(player, entitys, map);
	move_entity(player, entitys, map);
	block_movement(entitys, map);
	field_of_vison(player, map);
	increment_player_health(player);
	//is_player_trigerd_trap(ma)
	player_trap_calculations(map, player, entitys);
	COUNTMOVES++;
	if(OPENDOOR == SDL_TRUE) {
		//player_open_door(player, map);
		player_destroy_boolder(player, map);
		}


	if(PICKITEM == SDL_TRUE) {
		picking_item_from_list(player, items);
		PICKITEM = SDL_FALSE;
		}
	calculate_diakstra_map(player, map, entitys, player->pos.x, player->pos.y);
	}


Entitiy* create_inventory(i32 size) {
	if(size < 0) {
		ASSERT("Size for inventory is zero");
		}
	Entitiy* inventory = calloc(size, sizeof(Entitiy*));
	if(inventory == NULL) {
		ASSERT("Calloc is null");
		}
	return inventory;
	}

SDL_bool check_if_item_and_player_colide(Entitiy* player, Item* item) {
	if(player->pos.x == item->pos.x && player->pos.y == item->pos.y) {
		return SDL_TRUE;
		}
	return SDL_FALSE;
	}

void picking_item_from_list(Entitiy* entity, Item_DA *items) {
	for(u64 i = 0; i < items->count; i++) {
		Item item = items->items[i];
		if(check_if_item_and_player_colide(entity, &item) == SDL_TRUE) {
			pick_item_from_ground(&items->items[i], &entity->inventory);
			char* msg = calloc(MAX_NAME, sizeof(char*));
			snprintf(msg, MAX_NAME, "You picked %s", item.name);
			da_append(&MESSAGES, msg);
			}
		}
	}



void calculate_diakstra_map(Entitiy* player, Tile* map, Entitiy_DA* entitys, i32 goalX, i32 goalY) {


	for (i32 i = 0; i < MAP_X * MAP_Y; i++) {
		map[i].distance = INF;
		}

	MAP_DIJKSTRA(map, goalX, goalY) = 0.0f;
	Position* queue = calloc(MAP_X * MAP_Y, sizeof(Position));
	i32 front = 0, back = 0;

	queue[back++] = (Position) {
		goalX, goalY
		};

	// 8 directions: N, NE, E, SE, S, SW, W, NW
	const i32 dx[8] = {  0,  1, 1,  1,  0, -1, -1, -1 };
	const i32 dy[8] = { -1, -1, 0,  1,  1,  1,  0, -1 };

	while (front < back) {
		Position p = queue[front++];
		f64 current_dist = MAP_DIJKSTRA(map, p.x, p.y);

		for (i32 dir = 0; dir < 8; dir++) {
			i32 nx = p.x + dx[dir];
			i32 ny = p.y + dy[dir];

			if (nx < 0 || ny < 0 || nx >= MAP_X || ny >= MAP_Y) continue;
			if (MAP_ISW(map, nx, ny) == SDL_FALSE) continue;

			if (MAP_DIJKSTRA(map, nx, ny) > current_dist + 1.0f) {
				MAP_DIJKSTRA(map, nx, ny) = current_dist + 1.0f;
				queue[back++] = (Position) {
					nx, ny
					};
				}
			}
		}
	free(queue);
	/* //HEARD BEHAIVIOR
	for(u64 i = 0 ; i < entitys->count; i++){
		Entitiy ent = entitys->items[i];
		if(Is_Monster(ent.ch)){
			i32 startX = ent.pos.x - 1;
			CLAMP(startX, 0, MAP_X - 1);
			i32 startY = ent.pos.y - 1;
			CLAMP(startY, 0, MAP_Y - 1);
			i32 stopX = ent.pos.x - 1;
			CLAMP(stopX, 0, MAP_X - 1);
			i32 stopY = ent.pos.y - 1;
			CLAMP(stopY, 0, MAP_Y - 1);
			for (i32 y = startY; y < stopY; y++){
				for (i32 x = startX; x < stopX ; x++){
					MAP_DIJKSTRA(map, x, y) += 1;
				}
			}

		}
	}  */
	for(u64 i = 0 ; i < entitys->count; i++) {
		Entitiy ent = entitys->items[i];
		if(Is_Monster(ent.ch)) {
			i32 startX = ent.pos.x - 2;
			CLAMP(startX, 2, MAP_X - 2);
			i32 startY = ent.pos.y - 2;
			CLAMP(startY, 2, MAP_Y - 2);
			i32 stopX = ent.pos.x - 2;
			CLAMP(stopX, 2, MAP_X - 2);
			i32 stopY = ent.pos.y - 2;
			CLAMP(stopY, 2, MAP_Y - 2);
			for (i32 y = startY; y < stopY; y++) {
				for (i32 x = startX; x < stopX ; x++) {
					MAP_DIJKSTRA(map, x, y) += 2;
					CLAMP(MAP_DIJKSTRA(map, x, y), 0, INF);
					}
				}

			}
		}
	DROP(entitys);
	DROP(player);
	}



void use_item(Entitiy* player, Entitiy_DA *entitis, Item_DA *items, u64 numItem) {
	Item itemToEquipt = items->items[numItem];
	Item_Equipted type = itemToEquipt.equipedTo;
	switch(itemToEquipt.type) {
		case HEALING_ITEM: {
				i32 isCursed = 0;
				if(itemToEquipt.isCursed == NORMAL || itemToEquipt.isCursed == BLESED) {
					da_append(&MESSAGES,"Seems to restor the health");
					player->health+= player->maxHealth * itemToEquipt.health / 100;
					CLAMP(player->health, 0, player->maxHealth);
					da_append(&MESSAGES, "This seem to be potion of healing");
					}
				else {
					da_append(&MESSAGES,"Seems to decreased the health");
					player->health-= player->maxHealth * itemToEquipt.health / 100;
					CLAMP(player->health, 0, player->maxHealth);
					da_append(&MESSAGES, "This seem to be cursed potion of healing");

					}
				da_remove_unordered(items, numItem);
				break;
				}
		case GOLD_ITEM: {
				da_append(&MESSAGES, "Use it for what");
				break;
				}
		default: {
				ASSERT("UNRECHABLE");
				break;
				}
		}
	DROP(entitis);
	}

void export_generators() {
	generators[GENERATOR_GRAVEYARD].type = GENERATOR_GRAVEYARD;
	memset(generators[GENERATOR_GRAVEYARD].chanceToSpawn, 0.0f, NUM_MONSTER * sizeof(f64));
	generators[GENERATOR_GRAVEYARD].chanceToSpawn[ACOLAYT_MONSTER] = 0.9f;
	generators[GENERATOR_GRAVEYARD].chanceToSpawn[NECROMANCER_MONSTER] = 0.05f;
	generators[GENERATOR_GRAVEYARD].chanceToSpawn[BANSHIE_MONSTER] = 0.1f;
	generators[GENERATOR_GRAVEYARD].levelDungon = 1;
	generators[GENERATOR_GRAVEYARD].monsterNumber = 0;
	generators[GENERATOR_GRAVEYARD].maxDistanceDikstra = 20;
	generators[GENERATOR_GRAVEYARD].typeOfTile = TILE_BLIGHT;

	generators[GENERATOR_CAVE].type = GENERATOR_CAVE;
	memset(generators[GENERATOR_CAVE].chanceToSpawn, 0.0f, NUM_MONSTER * sizeof(f64));
	generators[GENERATOR_CAVE].chanceToSpawn[GOBLIN_MOSNTER] = 0.9f;
	generators[GENERATOR_CAVE].chanceToSpawn[RAT_MONSTER] = 0.04f;
	generators[GENERATOR_CAVE].chanceToSpawn[ARCHER_MONSTER] = 0.5f;
	generators[GENERATOR_CAVE].chanceToSpawn[WITCH_MONSTER] = 0.01f;
	generators[GENERATOR_CAVE].levelDungon = 1;
	generators[GENERATOR_CAVE].monsterNumber = 1;
	generators[GENERATOR_CAVE].maxDistanceDikstra = 10;
	generators[GENERATOR_CAVE].typeOfTile = TILE_RUINS;

	//ORC
	generators[GENERATOR_ORC].type = GENERATOR_ORC;
	memset(generators[GENERATOR_ORC].chanceToSpawn, 0.0f, NUM_MONSTER * sizeof(f64));
	generators[GENERATOR_ORC].chanceToSpawn[GRUNT_MONSTER] = 0.9f;
	generators[GENERATOR_ORC].chanceToSpawn[RAT_MONSTER] = 0.04f;
	generators[GENERATOR_ORC].chanceToSpawn[ARCHER_MONSTER] = 0.1f;
	generators[GENERATOR_ORC].chanceToSpawn[WITCH_MONSTER] = 0.1f;
	generators[GENERATOR_ORC].chanceToSpawn[BERSERKER_MONSTER] = 0.9f;
	generators[GENERATOR_ORC].levelDungon = 2;
	generators[GENERATOR_ORC].monsterNumber = 1;
	generators[GENERATOR_ORC].maxDistanceDikstra = 20;
	generators[GENERATOR_ORC].typeOfTile = TILE_GRASS;

	//FIEND
	generators[GENERATOR_FIEND].type = GENERATOR_FIEND;
	memset(generators[GENERATOR_FIEND].chanceToSpawn, 0.0f, NUM_MONSTER * sizeof(f64));
	generators[GENERATOR_FIEND].chanceToSpawn[SPIDER_MONSTER] = 0.9f;
	generators[GENERATOR_FIEND].chanceToSpawn[NECROMANCER_MONSTER] = 0.04f;
	generators[GENERATOR_FIEND].chanceToSpawn[GHOUL_MONSTER] = 0.1f;
	generators[GENERATOR_FIEND].levelDungon = 2;
	generators[GENERATOR_FIEND].monsterNumber = 1;
	generators[GENERATOR_FIEND].maxDistanceDikstra = 20;
	generators[GENERATOR_FIEND].typeOfTile = TILE_BLIGHT;

	//NECRO
	generators[GENERATOR_NECRO].type = GENERATOR_NECRO;
	memset(generators[GENERATOR_NECRO].chanceToSpawn, 0.0f, NUM_MONSTER * sizeof(f64));
	generators[GENERATOR_NECRO].chanceToSpawn[BANSHIE_MONSTER] = 0.4f;
	generators[GENERATOR_NECRO].chanceToSpawn[NECROMANCER_MONSTER] = 0.4f;
	generators[GENERATOR_NECRO].chanceToSpawn[GHOUL_MONSTER] = 0.9f;
	generators[GENERATOR_NECRO].levelDungon = 2;
	generators[GENERATOR_NECRO].monsterNumber = 1;
	generators[GENERATOR_NECRO].maxDistanceDikstra = 20;
	generators[GENERATOR_NECRO].typeOfTile = TILE_BLIGHT;

	//NECRO
	generators[GENERATOR_DRAGON].type = GENERATOR_DRAGON;
	memset(generators[GENERATOR_DRAGON].chanceToSpawn, 0.0f, NUM_MONSTER * sizeof(f64));
	generators[GENERATOR_DRAGON].chanceToSpawn[DRAGON_MONSTER] = 1.0f;
	generators[GENERATOR_DRAGON].levelDungon = 40;
	generators[GENERATOR_DRAGON].monsterNumber = 0;
	generators[GENERATOR_DRAGON].maxDistanceDikstra = 20;
	generators[GENERATOR_DRAGON].typeOfTile = TILE_ROAD;






	}
void genereate_monsters_generator(Entitiy* player, Entitiy_DA *monsters, Tile *map, i32 level, Room room) {
	i32 stopY = room.pos.y + room.height;
	CLAMP(stopY, 3, MAP_Y - 2);
	i32 stopX = room.pos.x + room.width;
	CLAMP(stopX, 3, MAP_X - 2);
	i32 startX = room.pos.x;
	CLAMP(startX, 3, MAP_Y - 2);
	i32 startY = room.pos.y - 1;
	CLAMP(startY, 3, MAP_Y - 2);
	i32 generatedGenerators = 1; //
	Generator gen;
	i32 counter1=0, counter2=0, counter3=0, counter4=0, counter5=0;
	LOG("\nNum OF GENERATORS %d\n", generatedGenerators);

	while(generatedGenerators > 0) {
		counter1++;
		if(counter1 >= 1000) {
			break;
			}
		Position pos = {0, 0};
		gen.levelDungon = (i32)INF;

		//GENERATE APROPRITE FOR LEVEL
		counter2 = 0;
		while(gen.levelDungon > level) {
			counter2++;
			if(counter2 == 1000) {
				break;
				}
			i32 chance = rand()%GENERATOR_NUM;
			//memcpy(&gen, &generators[chance], sizeof(generators[chance]));
			gen = generators[chance];
			LOG("Generator %d \n", chance);
			LOG("gen.monsterNumber = %d\n", gen.monsterNumber);
			//system("pause");
			}
		//GENERATE CORDINATES FOR START
		counter3 = 0;
		while(MAP_CH(map, pos.x, pos.y) != TILE_FLOOR) {
			counter3++;
			if(counter3 == 1000) {
				break;
				}
			//MUST BE POSITIVE
			//pos.x = (i32)(rand()*(sin(generatedGenerators*pos.y) + 1))%(MAP_X);
			//pos.y = (i32)(rand()*(sin(generatedGenerators*pos.x) + 1))%(MAP_Y);
			pos.x = startX + rand()%room.width;
			pos.y = startY + rand()%room.height;
			CLAMP(pos.x, startX, stopX);
			CLAMP(pos.y, startY, stopY);
			//LOG("Generator pos(%d %d %c)\n", pos.x, pos.y, MAP_CH(map, pos.x, pos.y));
			//system("pause");
			}
		LOG("Generator pos(%d %d %c)\n", pos.x, pos.y, MAP_CH(map, pos.x, pos.y));
		//CALCULATE DIKSTRA DEPENDING ON POS
		//calculate_diakstra_map(player, map, monsters, pos.x, pos.y);
		//GENERATE DEPENDING ON GENERATOR AND CORDINATE

		while(gen.monsterNumber >= 0) {
			//LOG("gen.monsterNumber = %d\n", gen.monsterNumber);
			//system("pause");
			counter4++;
			if(counter4 == 1000) {
				break;
				}
			calculate_diakstra_map(player, map, monsters, pos.x, pos.y);

			//GENERATING RANDOM TILES
			caved_part_generator(gen.typeOfTile, map, gen.maxDistanceDikstra);
			//
			i32 x = 0, y = 0;
			i32 counter6 = 0;
			while(1) {
				counter6++;
				if(counter6 == 1000) {
					break;
					}
				x = rand()%(MAP_X);
				y = rand()%(MAP_Y);
				CLAMP(x, startX, stopX);
				CLAMP(y, startY, stopY);
				//LOG("(X,Y)=>(%d, %d) (%c)\n", x, y, MAP_CH(map, x, y));
				if(MAP_CH(map, x, y) == TILE_FLOOR) {
					//system("pause");
					break;
					}
				}


			if(MAP_DIJKSTRA(map, x, y) <= gen.maxDistanceDikstra) {
				SDL_bool isGenerated = SDL_FALSE;
				i32 type = 0;
				while(!isGenerated) {
					counter5++;
					if(counter5 == 1000) {
						break;
						}
					type = rand()%NUM_MONSTER;
					f64 chance = rand_f64();
					if(chance < gen.chanceToSpawn[type]) {
						isGenerated = SDL_TRUE;
						}
					}
				Entitiy *temp = create_entity(monsterChar[type], monsterName[type], 1, 3, (Position) {
					.x = x, .y = y
					}, WHITE);
				LOG("Monster cordinates (%d %d)\n", x, y);
				temp->state = STATE_WANDERING;
				//items on monsters probobly when are created

				Entitiy ent = *temp;
				da_append(monsters, (ent));
				gen.monsterNumber--;
				//system("pause");
				}


			}


		//TBD TILES TYPES
		generatedGenerators--;
		}
//ITEMS
	}

void player_trap_calculations(Tile* map, Entitiy *player, Entitiy_DA *monsters) {
	if(!is_trap(map, player->pos.x, player->pos.y)) {
		return;
		}

	switch(MAP_CH(map, player->pos.x, player->pos.y)) {
		case TILE_STUN_TRAP: {
				da_append(&MESSAGES, "You steped on stun trap\n");
				//MAYBE CHANCE
				player->isStunded +=1;
				da_append(&MESSAGES, "You are stuned\n");
				MAP_CH(map, player->pos.x, player->pos.y) = TILE_FLOOR;
				break;
				}
		case TILE_RUINS_TRAP: {
				da_append(&MESSAGES, "Seling around you is colapsed");
				da_append(&MESSAGES, "You are traped");
				i32 startX = player->pos.x - 1;
				i32 stopX  = player->pos.x + 1;
				i32 startY = player->pos.y - 1;
				i32 stopY  = player->pos.y + 1;
				f64 chance = rand_f64();
				CLAMP(startX, 0, MAP_X - 1);
				CLAMP(stopX,  0, MAP_X - 1);
				CLAMP(startY, 0, MAP_Y - 1);
				CLAMP(stopY,  0, MAP_Y - 1);
				if(chance < CHANCE_DROP_SELING_CAUSE_DAMAGE) {
					i32 percentDamage = rand_f64();
					i32 damage = (i32)(player->maxHealth * rand_f64());
					player->health -= damage;
					CLAMP(player->health, 0, player->maxHealth);
					}
				for(i32 y = startY; y < stopY; y++) {
					for(i32 x= startX; x < stopX; x++) {
						if(MAP_ISW(map, x, y) == SDL_TRUE || MAP_CH(map, x, y) == TILE_WALL) {
							if(rand_f64() < CHANCE_DROP_SELING_CAUSE_DAMAGE) {
								MAP_CH(map, x, y)  = TILE_RUINS;
								MAP_ISW(map, x, y) = SDL_FALSE;
								}
							}
						}
					}

				MAP_CH(map, player->pos.x, player->pos.y) = TILE_FLOOR;
				break;
				}

		default: {
				ASSERT("UNRECHABLE");
				break;
				}
		}

	DROP(monsters);
	}
