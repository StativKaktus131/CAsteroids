#include <SDL3/SDL.h>
#include <stdlib.h>

typedef struct Projectile
{
    float x, y;
    float w, h;
    float vel_x, vel_y;
} projectile_t;


projectile_t* create_projectile(float x, float y, float vel_x, float vel_y);
void free_projectile(projectile_t* p);

void update_projectile(projectile_t* projectile, float dt);
void draw_projectile(SDL_Renderer* renderer, projectile_t* projectile);