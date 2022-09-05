#include <stdbool.h>

#include "mbox.h"
#include "uart.h"

#define NUM_PAWNS 30
#define PAWN_POINTS 5
#define PAWN_HEALTH 1

#define NUM_QUEENS 10
#define QUEEN_POINTS 100
#define QUEEN_HEALTH 3

#define NUM_ENEMIES (NUM_PAWNS + NUM_QUEENS)

#define NUM_BUNKERS 3
#define BUNKER_HEALTH 10

#define alien_initial_y 0
#define alien_initial_x 200
#define HORIZONTAL_OFFSET 60
#define VERTICAL_OFFSET 0

#define PLAYER_SPEED 15
#define VERTICAL_SPEED 6
#define HORIZONTAL_SPEED 4

#define TOP_MAX 162
// #define TOP_MAX 60
#define BOTTOM_MAX 917
#define MAX_BULLETS 10
#define BULLET_VELOCITY 15

#define MAP_WIDTH 1024
#define MAP_HEIGHT 768

#define MAX_SHOOTERS 10

#define SCORE_ORIGINX 500
#define SCORE_ORIGINY 720
#define SHIFT 32

#define BAR_ORIGINX 1150
#define BAR_ORIGINY 180

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

typedef struct {
    float x, y;
} Velocity;

typedef struct {
    int score;
    bool needsUpdate;
    bool needsRender;
} Score;

typedef enum { PLAYER = 1, PAWN = 2, QUEEN = 3, BUNKER = 4 } Type;


typedef struct {
    Position position;
    Position previous_pos;
    Velocity velocity;
    Dimension dimension;
    bool needs_update;
    bool needs_render;
    bool needs_clear;
    bool active;
} Missile;
/* ship, alien, bunker */
typedef struct {
    Velocity velocity;
    Dimension dimension;
    Position position;
    Position previous_pos;
    Missile projectile[MAX_BULLETS];
    Type type;
    Health health;
    bool needs_update;
    bool needs_render;
    bool needs_clear;
    bool combat_update;
    bool enabled;
} Entity;

typedef struct map {
    Entity player;
    Entity enemies[NUM_ENEMIES];
    Entity bunkers[NUM_BUNKERS];
    int shooters[MAX_SHOOTERS];
    Entity life;
    int left_most_enemies[6];
    int right_most_enemies[6];
    Score playerScore;

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
void init_life(Entity *life);
void render(World *world);  // framebf.c
void move_player(World *world);
void update_player_position(World *world);
void move_entity(Entity *entity, Direction direction);
void drawEntity(Entity entity);

void entity_shoot(Entity *entity, Direction direction);
Missile *create_bullet(Entity owner);
void move_bullet(Missile *projectile, Direction direction);
void *memcpy(void *dest, const void *src, unsigned long n);
void draw_projectile(Type type, Position position, Dimension dimension);
void clear_projectile(Position position, Dimension dimension);
void render_health(World *world);

void render_score(World *world);

void update_combat_system(World *world);
void update_collision_system(World *world);
void clear(Entity entity);
void update_score(World *world, Type type);
void enemy_shoot(World *world);
int rand(void);