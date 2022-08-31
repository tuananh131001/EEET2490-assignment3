#include "mbox.h"
#include "uart.h"
#define NUM_PAWNS 30
#define NUM_ENEMIES (NUM_PAWNS)

#define alien_initial_y 162
#define alien_initial_x 675
#define HORIZONTAL_OFFSET 61
#define VERTICAL_OFFSET 61


#define MAP_WIDTH 1920
#define MAP_HEIGHT 1080

typedef struct {
    float x, y;
} Position;
typedef struct {
    float width, height;
} Dimension;

typedef enum { PLAYER = 1, PAWN = 2 } Type;
/* ship, alien, bunker */
typedef struct {
    Type type;
    
    Position position;
    Dimension dimension;
} Entity;

typedef struct map {
    Entity player;
    Entity enemies[NUM_ENEMIES];
} World;

typedef struct {
    World world;
} Game;

void init_game(Game *world);
void init_map(World *world);

void init_player(Entity *player);
void init_enemies(World *world);

void render(World *world); // framebf.c