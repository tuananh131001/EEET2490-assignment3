#include "game.h"

#include "display_image.h"
#include "framebf.h"
#include "helper.h"
#include "object.h"
#include "printf.h"
int wait_time_shoot = 50;
int stage = 1;

// Create new game
void init_game(Game *world) {
    world->game_over = false;
    world->game_start = false;
    world->main_menu.on_game_menu = true;
    world->main_menu.game_start_menu = true;
    world->game_win = false;
    restartGame = false;
    pauseGame = false;
    quitGame = false;

    init_map(&world->world);
    fb_init();
}
// Create the stage of the game
void init_map(World *world) {
    // setting variable for stage 2
    if (stage == 2) {
        init_gamer(&world->player);
        init_aliens_stage2(world);
        create_bunkers(world->bunkers);
        init_stage(&world->stage, 2);
        world->playerScore.needsUpdate = false;
        world->playerScore.needsRender = true;
        init_life(&world->life);
        world->enemies_alive = NUM_ENEMIES;
        world->game_menu.game_menu_option = 0;
        world->game_menu.on_gameMenu_menu = false;
        world->game_over = false;
    } else {
    // setting variable for stage 1
        init_gamer(&world->player);
        init_aliens(world);
        init_stage(&world->stage, 1);
        create_bunkers(world->bunkers);
        init_gamer_score(&world->playerScore);
        init_life(&world->life);
        world->enemies_alive = NUM_ENEMIES;
        world->game_menu.game_menu_option = 0;
        world->game_menu.on_gameMenu_menu = false;
        world->game_over = false;
    }
}
// Setting the value to the stage where game is new
void restart_game(Game *world) {
    init_map(&world->world);
    world->game_over = false;
    world->game_start = false;
    world->main_menu.on_game_menu = true;
    world->main_menu.game_start_menu = true;
    world->game_win = false;
    restartGame = false;
    clear_emulator_screen(1920, 1080);
    pauseGame = false;
    quitGame = false;
}
// Setting the value for player with 3 heath and 0 score
void init_gamer(Entity *player) {
    player->dimension.height = 46;
    player->dimension.width = 50;
    player->position.x = (MAP_WIDTH / 2) - (player->dimension.width / 2);
    player->position.y = MAP_HEIGHT - 50;
    for (int i = 0; i < MAX_BULLETS; i++) player->projectile[i].active = false;
    player->health.current_health = 3;
    player->type = PLAYER;
    player->needs_update = true;
    player->needs_render = false;
    player->needs_clear = false;
    player->combat_update = false;
    player->enabled = true;
}


/* The code above does the following:
1. Initializes the enemies with the appropriate sprite and health
2. Initializes the enemies' starting position
3. Initializes the array of shooters, which is used to determine which enemy
has the right to shoot a projectile
4. Initializes the array of projectiles, which is used to determine if an
enemy has a projectile active or not
// Setting the value for aliens
// Init the aliens in the correct position */
void init_aliens(World *world) {
    for (int i = 0, j = 0; i < NUM_ENEMIES; i++) {
        if (i < NUM_PAWNS) {
            if (i < 10) {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * i);
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 3);
            } else if (i < 20) {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 10));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 2);
            } else {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 20));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 1);
            }
            world->enemies[i].dimension.height = pawn_sprite.height;
            world->enemies[i].dimension.width = pawn_sprite.width;
            world->enemies[i].health.current_health = PAWN_HEALTH;
            world->enemies[i].type = PAWN;
        } else if (i < 30) {
            if (j < 10) {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 20));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 1);
            } else {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 40));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 2);
            }
            world->enemies[i].dimension.height = knight_sprite.height;
            world->enemies[i].dimension.width = knight_sprite.width;
            world->enemies[i].health.current_health = KNIGHT_HEALTH;
            world->enemies[i].type = KNIGHT;
            j++;
        } else if (i >= 30) {
            world->enemies[i].position.x =
                alien_initial_x + (HORIZONTAL_OFFSET * (i % 30));
            world->enemies[i].position.y = alien_initial_y + (VERTICAL_OFFSET);
            world->enemies[i].dimension.height = queen_sprite.height;
            world->enemies[i].dimension.width = queen_sprite.width;
            world->enemies[i].health.current_health = QUEEN_HEALTH;
            world->enemies[i].type = QUEEN;
        }
        world->enemies[i].needs_render = false;
        world->enemies[i].needs_update = true;
        world->enemies[i].enabled = true;
        world->enemies[i].combat_update = false;
        for (int j = 0; j < MAX_BULLETS; j++)
            world->enemies[i].projectile[j].active = false;
    }

    for (int i = 0; i < MAX_SHOOTERS; i++) {
        world->shooters[i] = i;
    }
}
// Like above but for stage 2 with most of enermies is knight
void init_aliens_stage2(World *world) {
    for (int i = 0, j = 0; i < NUM_ENEMIES; i++) {
        if (i < NUM_PAWNS) {
            if (i < 10) {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * i);
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 3);
            } else if (i < 20) {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 10));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 2);
            } else {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 20));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 1);
            }
            world->enemies[i].dimension.height = knight_sprite.height;
            world->enemies[i].dimension.width = knight_sprite.width;
            world->enemies[i].health.current_health = KNIGHT_HEALTH_2;
            world->enemies[i].type = KNIGHT;
        } else if (i < 30) {
            if (j < 10) {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 20));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 1);
            } else {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 40));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 2);
            }
            world->enemies[i].dimension.height = knight_sprite.height;
            world->enemies[i].dimension.width = knight_sprite.width;
            world->enemies[i].health.current_health = KNIGHT_HEALTH_2;
            world->enemies[i].type = KNIGHT;
            j++;
        } else if (i >= 30) {
            world->enemies[i].position.x =
                alien_initial_x + (HORIZONTAL_OFFSET * (i % 30));
            world->enemies[i].position.y = alien_initial_y + (VERTICAL_OFFSET);
            world->enemies[i].dimension.height = queen_sprite.height;
            world->enemies[i].dimension.width = queen_sprite.width;
            world->enemies[i].health.current_health = QUEEN_HEALTH_2;
            world->enemies[i].type = QUEEN;
        }
        world->enemies[i].needs_render = false;
        world->enemies[i].needs_update = true;
        world->enemies[i].enabled = true;
        world->enemies[i].combat_update = false;
        for (int j = 0; j < MAX_BULLETS; j++)
            world->enemies[i].projectile[j].active = false;
    }

    for (int i = 0; i < MAX_SHOOTERS; i++) {
        world->shooters[i] = i;
    }
}

// Setting the value for bunker
void create_bunkers(Entity bunkers[]) {
    for (int i = 0; i < NUM_BUNKERS; i++) {
        bunkers[i].position.x = 20 + (150 * (i + 1)) + (120 * i);
        bunkers[i].position.y = (MAP_HEIGHT)-150;
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
/* 
1. Waits for the user to press a key on the keyboard.
2. If the user presses the 'a' key, the player moves left.
3. If the user presses the 'd' key, the player moves right.
4. If the user presses the 's' key, the player moves down.
5. If the user presses the 'w' key, the player moves up.
6. If the user presses the 'space' key, the player shoots.
7. If the user presses the 'p' key, the game is paused.
8. If the user presses the 'q' key, the game is quit.
9. If the user presses the 'r' key, the game is restarted. */
void move_player(World *world) {
    uart_puts("Press A to move left: \n");
    uart_puts("Press D to move right: \n");
    uart_puts("Press W to move up: \n");
    uart_puts("Press S to move down: \n");
    uart_puts("Press SPACE to move down: \n");
    uart_puts("Press P to stop: \n");
    while (!quitGame && !restartGame) {
        while (!pauseGame) {
            char character = uart_getc_game();
            if (character != '\n' && character != '\b') {
            }
            if (character == 'a') {
                move_entity(&world->player, LEFT);
                world->player.velocity.y = 0;
            }
            // if character = s, scroll down -> screen down
            else if (character == 'd') {
                move_entity(&world->player, RIGHT);
                world->player.velocity.y = 0;
            } else if (character == 'w') {
                world->player.velocity.y = -VERTICAL_SPEED;
                world->player.velocity.x = 0;
                world->player.needs_update = true;
            } else if (character == 's') {
                world->player.velocity.y = VERTICAL_SPEED;
                world->player.velocity.x = 0;
                world->player.needs_update = true;
            } else if (character == ' ') {
                wait_msec(5000);
                entity_shoot(&world->player, UP);
            } else if (character == 'p') {
                show_game_menu(world);
            }
            wait_msec(5000);
            // Game logic looping
            update_AI_system(world);
            update_collision_system(world);
            update_combat_system(world);
            update_all_position(world);
            render(world);
            if (wait_time_shoot == 50) {
                enemy_shoot(world);
                wait_time_shoot = 0;
            }
            { wait_time_shoot++; }
        }
    }
}

/* The code above does the following
1. Draw the main menu
2. Wait for the user to press a button
3. If the user presses w, select start
4. If the user presses s, select quit
5. If the user presses space, start the game */
void show_main_menu(Game *game) {
    uart_puts("Press s to move down: \n");
    uart_puts("Press w to move up: \n");
    uart_puts("Press space to choose: \n");
    drawLogo();
    drawMainMenu(game);
    while (game->main_menu.on_game_menu) {
        char character = uart_getc_game();
        if (character == 'w') {
            game->main_menu.game_start_menu = true;  // select start
            drawMainMenu(game);
        } else if (character == 's') {
            game->main_menu.game_start_menu = false;  // select quit
            drawMainMenu(game);
        }

        else if (character == ' ') {
            game->main_menu.on_game_menu = false;
            if (game->main_menu.game_start_menu)
                game->game_start = true;
            else
                game->game_start = false;
            drawBackground();
        }
    }
}
// Menu to resume, restart or quit the game
void show_game_menu(World *world) {
    world->game_menu.game_menu_option = 1;
    world->game_menu.on_gameMenu_menu = true;
    pauseGame = true;
    while (world->game_menu.on_gameMenu_menu) {
        drawGameMenu(world);
        char character = uart_getc_game();
        printf("\n%d", world->game_menu.game_menu_option);
        if (character == 'w')  // up
        {
            if (world->game_menu.game_menu_option < 2) {
                world->game_menu.game_menu_option++;
            }
        } else if (character == 's')  // down
        {
            if (world->game_menu.game_menu_option > 0) {
                world->game_menu.game_menu_option--;
            }
        } else if (character == ' ')  // B
        {
            if (world->game_menu.game_menu_option == 2) {
                world->game_menu.on_gameMenu_menu = false;
                clear_emulator_screen(1024, 768);
                printf("\nSELECT: Pause");
                world->life.needs_render = true;
                world->playerScore.needsRender = true;
                world->player.needs_render = true;
                pauseGame = false;
            } else if (world->game_menu.game_menu_option == 1) {
                clear_emulator_screen(1024, 768);
                printf("\nSELECT: Restart");
                restartGame = true;
                return;
            } else if (world->game_menu.game_menu_option == 0) {
                clear_emulator_screen(1024, 768);
                printf("\nSELECT: Quit");
                quitGame = true;
                return;
            }
        }
    }
    return;
}

void move_entity(Entity *entity, Direction direction) {
    switch (direction) {
        case LEFT:
            entity->velocity.x =
                (entity->type == PLAYER) ? -PLAYER_SPEED : -HORIZONTAL_SPEED;
            // entity->velocity.y = 0;
            entity->needs_update = true;
            break;
        case RIGHT:
            entity->velocity.x =
                (entity->type == PLAYER) ? PLAYER_SPEED : HORIZONTAL_SPEED;

            entity->needs_update = true;
            break;
        case UP:
            // entity->velocity.x = 0;
            entity->velocity.y = -VERTICAL_SPEED;
            entity->needs_update = true;
            break;
        case DOWN:
            // entity->velocity.x = 0;
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

// Check any entity need updates it will render and clear at the new position
void update_all_position(World *world) {
    if (world->player.needs_update) {
        world->player.previous_pos = world->player.position;
        world->player.position.x += world->player.velocity.x;
        world->player.position.y += world->player.velocity.y;
        // Stop moving at edge
        if (world->player.position.x < 0) {
            world->player.position.x = 0;
        } else if (world->player.position.x >
                   MAP_WIDTH - world->player.dimension.width) {
            world->player.position.x =
                MAP_WIDTH - world->player.dimension.width;
        }
        if (world->player.position.y < 0) {
            world->player.position.y = 0;
        } else if (world->player.position.y >
                   MAP_HEIGHT - world->player.dimension.height) {
            world->player.position.y =
                MAP_HEIGHT - world->player.dimension.height;
        }
        world->player.needs_render = true;
        world->player.needs_update = false;
    }

    for (int i = 0; i < NUM_BUNKERS; i++) {
        // Stop player when hit bunker
        if (world->player.position.x + world->player.dimension.width >
                world->bunkers[i].position.x &&
            world->player.position.x < world->bunkers[i].position.x +
                                           world->bunkers[i].dimension.width &&
            world->player.position.y + world->player.dimension.height >
                world->bunkers[i].position.y &&
            world->player.position.y < world->bunkers[i].position.y +
                                           world->bunkers[i].dimension.height) {
            world->player.position = world->player.previous_pos;
            world->player.needs_render = true;
            world->player.needs_update = false;
        }
    }

    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (world->enemies[i].needs_update) {
            world->enemies[i].previous_pos = world->enemies[i].position;
            world->enemies[i].position.x += world->enemies[i].velocity.x;
            world->enemies[i].position.y += world->enemies[i].velocity.y;
            world->enemies[i].needs_render = true;
            world->enemies[i].needs_update = false;
        }
        // Check if enemy hit player
        if (world->enemies[i].position.x <
                world->player.position.x + world->player.dimension.width &&
            world->enemies[i].position.x + world->enemies[i].dimension.width >
                world->player.position.x &&
            world->enemies[i].position.y <
                world->player.position.y + world->player.dimension.height &&
            world->enemies[i].position.y + world->enemies[i].dimension.height >
                world->player.position.y) {
            endScreen(0, world);
        }
    }
    // Update position for bullets of player
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
    // Update position for bullet
    for (int index = 0; index < 10; index++) {
        int i = world->shooters[index];
        for (int j = 0; j < MAX_BULLETS; j++) {
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
}
// shot bullet for enermy
void enemy_shoot(World *world) {
    wait_msec(5000);
    int random = (rand() % 100) % 10;
    if (world->enemies[world->shooters[random]].enabled ==
        1)  // if enermy is still alive
    {
        entity_shoot(&world->enemies[world->shooters[random]], DOWN);
    }
}
unsigned long int next = 1;
//helper function to create random index of enermy
int rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}
// create bullet for player and enermy
void entity_shoot(Entity *entity, Direction direction) {
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
            // printf("\nprojectve x: %f", entity->projectile[i].position.x);
            // printf("\nprojectve y: %f\n", entity->projectile[i].position.y);
            entity->projectile[i].needs_update = true;
            entity->projectile[i].needs_render = true;
            entity->projectile[i].active = true;
            move_bullet(&entity->projectile[i], direction);
            return;
        }
    }
}
// Update moving of enermies
void update_AI_system(World *world) {
    /* vertical reset */
    for (int i = 0; i < NUM_ENEMIES; i++)
        move_entity(&world->enemies[i], RESET_VERTICAL);

    /* check wall collisions */
    for (int i = 0; i < 6; i++) {
        if ((world->enemies[9].position.x +
             world->enemies[9].dimension.width) >= (RIGHT_MAX)) {
            travel_right = false;
            if (!enemies_at_bottom(world)) {
                for (int j = 0; j < NUM_ENEMIES; j++) {
                    move_entity(&world->enemies[j], DOWN);
                }
            }
        } else if ((world->enemies[0].position.x) <= (LEFT_MAX)) {
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
// Update the speed of bullet
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
// check if bullet hit enermy or player
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

// when bullet hit player trigger the steep to remove the bullet and entity
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
/* The code above does the following:
1. Check if the player's projectiles are active
2. If so, check if they collide with any of the enemies or bunkers
3. Check if the enemies' projectiles are active
4. If so, check if they collide with the player or any of the bunkers */
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
    for (int i = 0; i < 10; i++) {
        int index = world->shooters[i];
        for (int j = 0; j < MAX_BULLETS; j++) {
            if (enemy[index].projectile[j].active) {
                resolve_collisions(&enemy[index].projectile[j], player);
                for (int k = 0; k < NUM_BUNKERS; k++)
                    resolve_collisions(&enemy[index].projectile[j], &bunker[k]);
            }
        }
    }
}
/* The code above does the following:
1. If the index of the player is in the shooters array, then it is updated.
2. The shooter array will now have a value that is 10 higher than the index of the player.
3. The shooter array will have a value that is not the index of the player. */
void update_shooters(World *world, int index) {
    for (int i = 0; i < MAX_SHOOTERS; i++) {
        if (world->shooters[i] == index) {
            world->shooters[i] += 10;
        }
    }
}

/* The code above does the following:
1. Checks if the player has been hit by an enemy bullet
2. If the player has been hit, it will display the explosion animation
3. Then it will wait for half a second
4. Then it will redraw the spaceship
5. If the player has no lives left, it will end the game
6. If the player has been hit, it will decrement the player's health by 1
7. If the player has no health left, it will end the game
8. Checks if any enemies have been hit by the player's bullets
9. If an enemy has been hit, it will display the explosion animation
10. Then it will wait for half a second
11. Then it will update the score
12. Then it will update the number of enemies left
13. If there are no enemies left, it will end the game
14. Checks if any bunkers have been hit
15. If a bunker has been hit, it will decrement the bunker's health by 1
16. If a bunker has no health left, it will disable the bunker
17. If a bunker has no health left, it will move the bunker off the screen
18. If a bunker has no health left, it will clear the bunker from the screen */
void update_combat_system(World *world) {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (world->enemies[i].combat_update) {
            world->enemies[i].health.current_health -= 1;
            if (world->enemies[i].health.current_health <= 0) {
                world->enemies[i].enabled = false;
                world->enemies[i].needs_clear = true;
                // delete bullet
                for (int j = 0; j < MAX_BULLETS; j++) {
                    world->enemies[i].projectile[j].active = false;
                    world->enemies[i].projectile[j].needs_update = false;
                    world->enemies[i].projectile[j].needs_render = false;
                    world->enemies[i].projectile[j].needs_clear = true;
                }
                drawExplosion(world->enemies[i]);
                wait_msec(500);
                world->playerScore.needsRender = true;
                update_score(world, world->enemies[i].type);
                update_shooters(world, i);
                world->enemies_alive -= 1;
            }
            world->enemies[i].combat_update = false;
            if (world->enemies_alive == 0) {
                char *type = "d";
                drawScore(world, type);
                endScreen(1, world);
            }
        }
    }

    for (int i = 0; i < NUM_BUNKERS; i++) {
        if (world->bunkers[i].combat_update) {
            world->bunkers[i].health.current_health -= 1;

            if (world->bunkers[i].health.current_health <= 0) {
                world->bunkers[i].enabled = false;
                world->bunkers[i].position.x = 0;
                world->bunkers[i].position.y = 0;

                world->bunkers[i].needs_clear = true;
            }
            world->bunkers[i].combat_update = false;
        }
    }

    if (world->player.combat_update) {
        drawExplosion(world->player);
        wait_msec(500);
        drawSpaceShip(world->player, world);
        world->life.needs_render = true;
        world->player.health.current_health -= 1;
        if (world->player.health.current_health <= 0) {
            world->player.enabled = false;
            world->player.needs_clear = true;
            endScreen(0, world);
        }
        world->player.combat_update = false;
        if (world->player.health.current_health == 0) {
        }
    }
}
// stop enemy when it reach the bottom
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
    // wait_msec(5000);

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
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (world->enemies[i].needs_render && world->enemies[i].enabled) {
            wait_msec(100);
            clear(world->enemies[i]);
            drawEntity(world->enemies[i]);
            world->enemies[i].needs_render = false;  // false default
        } else if (world->enemies[i].needs_clear) {
            clear(world->enemies[i]);
            // fix bug bullet not clear
            for (int j = 0; j < MAX_BULLETS; j++) {
                clear_projectile(world->enemies[i].projectile[j].position,
                                 world->enemies[i].projectile[j].dimension);
            }
            drawSpaceShip(world->player, world);
            world->enemies[i].needs_clear = false;
        }
    }
    // clear and draw bunker
    for (int i = 0; i < NUM_BUNKERS; i++) {
        if (world->bunkers[i].enabled) {
            wait_msec(1000);
            clear(world->bunkers[i]);
            drawEntity(world->bunkers[i]);
        } else if (world->bunkers[i].needs_clear) {
            clear(world->bunkers[i]);
            drawSpaceShip(world->player, world);
            world->bunkers[i].needs_clear = false;
        }
    }
    // clean and draw bullets
    for (int i = 0; i < MAX_SHOOTERS; i++) {
        for (int j = 0; j < MAX_BULLETS; j++) {
            int index = world->shooters[i];
            Type type = world->enemies[index].type;
            if (world->enemies[index].projectile[j].needs_render &&
                world->enemies[index].enabled &&
                world->enemies[index].projectile[j].position.y > 50 &&
                world->enemies[index].projectile[j].position.x >
                    50) {  // fix bug bullet from top left
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
    // clear and draw player
    if (world->player.needs_render && world->player.enabled) {
        clear(world->player);
        drawSpaceShip(world->player, world);
        world->player.needs_render = false;
    } else if (world->player.needs_clear) {
        clear(world->player);
        world->player.needs_clear = false;
    }
    // clear and draw life
    if (world->life.needs_render) {
        render_health(world);
    }
    // clear and draw score
    if (world->playerScore.needsRender) {
        char *type = "";
        drawScore(world, type);
        world->playerScore.needsRender = false;
    }
    // clear and draw stage
    if (world->stage.needsRender) {
        if (world->stage.current_stage == 1) {
            displayStage1(450, 5);
        } else if (world->stage.current_stage == 2) {
            displayStage2(450, 5);
        }
    }
}

// draw score
void drawScore(World *world, char *type) {
    int x = 0;
    int y = 0;
    if (type == "d") {
        x = 350;
        y = 500;
        displayScore(770 - x, 10 + y);
    }
    if (type == "") {
        x = 0;
        y = 0;
        displayScore(770, 10);
    }

    int ones = (world->playerScore.score % 10);
    int tens = (world->playerScore.score % 100) / 10;
    int hundreds = (world->playerScore.score % 1000) / 100;
    int thousands = (world->playerScore.score % 10000) / 1000;

    int score = world->playerScore.score;
    if (score >= 0 && score < 10) {
        clearScore(ones, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
        render_score(ones, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
    } else if (score >= 10 && score < 100) {
        clearScore(tens, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
        clearScore(ones, SCORE_ORIGINX - x + SHIFT, SCORE_ORIGINY + y);
        render_score(tens, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
        render_score(ones, SCORE_ORIGINX - x + SHIFT, SCORE_ORIGINY + y);
    } else if (score >= 100 && score < 1000) {
        clearScore(hundreds, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
        clearScore(tens, SCORE_ORIGINX - x + SHIFT, SCORE_ORIGINY + y);
        clearScore(ones, SCORE_ORIGINX - x + SHIFT + SHIFT, SCORE_ORIGINY + y);
        render_score(hundreds, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
        render_score(tens, SCORE_ORIGINX - x + SHIFT, SCORE_ORIGINY + y);
        render_score(ones, SCORE_ORIGINX - x + SHIFT + SHIFT,
                     SCORE_ORIGINY + y);
    } else if (score >= 1000) {
        clearScore(thousands, SCORE_ORIGINX, SCORE_ORIGINY);
        clearScore(hundreds, SCORE_ORIGINX + SHIFT, SCORE_ORIGINY);
        clearScore(tens, SCORE_ORIGINX + SHIFT + SHIFT, SCORE_ORIGINY);
        clearScore(ones, SCORE_ORIGINX + SHIFT + SHIFT + SHIFT, SCORE_ORIGINY);

        render_score(thousands, SCORE_ORIGINX, SCORE_ORIGINY);
        render_score(hundreds, SCORE_ORIGINX + SHIFT, SCORE_ORIGINY);
        render_score(tens, SCORE_ORIGINX + SHIFT + SHIFT, SCORE_ORIGINY);
        render_score(ones, SCORE_ORIGINX + SHIFT + SHIFT + SHIFT,
                     SCORE_ORIGINY);
    }
}

// draw health left of player
void render_health(World *world) {
    int clife = (world->player.health.current_health);
    printf("health: %d\n", clife);
    displayWordPlayerLife(100, 10);
    if (clife == 0) {
        clearPlayerLife(170, 30);
        displayScore0(170, 10);
    }

    if (clife == 1) {
        clearPlayerLife(210, 20);
        displayPlayerLife(170, 20);
    }
    if (clife == 2) {
        clearPlayerLife(250, 20);
        displayPlayerLife(170, 20);
        displayPlayerLife(210, 20);
    }
    if (clife == 3) {
        displayPlayerLife(170, 20);
        displayPlayerLife(210, 20);
        displayPlayerLife(250, 20);
    }

    world->life.needs_render = false;
}

// clear health of player 
void clear_health(int x, int y) {
    for (int h = 0; h < x; h++) {
        for (int w = 0; w < y; w++) {
            drawPixelARGB32(w, h, 0);
        }
    }
}
// render score
void render_score(int num, int x, int y) {
    if (num == 1)
        displayScore1(x, y);
    else if (num == 2)
        displayScore2(x, y);
    else if (num == 3)
        displayScore3(x, y);
    else if (num == 4)
        displayScore4(x, y);
    else if (num == 5)
        displayScore5(x, y);
    else if (num == 6)
        displayScore6(x, y);
    else if (num == 7)
        displayScore7(x, y);
    else if (num == 8)
        displayScore8(x, y);
    else if (num == 9)
        displayScore9(x, y);
    else if (num == 0)
        displayScore0(x, y);
}

// init life of 3 hearts
void init_life(Entity *life) {
    life->health.player_health = 3;
    life->needs_update = false;
    life->needs_render = true;
}
// init score
void init_gamer_score(Score *playerScore) {
    playerScore->score = 0;
    playerScore->needsUpdate = false;
    playerScore->needsRender = true;
}
// init stage
void init_stage(Stage *stage, int stageNum) {
    stage->current_stage = stageNum;
    stage->needsUpdate = false;
    stage->needsRender = true;
}
//  init player
void update_score(World *world, Type type) {
    if (type == PAWN) world->playerScore.score += PAWN_POINTS;
    if (type == KNIGHT) world->playerScore.score += KNIGHT_POINTS;
    if (type == QUEEN) world->playerScore.score += QUEEN_POINTS;
}

void *memcpy(void *dest, const void *src, unsigned long n) {
    for (unsigned long i = 0; i < n; i++) {
        ((char *)dest)[i] = ((char *)src)[i];
    }
}

// draw game menu when player press p and select resume , restart, quit
void drawGameMenu(World *game) {
    int *colorptrMenu;
    int widthMenu = game_menu_pause.width;
    int heightMenu = game_menu_pause.height;
    if (game->game_menu.game_menu_option == 2)
        colorptrMenu = (int *)game_menu_pause.image_pixels;
    else if (game->game_menu.game_menu_option == 1)
        colorptrMenu = (int *)game_menu_restart.image_pixels;
    else if (game->game_menu.game_menu_option == 0)
        colorptrMenu = (int *)game_menu_quit.image_pixels;

    int xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
    int yMenu = TOP_MAX;

    for (int i = 0; i < (widthMenu * heightMenu); i++) {
        xMenu++;
        if (i % widthMenu == 0) {
            yMenu++;
            xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
        }
        drawPixelARGB32(xMenu, yMenu, colorptrMenu[i]);
    }
}
// draw main menu where can select start game or exit
void drawMainMenu(Game *game) {
    int *colorptrMenu;
    int widthMenu = main_menu_quit.width;
    int heightMenu = main_menu_quit.height;

    if ((game->main_menu.game_start_menu))
        colorptrMenu = (int *)main_menu_start.image_pixels;
    else
        colorptrMenu = (int *)main_menu_quit.image_pixels;

    int xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
    int yMenu = TOP_MAX + LOGO.height;

    for (int i = 0; i < (widthMenu * heightMenu); i++) {
        xMenu++;
        if (i % widthMenu == 0) {
            yMenu++;
            xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
        }
        drawPixelARGB32(xMenu, yMenu, colorptrMenu[i]);
    }
    // drawAuthors();
    displayAuthorsImage(80, 620);
}

// announce loss or win and n to next stage
void endScreen(bool won, World *world) {
    pauseGame = true;
    uart_puts("\n\n");
    uart_puts("Press o to out: \n");
    uart_puts("Press r to restart: \n");
    // drawBackground();
    if (stage == 1 && won) {
        uart_puts("Press n to next stage: \n");
    }
    clear_emulator_screen(1024, 768);

    if (won) {
        char *type = "";

        drawScore(world, type);
        displayGameWinImage(300, 100);
    } else {
        char *type = "d";

        drawScore(world, type);
        displayGameOverImage(300, 100);
    }

    while (!restartGame) {
        char character = uart_getc();

        if (character == 'o') {
            quitGame = true;
            uart_puts("\n\nSuccessfully out!\n");
            return;
        }
        if (character == 'r') {
            restartGame = true;
        }
        if (stage == 1 && won) {
            uart_puts("Press n to next stage: \n");
            if (character == 'n') {
                stage = 2;
                restartGame = true;
            }
        }
    }
    if (won) {
        char *type = "";
        drawScore(world, type);
        displayGameWinImage(300, 100);
    } else {
        char *type = "d";
        drawScore(world, type);
        displayGameOverImage(300, 100);
    }
    return;
}

// draw a small explosion  when bullet hit enemy
void drawExplosion(Entity entity) {
    int x = entity.position.x;
    int oldX = x;
    int y = entity.position.y;
    if (entity.type == PLAYER) {
        displayExplosion2(x, y);
    } else {
        displayExplosion(x, y);
    }
}
// draw spaceship with different level
void drawSpaceShip(Entity entity, World *world) {
    int score = world->playerScore.score;
    int width = entity.dimension.width;
    int height = entity.dimension.height;

    int x = entity.position.x;
    int oldX = x;
    int y = entity.position.y;
    if (entity.type == PLAYER) {
        if (score <= 100) {
            displaySpaceShipImage(x, y);
        } else if (score > 100 && score <= 250) {
            clear(world->player);
            displaySpaceShipImageLevel2(x, y);
        } else if (score > 250 && score <= 700) {
            clear(world->player);
            displaySpaceShipImageLevel3(x, y);
        } else {
            clear(world->player);
            displaySpaceShipImageLevel4(x, y);
        }
    }
}
// clear the entity on screen
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
        drawPixelARGB32(x, y, 0);
    }
}
// Draw an object like ship , alien with pixel data in object.h
void drawEntity(Entity entity) {
    int *colorptr;
    int width = entity.dimension.width;
    int height = entity.dimension.height;

    int x = entity.position.x;
    int oldX = x;
    int y = entity.position.y;
    if (entity.type == PAWN) {
        colorptr = (int *)pawn_sprite.image_pixels;
    } else if (entity.type == KNIGHT)
        colorptr = (int *)knight_sprite.image_pixels;
    else if (entity.type == QUEEN)
        colorptr = (int *)queen_sprite.image_pixels;
    else if (entity.type == PLAYER)
        colorptr = (int *)blue_ship_sprite.image_pixels;
    else if (entity.type == BUNKER) {
        if (entity.health.current_health <= BUNKER_HEALTH / 3) {
            colorptr = (int *)bunker_3.image_pixels;
            width = bunker_3.width;
            height = bunker_3.height;
        } else if (entity.health.current_health <= BUNKER_HEALTH / 2) {
            colorptr = (int *)bunker_2.image_pixels;
            width = bunker_2.width;
            height = bunker_2.height;
        } else {
            colorptr = (int *)bunker_1.image_pixels;
            width = bunker_1.width;
            height = bunker_1.height;
        }
    }

    // draw in 2D
    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        drawPixelARGB32(x, y, colorptr[i]);
    }
}
// clear bullet
void clear_projectile(Position position, Dimension dimension) {
    int width = dimension.width;
    int height = dimension.height;

    int x = position.x;
    int oldX = x;
    int y = position.y;

    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        drawPixelARGB32(x, y, 0);
    }
}
// draw bullet
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
        drawPixelARGB32(x, y, colorptr[i]);
    }
}
// draw score
void drawBar(int health, int x, int y) {
    int *colorptr;
    int width = 40;
    int height = 40;

    int oldX = x;
    colorptr = (int *)blue_ship_sprite.image_pixels;
    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        drawPixel(x, y, colorptr[i]);
    }
}
// draw line
void drawLine(int x1, int y1, int x2, int y2, unsigned char attr) {
    int dx, dy, p, x, y;

    dx = x2 - x1;
    dy = y2 - y1;
    x = x1;
    y = y1;
    p = 2 * dy - dx;

    while (x < x2) {
        if (p >= 0) {
            drawPixel(x, y, attr);
            y++;
            p = p + 2 * dy - 2 * dx;
        } else {
            drawPixel(x, y, attr);
            p = p + 2 * dy;
        }
        x++;
    }
}
// draw circle
void drawCircle(int x0, int y0, int radius, unsigned char attr, int fill) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        if (fill) {
            drawLine(x0 - y, y0 + x, x0 + y, y0 + x, (attr & 0xf0) >> 4);
            drawLine(x0 - x, y0 + y, x0 + x, y0 + y, (attr & 0xf0) >> 4);
            drawLine(x0 - x, y0 - y, x0 + x, y0 - y, (attr & 0xf0) >> 4);
            drawLine(x0 - y, y0 - x, x0 + y, y0 - x, (attr & 0xf0) >> 4);
        }
        drawPixel(x0 - y, y0 + x, attr);
        drawPixel(x0 + y, y0 + x, attr);
        drawPixel(x0 - x, y0 + y, attr);
        drawPixel(x0 + x, y0 + y, attr);
        drawPixel(x0 - x, y0 - y, attr);
        drawPixel(x0 + x, y0 - y, attr);
        drawPixel(x0 - y, y0 - x, attr);
        drawPixel(x0 + y, y0 - x, attr);

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }

        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}
// draw background
void drawBackground() {
    int width = RIGHT_MAX - LEFT_MAX;
    int height = BOTTOM_MAX - TOP_MAX + 60;
    int x = LEFT_MAX;
    int y = TOP_MAX;
    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = LEFT_MAX;
        }
        drawPixelARGB32(x, y, 0);
    }
}
// draw logo
void drawLogo() {
    int *colorptrLogo = (int *)LOGO.image_pixels;
    int widthLogo = LOGO.width;
    int heightLogo = LOGO.height;

    int xLogo = (int)((MAP_WIDTH / 2) - (widthLogo / 2));
    int yLogo = TOP_MAX;

    for (int i = 0; i < (widthLogo * heightLogo); i++) {
        xLogo++;
        if (i % widthLogo == 0) {
            yLogo++;
            xLogo = (int)((MAP_WIDTH / 2) - (widthLogo / 2));
        }
        drawPixelARGB32(xLogo, yLogo, colorptrLogo[i]);
    }
}
// draw Tuan Anh name and Huy
void drawAuthors() {
    int *colorptrNames;
    int widthNames = authors.width;
    int heightNames = authors.height;
    colorptrNames = (int *)authors.image_pixels;

    int xMenu = (int)((MAP_WIDTH / 2) - (widthNames / 2));
    int yMenu = TOP_MAX + LOGO.height + main_menu_quit.height;

    for (int i = 0; i < (widthNames * heightNames); i++) {
        xMenu++;
        if (i % widthNames == 0) {
            yMenu++;
            xMenu = (int)((MAP_WIDTH / 2) - (widthNames / 2));
        }
        drawPixelARGB32(xMenu, yMenu, colorptrNames[i]);
    }
}
// draw you win
void gameWinEndDisplay() {
    int *colorptr;
    int width = game_win_logo.width;
    int height = game_win_logo.height;
    colorptr = (int *)game_win_logo.image_pixels;
    int x = (int)((MAP_WIDTH / 2) - (width / 2));
    int y = 400;

    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = (int)((MAP_WIDTH / 2) - (width / 2));
        }
        drawPixelARGB32(x, y, colorptr[i]);
    }
}
