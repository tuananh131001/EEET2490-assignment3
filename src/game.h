#include <stdbool.h>

#include "mbox.h"
#include "uart.h"

#define NUM_PAWNS 20
#define PAWN_POINTS 10
#define PAWN_HEALTH 1

#define NUM_KNIGHTS 10
#define KNIGHT_POINTS 30
#define KNIGHT_HEALTH 2

#define NUM_QUEENS 10
#define QUEEN_POINTS 50
#define QUEEN_HEALTH 3


#define NUM_BUNKERS 3
#define BUNKER_HEALTH 10

#define alien_initial_y 0
#define alien_initial_x 200
#define HORIZONTAL_OFFSET 61
#define VERTICAL_OFFSET 61

#define ENEMIES_VERTICAL_MAX (1080 - 262 - VERTICAL_OFFSET)
#define NUM_ENEMIES  (NUM_PAWNS + NUM_QUEENS + NUM_KNIGHTS)

#define PLAYER_SPEED 15
#define VERTICAL_SPEED 30
#define HORIZONTAL_SPEED 1

#define LEFT_MAX 50
#define RIGHT_MAX (MAP_WIDTH - 50) 
#define TOP_MAX 20
#define BOTTOM_MAX 917
#define MAX_BULLETS 1
#define BULLET_VELOCITY 15

#define MAP_WIDTH 1024
#define MAP_HEIGHT 768

#define MAX_SHOOTERS 10

#define SCORE_ORIGINX 850
#define SCORE_ORIGINY 5
#define SHIFT 35

#define BAR_ORIGINX 1150
#define BAR_ORIGINY 180

static bool travel_right = true;

bool quitGame;
bool restartGame;
bool pauseGame;
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

typedef enum { PLAYER = 1, PAWN = 2, QUEEN = 3, BUNKER = 4,KNIGHT = 5} Type;

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
typedef struct {
    int game_menu_option;
    bool on_gameMenu_menu;
} GameMenu;

typedef struct {
    bool game_start_menu;
    bool on_game_menu;
} MainMenu;

typedef struct map {
    Entity player;
    Entity bunkers[NUM_BUNKERS];
    Entity enemies[NUM_ENEMIES];
    int shooters[MAX_SHOOTERS];
    int left_most_enemies[6];
    int right_most_enemies[6];
    int enemies_alive;
    bool game_over;
    Score playerScore;
    Entity life;
    GameMenu game_menu;

} World;

typedef struct {
    World world;
    bool game_win;
    bool game_over;
    bool game_start;
    MainMenu main_menu;
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
// void move_player(World *world);
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

void render_score(int num,int x, int y);

void update_combat_system(World *world);
void update_collision_system(World *world);
void clear(Entity entity);
void update_score(World *world, Type type);
void enemy_shoot(World *world);
int rand(void);

void update_AI_system(World *world);
bool enemies_at_bottom(World *world);
void update_shooters(World *world, int index);
void drawGameMenu(World *game);
void drawMainMenu(Game *game);
void show_game_menu(World *world);
void show_main_menu(Game *game) ;
void drawScore(World *world,char *type);
void init_bunkers(Entity bunkers[]);
void init_playerScore(Score *playerScore);
void endScreen(bool won, World *world) ;
void restart_game(Game *world);
void drawSpaceShip(Entity entity, World *world);
