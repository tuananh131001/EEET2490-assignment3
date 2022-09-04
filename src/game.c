#include "game.h"

#include "framebf.h"
#include "helper.h"
#include "object.h"
#include "printf.h"
#include "string_manipulation.h"

void init_game(Game *world) {
    init_map(&world->world);
    fb_init();
}

// Create the stage
void init_map(World *world) {
    init_player(&world->player);
    init_enemies(world);
    // init_life(&world->life);
}
// Setting the value for player
void init_player(Entity *player) {
    player->dimension.height = blue_ship_sprite.height;
    player->dimension.width = blue_ship_sprite.width;
    player->position.x = (MAP_WIDTH / 2) - (player->dimension.width / 2);
    player->position.y = MAP_HEIGHT - 162;
    player->health.current_health = 1;
    player->type = PLAYER;
    player->needs_update = true;
    player->needs_render = true;
    player->needs_clear = false;
    player->combat_update = false;
    player->enabled = true;
}

// Move player
void move_player(World *world) {
    uart_puts("Press a to move left: \n");
    uart_puts("Press d to move right: \n");
    uart_puts("Press w to move up: \n");
    uart_puts("Press s to move down: \n");
    while (1) {
        char character = uart_getc();
        if (character != '\n' && character != '\b') {
        }
        if (character == 'a') {
            move_entity(&world->player, LEFT);
        }
        // if character = s, scroll down -> screen down
        else if (character == 'd') {
            move_entity(&world->player, RIGHT);

        } else if (character == 'w') {
            move_entity(&world->player, UP);

        } else if (character == 's') {
            move_entity(&world->player, DOWN);
        } else if (character == ' ') {
            entity_shoot(&world->player, UP);
        }
        render_health(world);
        update_player_position(world);
        render(world);
    }
}

void move_entity(Entity *entity, Direction direction) {
    switch (direction) {
        case LEFT:
            entity->velocity.x =
                (entity->type == PLAYER) ? -PLAYER_SPEED : -HORIZONTAL_SPEED;
            entity->velocity.y = 0;

            entity->needs_update = true;
            break;
        case RIGHT:
            entity->velocity.x =
                (entity->type == PLAYER) ? PLAYER_SPEED : HORIZONTAL_SPEED;
            entity->velocity.y = 0;
            entity->needs_update = true;
            break;
        case UP:
            entity->velocity.x = 0;
            entity->velocity.y = -VERTICAL_SPEED;
            entity->needs_update = true;
            break;
        case DOWN:
            entity->velocity.x = 0;
            entity->velocity.y = VERTICAL_SPEED;
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
// void update_movement_system(World *world) {
void update_player_position(World *world) {
    if (world->player.needs_update) {
        world->player.previous_pos = world->player.position;
        world->player.position.x += world->player.velocity.x;
        world->player.position.y += world->player.velocity.y;

        world->player.needs_render = true;
        world->player.needs_update = false;
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (world->player.projectile[i].needs_update) {
            if (world->player.projectile[i].position.y > TOP_MAX) {
                world->player.projectile[i].previous_pos =
                    world->player.projectile[i].position;
                world->player.projectile[i].position.x +=
                    world->player.projectile[i].velocity.x;
                world->player.projectile[i].position.y +=
                    world->player.projectile[i].velocity.y;
                world->player.projectile[i].needs_render = true;
            } else {
                world->player.projectile[i].needs_render = false;
                world->player.projectile[i].active = false;
                world->player.projectile[i].needs_clear = true;
            }
        }
    }

    // world->player.position.x += world->player.velocity.x;
    // world->player.position.y += world->player.velocity.y;

    // world->player.needs_render = true;
    // world->player.needs_update = false;
    // }
}
void entity_shoot(Entity *entity, Direction direction) {
    // if (clock() < entity->timer) return;

    // entity->timer = clock() + CLOCKS_PER_SEC / 2;

    for (int i = 0; i < MAX_BULLETS; i++) {
        // if (!entity->projectile[i].active) {
        // entity->projectile[i] = *create_bullet(*entity);

        // Initial a bullet
        entity->projectile[i].position.x =
            entity->position.x + (entity->dimension.width / 2);
        if (entity->type == PLAYER)
            entity->projectile[i].position.y =
                entity->position.y - entity->dimension.height;
        else
            entity->projectile[i].position.y =
                entity->position.y + entity->dimension.height;
        entity->projectile[i].dimension.height = red_laser.height;
        entity->projectile[i].dimension.width = red_laser.width;
        printf("\nprojectve x: %f", entity->projectile[i].position.x);
        printf("\nprojectve y: %f\n", entity->projectile[i].position.y);
        entity->projectile[i].needs_update = true;
        entity->projectile[i].needs_render = true;
        entity->projectile[i].active = true;
        move_bullet(&entity->projectile[i], direction);
        return;
        // }
    }
}

void move_bullet(Missile *projectile, Direction direction) {
    switch (direction) {
        case UP:
            projectile->velocity.y = -BULLET_VELOCITY;
            break;
        case DOWN:
            projectile->velocity.y = BULLET_VELOCITY;
            break;
        default:
            projectile->velocity.y = 0;
    }
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
    render_health(world);
    if (world->player.needs_render && world->player.enabled) {
        // clear(world->player);
        clear_emulator_screen(1024, 768);
        drawEntity(world->player);
        world->player.needs_render = false;
    } else if (world->player.needs_clear) {
        clear(world->player);
        world->player.needs_clear = false;
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        Type type = world->player.type;
        if (world->player.projectile[i].needs_render) {
            clear_projectile(world->player.projectile[i].previous_pos,
                             world->player.projectile[i].dimension);
            draw_projectile(type, world->player.projectile[i].position,
                            world->player.projectile[i].dimension);
        } else if (world->player.projectile[i].needs_clear) {
            clear_projectile(world->player.projectile[i].position,
                             world->player.projectile[i].dimension);
            world->player.projectile[i].needs_clear = false;
        }
    }
    // for (int i = 0; i < NUM_ENEMIES; i++) {
    //     drawEntity(world->enemies[i]);
    // }
    // if (world->player.needs_render && world->player.enabled) {
    // clear(world->player);
    // drawEntity(world->player);
    // world->player.needs_render = false;
    // }
}

void render_health(World *world) {
    int chealth = (world->player.health.current_health);
    char *health = integer_to_character(chealth);
    drawLine(0, 720, 1024, 720, 0x0c);
    drawChar(health, 20, 730, 0x0c);

    if (chealth == 3) {
        drawBar(5, 60, 720);
        drawBar(5, 110, 720);
        drawBar(5, 160, 720);

    } else if (chealth == 2) {
        drawBar(5, 60, 720);
        drawBar(5, 110, 720);
    } else if (chealth == 1) {
        drawBar(5, 60, 720);

    } else if (chealth == 0) {
        clear_emulator_screen(60, 720);
    }
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

void clearBar(int x, int y) {
    int width = 200;
    int height = 33;

    int oldX = x;

    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        clear_emulator_screen(x, y);
    }
}

void init_life(Entity *life) {
    life->health.player_health = 5;
    life->needs_update = false;
    life->needs_render = true;
}
void *memcpy(void *dest, const void *src, unsigned long n) {
    for (unsigned long i = 0; i < n; i++) {
        ((char *)dest)[i] = ((char *)src)[i];
    }
}