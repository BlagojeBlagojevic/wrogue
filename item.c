#include "item.h"

Item* create_item(i32 x, i32 y, i32 health,  const char* name, char ch, SDL_Color color) {
	Item* item = calloc(1, sizeof(Item));
	if(item == NULL) {
		ASSERT("CALLOC FAIL");
		}
	item->ch = ch;
	item->health = health;
	item->name = calloc(MAX_NAME, sizeof(char));
	if(item->name == NULL) {
		ASSERT("CALLOC FAIL");
		}
	else {
		i32 len =  strlen(name);
		CLAMP(len, 0, (MAX_NAME - 1));
		memcpy(item->name, name, len);
		}
	item->pos.x = x;
	item->pos.y = y;
	item->color = color;
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
