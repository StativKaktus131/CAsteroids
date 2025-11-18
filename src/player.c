#include "player.h"
#include "util.h"

float player_x, player_y, player_w, player_h;
float player_rot_degrees;

float acceleration_force = 800;
float friction = 200;

int accelerating = FALSE;
float speed = 0;
float max_speed = 300;
float vel_x = 0;
float vel_y = 0;


void set_player_to_screen_center()
{
	// set player size
	player_w = player_h = 32;

	player_x = WINDOW_WIDTH / 2 - player_w / 2;
	player_y = WINDOW_HEIGHT / 2 - player_h / 2;
}

void move_player(float dt)
{
	float rot_radians = player_rot_degrees * (PI / 180.0);

	if (accelerating)
	{
		vel_x += cosf(rot_radians) * acceleration_force * dt;
		vel_y += sinf(rot_radians) * acceleration_force * dt;
	}
	else
	{
		vel_x = move_toward(vel_x, 0.0, friction * dt);
		vel_y = move_toward(vel_y, 0.0, friction * dt);
	}

	float normal_speed = sqrt(vel_x * vel_x + vel_y * vel_y);
	if (normal_speed >= max_speed)
	{
		vel_x = vel_x / normal_speed * max_speed;
		vel_y = vel_y / normal_speed * max_speed;
	}

	player_x += vel_x * dt;
	player_y += vel_y * dt;


	// screen wrapping

	if (player_x < -player_w)
		player_x = WINDOW_WIDTH;
	else if (player_x > WINDOW_WIDTH)
		player_x = -player_w;

	if (player_y < -player_h)
		player_y = WINDOW_HEIGHT;
	else if (player_y > WINDOW_HEIGHT)
		player_y = -player_h;
}

void draw_player(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColorFloat(renderer, 1.0, 0.0, 0.0, SDL_ALPHA_OPAQUE_FLOAT);

	SDL_Vertex corners[4];
	float polar_corners[4][2] = { {0, 1}, {135, 1}, {180, 0.5}, {225, 1} };
	int indices[6] = {0, 1, 2, 2, 3, 0};

	float rot_radians = player_rot_degrees * (PI / 180.0);
	
	for (int i = 0; i < 4; i++)
	{
		float angle = polar_corners[i][0] * (PI / 180.0) + rot_radians;
		float r = polar_corners[i][1];

		float center_x = player_x + player_w / 2;
		float center_y = player_y + player_h / 2;

		float corner_x = center_x + cosf(angle) * r * (player_w / 2);
		float corner_y = center_y + sinf(angle) * r * (player_h / 2);

		corners[i] = (SDL_Vertex) { .position = {corner_x, corner_y}, .color = { 1.0, 1.0, 1.0 } };
	}

	SDL_RenderGeometry(renderer, NULL, corners, 4, indices, 6);

}

void rotate_player(int direction, float amount)
{
	player_rot_degrees += direction * amount;
}

void set_accelerating(int to)
{
	accelerating = to;
}

float get_player_x()
{
	return player_x;
}

float get_player_y()
{
	return player_y;
}

float get_player_w()
{
	return player_w;
}

float get_player_h()
{
	return player_h;
}

float get_player_rotation_radians()
{
	return player_rot_degrees * (PI / 180.0);
}