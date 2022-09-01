#include "game.h"

#include "helper.h"
#include "object.h"

void init_game(Game *world)
{
    init_map(&world->world);
    fb_init();
}

// Create the stage
void init_map(World *world)
{
    init_player(&world->player);
    init_enemies(world);
}
// Setting the value for player
void init_player(Entity *player)
{
    player->dimension.height = blue_ship_sprite.height;
    player->dimension.width = blue_ship_sprite.width;
    player->position.x = (MAP_WIDTH / 2) - (player->dimension.width / 2);
    player->position.y = MAP_HEIGHT - 162;
    player->type = PLAYER;
}

// Move player
void move_player(World *world)
{
    uart_puts("Press a to move left: \n");
    uart_puts("Press d to move right: \n");
    uart_puts("Press w to move up: \n");
    uart_puts("Press s to move down: \n");
    char vertical = "v";
    while (1)
    {
        char character = uart_getc();
        if (character != '\n' && character != '\b')
        {
        }
        if (character == 'a')
        {
            char *type = "h";
            clear_emulator_screen(1024, 768);
            move_entity(&world->player, LEFT);
            update_player_position(world, type);
            drawEntity(world->player);
        }
        // if character = s, scroll down -> screen down
        else if (character == 'd')
        {
            char *type = "h";
            clear_emulator_screen(1024, 768);
            move_entity(&world->player, RIGHT);
            update_player_position(world, type);
            drawEntity(world->player);
        }
        else if (character == 'w')
        {
            char *type = "v";
            clear_emulator_screen(1024, 768);
            move_entity(&world->player, UP);
            update_player_position(world, type);
            drawEntity(world->player);
        }
        else if (character == 's')
        {
            char *type = "v";

            clear_emulator_screen(1024, 768);
            move_entity(&world->player, DOWN);
            update_player_position(world, type);
            drawEntity(world->player);
        }
    }
}

void move_entity(Entity *entity, Direction direction)
{
    switch (direction)
    {
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

void update_player_position(World *world, char *type)
{
    // if (world->player.needs_update) {
    // Process if the type is vertical
    if (type == "v")
    {
        // Check the current y position is at the top edge
        if (world->player.position.y + world->player.velocity.y < 0)
        {
            // Update vertical position
            world->player.position.y = 0;
        }
        // Check the current y position is at the bottom edge
        else if (world->player.position.y + world->player.velocity.y > MAP_HEIGHT - world->player.dimension.height)
        {
            // Update vertical position
            world->player.position.y = MAP_HEIGHT - world->player.dimension.height;
        }
        else
        { // Update vertical position
            world->player.position.y += world->player.velocity.y;
        }
        // world->player.previous_pos = world->player.position;
        // world->player.position.y += world->player.velocity.y;
    }
    // Process if the type is horizontal
    else if (type == "h")
    {
        // Check  the current x position is at the left edge

        if (world->player.position.x + world->player.velocity.x < 0)
        {
            // updating the horizontal position

            world->player.position.x = 0;
        }
        // Check  the current x position is at the right edge

        else if (world->player.position.x + world->player.velocity.x > MAP_WIDTH - world->player.dimension.width)
        {
            // updating the horizontal position
            world->player.position.x = MAP_WIDTH - world->player.dimension.width;
        }
        else
        {
            // updating the horizontal position
            world->player.previous_pos = world->player.position;
            world->player.position.x += world->player.velocity.x;
        }
    }
    // world->player.position.x += world->player.velocity.x;
    // world->player.position.y += world->player.velocity.y;

    // world->player.needs_render = true;
    // world->player.needs_update = false;
    // }
}

// Setting the value for aliens
void init_enemies(World *world)
{
    for (int i = 0, j = 0; i < NUM_ENEMIES; i++)
    {
        if (i < 10)
        {
            world->enemies[i].position.x =
                alien_initial_x + (HORIZONTAL_OFFSET * i);
            world->enemies[i].position.y =
                alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 5);
        }
        else if (i < 20)
        {
            world->enemies[i].position.x =
                alien_initial_x + (HORIZONTAL_OFFSET * (i % 10));
            world->enemies[i].position.y =
                alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 4);
        }
        else
        {
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
void render(World *world)
{
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

void clear(Entity entity)
{
    int width = entity.dimension.width;
    int height = entity.dimension.height;

    int x = entity.previous_pos.x;
    int oldX = x;
    int y = entity.previous_pos.y;

    for (int i = 0; i < (width * height); i++)
    {
        x++;
        if (i % width == 0)
        {
            y++;
            x = oldX;
        }
        clear_emulator_screen(x, y);
    }
}