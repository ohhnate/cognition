#ifndef COG_MATH_H
#define COG_MATH_H

#include "cog_types.h"
#include <math.h>

#define PI 3.14159265358979323846f
#define TAU (2.0f * PI)
#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)

typedef Vector4 Quaternion;

typedef struct {
    float m[4][4];
} Matrix4;

// Vector2 operations
Vector2 vector2_add(Vector2 v1, Vector2 v2);
Vector2 vector2_subtract(Vector2 v1, Vector2 v2);
Vector2 vector2_multiply(Vector2 v, float scalar);
Vector2 vector2_divide(Vector2 v, float scalar);
float vector2_dot(Vector2 v1, Vector2 v2);
float vector2_length(Vector2 v);
Vector2 vector2_normalize(Vector2 v);
float vector2_distance(Vector2 v1, Vector2 v2);
Vector2 vector2_lerp(Vector2 v1, Vector2 v2, float t);
Vector2 vector2_rotate(Vector2 v, float angle);

// Vector3 operations
Vector3 vector3_add(Vector3 v1, Vector3 v2);
Vector3 vector3_subtract(Vector3 v1, Vector3 v2);
Vector3 vector3_multiply(Vector3 v, float scalar);
Vector3 vector3_divide(Vector3 v, float scalar);
float vector3_dot(Vector3 v1, Vector3 v2);
Vector3 vector3_cross(Vector3 v1, Vector3 v2);
float vector3_length(Vector3 v);
Vector3 vector3_normalize(Vector3 v);
float vector3_distance(Vector3 v1, Vector3 v2);
Vector3 vector3_lerp(Vector3 v1, Vector3 v2, float t);

// Matrix4 operations
Matrix4 matrix4_identity(void);
Matrix4 matrix4_multiply(Matrix4 m1, Matrix4 m2);
Vector4 matrix4_multiply_vector4(Matrix4 m, Vector4 v);
Matrix4 matrix4_translate(Matrix4 m, Vector3 v);
Matrix4 matrix4_scale(Matrix4 m, Vector3 v);
Matrix4 matrix4_rotate_x(Matrix4 m, float angle);
Matrix4 matrix4_rotate_y(Matrix4 m, float angle);
Matrix4 matrix4_rotate_z(Matrix4 m, float angle);
Matrix4 matrix4_perspective(float fov, float aspect, float near, float far);
Matrix4 matrix4_orthographic(float left, float right, float bottom, float top, float near, float far);
Matrix4 matrix4_look_at(Vector3 eye, Vector3 center, Vector3 up);

// Quaternion operations
Quaternion quaternion_identity(void);
Quaternion quaternion_from_axis_angle(Vector3 axis, float angle);
Quaternion quaternion_multiply(Quaternion q1, Quaternion q2);
Vector3 quaternion_rotate_vector3(Quaternion q, Vector3 v);
Quaternion quaternion_slerp(Quaternion q1, Quaternion q2, float t);
Matrix4 quaternion_to_matrix4(Quaternion q);

// Utility functions
float lerp(float a, float b, float t);
float clamp(float value, float min, float max);
float deg_to_rad(float degrees);
float rad_to_deg(float radians);
float sign(float x);
float step(float edge, float x);
float smoothstep(float edge0, float edge1, float x);
float map(float value, float start1, float stop1, float start2, float stop2);

#endif // COG_MATH_H