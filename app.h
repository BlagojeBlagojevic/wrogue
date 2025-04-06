#ifndef _APP_H
#define _APP_H

#include "utils.h"
#include "map.h"
#include "entity.h"

#define PLAYER_VISION

void SDL_ERR(int code);
void *P_SDL_ERR(void *ptr);
void Text_Renderer_C(SDL_Renderer *renderer, TTF_Font *font, i32 startX, i32 startY, i32 w_c, i32 h_c, char *c, SDL_Color textColor);
void render_player(Entitiy *player);
void player_input(SDL_Event *event, Entitiy* player, Entitiy_DA *entitis, Tile* map);
void render_stats(Entitiy *player);
void render_monsters(Entitiy_DA *monsters, Entitiy *player);
void render_messages(i32 startX, i32 startY, char* message);
void render_map(Tile *map, Entitiy *player);
void main_renderer(Entitiy* player, Entitiy_DA *monster, Tile *map);
void event_user(Entitiy *player, Entitiy_DA *entitis, Tile* map);

#endif
