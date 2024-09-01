#ifndef MATH_H
#define MATH_H

#include "types.h"

// Vector operations
Vector2 vector2_add(Vector2 v1, Vector2 v2);
Vector2 vector2_subtract(Vector2 v1, Vector2 v2);
Vector2 vector2_multiply(Vector2 v, float scalar);
float vector2_dot(Vector2 v1, Vector2 v2);
float vector2_length(Vector2 v);
Vector2 vector2_normalize(Vector2 v);

// Angle operations
float deg_to_rad(float degrees);
float rad_to_deg(float radians);

// Interpolation
float lerp(float a, float b, float t);
Vector2 vector2_lerp(Vector2 v1, Vector2 v2, float t);

// Random number generation
void set_random_seed(unsigned int seed);
int random_int(int min, int max);
float random_float(float min, float max);

// Other utility functions
float distance(float x1, float y1, float x2, float y2);
bool rect_contains_point(Rectangle rect, Vector2 point);

#endif // MATH_H