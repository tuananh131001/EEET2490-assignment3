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

// Setting the value for aliens
void init_enemies(World *world) {
    for (int i = 0, j = 0; i < NUM_ENEMIES; i++) {
        if (i < NUM_PAWNS) {
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
            world->enemies[i].health.current_health = PAWN_HEALTH;
            world->enemies[i].type = PAWN;
        } else if (i < 50) {
            if (j < 10) {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 30));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 2);
            } else {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 40));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 1);
            }
            world->enemies[i].dimension.height = queen_sprite.height;
            world->enemies[i].dimension.width = queen_sprite.width;
            world->enemies[i].health.current_health = QUEEN_HEALTH;
            world->enemies[i].type = QUEEN;
            j++;
        }

        world->enemies[i].needs_render = true;
        world->enemies[i].needs_update = true;
        world->enemies[i].enabled = true;
        world->enemies[i].combat_update = false;
        for (int j = 0; j < MAX_BULLETS; j++)
            world->enemies[i].projectile[j].active = false;
    }

    // for (int i = 0; i < 6; i++) {
    //     world->left_most_enemies[i] = 10 * i;
    //     world->right_most_enemies[i] = 10 * i + 9;
    // }
    for (int i = 0; i < MAX_SHOOTERS; i++) {
        world->shooters[i] = i;
    }
}
// Setting the value for player
void init_player(Entity *player) {
    player->dimension.height = blue_ship_sprite.height;
    player->dimension.width = blue_ship_sprite.width;
    player->position.x = (MAP_WIDTH / 2) - (player->dimension.width / 2);
    player->position.y = MAP_HEIGHT - 100;

    player->health.current_health = 3;

    player->type = PLAYER;
    player->needs_update = true;
    player->needs_render = true;
    player->needs_clear = false;
    player->combat_update = false;
    player->enabled = true;
}

// Setting the value for bunker
void init_bunkers(Entity bunkers[]) {
    for (int i = 0; i < NUM_BUNKERS; i++) {
        bunkers[i].position.x = 290 + (190 * (i + 1)) + (120 * i);
        bunkers[i].position.y = (MAP_HEIGHT)-250;
        bunkers[i].previous_pos = bunkers[i].position;
        bunkers[i].health.current_health = BUNKER_HEALTH;
        bunkers[i].dimension.height = bunker_1.height;
        bunkers[i].dimension.width = bunker_1.width;
        bunkers[i].health.current_health = BUNKER_HEALTH;
        bunkers[i].type = BUNKER;
        bunkers[i].enabled = true;
        bunkers[i].needs_update = true;
        bunkers[i].needs_render = true;
        bunkers[i].combat_update = false;
    }
}

// Move player
void move_player(World *world) {

    uart_puts("Press a to move left: \n");
    uart_puts("Press d to move right: \n");
    uart_puts("Press w to move up: \n");
    uart_puts("Press s to move down: \n");

    while (1) {
        char character = uart_getc_game();
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

        update_collision_system(world);
        update_combat_system(world);
        update_player_position(world);
        enemy_shoot(world);
        update_AI_system(world);
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

    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (world->enemies[i].needs_update) {
            world->enemies[i].previous_pos = world->enemies[i].position;
            world->enemies[i].position.x += world->enemies[i].velocity.x;
            world->enemies[i].position.y += world->enemies[i].velocity.y;
            world->enemies[i].needs_render = true;
            world->enemies[i].needs_update = false;
        }
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
    for (int index = 0; index < 10; index++) {
        int i = world->shooters[index];
        for (int j = 0; j < MAX_BULLETS; j++) {
            world->enemies[i].projectile[j].needs_update = true;
            if (world->enemies[i].projectile[j].needs_update) {
                if (world->enemies[i].projectile[j].position.y < BOTTOM_MAX) {
                    world->enemies[i].projectile[j].previous_pos =
                        world->enemies[i].projectile[j].position;
                    world->enemies[i].projectile[j].position.x +=
                        world->enemies[i].projectile[j].velocity.x;
                    world->enemies[i].projectile[j].position.y +=
                        world->enemies[i].projectile[j].velocity.y;
                    world->enemies[i].projectile[j].needs_render = true;
                } else {
                    world->enemies[i].projectile[j].needs_render = false;
                    world->enemies[i].projectile[j].active = false;
                    world->enemies[i].projectile[j].needs_clear = true;
                }
            }
        }
    }

    // world->player.position.x += world->player.velocity.x;
    // world->player.position.y += world->player.velocity.y;

    // world->player.needs_render = true;
    // world->player.needs_update = false;
    // }
}
void enemy_shoot(World *world) {
    // if (clock() < before) return;
    // before = clock() + CLOCKS_PER_SEC / 2;
    set_wait_timer(0, 10000);

    int random = (rand() % 100) % 10;
    // printf("\n%d", random);
    entity_shoot(&world->enemies[world->shooters[random]], DOWN);
}
unsigned long int next = 1;
int rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}
void entity_shoot(Entity *entity, Direction direction) {
    // if (clock() < entity->timer) return;

    // entity->timer = clock() + CLOCKS_PER_SEC / 2;
    wait_msec(30000);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!entity->projectile[i].active) {
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
        }
    }
}
void update_AI_system(World *world) {
    /* vertical reset */
    for (int i = 0; i < NUM_ENEMIES; i++)
        move_entity(&world->enemies[i], RESET_VERTICAL);

    /* check wall collisions */
    for (int i = 0; i < 6; i++) {
        if ((world->enemies[world->right_most_enemies[i]].position.x +
             world->enemies[world->right_most_enemies[i]].dimension.width) >=
            (RIGHT_MAX)) {
            travel_right = false;
            if (!enemies_at_bottom(world)) {
                for (int j = 0; j < NUM_ENEMIES; j++) {
                    move_entity(&world->enemies[j], DOWN);
                }
            }
        } else if ((world->enemies[world->left_most_enemies[i]].position.x) <=
                   (LEFT_MAX)) {
            travel_right = true;
            if (!enemies_at_bottom(world)) {
                for (int j = 0; j < NUM_ENEMIES; j++) {
                    move_entity(&world->enemies[j], DOWN);
                }
            }
        }
    }

    /* move enemies right or left */
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (travel_right) {
            move_entity(&world->enemies[i], RIGHT);
        } else {
            move_entity(&world->enemies[i], LEFT);
        }
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

void update_left_most(World *world, int index) {
    for (int i = 0; i < 6; i++) {
        if (world->left_most_enemies[i] == index) {
            world->left_most_enemies[i] += 1;
        }
    }
}

void update_right_most(World *world, int index) {
    for (int i = 0; i < 6; i++) {
        if (world->right_most_enemies[i] == index) {
            world->right_most_enemies[i] -= 1;
        }
    }
}

bool intersectAABB(Missile *projectile, Entity *entity) {
    return projectile->position.x <
               (entity->position.x + entity->dimension.width) &&
           (projectile->position.x + projectile->dimension.width) >
               entity->position.x &&
           projectile->position.y <
               (entity->position.y + entity->dimension.height) &&
           (projectile->position.y + projectile->dimension.height) >
               entity->position.y;
}

void resolve_collisions(Missile *projectile, Entity *entity) {
    bool isEnabled = entity->enabled;
    bool intersects = intersectAABB(projectile, entity);
    if (isEnabled && intersects) {
        projectile->active = false;
        projectile->needs_update = false;
        projectile->needs_render = false;
        projectile->needs_clear = true;
        entity->combat_update = true;
    }
}

void update_collision_system(World *world) {
    Entity *player = &world->player;
    Entity *enemy = world->enemies;
    Entity *bunker = world->bunkers;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (player->projectile[i].active) {
            for (int j = 0; j < NUM_ENEMIES; j++)
                resolve_collisions(&player->projectile[i], &enemy[j]);
            for (int k = 0; k < NUM_BUNKERS; k++)
                resolve_collisions(&player->projectile[i], &bunker[k]);
        }
    }
    // for (int i = 0; i < 10; i++) {
    //     int index = world->shooters[i];
    //     for (int j = 0; j < MAX_BULLETS; j++) {
    //         if (enemy[index].projectile[j].active) {
    //             resolve_collisions(&enemy[index].projectile[j], player);
    //             // for (int k = 0; k < NUM_BUNKERS; k++)
    //             //     resolve_collisions(&enemy[index].projectile[j],
    //             &bunker[k]);
    //         }
    //     }
    // }
}

void update_combat_system(World *world) {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (world->enemies[i].combat_update) {
            world->enemies[i].health.current_health -= 1;
            if (world->enemies[i].health.current_health <= 0) {
                world->enemies[i].enabled = false;
                world->enemies[i].needs_clear = true;
                world->playerScore.needsRender = true;
                update_score(world, world->enemies[i].type);
                // update_shooters(world, i);
                // update_left_most(world, i);
                // update_right_most(world, i);
                // world->enemies_alive -= 1;
            }
            world->enemies[i].combat_update = false;
            // if (world->enemies_alive == 0) {
            //     endScreen(1);
            // }
        }
    }

    for (int i = 0; i < NUM_BUNKERS; i++) {
        if (world->bunkers[i].combat_update) {
            world->bunkers[i].health.current_health -= 1;

            printf("Health: %d \n", world->bunkers[i].health.current_health);

            if (world->bunkers[i].health.current_health <= 0) {
                world->bunkers[i].enabled = false;
                world->bunkers[i].needs_clear = true;
            }
            world->bunkers[i].combat_update = false;
        }
    }

    // if (world->player.combat_update) {
    //     world->life.needs_render = true;
    //     world->player.health.current_health -= 1;
    //     if (world->player.health.current_health <= 0) {
    //         world->player.enabled = false;
    //         world->player.needs_clear = true;
    //     }
    //     world->player.combat_update = false;
    //     if (world->player.health.current_health == 0) {
    //         endScreen(0);
    //     }
    // }
}
bool enemies_at_bottom(World *world) {
    int bottom_most = 0;
    for (int i = 0; i < 10; i++) {
        if (world->enemies[bottom_most].position.y <
            world->enemies[i].position.y) {
            bottom_most = world->shooters[i];
        }
    }
    return (world->enemies[bottom_most].position.y +
            world->enemies[bottom_most].dimension.height) >
           ENEMIES_VERTICAL_MAX;
}
// Draw the enity using the data has set
void render(World *world) {
    wait_msec(10000);
    render_health(world);
    if (world->player.needs_render && world->player.enabled) {
        clear(world->player);
        // clear_emulator_screen(1024, 768);
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
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (world->enemies[i].needs_render && world->enemies[i].enabled) {
            clear(world->enemies[i]);
            // clear_emulator_screen(1024, 768);
            drawEntity(world->enemies[i]);

            world->enemies[i].needs_render = true;  // false default
        } else if (world->enemies[i].needs_clear) {
            // clear(world->enemies[i]);
            clear_emulator_screen(1920, 1080);
            // clear_emulator_screen(1024, 768);
            drawEntity(world->player);
            world->enemies[i].needs_clear = false;
        }
    }
    for (int i = 0; i < NUM_BUNKERS; i++) {
        if (world->bunkers[i].enabled) {
            // clear_emulator_screen(1024, 768);
            clear(world->bunkers[i]);
            drawEntity(world->bunkers[i]);
        } else if (world->bunkers[i].needs_clear) {
            clear(world->bunkers[i]);

            drawEntity(world->player);
            world->bunkers[i].needs_clear = false;
        }
    }
    for (int i = 0; i < MAX_SHOOTERS; i++) {
        for (int j = 0; j < MAX_BULLETS; j++) {
            int index = world->shooters[i];
            Type type = world->enemies[index].type;
            world->enemies[index].projectile[j].needs_render = true;
            if (world->enemies[index].projectile[j].needs_render) {
                clear_projectile(
                    world->enemies[index].projectile[j].previous_pos,
                    world->enemies[index].projectile[j].dimension);
                draw_projectile(type,
                                world->enemies[index].projectile[j].position,
                                world->enemies[index].projectile[j].dimension);
            } else if (world->enemies[index].projectile[j].needs_clear) {
                clear_projectile(world->enemies[index].projectile[j].position,
                                 world->enemies[index].projectile[j].dimension);
                world->enemies[index].projectile[j].needs_clear = false;
            }
        }
    }

    // if (world->player.needs_render && world->player.enabled) {
    // clear(world->player);
    // drawEntity(world->player);
    // world->player.needs_render = false;
    // }

    if (world->playerScore.needsRender) {
        render_score(world);
    }
}

void render_health(World *world) {
    int chealth = (world->player.health.current_health);
    char health = integer_to_character(chealth);
    drawLine(0, 1080, 1920, 1080, 0x0c);
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

void render_score(World *world) {
    int ones = (world->playerScore.score % 10);
    printf("ones: %d \n", ones);
    int tens = (world->playerScore.score % 100) / 10;
    printf("tens: %d \n", tens);
    int hundreds = (world->playerScore.score % 1000) / 100;
    printf("hundreds: %d \n", hundreds);
    int thousands = (world->playerScore.score % 10000) / 1000;
    printf("thousands: %d \n", thousands);

    drawScore(thousands, SCORE_ORIGINX, SCORE_ORIGINY);
    drawScore(hundreds, SCORE_ORIGINX + SHIFT, SCORE_ORIGINY);
    drawScore(tens, SCORE_ORIGINX + SHIFT + SHIFT, SCORE_ORIGINY);
    drawScore(ones, SCORE_ORIGINX + SHIFT + SHIFT + SHIFT, SCORE_ORIGINY);

    world->playerScore.needsRender = false;
}

void init_life(Entity *life) {
    life->health.player_health = 5;
    life->needs_update = false;
    life->needs_render = true;
}

void init_playerScore(Score *playerScore) {
    playerScore->score = 0;
    playerScore->needsUpdate = false;
    playerScore->needsRender = true;
}

void update_score(World *world, Type type) {
    if (type == PAWN) world->playerScore.score += PAWN_POINTS;
    if (type == QUEEN) world->playerScore.score += QUEEN_POINTS;
}

void *memcpy(void *dest, const void *src, unsigned long n) {
    for (unsigned long i = 0; i < n; i++) {
        ((char *)dest)[i] = ((char *)src)[i];
    }
}

// Create the stage
void init_map(World *world) {
    init_player(&world->player);
    init_enemies(world);
    init_bunkers(world->bunkers);
    init_playerScore(&world->playerScore);
    init_life(&world->life);
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
        drawPixel(x, y, 0);
    }
}
