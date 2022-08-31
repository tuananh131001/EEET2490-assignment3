#include "mbox.h"
#include "uart.h"

#define MAP_WIDTH 1920
#define MAP_HEIGHT 1080

typedef struct {
    float x, y;
} Position;
typedef struct {
    float width, height;
} Dimension;
/* ship, alien, bunker */

typedef struct {
    Position position;
    Dimension dimension;
} Entity;

typedef struct map {
    Entity player;
} World;

typedef struct {
    World world;
} Game;

void init_game(Game *world);
void init_map(World *world);
void init_player(Entity *player);
void render(World *world);