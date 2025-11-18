#include "util.h"

float clamp(float var, float min, float max)
{
    return var < min ? min : var > max ? max : var;
}

float absf(float f) { return sqrtf(f*f); }

float move_toward(float var, float target, float step)
{
    int direction = (int) ((target - var) / absf(target - var));
    return var + direction * (clamp(step, 0, absf(target - var)));
}