#include "game.h"

#include "helper.h"
#include "object.h"

void init_game(Game *world) {
    init_map(&world->world);
    fb_init();
}

// Create the stage
void init_map(World *world) {
    init_player(&world->player);
    init_enemies(world);
}
// Setting the value for player
void init_player(Entity *player) {
    player->dimension.height = blue_ship_sprite.height;
    player->dimension.width = blue_ship_sprite.width;
    player->position.x = (MAP_WIDTH / 2) - (player->dimension.width / 2);
    player->position.y = MAP_HEIGHT - 162;
    player->type = PLAYER;
}

// Move player

void move_player(World *world) {
    uart_puts("Press a to move left: \n");
    uart_puts("Press d to move right: \n");
    while (1) {
        char character = uart_getc();
        if (character != '\n' && character != '\b') {
            uart_sendc(character);  // print the received char
        }
        if (character == 'a') {
            clear_emulator_screen(1280,720);
            uart_puts("has been left");  // print the received char
            move_entity(&world->player, LEFT);
            update_player_position(&world);
            world->player.position.x += -10;
            drawEntity(world->player);
        }
        // if character = s, scroll down -> screen down
        else if (character == 'd') {
            move_entity(&world->player, RIGHT);
        }
    }
}

void move_entity(Entity *entity, Direction direction) {
    switch (direction) {
        case LEFT:
            entity->velocity.x = -20;

            entity->needs_update = true;
            uart_puts("increase velo");
            break;
        case RIGHT:
            entity->velocity.x = (entity->type == PLAYER) ? 20 : 10;
            entity->needs_update = true;
            break;
        case UP:
            entity->velocity.y = -20;
            entity->needs_update = true;
            break;
        case DOWN:
            entity->velocity.y = 20;
            entity->needs_update = true;
            break;
        case RESET_VERTICAL:
            entity->velocity.y = 0;
            entity->needs_update = true;
            break;
        case RESET_HORIZONTAL:
            entity->velocity.x = 0;
            entity->needs_update = true;
            break;
        default:
            entity->velocity.x = 0;
            entity->velocity.y = 0;
            entity->needs_update = true;
    }
}

void update_player_position(World *world) {
    // if (world->player.needs_update) {
    world->player.previous_pos = world->player.position;
    world->player.position.x += world->player.velocity.x;
    // world->player.needs_render = true;
    // world->player.needs_update = false;
    // }
}

void *updateWorld(void *arg) {
    while (1) {
        update_player_position(arg);
    }
    return;
}

void *updateRender(void *arg) {
    while (1) {
        render(arg);
    }
    return;
}

// Setting the value for aliens
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

// Draw the enity using the data has set
void render(World *world) {
    drawEntity(world->player);
    // for (int i = 0; i < NUM_ENEMIES; i++) {
    //     drawEntity(world->enemies[i]);
    // }
    // if (world->player.needs_render && world->player.enabled) {
    // clear(world->player);
    // drawEntity(world->player);
    // world->player.needs_render = false;
    // }
}

void clear(Entity entity) {
    int width = entity.dimension.width;
    int height = entity.dimension.height;

    int x = entity.previous_pos.x;
    int oldX = x;
    int y = entity.previous_pos.y;

    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        clear_emulator_screen(x, y);
    }
}