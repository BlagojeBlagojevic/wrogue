#ifndef _ENTITY_H
#define _ENTITY_H

#include "utils.h"
#include "map.h"
#include "item.h"

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


typedef enum {

	BASIC_MONSTER,
	ZOMBIE_MONSTER,
	WIZARD_MONSTER,
	BEAR_MONSTER,
	CROW_MONSTER,
	DEMON_MONSTER,
	GHOST_MONSTER,
	NUM_MONSTER
	//UPDATE ARR
	} Monster_Types;

//DEFINES FOR INITING OF A MONSTER




static const char* monsterChar = "MZWBCDG ";

static const char* monsterName[] = {
	"Monster",
	"Zombie",
	"Wizard",
	"Bear",
	"Crow",
	"Demon",
	"Ghost",
	};



typedef struct Entity Entitiy;

typedef struct Entity {
	Position pos;
	SDL_Color color;
	i32 radius;
	i32 health;
	i32 maxHealth;
	char* name;
	char ch;
	i32 attack[DAMAGE_NUM];
	i32 defence[DAMAGE_NUM];
	f64 runWoundedPercent;
	SDL_bool isRunning;
	Item_DA inventory;
	} Entitiy;

typedef struct {
	Entitiy *items;
	u64 capacity;
	u64 count;
	} Entitiy_DA;

//MONSTER TYPES  as global for now
//later it will be load from file for now  monster_definitions_export()
static Entitiy monsters[NUM_MONSTER];

Entitiy* create_entity(char ch, const char* name, i32 radius, i32 health, Position startPos);
i32 roll_the_dice(i32 attack, i32 defence);
void message_attacked_by_monster(Entitiy* player, Entitiy* entity, i32 damage, Damage_Types type);
void message_attacked_by_player(Entitiy* player, Entitiy* entity, i32 damage);
void player_attack(Entitiy *player, Entitiy* entity, Item_DA *items, Tile* map);
void monster_attack(Entitiy *player, Entitiy* entity, f64 distance);
void monster_definitions_export();
void item_definitions_export();
SDL_bool Is_Monster(char c);
i32 is_monster_on_entity(i32 x, i32 y, Entitiy_DA* entities);
void genereate_monsters(Entitiy_DA *monsters, Tile *map);
void block_movement(Entitiy_DA *entitys, Tile *map);
SDL_bool check_colison_entitiy(Entitiy* player, Entitiy* ent, Tile* map);
void cast_ray(Entitiy *entity, Tile* map, f64 x, f64 y);
void field_of_vison(Entitiy *entity, Tile* map);

void make_best_move(Entitiy* player, Entitiy*  ent, Tile *map);
void make_run_move(Entitiy* player, Entitiy*  ent, Tile *map);
void move_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map);
void increment_player_health(Entitiy* player);
void player_open_door(Entitiy *player, Tile* map);
void update_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map, Item_DA *items);

//ITEMS
void picking_item_from_list(Entitiy* entity, Item_DA *items);
SDL_bool check_if_item_and_player_colide(Entitiy* player, Item* item);
void picking_item_from_list(Entitiy* entity, Item_DA *items);

#endif
