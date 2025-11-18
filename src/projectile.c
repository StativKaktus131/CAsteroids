#include "projectile.h"
#include <stdio.h>


projectile_t* create_projectile(float x, float y, float vel_x, float vel_y)
{
    projectile_t* ret = (projectile_t*) malloc(sizeof(projectile_t));
    
    ret->x = x;
    ret->y = y;

    ret->w = ret->h = 2;

    ret->vel_x = vel_x;
    ret->vel_y = vel_y;

    return ret;
}

void free_projectile(projectile_t* p)
{
    free(p);
}


void update_projectile(projectile_t* projectile, float dt)
{
    projectile->x += projectile->vel_x * dt;
    projectile->y += projectile->vel_y * dt;
}


void draw_projectile(SDL_Renderer* renderer, projectile_t* projectile)
{
    SDL_SetRenderDrawColorFloat(renderer, 1.0, 0.0, 0.0, SDL_ALPHA_OPAQUE_FLOAT);

    SDL_FRect rect = { .x = projectile->x, .y = projectile->y, .w = projectile->w, .h = projectile->h };

    SDL_RenderFillRect(renderer, &rect);
}