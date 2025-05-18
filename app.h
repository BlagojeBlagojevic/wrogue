#ifndef _APP_H
#define _APP_H

#include "utils.h"
#include "map.h"
#include "entity.h"
#include "item.h"

//#define PLAYER_VISION

extern SDL_Texture* monstersTextures;
extern SDL_Texture* groundTextures;
extern SDL_Texture* wallTextures;
extern SDL_Texture* poisonTextures;
extern SDL_Texture* blightTextures;
extern SDL_Texture* playerTextures;
extern SDL_Texture* cloudTextures;
extern SDL_Texture*      itemTextures;
void init_texture();
 

void SDL_ERR(int code);
void *P_SDL_ERR(void *ptr);
void Text_Renderer_C(SDL_Renderer *renderer, TTF_Font *font, i32 startX, i32 startY, i32 w_c, i32 h_c, const char *c, SDL_Color textColor);
void render_player(Entitiy *player);
void player_input(SDL_Event *event, Entitiy* player, Entitiy_DA *entitis, Item_DA *items, Tile* map);
void render_stats(Entitiy *player);
void render_monsters(Entitiy_DA *monsters, Entitiy *player, Tile *map);
void render_messages(i32 startX, i32 startY, char* message);
void render_map(Tile *map, Entitiy *player);
void render_map_fov(Entitiy *player, Tile *map);
void main_renderer(Entitiy* player, Entitiy_DA *monster, Item_DA *items, Tile *map);
void event_user(Entitiy *player, Entitiy_DA *entitis, Item_DA *items, Tile* map);
void render_inventory(Item_DA *inventory);


#endif
