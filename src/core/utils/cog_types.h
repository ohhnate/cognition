#ifndef COG_TYPES_H
#define COG_TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    float x, y, z, w;
} Vector4;

typedef struct {
    uint8_t r, g, b, a;
} Color;

#define MAX_ENTITIES 100000
#define MAX_COMPONENTS 256
#define MAX_SYSTEMS 256

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#endif // COG_TYPES_H