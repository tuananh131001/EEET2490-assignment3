#include "game.h"
#include "object.h"

void init_game(Game *world) {
    init_map(&world->world);
    fb_init();
}

void init_map(World *world) { init_player(&world->player); }
void init_player(Entity *player) {
    player->dimension.height = blue_ship_sprite.height;
    player->dimension.width = blue_ship_sprite.width;
    player->position.x = (MAP_WIDTH / 2) - (player->dimension.width / 2);
    player->position.y = MAP_HEIGHT - 162;
}
void render(World *world) { drawEntity(world->player); }