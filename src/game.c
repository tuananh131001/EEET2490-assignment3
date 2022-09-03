#include "game.h"

#include "helper.h"
#include "object.h"
#include "string_manipulation.h"

void init_game(Game *world) {
    init_map(&world->world);
    fb_init();
}

// Create the stage
void init_map(World *world) {
    init_player(&world->player);
    init_enemies(world);
    init_life(&world->life);
}
// Setting the value for player
void init_player(Entity *player) {
    player->dimension.height = blue_ship_sprite.height;
    player->dimension.width = blue_ship_sprite.width;
    player->position.x = (MAP_WIDTH / 2) - (player->dimension.width / 2);
    player->position.y = MAP_HEIGHT - 162;
    player->health.current_health = 1;
    player->needs_render = false;
    player->type = PLAYER;
}

// Move player
void move_player(World *world) {
    uart_puts("Press a to move left: \n");
    uart_puts("Press d to move right: \n");
    uart_puts("Press w to move up: \n");
    uart_puts("Press s to move down: \n");
    char vertical = "v";
    while (1) {
        char character = uart_getc();
        if (character != '\n' && character != '\b') {
        }
        if (character == 'a') {
            char *type = "h";
            clear_emulator_screen(1024, 768);
            move_entity(&world->player, LEFT);
            update_player_position(world, type);
            drawEntity(world->player);
            render_health(world);
        }
        // if character = s, scroll down -> screen down
        else if (character == 'd') {
            char *type = "h";
            clear_emulator_screen(1024, 768);
            move_entity(&world->player, RIGHT);
            update_player_position(world, type);
            drawEntity(world->player);
            render_health(world);

        } else if (character == 'w') {
            char *type = "v";
            clear_emulator_screen(1024, 768);
            move_entity(&world->player, UP);
            update_player_position(world, type);
            drawEntity(world->player);
            render_health(world);
        } else if (character == 's') {
            char *type = "v";

            clear_emulator_screen(1024, 768);
            move_entity(&world->player, DOWN);
            update_player_position(world, type);
            drawEntity(world->player);
            render_health(world);
        } else if (character == ' ') {
            char *type = "shoot";
            uart_puts("This is space");
            clear_emulator_screen(1024, 768);
            entity_shoot(&world->player, UP);
            update_player_position(world, type);
            drawEntity(world->player);
            for (int i = 0; i < MAX_BULLETS; i++) {
                Type type = world->player.type;
                // if (world->player.projectile[i].needs_render) {
                // clear_projectile(world->player.projectile[i].previous_pos,
                //                  world->player.projectile[i].dimension);
                draw_projectile(type, world->player.projectile[i].position,
                                world->player.projectile[i].dimension);
                // } else if (world->player.projectile[i].needs_clear) {
                //     clear_projectile(world->player.projectile[i].position,
                //                      world->player.projectile[i].dimension);
                //     world->player.projectile[i].needs_clear = false;
                // }
            }
        }
    }
}

void move_entity(Entity *entity, Direction direction) {
    switch (direction) {
        case LEFT:
            entity->velocity.x = -20;
            entity->needs_update = true;
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

// void update_movement_system(World *world) {
void update_player_position(World *world, char *type) {
    // if (world->player.needs_update) {
    // Process if the type is vertical
    if (type == "v") {
        // Check the current y position is at the top edge
        if (world->player.position.y + world->player.velocity.y < 0) {
            // Update vertical position
            world->player.position.y = 0;
        }
        // Check the current y position is at the bottom edge
        else if (world->player.position.y + world->player.velocity.y >
                 720 - world->player.dimension.height) {
            // Update vertical position
            world->player.position.y = 720 - world->player.dimension.height;
        } else {  // Update vertical position
            world->player.position.y += world->player.velocity.y;
        }
        // world->player.previous_pos = world->player.position;
        // world->player.position.y += world->player.velocity.y;
    }
    // Process if the type is horizontal
    else if (type == "h") {
        // Check  the current x position is at the left edge

        if (world->player.position.x + world->player.velocity.x < 0) {
            // updating the horizontal position

            world->player.position.x = 0;
        }
        // Check  the current x position is at the right edge

        else if (world->player.position.x + world->player.velocity.x >
                 MAP_WIDTH - world->player.dimension.width) {
            // updating the horizontal position
            world->player.position.x =
                MAP_WIDTH - world->player.dimension.width;
        } else {
            // updating the horizontal position
            world->player.previous_pos = world->player.position;
            world->player.position.x += world->player.velocity.x;
        }
    } else if (type == "shoot") {
        for (int i = 0; i < MAX_BULLETS; i++) {
            // if (world->player.projectile[i].needs_update) {
            if (world->player.projectile[i].position.y > TOP_MAX) {
                world->player.projectile[i].previous_pos =
                    world->player.projectile[i].position;
                world->player.projectile[i].position.x +=
                    world->player.projectile[i].velocity.x;
                world->player.projectile[i].position.y +=
                    world->player.projectile[i].velocity.y;
                // world->player.projectile[i].needs_render = true;
                // } else {
                //     world->player.projectile[i].needs_render = false;
                //     world->player.projectile[i].active = false;
                //     world->player.projectile[i].needs_clear = true;
                // }
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
        entity->projectile[i] = *create_bullet(*entity);
        move_bullet(&entity->projectile[i], direction);
        return;
        // }
    }
}

Missile *create_bullet(Entity owner) {
    Missile bullet[30];
    bullet->position.x = owner.position.x + (owner.dimension.width / 2);
    if (owner.type == PLAYER)
        bullet->position.y = owner.position.y - owner.dimension.height;
    else
        bullet->position.y = owner.position.y + owner.dimension.height;
    bullet->dimension.height = red_laser.height;
    bullet->dimension.width = red_laser.width;
    return bullet;
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
    drawEntity(world->player);
    render_health(world);

    // for (int i = 0; i < NUM_ENEMIES; i++) {
    //     drawEntity(world->enemies[i]);
    // }
    // if (world->player.needs_render && world->player.enabled) {
    // clear(world->player);
    // drawEntity(world->player);
    // world->player.needs_render = false;
    // }
}
void draw_projectile(Type type, Position position, Dimension dimension) {
    int *colorptr;
    int width = dimension.width;
    int height = dimension.height;

    if (type != PLAYER)
        colorptr = (int *)green_laser.image_pixels;
    else
        colorptr = (int *)red_laser.image_pixels;

    int x = position.x;
    int oldX = x;
    int y = position.y;

    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        drawPixel(x, y, colorptr[i]);
    }
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