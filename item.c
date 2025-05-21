#include "item.h"

Item* create_item(i32 x, i32 y, i32 health,  const char* name, char ch, SDL_Color color,	Item_Type type, u8 isCursed, SDL_bool isEquipped) {
	Item* item = calloc(1, sizeof(Item));
	if(item == NULL) {
		ASSERT("CALLOC FAIL");
		}
	item->ch = ch;
	item->health = health;
	item->name = calloc(MAX_NAME, sizeof(char));
	item->descripction = calloc(MAX_DESCRIPTION, sizeof(char));
	if(item->name == NULL || item->descripction == NULL) {
		ASSERT("CALLOC FAIL");
		}
	else {
		i32 len =  strlen(name);
		CLAMP(len, 0, (MAX_NAME - 1));
		memcpy(item->name, name, len);
		memcpy(item->descripction, name, len);

		}

	item->pos.x = x;
	item->pos.y = y;
	item->color = color;
	item->isEquiped = isEquipped;
	item->isCursed = isCursed;
	switch(type) {
		//TBD ADD TYPE
		case SWORD_ITEM: {
				for(i32 i = 0; i < DAMAGE_NUM; i++) {
					item->attack[i]   = 0;
					item->defence[i]  = 0;
					}
				item->attack[0]  = rand()%(5 + LEVEL);  //SWORDS ADD +1 FOR BASIC ATTACK AND DEFENCE PROBOBLY EXPORT FOR ALL;
				item->defence[0] = 1;
				item->equipedTo = EQUIPTED_WEPON;
				if(rand_f64() < CHANCE_LIFESTEAL) {
					item->lifeSteal = 1;
					item->lifeStealChance = rand_f64();
					}
				char msg[150];
				snprintf(msg, 150, " B(%d),BD(%d),L(%d,%.2f)", item->attack[DAMAGE_BASIC], item->defence[DAMAGE_BASIC]
				         ,item->lifeSteal, item->lifeStealChance);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;
				}
		case AXE_ITEM: {
				for(i32 i = 0; i < DAMAGE_NUM; i++) {
					item->attack[i]   = 0;
					item->defence[i]  = 0;
					}
				item->attack[DAMAGE_POISON]  = rand()%(5 + LEVEL);  //AXE ADD +1 FOR BASIC ATTACK AND DEFENCE PROBOBLY EXPORT FOR ALL;
				item->defence[DAMAGE_POISON] = 1;
				item->equipedTo = EQUIPTED_WEPON;
				if(rand_f64() < CHANCE_CRIT) {
					item->critDamage = 10;  //RAND()%10
					item->critDamageChance = rand_f64();
					}
				char msg[150];
				snprintf(msg, 150, "H(%d),HD(%d) B(%d),BD(%d)", item->attack[DAMAGE_POISON], item->defence[DAMAGE_POISON],
				         item->attack[DAMAGE_BASIC], item->defence[DAMAGE_BASIC]);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;

				}

		case DAGER_ITEM: {
				for(i32 i = 0; i < DAMAGE_NUM; i++) {
					item->attack[i]   = 0;
					item->defence[i]  = 0;
					}
				item->attack[DAMAGE_BASIC]  = rand()%(5 + LEVEL) + 1;  //AXE ADD +1 FOR BASIC ATTACK AND DEFENCE PROBOBLY EXPORT FOR ALL;
				//item->defence[DAMAGE_POISON] = 1;
				item->equipedTo = EQUIPTED_WEPON;
				if(rand_f64() < CHANCE_CRIT) {
					item->poisonDamage = 10;  //RAND()%10
					item->poisonChance = rand_f64();
					}
				char msg[150];
				snprintf(msg, 150, " H(%d),HD(%d),P(%d,%.2f)", item->attack[DAMAGE_BASIC], item->defence[DAMAGE_BASIC]
				         ,item->poisonDamage, item->poisonChance);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;
				}

		case PIKE_ITEM: {
				for(i32 i = 0; i < DAMAGE_NUM; i++) {
					item->attack[i]   = 0;
					item->defence[i]  = 0;
					}
				item->attack[DAMAGE_BASIC]  = rand()%(5 + LEVEL);  //AXE ADD +1 FOR BASIC ATTACK AND DEFENCE PROBOBLY EXPORT FOR ALL;
				item->defence[DAMAGE_BASIC] = 1;
				item->attack[DAMAGE_POISON]  = rand()%(5 + LEVEL);  //AXE ADD +1 FOR BASIC ATTACK AND DEFENCE PROBOBLY EXPORT FOR ALL;
				item->defence[DAMAGE_POISON] = 1;
				item->equipedTo = EQUIPTED_WEPON;
				if(rand_f64() < CHANCE_CRIT) {
					item->critDamage = 10;  //RAND()%10
					item->critDamageChance = rand_f64();
					}
				char msg[150];
				snprintf(msg, 150, " H(%d),HD(%d),C(%d,%.2f)", item->attack[DAMAGE_BASIC], item->defence[DAMAGE_BASIC]
				         ,item->critDamage, item->critDamageChance);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;

				}

		case SABER_ITEM: {
				for(i32 i = 0; i < DAMAGE_NUM; i++) {
					item->attack[i]   = 0;
					item->defence[i]  = 0;
					}
				item->attack[DAMAGE_BASIC]  = rand()%((5 + LEVEL)/2);  //AXE ADD +1 FOR BASIC ATTACK AND DEFENCE PROBOBLY EXPORT FOR ALL;
				item->defence[DAMAGE_BASIC]  = rand()%((5 + LEVEL)/2);
				item->equipedTo = EQUIPTED_WEPON;
				char msg[150];
				snprintf(msg, 150, " B(%d),BD(%d)", item->attack[DAMAGE_BASIC], item->defence[DAMAGE_BASIC]);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;
				}




		case PLAYER_SWORD_ITEM: {
				for(i32 i = 0; i < DAMAGE_NUM; i++) {
					item->attack[i]   = 0;
					item->defence[i]  = 0;
					}
				item->attack[0]  = 4;  //SWORDS ADD +1 FOR BASIC ATTACK AND DEFENCE PROBOBLY EXPORT FOR ALL;
				item->defence[0] = 1;
				item->equipedTo = EQUIPTED_WEPON;
				if(rand_f64() < CHANCE_LIFESTEAL) {
					item->lifeSteal = 1;
					item->lifeStealChance = rand_f64();
					}
				char msg[150];
				snprintf(msg, 150, " B(%d),BD(%d),L(%d,%.2f)", item->attack[DAMAGE_BASIC], item->defence[DAMAGE_BASIC]
				         ,item->lifeSteal, item->lifeStealChance);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;

				}
		case PLAYER_ARMOR_ITEM: {
				for(i32 i = 0; i < DAMAGE_NUMi; i++) {
					item->attack[i]   = 0;
					item->defence[i]  = rand()%5;
					}
				item->equipedTo = EQUIPTED_ARMOR;
				item->defence[0] = 3;
				char msg[150];
				snprintf(msg, 150, " B(%d),P(%d),R(%d),S(%d),L(%d)",  item->defence[DAMAGE_BASIC], item->defence[DAMAGE_POISON],
				         item->defence[DAMAGE_RANGE], item->defence[DAMAGE_SPELL], item->lifeSteal);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;
				}

		case ARMOR_ITEM: {
				for(i32 i = 0; i < DAMAGE_NUMi; i++) {
					item->attack[i]   = 0;
					item->defence[i]  = rand()%5 + LEVEL;
					}
				item->equipedTo = EQUIPTED_ARMOR;

				char msg[150];
				snprintf(msg, 150, "B(%d),P(%d),R(%d),S(%d),L(%d)", item->defence[DAMAGE_BASIC], item->defence[DAMAGE_POISON],
				         item->defence[DAMAGE_RANGE], item->defence[DAMAGE_SPELL], item->lifeSteal);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;
				}
		case HELMET_ITEM: {

				item->equipedTo = EQUIPTED_HEAD;
				item->defence[DAMAGE_BASIC]  = 1 + rand()%5;
				item->defence[DAMAGE_POISON] = rand()%2;
				item->defence[DAMAGE_RANGE]  = 1 + rand()%3;
				char msg[150];
				snprintf(msg, 150, " B(%d),P(%d),R(%d),S(%d),B(%d),P(%d),R(%d),S(%d),L(%d)", item->attack[DAMAGE_BASIC], item->attack[DAMAGE_POISON],
				         item->attack[DAMAGE_RANGE], item->attack[DAMAGE_SPELL], item->defence[DAMAGE_BASIC], item->defence[DAMAGE_POISON],
				         item->defence[DAMAGE_RANGE], item->defence[DAMAGE_SPELL], item->lifeSteal);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;
				}

		case SHIELD_ITEM: {
				item->equipedTo = EQUIPTED_SHIELD;
				item->attack[DAMAGE_BASIC] = rand()%2;
				item->defence[DAMAGE_BASIC]  = 1 + rand()%5;
				item->defence[DAMAGE_POISON] = rand()%2;
				item->defence[DAMAGE_RANGE]  = 1 + rand()%3;
				char msg[150];
				snprintf(msg, 150, " B(%d),P(%d),R(%d),S(%d),B(%d),P(%d),R(%d),S(%d),L(%d)", item->attack[DAMAGE_BASIC], item->attack[DAMAGE_POISON],
				         item->attack[DAMAGE_RANGE], item->attack[DAMAGE_SPELL], item->defence[DAMAGE_BASIC], item->defence[DAMAGE_POISON],
				         item->defence[DAMAGE_RANGE], item->defence[DAMAGE_SPELL], item->lifeSteal);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;
				}

		case SHOES_ITEM: {
				item->equipedTo = EQUIPTED_LEGS;
				item->defence[DAMAGE_BASIC]  = 1;
				item->defence[DAMAGE_POISON] = rand()%2;
				item->defence[DAMAGE_SPELL]  =rand()%2;
				char msg[150];
				snprintf(msg, 150, " B(%d),P(%d),R(%d),S(%d),B(%d),P(%d),R(%d),S(%d),L(%d)", item->attack[DAMAGE_BASIC], item->attack[DAMAGE_POISON],
				         item->attack[DAMAGE_RANGE], item->attack[DAMAGE_SPELL], item->defence[DAMAGE_BASIC], item->defence[DAMAGE_POISON],
				         item->defence[DAMAGE_RANGE], item->defence[DAMAGE_SPELL], item->lifeSteal);
				strncat(item->descripction, msg, MAX_DESCRIPTION);
				break;
				}
		case HEALING_ITEM: {
				char* text = calloc(MAX_NAME, sizeof(char));
				item->equipedTo = EQUIPTED_USE;
				item->health = rand()%90 + 10;  //PERCENTAGE RESTORE
				item->type = HEALING_ITEM;
				if(CHANCE_ITEM_NOT_IDENT > rand_f64()) {
					for(i32 i = 0; i < 10; i++) {
						text[i] = rand()%(125 - 33) + 33;
						}
					}
				else {
					strcpy(text, "life");
					}

				memcpy(item->name, text, MAX_NAME);
				snprintf(text, MAX_NAME, "Potion named %s", item->name);
				strncat(item->descripction, text, MAX_DESCRIPTION);

				free(text);
				break;
				}

		case STRENGTH_POTION: {
				char* text = calloc(MAX_NAME, sizeof(char));
				item->equipedTo = EQUIPTED_USE;
				item->health = rand()%10 + 10;  //PERCENTAGE RESTORE
				item->type = STRENGTH_POTION;
				if(CHANCE_ITEM_NOT_IDENT > rand_f64()) {
					for(i32 i = 0; i < 10; i++) {
						text[i] = rand()%(125 - 33) + 33;
						}
					}
				else {
					strcpy(text, "Strength");
					}

				memcpy(item->name, text, MAX_NAME);
				snprintf(text, MAX_NAME, "Potion named %s", item->name);
				strncat(item->descripction, text, MAX_DESCRIPTION);

				free(text);
				break;
				}
		case AGILITY_POTION: {
				char* text = calloc(MAX_NAME, sizeof(char));
				item->equipedTo = EQUIPTED_USE;
				item->health = rand()%LEVEL + 1;  //max
				item->type = STRENGTH_POTION;
				if(CHANCE_ITEM_NOT_IDENT > rand_f64()) {
					for(i32 i = 0; i < 10; i++) {
						text[i] = rand()%(125 - 33) + 33;
						}
					}
				else {
					strcpy(text, "Agility");
					}

				memcpy(item->name, text, MAX_NAME);
				snprintf(text, MAX_NAME, "Potion named %s", item->name);
				strncat(item->descripction, text, MAX_DESCRIPTION);

				free(text);
				break;
				}

		case DEFENCE_POTION: {
				char* text = calloc(MAX_NAME, sizeof(char));
				item->equipedTo = EQUIPTED_USE;
				item->health = rand()%LEVEL + 1;  //max
				item->type = DEFENCE_POTION;
				i32 what = rand()%DAMAGE_NUM;
				item->defence[what] = item->health;
				if(CHANCE_ITEM_NOT_IDENT > rand_f64()) {
					for(i32 i = 0; i < 10; i++) {
						text[i] = rand()%(125 - 33) + 33;
						}
					}
				else {
					strcpy(text, "Resistance");
					}

				memcpy(item->name, text, MAX_NAME);
				snprintf(text, MAX_NAME, "Potion named %s", item->name);
				strncat(item->descripction, text, MAX_DESCRIPTION);

				free(text);
				break;
				}

		case VITALITY_POTION: {
				char* text = calloc(MAX_NAME, sizeof(char));
				item->equipedTo = EQUIPTED_USE;
				item->health = rand()%3 + 1;  //max
				item->type = DEFENCE_POTION;
				i32 what = rand()%DAMAGE_NUM;
				item->defence[what] = item->health;
				if(CHANCE_ITEM_NOT_IDENT > rand_f64()) {
					for(i32 i = 0; i < 10; i++) {
						text[i] = rand()%(125 - 33) + 33;
						}
					}
				else {
					strcpy(text, "Vitality");
					}

				memcpy(item->name, text, MAX_NAME);
				snprintf(text, MAX_NAME, "Potion named %s", item->name);
				strncat(item->descripction, text, MAX_DESCRIPTION);

				free(text);
				break;
				}



		case APPLE_ITEM: {
				char* text = calloc(MAX_NAME, sizeof(char));
				item->equipedTo = EQUIPTED_USE;
				item->health = rand()%30 + 10;  //PERCENTAGE RESTORE
				item->type = APPLE_ITEM;
				memcpy(item->name, text, MAX_NAME);
				snprintf(text, MAX_NAME, "%s", item->name);
				strncat(item->descripction, text, MAX_DESCRIPTION);

				free(text);
				break;
				}

		case MEAT_ITEM: {
				char* text = calloc(MAX_NAME, sizeof(char));
				item->equipedTo = EQUIPTED_USE;
				item->health = rand()%30 + 10;  //PERCENTAGE RESTORE
				item->type = MEAT_ITEM;
				memcpy(item->name, text, MAX_NAME);
				snprintf(text, MAX_NAME, "%s", item->name);
				strncat(item->descripction, text, MAX_DESCRIPTION);

				free(text);
				break;
				}

		case BERRY_ITEM: {
				char* text = calloc(MAX_NAME, sizeof(char));
				item->equipedTo = EQUIPTED_USE;
				item->health = rand()%30 + 10;  //PERCENTAGE RESTORE
				item->type = APPLE_ITEM;
				memcpy(item->name, text, MAX_NAME);
				snprintf(text, MAX_NAME, "%s", item->name);
				strncat(item->descripction, text, MAX_DESCRIPTION);

				free(text);
				break;
				}

		case GOLD_ITEM: {
				char* text = calloc(MAX_NAME, sizeof(char));
				item->equipedTo = EQUIPTED_USE;
				//i//tem->health = rand()%90 + 10;  //PERCENTAGE RESTORE
				item->type = GOLD_ITEM;
				item->itemValue =  rand()%50 + 10;
				memcpy(item->name, text, MAX_NAME);
				snprintf(text, MAX_NAME, "Baga of gold %s %d", item->name, item->itemValue);
				strncat(item->descripction, text, MAX_DESCRIPTION);
				break;
				}

		default: {
				ASSERT("UNRECHABLE");
				break;
				}
		}
	if(rand_f64() < CHANCE_ITEM_CURSED) {
		item->isCursed = CURSED;
		}
	else {
		item->isCursed = NORMAL;
		}
	item->type = type;
	return item;
	}

void pick_item_from_ground(Item* item, Item_DA *inventory) {
	//POP FROM ARRAY
	//PUSH INTO A ARRAY
	//I SHOUD MAKE LIST BUT WHATEVER
	item->pos.x = -200;
	item->pos.y = -200; //JUST HIDE A ITEM
	da_append(inventory, (*item));
	//IF CURSED OR OTHER STUFS
	}

void equiped_item(Item_DA *items, u64 numItem) {
	if(items->items  == NULL || items->count == 0) {
		da_append(&MESSAGES, "Number is larger then number of items");
		return;
		}
	if(numItem > items->count-1) {
		da_append(&MESSAGES, "Number is larger then number of items");
		EQUITEM = SDL_FALSE;
		return;
		}

	Item itemToEquipt = items->items[numItem];
	Item_Equipted type = itemToEquipt.equipedTo;
	if(type == EQUIPTED_USE) {
		USEITEM = SDL_TRUE;
		//da_append(&MESSAGES, "Item is not equipped");
		return;
		}
	itemToEquipt.isEquiped = SDL_TRUE;
	for(u64 i = 0; i < items->count; i++) {
		if(items->items[i].equipedTo == type) {
			if(items->items[i].isEquiped == SDL_TRUE) {
				items->items[i].isEquiped = SDL_FALSE;
				break;
				}
			}
		}
	char* msg = calloc(MAX_NAME, sizeof(char));
	snprintf(msg, MAX_NAME, "You equipped(%d) %s", (i32)numItem, itemToEquipt.name);
	da_append(&MESSAGES, msg);
	items->items[numItem] = itemToEquipt;
	}
