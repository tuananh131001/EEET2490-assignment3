#include "game.h"

#include "object.h"

void init_game(Game *world) {
    init_map(&world->world);
    fb_init();
}

void init_map(World *world) { 
    init_player(&world->player); 
    init_enemies(world);
    }
void init_player(Entity *player) {
    player->dimension.height = blue_ship_sprite.height;
    player->dimension.width = blue_ship_sprite.width;
    player->position.x = (MAP_WIDTH / 2) - (player->dimension.width / 2);
    player->position.y = MAP_HEIGHT - 162;
    player->type = PLAYER;
}

void init_enemies(World *world) {
    for (int i = 0, j = 0; i < NUM_ENEMIES; i++) {
        if (i < 10) {
            world->enemies[i].position.x =
                alien_initial_x + (HORIZONTAL_OFFSET * i);
            world->enemies[i].position.y =
                alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 5);
        } else if (i < 20) {
            world->enemies[i].position.x =
                alien_initial_x + (HORIZONTAL_OFFSET * (i % 10));
            world->enemies[i].position.y =
                alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 4);

        } else {
            world->enemies[i].position.x =
                alien_initial_x + (HORIZONTAL_OFFSET * (i % 20));
            world->enemies[i].position.y =
                alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 3);
        }
        world->enemies[i].dimension.height = pawn_sprite.height;
        world->enemies[i].dimension.width = pawn_sprite.width;
        // world->enemies[i].health.current_health = PAWN_HEALTH;
        world->enemies[i].type = PAWN;
    }

    // for (int i = 0; i < 6; i++) {
    //     world->left_most_enemies[i] = 10 * i;
    //     world->right_most_enemies[i] = 10 * i + 9;
    // }
}

void render(World *world) {
    drawEntity(world->player);
    for (int i = 0; i < NUM_ENEMIES; i++) {
        drawEntity(world->enemies[i]);
    }
}