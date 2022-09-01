#include "mbox.h"
#include "uart.h"
#include <stdbool.h>

#define NUM_PAWNS 30
#define NUM_ENEMIES (NUM_PAWNS)

#define alien_initial_y 162
#define alien_initial_x 675
#define HORIZONTAL_OFFSET 61
#define VERTICAL_OFFSET 61

#define MAP_WIDTH 1024
#define MAP_HEIGHT 768


typedef union {
    int current_health;
    int player_health;
} Health;

typedef struct {
    float x, y;
} Position;

typedef struct {
    float width, height;
} Dimension;

typedef struct { float x, y; } Velocity;



typedef enum { PLAYER = 1, PAWN = 2 } Type;
/* ship, alien, bunker */
typedef struct {
    Velocity velocity;
    Dimension dimension;
    Position position;
    Position previous_pos;
    bool needs_render;
    Type type;
    bool needs_update;
    bool enabled;
} Entity;

typedef struct map {
    Entity player;
    Entity enemies[NUM_ENEMIES];
} World;

typedef struct {
    World world;
} Game;

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    STOP,
    RESET_VERTICAL,
    RESET_HORIZONTAL
} Direction;

void init_game(Game *world);
void init_map(World *world);

void init_player(Entity *player);
void init_enemies(World *world);

void render(World *world); // framebf.c
void move_player(World *world);

void clear(Entity entity);
void *updateRender(void *arg);
void *updateWorld(void *arg);
