#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

// Basic vector types
typedef struct {
    float x, y;
} Vector2;

typedef struct {
    float x, y, z;
} Vector3;

// Color
typedef struct {
    uint8_t r, g, b, a;
} Color;

// Rectangle
typedef struct {
    float x, y, width, height;
} Rectangle;

// Constants
#define PI 3.14159265358979323846f
#define TAU (2.0f * PI)
#define DEG_TO_RAD (PI / 180.0f)
#define RAD_TO_DEG (180.0f / PI)

// Utility macros
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#endif // TYPES_H