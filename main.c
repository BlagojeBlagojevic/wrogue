#include "map.h"
#include "entity.h"
#include "app.h"
#include <time.h>
#include <emscripten.h>

#define SEED 12344

Graphics_State mainGraphics;
SDL_Texture* monstersTextures;
SDL_Texture* groundTextures;
SDL_Texture* wallTextures;
SDL_Texture* poisonTextures;
SDL_Texture* blightTextures;
SDL_Texture* playerTextures;
SDL_Texture* cloudTextures;
SDL_Texture* itemTextures;
SDL_Texture* swordTextures;
SDL_Texture* grassTextures;
SDL_Texture* boulderTextures;
SDL_Texture* treeTextures;
SDL_Texture* stairTextures;
SDL_Texture* rangeItemsTextures;

static Tile* map;
static Entitiy_DA monster;
static Entitiy* player;
static Item_DA items;

void generate_level() {
    Room_DA rooms = {0};
    LEVEL++;
    i32 whatMap = rand() % 4;
    map = (whatMap == 0) ? init_map_BSP(&rooms, 4) : init_map_RA(&rooms);

    monster.count = 0;
    items.count = 0;

    if (rooms.count <= 2) {
        Room room = {.pos = {0, 0}, .width = 20, .height = 20};
        for (i32 y = 0; y < MAP_Y - room.height; y += room.height) {
            room.pos.y = y;
            for (i32 x = 0; x < MAP_X - room.width; x += room.width) {
                room.pos.x = x;
            }
        }
    } else {
        for (u64 i = 0; i < rooms.count; i++) {
            genereate_monsters_generator(player, &monster, map, LEVEL, rooms.items[i], SDL_TRUE);
        }
    }

    calculate_diakstra_map(player, map, &monster, rooms.items[0].pos.x, rooms.items[0].pos.y);
    caved_part_generator(TILE_TREE, map, 2);
    calculate_diakstra_map(player, map, &monster, rooms.items[0].pos.x, rooms.items[0].pos.y);
    caved_part_generator(TILE_GRASS, map, 100);

    MOVMENT = 0;
    COUNTMOVES = 0;

    player->pos = rooms.items[0].center;

    if (rooms.items) free(rooms.items);

    Item* it = create_item(player->pos.x, player->pos.y, BOW_CREATE());
    da_append(&items, *it);
    it = create_item(player->pos.x + 1, player->pos.y, ARROW_CREATE());
    da_append(&items, *it);
    it = create_item(player->pos.x + 2, player->pos.y, ARROW_CREATE());
    da_append(&items, *it);
}

void game_loop() {
    if (QUIT) {
        emscripten_cancel_main_loop();
        return;
    }

    if (MAP_CH(map, player->pos.x, player->pos.y) == TILE_STAIRS) {
        for (int i = 0; i < 4; i++) {
            player->attack[i]++;
            player->defence[i]++;
        }
        player->maxHealth += 10;
        generate_level();
        DEPTH--;
        if (DEPTH < 0) {
            LOG("You win!\n");
            QUIT = 1;
            return;
        }
    }

    if (!EQUITEM) {
        main_renderer(player, &monster, &items, map);
        update_entity(player, &monster, map, &items);
    }

    main_renderer(player, &monster, &items, map);
    //SDL_Delay(100);
    event_user(player, &monster, &items, map);
    lingering_map_tile(map, player, &monster);

    if (monster.count < 12 || (COUNTMOVES + 1) % 200 == 0) {
        Room room = {.pos = {0, 0}, .width = MAP_Y - 1, .height = MAP_X - 1};
        genereate_monsters_generator(player, &monster, map, LEVEL + 1, room, SDL_FALSE);
    }

    if (player->health <= 0) {
        da_append(&MESSAGES, "You lose");
        // Restart game logic (instead of exiting)
        player->health = 10;
        player->maxHealth = 10;
        LEVEL = 0;
        DEPTH = rand() % 7 + 1;
        generate_level();
        return;
    }

    if (EQUITEM) {
        main_renderer(player, &monster, &items, map);
        for (u64 i = 0; i < player->inventory.count; i++) {
            if (player->inventory.items[i].isEquiped) {
                da_append(&MESSAGES, player->inventory.items[i].descripction);
            }
        }
    }
}

int main() {
    SDL_ERR(SDL_Init(SDL_INIT_VIDEO));
    SDL_ERR(TTF_Init());

    u64 seed = (u64)time(0);
    srand(seed);

    for (i32 i = 0; i < NUM_RENDER_MSG; i++) {
        da_append(&MESSAGES, "   ");
    }

    char* msg = calloc(30, sizeof(char));
    snprintf(msg, 30, "Your seed is %d", (i16)seed);
    da_append(&MESSAGES, msg);

    WINDOW = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1, 800, SDL_WINDOW_OPENGL);
    RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetWindowResizable(WINDOW, SDL_TRUE);
    FONT = TTF_OpenFont(fontLoc, 128);

    init_texture();

    QUIT = 0;
    MOVMENT = SDL_TRUE;
    ITEMSREND = SDL_TRUE;

    player = create_entity('@', "Some Name", 10, 20, (Position){40, 40}, WHITE);
    for (int i = 0; i < 4; i++) {
        player->attack[i] = 2;
        player->defence[i] = 2;
    }

    player->stamina = 15;
    player->maxStamina = 15;
    player->chanceToDecressStaminaMove = 0.1f;

    da_append(&player->inventory, *create_item(0, 0, DAGER_CREATE()));
    da_append(&player->inventory, *create_item(0, 0, PLAYER_ARMOR_CREATE()));

    da_append(&items, *create_item(37, 38, HEALING_CREATE()));

    monster_definitions_export();
    export_generators();

    LEVEL = 0;
    DEPTH = 7;
    generate_level();

    // START GAME LOOP
    emscripten_set_main_loop(game_loop, 0, 1);

    return 0;
}
