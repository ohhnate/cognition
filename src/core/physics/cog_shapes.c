#include "cog_shapes.h"
#include "core/utils/cog_math.h"
#include <string.h>
#include <math.h>
#include <float.h>

Shape shape_create_circle(Vector2 center, float radius) {
    Shape shape;
    shape.type = SHAPE_CIRCLE;
    shape.circle.center = center;
    shape.circle.radius = radius;
    return shape;
}

Shape shape_create_rectangle(Vector2 position, float width, float height) {
    Shape shape;
    shape.type = SHAPE_RECTANGLE;
    shape.rectangle.position = position;
    shape.rectangle.width = width;
    shape.rectangle.height = height;
    return shape;
}

Shape shape_create_polygon(Vector2* vertices, int vertex_count) {
    Shape shape;
    shape.type = SHAPE_POLYGON;
    shape.polygon.vertex_count = MIN(vertex_count, MAX_POLYGON_VERTICES);
    memcpy(shape.polygon.vertices, vertices, shape.polygon.vertex_count * sizeof(Vector2));
    return shape;
}

void shape_translate(Shape* shape, Vector2 translation) {
    switch (shape->type) {
        case SHAPE_CIRCLE:
            shape->circle.center = vector2_add(shape->circle.center, translation);
            break;
        case SHAPE_RECTANGLE:
            shape->rectangle.position = vector2_add(shape->rectangle.position, translation);
            break;
        case SHAPE_POLYGON:
            for (int i = 0; i < shape->polygon.vertex_count; i++) {
                shape->polygon.vertices[i] = vector2_add(shape->polygon.vertices[i], translation);
            }
            break;
    }
}

void shape_scale(Shape* shape, float scale_factor) {
    switch (shape->type) {
        case SHAPE_CIRCLE:
            shape->circle.radius *= scale_factor;
            break;
        case SHAPE_RECTANGLE:
            shape->rectangle.width *= scale_factor;
            shape->rectangle.height *= scale_factor;
            break;
        case SHAPE_POLYGON:
            {
                Vector2 center = shape_get_center(shape);
                for (int i = 0; i < shape->polygon.vertex_count; i++) {
                    Vector2 to_vertex = vector2_subtract(shape->polygon.vertices[i], center);
                    to_vertex = vector2_multiply(to_vertex, scale_factor);
                    shape->polygon.vertices[i] = vector2_add(center, to_vertex);
                }
            }
            break;
    }
}

void shape_rotate(Shape* shape, float angle) {
    switch (shape->type) {
        case SHAPE_CIRCLE:
            // Circles don't need rotation
            break;
        case SHAPE_RECTANGLE:
            {
                Vector2 center = {
                    shape->rectangle.position.x + shape->rectangle.width / 2,
                    shape->rectangle.position.y + shape->rectangle.height / 2
                };
                shape->type = SHAPE_POLYGON;
                shape->polygon.vertex_count = 4;
                float half_width = shape->rectangle.width / 2;
                float half_height = shape->rectangle.height / 2;
                shape->polygon.vertices[0] = (Vector2){-half_width, -half_height};
                shape->polygon.vertices[1] = (Vector2){ half_width, -half_height};
                shape->polygon.vertices[2] = (Vector2){ half_width,  half_height};
                shape->polygon.vertices[3] = (Vector2){-half_width,  half_height};
                
                for (int i = 0; i < 4; i++) {
                    Vector2 rotated = vector2_rotate(shape->polygon.vertices[i], angle);
                    shape->polygon.vertices[i] = vector2_add(rotated, center);
                }
            }
            break;
        case SHAPE_POLYGON:
            {
                Vector2 center = shape_get_center(shape);
                for (int i = 0; i < shape->polygon.vertex_count; i++) {
                    Vector2 to_center = vector2_subtract(shape->polygon.vertices[i], center);
                    Vector2 rotated = vector2_rotate(to_center, angle);
                    shape->polygon.vertices[i] = vector2_add(rotated, center);
                }
            }
            break;
    }
}

static void project_shape(const Shape* shape, Vector2 axis, float* min, float* max) {
    *min = FLT_MAX;
    *max = -FLT_MAX;
    
    switch (shape->type) {
        case SHAPE_CIRCLE:
            {
                float projection = vector2_dot(shape->circle.center, axis);
                *min = projection - shape->circle.radius;
                *max = projection + shape->circle.radius;
            }
            break;
        case SHAPE_RECTANGLE:
        case SHAPE_POLYGON:
            {
                int vertex_count = (shape->type == SHAPE_RECTANGLE) ? 4 : shape->polygon.vertex_count;
                const Vector2* vertices = (shape->type == SHAPE_RECTANGLE) ? 
                    (Vector2[]){
                        shape->rectangle.position,
                        {shape->rectangle.position.x + shape->rectangle.width, shape->rectangle.position.y},
                        {shape->rectangle.position.x + shape->rectangle.width, shape->rectangle.position.y + shape->rectangle.height},
                        {shape->rectangle.position.x, shape->rectangle.position.y + shape->rectangle.height}
                    } : shape->polygon.vertices;
                
                for (int i = 0; i < vertex_count; i++) {
                    float projection = vector2_dot(vertices[i], axis);
                    *min = MIN(*min, projection);
                    *max = MAX(*max, projection);
                }
            }
            break;
    }
}

static bool check_axis_separation(const Shape* shape1, const Shape* shape2, Vector2 axis) {
    float min1, max1, min2, max2;
    project_shape(shape1, axis, &min1, &max1);
    project_shape(shape2, axis, &min2, &max2);
    return (min1 > max2) || (min2 > max1);
}

bool shape_collide(const Shape* shape1, const Shape* shape2) {
    if (shape1->type == SHAPE_CIRCLE && shape2->type == SHAPE_CIRCLE) {
        float distance = vector2_distance(shape1->circle.center, shape2->circle.center);
        return distance < (shape1->circle.radius + shape2->circle.radius);
    }

    Vector2 axes[MAX_POLYGON_VERTICES * 2];
    int axis_count = 0;

    // Generate axes for shape1
    if (shape1->type == SHAPE_POLYGON || shape1->type == SHAPE_RECTANGLE) {
        int vertex_count = (shape1->type == SHAPE_RECTANGLE) ? 4 : shape1->polygon.vertex_count;
        const Vector2* vertices = (shape1->type == SHAPE_RECTANGLE) ? 
            (Vector2[]){
                shape1->rectangle.position,
                {shape1->rectangle.position.x + shape1->rectangle.width, shape1->rectangle.position.y},
                {shape1->rectangle.position.x + shape1->rectangle.width, shape1->rectangle.position.y + shape1->rectangle.height},
                {shape1->rectangle.position.x, shape1->rectangle.position.y + shape1->rectangle.height}
            } : shape1->polygon.vertices;
        
        for (int i = 0; i < vertex_count; i++) {
            int j = (i + 1) % vertex_count;
            Vector2 edge = vector2_subtract(vertices[j], vertices[i]);
            axes[axis_count++] = vector2_normalize((Vector2){-edge.y, edge.x});
        }
    }

    // Generate axes for shape2
    if (shape2->type == SHAPE_POLYGON || shape2->type == SHAPE_RECTANGLE) {
        int vertex_count = (shape2->type == SHAPE_RECTANGLE) ? 4 : shape2->polygon.vertex_count;
        const Vector2* vertices = (shape2->type == SHAPE_RECTANGLE) ? 
            (Vector2[]){
                shape2->rectangle.position,
                {shape2->rectangle.position.x + shape2->rectangle.width, shape2->rectangle.position.y},
                {shape2->rectangle.position.x + shape2->rectangle.width, shape2->rectangle.position.y + shape2->rectangle.height},
                {shape2->rectangle.position.x, shape2->rectangle.position.y + shape2->rectangle.height}
            } : shape2->polygon.vertices;
        
        for (int i = 0; i < vertex_count; i++) {
            int j = (i + 1) % vertex_count;
            Vector2 edge = vector2_subtract(vertices[j], vertices[i]);
            axes[axis_count++] = vector2_normalize((Vector2){-edge.y, edge.x});
        }
    }

    // Check for separation along all axes
    for (int i = 0; i < axis_count; i++) {
        if (check_axis_separation(shape1, shape2, axes[i])) {
            return false;
        }
    }

    // If we reach here, no separation was found
    return true;
}

Vector2 shape_get_center(const Shape* shape) {
    switch (shape->type) {
        case SHAPE_CIRCLE:
            return shape->circle.center;
        case SHAPE_RECTANGLE:
            return (Vector2){
                shape->rectangle.position.x + shape->rectangle.width / 2,
                shape->rectangle.position.y + shape->rectangle.height / 2
            };
        case SHAPE_POLYGON:
            {
                Vector2 center = {0, 0};
                for (int i = 0; i < shape->polygon.vertex_count; i++) {
                    center = vector2_add(center, shape->polygon.vertices[i]);
                }
                return vector2_divide(center, (float)shape->polygon.vertex_count);
            }
    }
    return (Vector2){0, 0}; // Should never reach here
}

float shape_get_area(const Shape* shape) {
    switch (shape->type) {
        case SHAPE_CIRCLE:
            return PI * shape->circle.radius * shape->circle.radius;
        case SHAPE_RECTANGLE:
            return shape->rectangle.width * shape->rectangle.height;
        case SHAPE_POLYGON:
            {
                float area = 0;
                for (int i = 0; i < shape->polygon.vertex_count; i++) {
                    int j = (i + 1) % shape->polygon.vertex_count;
                    area += shape->polygon.vertices[i].x * shape->polygon.vertices[j].y;
                    area -= shape->polygon.vertices[j].x * shape->polygon.vertices[i].y;
                }
                return fabsf(area) / 2;
            }
    }
    return 0; // Should never reach here
}