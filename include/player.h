#include "util.h"


void set_player_to_screen_center();
void move_player(float dt);
void draw_player(SDL_Renderer* renderer);
void rotate_player(int direction, float amount);
void set_accelerating(int to);

float get_player_x();
float get_player_y();
float get_player_w();
float get_player_h();
float get_player_rotation_radians();