#ifndef COG_SHAPES_H
#define COG_SHAPES_H

#include "core/utils/cog_types.h"

#define MAX_POLYGON_VERTICES 64

typedef enum {
    SHAPE_CIRCLE,
    SHAPE_RECTANGLE,
    SHAPE_POLYGON
} ShapeType;

typedef struct {
    Vector2 center;
    float radius;
} Circle;

typedef struct {
    Vector2 position;
    float width;
    float height;
} Rectangle;

typedef struct {
    Vector2 vertices[MAX_POLYGON_VERTICES];
    int vertex_count;
} Polygon;

typedef struct {
    ShapeType type;
    union {
        Circle circle;
        Rectangle rectangle;
        Polygon polygon;
    };
} Shape;

// Shape creation functions
Shape shape_create_circle(Vector2 center, float radius);
Shape shape_create_rectangle(Vector2 position, float width, float height);
Shape shape_create_polygon(Vector2* vertices, int vertex_count);

// Shape manipulation functions
void shape_translate(Shape* shape, Vector2 translation);
void shape_scale(Shape* shape, float scale_factor);

// Collision detection functions
bool shape_collide(const Shape* shape1, const Shape* shape2);

// Utility functions
Vector2 shape_get_center(const Shape* shape);
float shape_get_area(const Shape* shape);

#endif // COG_SHAPES_H