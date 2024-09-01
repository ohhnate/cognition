#include "math.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

Vector2 vector2_add(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x + v2.x, v1.y + v2.y};
}

Vector2 vector2_subtract(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x - v2.x, v1.y - v2.y};
}

Vector2 vector2_multiply(Vector2 v, float scalar) {
    return (Vector2){v.x * scalar, v.y * scalar};
}

float vector2_dot(Vector2 v1, Vector2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float vector2_length(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

Vector2 vector2_normalize(Vector2 v) {
    float length = vector2_length(v);
    if (length != 0) {
        return vector2_multiply(v, 1.0f / length);
    }
    return v;
}

float deg_to_rad(float degrees) {
    return degrees * DEG_TO_RAD;
}

float rad_to_deg(float radians) {
    return radians * RAD_TO_DEG;
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

Vector2 vector2_lerp(Vector2 v1, Vector2 v2, float t) {
    return (Vector2){
        lerp(v1.x, v2.x, t),
        lerp(v1.y, v2.y, t)
    };
}

void set_random_seed(unsigned int seed) {
    srand(seed);
}

int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

float random_float(float min, float max) {
    return min + (max - min) * ((float)rand() / RAND_MAX);
}

float distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrtf(dx * dx + dy * dy);
}

bool rect_contains_point(Rectangle rect, Vector2 point) {
    return point.x >= rect.x && point.x <= rect.x + rect.width &&
           point.y >= rect.y && point.y <= rect.y + rect.height;
}