#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#include "main.h"
#include "util.h"
#include "player.h"
#include "projectile.h"
#include "linkedlist.h"


// globals
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

// input
int left_down = FALSE, right_down = FALSE, up_down = FALSE;

uint64_t last_time;
float delta;

LinkedList* projectiles = NULL;




// app init method
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	// init player
	set_player_to_screen_center();

	last_time = SDL_GetTicks();
	projectiles = ll_create_empty();


	// SDL metadata
	SDL_SetAppMetadata("Asteroids", "1.0", "com.tetris");

	// try to initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// create window and renderer
	if (!SDL_CreateWindowAndRenderer("Asteroids", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
	{
		SDL_Log("Couldn't create window or renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

// event handling
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	switch (event->type)
	{
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
			break;


		case SDL_EVENT_KEY_DOWN:
			switch (event->key.scancode)
			{
				case SDL_SCANCODE_LEFT: left_down = TRUE; break;
				case SDL_SCANCODE_RIGHT: right_down = TRUE; break;
				case SDL_SCANCODE_UP: up_down = TRUE; break;
				case SDL_SCANCODE_SPACE: shoot(); break;
				case SDL_SCANCODE_ESCAPE: exit(0); break;
			}
			break;

		case SDL_EVENT_KEY_UP:
			switch (event->key.scancode)
			{
				case SDL_SCANCODE_LEFT: left_down = FALSE; break;
				case SDL_SCANCODE_RIGHT: right_down = FALSE; break;
				case SDL_SCANCODE_UP: up_down = FALSE; break;
			}
			break;
	}

	return SDL_APP_CONTINUE;
}



// once per frame
SDL_AppResult SDL_AppIterate(void* appstate)
{
	uint64_t now = SDL_GetTicks();
	delta = (now - last_time) / 1000.0;
	last_time = now;

	SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE);

	// clear window
	SDL_RenderClear(renderer);

	rotate_player(-left_down + right_down, delta * 300.0);
	set_accelerating(up_down);

	ll_for_each(projectiles, loop_draw_projectiles);

	printf("SIZE: %d\n", ll_size(projectiles));

	move_player(delta);
	draw_player(renderer);

	// present
	SDL_RenderPresent(renderer);
	
	return SDL_APP_CONTINUE;
}

// sdl cleans up window and renderer
void SDL_AppQuit(void* appstate, SDL_AppResult result) 
{
	ll_free_deep(projectiles);
}


void shoot()
{
	float speed = 60.f;
	float vel_x = cosf(get_player_rotation_radians()) * speed;
	float vel_y = sinf(get_player_rotation_radians()) * speed;

	float center_x = get_player_x() + get_player_w() / 2;
	float center_y = get_player_y() + get_player_h() / 2;

	center_x += cosf(get_player_rotation_radians()) * get_player_w() / 2.f;
	center_y += sinf(get_player_rotation_radians()) * get_player_h() / 2.f;

	projectile_t* projectile = create_projectile(center_x, center_y, vel_x, vel_y);

	ll_push_back(&projectiles, (void*) projectile);
}

void loop_draw_projectiles(int i, void* element)
{
	projectile_t* projectile = (projectile_t*) element;

	if (projectile->x < 0 || projectile->y < 0 || projectile->x + projectile->w >= WINDOW_WIDTH || projectile->y + projectile->h >= WINDOW_HEIGHT)
	{
		int idx = ll_indexof(projectiles, element);
		ll_remove_at(&projectiles, idx);
	}

	update_projectile(projectile, delta);
	draw_projectile(renderer, projectile);
}