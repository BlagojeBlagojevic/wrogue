#ifndef _ITEM_H
#define _ITEM_H

#include "utils.h"


typedef enum{
	SWORD_ITEM,
	DART_ITEM,
	ARMOR_ITEM,
	HELMET_ITEM,
	SHIELD_ITEM,
	SHOES_ITEM,
	POTION_ITEM,
	NUM_ITEM,
}Item_Type;

typedef enum{
	CURSED,
	NORMAL,
	BLESED,
	
}Item_Status;

typedef enum {
	DAMAGE_BASIC,
	DAMAGE_SPELL,
	DAMAGE_POISON,
	DAMAGE_RANGE,
	DAMAGE_NUM,

	} Damage_Types;


static const char* damageStr[] = {
	"BASIC",
	"SPELL",
	"POISON",
	"RANGE",
	"NUM",
	};

typedef enum{
	
	EQUIPTED_ARMOR,
	EQUIPTED_WEPON,
	EQUIPTED_SHIELD,
	EQUIPTED_LEGS,
	EQUIPTED_HEAD,
	EQUIPTED_NUM,
	
}Item_Equipted;

//THIS HAS TO BE CHANGED IF CHANGE AMOUNT OF TYPES OF THE ATTACKS
#define DAMAGE_NUMi 4 


#define SWORD_CREATE()     5, "SWORD",  '{', WHITE, SWORD_ITEM,  NORMAL, SDL_FALSE
#define ARMOR_CREATE()     5, "ARMOR",  '[', WHITE, ARMOR_ITEM,  NORMAL, SDL_FALSE 
#define HELMET_CREATE()    5, "HELMET", '[', WHITE, HELMET_ITEM, NORMAL, SDL_FALSE
#define SHIELD_CREATE()    5, "SHIELD", '[', WHITE, SHIELD_ITEM, NORMAL, SDL_FALSE
#define SHOES_CREATE()     5, "SHOES",  '[', WHITE, SHOES_ITEM,  NORMAL, SDL_FALSE
//#define DART_CREATE()   5, "DART",   '|', BLUE
//#define POTION_CREATE() 5, "POTION", '#', BLUE


typedef struct{
	
	i32 health;  //KINDA HOW MTCH IT COUDE SURVIVE UNTIL BREAKING AND FOR POTION BE USED FOR SOMETHING
	Position pos;
	char* name;
	char* descripction;
	char ch;
	SDL_Color color;
	i32 attack[DAMAGE_NUMi];
	i32 defence[DAMAGE_NUMi];
	u8 isCursed;
	SDL_bool isEquiped;
	Item_Equipted equipedTo;
	i32 lifeSteal;
	f64 lifeStealChance;
	//LET ALL BE ABLE to be TROW
	
}Item;

typedef struct {
	Item* items;
	u64   count;
	u64   capacity;
}Item_DA;

Item* create_item(i32 x, i32 y, i32 health,  const char* name, char ch, SDL_Color color,	Item_Type type, u8 isCursed, SDL_bool isEquipped);
void pick_item_from_ground(Item* item, Item_DA *inventory);
void equiped_item(Item_DA *items, u64 numItem);
//ALL LOGIC WILL BE HANDLED IN A player_attack and monster_attack for adjustin the stats

#endif
