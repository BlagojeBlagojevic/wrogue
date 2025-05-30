#ifndef _ITEM_H
#define _ITEM_H

#include "utils.h"

//TBD CURSED ITEMS
typedef enum{
	SWORD_ITEM,
	AXE_ITEM,
	DAGER_ITEM,
	PIKE_ITEM,
	SABER_ITEM,
	PLAYER_SWORD_ITEM,
	PLAYER_ARMOR_ITEM,
	DART_ITEM,
	ARMOR_ITEM,
	HELMET_ITEM,
	SHIELD_ITEM,
	SHOES_ITEM,
	
	//RANGE ITEMS
	BOW_ITEM,
	ARROW_ITEM,
	
	//SCROLS
	SCROL_TELEPORT_ITEM,  //FIRST
	SCROL_AGREGATE_ITEM,
	SCROL_ENCHANTING_ITEM,
	SCROL_RECHARGING_ITEM,
	SCROL_REPEL_ITEM,
	SCROL_SUMMON_ITEM,
	SCROL_SCARE_ITEM,
	SCROL_ACQ_ITEM,
	SCROL_IDENT_ITEM,	   //LAST
	//POTIONS
	HEALING_ITEM,
	STRENGTH_POTION,
	AGILITY_POTION,
	DEFENCE_POTION,
	VITALITY_POTION,
	//FOOD 
	APPLE_ITEM,
	MEAT_ITEM,
	BERRY_ITEM,
	
	GOLD_ITEM,
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
	EQUIPTED_RANGE,
	EQUIPTED_SHIELD,
	EQUIPTED_LEGS,
	EQUIPTED_HEAD,
	EQUIPTED_USE,
	EQUIPTED_NUM,
	
}Item_Equipted;

//THIS HAS TO BE CHANGED IF CHANGE AMOUNT OF TYPES OF THE ATTACKS
#define DAMAGE_NUMi 4 

//MELE
#define SWORD_CREATE()            5, "SWORD",  '{', WHITE,  SWORD_ITEM,           NORMAL, SDL_FALSE
#define AXE_CREATE()              5, "AXE",    '{', WHITE,  AXE_ITEM,             NORMAL, SDL_FALSE
#define DAGER_CREATE()            5, "DAGER",  '{', WHITE,  DAGER_ITEM,           NORMAL, SDL_FALSE
#define PIKE_CREATE()             5, "PIKE",   '{', WHITE,  PIKE_ITEM,            NORMAL, SDL_FALSE
#define SABER_CREATE()            5, "SABER",  '{', WHITE,  SABER_ITEM,           NORMAL, SDL_FALSE


#define PLAYER_SWORD_CREATE()     5, "SWORD",  '{', WHITE,  PLAYER_SWORD_ITEM,    NORMAL, SDL_FALSE
#define PLAYER_ARMOR_CREATE()     5, "ARMOR",  '[', WHITE,  PLAYER_ARMOR_ITEM,    NORMAL, SDL_FALSE
//DEF
#define ARMOR_CREATE()            5, "ARMOR",  '[', WHITE,  ARMOR_ITEM,           NORMAL, SDL_FALSE 
#define HELMET_CREATE()           5, "HELMET", '[', WHITE,  HELMET_ITEM,          NORMAL, SDL_FALSE
#define SHIELD_CREATE()           5, "SHIELD", '[', WHITE,  SHIELD_ITEM,          NORMAL, SDL_FALSE
#define SHOES_CREATE()            5, "SHOES",  '[', WHITE,  SHOES_ITEM,           NORMAL, SDL_FALSE
//RANGE
#define BOW_CREATE()              5, "BOW",    '|', WHITE,  BOW_ITEM,             NORMAL, SDL_FALSE
#define ARROW_CREATE()            5, "ARROW",  '|', WHITE,  ARROW_ITEM,           NORMAL, SDL_FALSE

#define SCROL_TELEPORT_CREATE()   5, "SCROL",  '|', WHITE,  SCROL_TELEPORT_ITEM,  NORMAL, SDL_FALSE
#define SCROL_AGREGATE_CREATE()   5, "SCROL",  '|', WHITE,  SCROL_AGREGATE_ITEM,  NORMAL, SDL_FALSE
#define SCROL_ENCHANTING_CREATE() 5, "SCROL",  '|', WHITE,  SCROL_ENCHANTING_ITEM,NORMAL, SDL_FALSE
#define SCROL_RECHARGING_CREATE() 5, "SCROL",  '|', WHITE,  SCROL_RECHARGING_ITEM,NORMAL, SDL_FALSE
#define SCROL_REPEL_CREATE()      5, "SCROL",  '|', WHITE,  SCROL_REPEL_ITEM,     NORMAL, SDL_FALSE
#define SCROL_SUMMON_CREATE()     5, "SCROL",  '|', WHITE,  SCROL_SUMMON_ITEM,    NORMAL, SDL_FALSE
#define SCROL_SCARE_CREATE()      5, "SCROL",  '|', WHITE,  SCROL_SCARE_ITEM,     NORMAL, SDL_FALSE
#define SCROL_ACQ_CREATE()        5, "SCROL",  '|', WHITE,  SCROL_ACQ_ITEM,       NORMAL, SDL_FALSE
#define SCROL_IDENT_CREATE()      5, "SCROL",  '|', WHITE,  SCROL_IDENT_ITEM,     NORMAL, SDL_FALSE

	



#define HEALING_CREATE()	        5, "",       '!', WHITE,  HEALING_ITEM,         NORMAL, SDL_FALSE
#define STRENGHT_CREATE()	        5, "",       '!', WHITE,  STRENGTH_POTION,      NORMAL, SDL_FALSE
#define AGILITY_CREATE()	        5, "",       '!', WHITE,  AGILITY_POTION,       NORMAL, SDL_FALSE
#define DEFENCE_CREATE()	        5, "",       '!', WHITE,  DEFENCE_POTION,       NORMAL, SDL_FALSE
#define VITALITY_CREATE()					5, "",       '!', WHITE,  VITALITY_POTION,      NORMAL, SDL_FALSE


#define APPLE_CREATE()	          5, "Apple",  '%',WHITE,  APPLE_ITEM,           NORMAL, SDL_FALSE
#define MEAT_CREATE()	            5, "Meat",   '%',WHITE,  MEAT_ITEM,            NORMAL, SDL_FALSE
#define BERRY_CREATE()	          5, "Bery",   '%',WHITE,  BERRY_ITEM,           NORMAL, SDL_FALSE

#define GOLD_CREATE()	            5, "",       'g', YELLOW, GOLD_ITEM,           NORMAL, SDL_FALSE
//#define DART_CREATE()   5, "DART",   '|', BLUE
//#define POTION_CREATE() 5, "POTION", '#', BLUE


typedef struct{
	
	i32 health;  //KINDA HOW MTCH IT COUDE SURVIVE UNTIL BREAKING AND FOR POTION BE USED FOR SOMETHING
	Position pos;
	char* name;
	char* descripction;
	char ch;
	SDL_Color color;
	Item_Type type;
	i32 attack[DAMAGE_NUMi];
	i32 defence[DAMAGE_NUMi];
	u8 isCursed;
	SDL_bool isEquiped;
	Item_Equipted equipedTo;
	i32 lifeSteal;
	f64 lifeStealChance;
	i32 critDamage;
	f64 critDamageChance;
	f64 poisonChance;
	i32 poisonDamage;
	i32 itemValue;
	i32 eatValue;
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
