#ifndef _ITEM_H
#define _ITEM_H

#include "utils.h"


typedef enum{
	SWORD_ITEM,
	DART_ITEM,
	POTION_ITEM,
}Item_Type;

#define SWORD_CREATE()  5, "SWORD",  '{', BLUE
#define DART_CREATE()   5, "DART",   '|', BLUE
#define POTION_CREATE() 5, "POTION", '#', BLUE


typedef struct{
	i32 health;
	Position pos;
	char* name;
	char ch;
	SDL_Color color;
	
}Item;

typedef struct {
	Item* items;
	u64   count;
	u64   capacity;
}Item_DA;

Item* create_item(i32 x, i32 y, i32 health,  const char* name, char ch, SDL_Color color);
void pick_item_from_ground(Item* item, Item_DA *inventory);

#endif