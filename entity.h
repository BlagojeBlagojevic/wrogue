#ifndef _ENTITY_H
#define _ENTITY_H

#include "utils.h"
#include "map.h"
#include "item.h"




typedef enum {

	BASIC_MONSTER,
	//UNDEAD
	ACOLAYT_MONSTER,
	ZOMBIE_MONSTER,
	GHOUL_MONSTER,
	ABOMINATION_MONSTER,
	WAGON_MONSTER,
	NECROMANCER_MONSTER,
	BANSHIE_MONSTER,
	SPIDER_MONSTER,
	GARGOYLE_MONSTER,
	DESTROYER_MONSTER,
	DRAGON_MONSTER,

	//ORCS
	GRUNT_MONSTER,
	BERSERKER_MONSTER,
	ARCHER_MONSTER,
	WITCH_MONSTER,
	//NEUTRAL
	RAT_MONSTER,
	GOBLIN_MOSNTER,

	NUM_MONSTER
	//UPDATE ARR
	} Monster_Types;

typedef struct {
	Monster_Types* items;
	u64 count;
	u64 capacity;
	} Monster_Types_DA;

//DEFINES FOR INITING OF A MONSTER




static const char* monsterChar = "MaZGIwNBFYEDOVAWRK   ";

static const char* monsterName[] = {
	"Monster",
	"Acolayt",
	"Zombie",
	"Ghoul",
	"Abomination",
	"Wagon",
	"Necromancer",
	"Banshie",
	"Spider",
	"Gargoyle",
	"Destroyer",
	"Dragon",
	"Grunt",
	"Berserker",
	"Ork Archer",
	"Witch Doc",
	"Rat",
	"Goblin",
	"Crow",
	"Demon",
	"Ghost",
	};

typedef enum {
	STATE_RUNING,
	STATE_MOVING_AWAY_RANGE,
	STATE_HUNTING,
	STATE_WANDERING,
	STATE_RESTING,
	STATE_BESERK,
	STATE_RESURECT,
	STATE_SUMMON,
	STATE_SPELL,
	STATE_NUM
	} State_Entity;

typedef struct {
	Item* items;
	u64   count;
	u64   capacity;
	} State_Entity_DA;

typedef enum {
	SPELL_SUMONM_GHOUL,
	SPELL_DECRESE_MAX_HEALTH,
	SPELL_STUN,
	SPELL_DOUBLE_MOVE,
	SPELL_KABUM,
	SPELL_DAMAGE,
	SPELL_SUMMON,
	SPELL_STATIS_TRAP,
	SPELL_NUM_ACTIVES,
	//PASIVES
	SPELL_PASSIVE_POISON,
	SPELL_PASSIVE_STATUE,
	
	SPELL_NUM_PASSIVE,
	
	SPELL_NUM
	} Spell_Type;

typedef struct spell {
	Spell_Type type;
	i32 value;
	i32 passedTurns;
	i32 cooldown;

	} Spell;

#define SPELL_SUMONM_GHOUL_EXPORT(entitiy){entitiy.spell.type = SPELL_SUMONM_GHOUL;entitiy.spell.value = GHOUL_MONSTER; entitiy.spell.passedTurns = 0; entitiy.spell.cooldown = 30;};
#define SPELL_DECRESE_MAX_HEALTH_EXPORT(entitiy){entitiy.spell.type = SPELL_DECRESE_MAX_HEALTH;entitiy.spell.value  = -1; entitiy.spell.passedTurns = 30; entitiy.spell.cooldown = 30;};
#define SPELL_SPIDER_EXPORT(entitiy){entitiy.spell.type = SPELL_STUN;entitiy.spell.value  = 2; entitiy.spell.passedTurns = 10; entitiy.spell.cooldown = 10;}
#define SPELL_DRAGON_EXPORT(entitiy){entitiy.spell.type = SPELL_STUN;entitiy.spell.value  = 4; entitiy.spell.passedTurns = 10; entitiy.spell.cooldown = 10;}

#define SPELL_BESERKER_EXPORT(entitiy){entitiy.spell.type = SPELL_DOUBLE_MOVE; entitiy.spell.value  = 2; entitiy.spell.passedTurns = 5; entitiy.spell.cooldown = 5;}
#define SPELL_SUMONM_RAT_EXPORT(entitiy){entitiy.spell.type = SPELL_SUMONM_GHOUL;entitiy.spell.value = RAT_MONSTER; entitiy.spell.passedTurns = 0; entitiy.spell.cooldown = 30;};
#define SPELL_GOBLIN_EXPORT(entitiy){entitiy.spell.type = SPELL_KABUM;entitiy.spell.value = 10; entitiy.spell.passedTurns = 0; entitiy.spell.cooldown = 4;};
#define SPELL_WITCH_EXPORT(entitiy){entitiy.spell.type = SPELL_STATIS_TRAP;entitiy.spell.value = 10; entitiy.spell.passedTurns = 0; entitiy.spell.cooldown = 4;};


#define SPELL_ABOMINATION_EXPORT(entitiy){entitiy.spell.type = SPELL_PASSIVE_POISON; entitiy.spell.value = 1;}
#define SPELL_GARGOYLE_EXPORT(entitiy){entitiy.spell.type = SPELL_PASSIVE_STATUE; entitiy.spell.cooldown = 20; entitiy.spell.passedTurns = 0;}
#define SPELL_DESTROYER_EXPORT(entitiy){entitiy.spell.type = SPELL_DAMAGE; entitiy.spell.value = 30; entitiy.spell.passedTurns = 0; entitiy.spell.cooldown = 20;};
#define SPELL_WAGON_EXPORT(entitiy){entitiy.spell.type = SPELL_SUMMON; entitiy.spell.value = ZOMBIE_MONSTER; entitiy.spell.passedTurns = 0; entitiy.spell.cooldown = 30;};
typedef struct Entity Entitiy;

typedef struct Entity {
	Position pos;
	Position oldPos;
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
	u8 state;
	u8 isAlive;
	f64 stateChance[STATE_NUM];
	f64 lifeStealChance;
	u8 lifeStealValue;
	i16 isStunded;
	Spell spell;
	i32 equiptedItems[EQUIPTED_NUM];
	i16 stamina;
	i16 maxStamina;
	f64 chanceToDecressStaminaMove;
	} Entitiy;

typedef struct {
	Entitiy *items;
	u64 capacity;
	u64 count;
	} Entitiy_DA;

typedef enum {
	GENERATOR_GRAVEYARD,
	GENERATOR_CAVE,
	GENERATOR_INFECTION,
	GENERATOR_ABOMINATION,
	GENERATOR_ORC,
	GENERATOR_FIEND,
	GENERATOR_NECRO,
	GENERATOR_DRAGON,
	GENERATOR_NUM
	} Generator_Type;

typedef struct __attribute__((packed)) generator {
	Generator_Type type;
	f64 chanceToSpawn[NUM_MONSTER];
	i32 maxDistanceDikstra;
	i32 levelDungon;
	i32 monsterNumber;
	Tile_Type typeOfTile;
	} Generator;

static Generator generators[GENERATOR_NUM];

//MONSTER TYPES  as global for now
//later it will be load from file for now  monster_definitions_export()
static Entitiy monsters[NUM_MONSTER];

Entitiy* create_entity(char ch, const char* name, i32 radius, i32 health, Position startPos, SDL_Color color);
void free_entity(Entitiy* ent);
i32 roll_the_dice(i32 attack, i32 defence);
void message_attacked_by_monster(Entitiy* player, Entitiy* entity, i32 damage, Damage_Types type);
void message_attacked_by_player(Entitiy* player, Entitiy* entity, i32 damage);
SDL_bool player_attack(Entitiy *player, Entitiy* entity, Item_DA *items, Tile* map);
void monster_attack(Entitiy *player, Entitiy* entity, f64 distance);
void monster_definitions_export();
void item_definitions_export();
SDL_bool Is_Monster(char c);
i32 is_monster_on_entity(i32 x, i32 y, Entitiy_DA* entities);
void genereate_monsters(Entitiy_DA *monsters, Tile *map);
void block_movement(Entitiy_DA *entitys, Tile *map);
SDL_bool is_monster_visible(Tile* map, Entitiy* ent);
SDL_bool check_colison_entitiy(Entitiy* player, Entitiy* ent, Tile* map);
void cast_ray(Entitiy *entity, Tile* map, f64 x, f64 y);
void field_of_vison(Entitiy *entity, Tile* map);

void make_best_move(Entitiy* player, Entitiy*  ent, Tile *map);
void make_run_move(Entitiy* player, Entitiy*  ent, Tile *map);
void move_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map);
void increment_player_health(Entitiy* player);
void player_destroy_boolder(Entitiy* player, Tile* map);
void update_entity(Entitiy* player, Entitiy_DA *entitys, Tile *map, Item_DA *items);

//ITEMS
void picking_item_from_list(Entitiy* entity, Item_DA *items);
SDL_bool check_if_item_and_player_colide(Entitiy* player, Item* item);
void picking_item_from_list(Entitiy* entity, Item_DA *items);

void calculate_diakstra_map(Entitiy* player, Tile* map, Entitiy_DA* entitys, i32 goalX, i32 goalY);

void use_item(Entitiy* player, Entitiy_DA *entitis, Item_DA *items, u64 numItem);

void export_generators(); //TBF FILE
void genereate_monsters_generator(Entitiy* player, Entitiy_DA *monsters, Tile *map, i32 level, Room room);

void player_trap_calculations(Tile* map, Entitiy *player, Entitiy_DA *monsters);
void player_negative_effect(Entitiy* player, Tile *map);
void lingering_map_tile(Tile* map, Entitiy *player, Entitiy_DA *monsters);

#endif
