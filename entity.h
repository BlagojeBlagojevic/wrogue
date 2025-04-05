#ifndef _ENTITY_H
#define _ENTITY_H

#include "utils.h"
#include "map.h"

typedef enum {
	DAMAGE_BASIC,
	DAMAGE_FIRE,
	DAMAGE_POISON,
	DAMAGE_NUM,

	} Damage_Types;


static const char* damageStr[] = {
	"BASIC",
	"FIRE",
	"POISON",
	"NUM",
	};


typedef enum{
	
	BASIC_MONSTER,
	ZOMBIE_MONSTER,
	WIZARD_MONSTER ,
	NUM_MONSTER
	
}Monster_Types;

static const char* monsterName = "MZW "; 



typedef struct {
	Position pos;
	SDL_Color color;
	i32 radius;
	i32 health;
	char ch;
	i32 attack[DAMAGE_NUM];
	i32 defence[DAMAGE_NUM];
	} Entitiy;

typedef struct {
	Entitiy *items;
	u64 capacity;
	u64 count;
	} Entitiy_DA;

Entitiy* create_entity(char ch, i32 radius, i32 health, Position startPos);
i32 roll_the_dice(i32 attack, i32 defence);
void message_attacked_by_monster(Entitiy* player, Entitiy* entity, i32 damage);
void message_attacked_by_player(Entitiy* player, Entitiy* entity, i32 damage);
void player_attack(Entitiy *player, Entitiy* entity);
void monster_attack(Entitiy *player, Entitiy* entity);
SDL_bool Is_Monster(char c);
i32 is_monster_on_entity(i32 x, i32 y, Entitiy_DA* entities);
void genereate_monsters(Entitiy_DA *monsters, Tile *map);
void block_movement(Entitiy_DA *entitys, Tile *map);
SDL_bool check_colison_entitiy(Entitiy* player, Entitiy*  ent);
void make_best_move(Entitiy* player, Entitiy*  ent, Tile *map);
void move_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map);
void update_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map);

#endif