#include <SDL3/SDL.h>
#include <math.h>
#include <stdio.h>


#define TRUE 1
#define FALSE 0

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define PI 3.1415926


float clamp(float var, float min, float max);
float move_toward(float var, float target, float step);