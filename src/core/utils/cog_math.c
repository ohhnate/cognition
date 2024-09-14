#include "cog_math.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

// Vector2 operations
Vector2 vector2_add(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x + v2.x, v1.y + v2.y};
}

Vector2 vector2_subtract(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x - v2.x, v1.y - v2.y};
}

Vector2 vector2_multiply(Vector2 v, float scalar) {
    return (Vector2){v.x * scalar, v.y * scalar};
}

Vector2 vector2_divide(Vector2 v, float scalar) {
    return (Vector2){v.x / scalar, v.y / scalar};
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
        return vector2_divide(v, length);
    }
    return v;
}

float vector2_distance(Vector2 v1, Vector2 v2) {
    return vector2_length(vector2_subtract(v2, v1));
}

Vector2 vector2_lerp(Vector2 v1, Vector2 v2, float t) {
    return (Vector2){
        lerp(v1.x, v2.x, t),
        lerp(v1.y, v2.y, t)
    };
}

Vector2 vector2_rotate(Vector2 v, float angle) {
    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);
    return (Vector2){
        v.x * cos_angle - v.y * sin_angle,
        v.x * sin_angle + v.y * cos_angle
    };
}

// Vector3 operations
Vector3 vector3_add(Vector3 v1, Vector3 v2) {
    return (Vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector3 vector3_subtract(Vector3 v1, Vector3 v2) {
    return (Vector3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vector3 vector3_multiply(Vector3 v, float scalar) {
    return (Vector3){v.x * scalar, v.y * scalar, v.z * scalar};
}

Vector3 vector3_divide(Vector3 v, float scalar) {
    return (Vector3){v.x / scalar, v.y / scalar, v.z / scalar};
}

float vector3_dot(Vector3 v1, Vector3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 vector3_cross(Vector3 v1, Vector3 v2) {
    return (Vector3){
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

float vector3_length(Vector3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 vector3_normalize(Vector3 v) {
    float length = vector3_length(v);
    if (length != 0) {
        return vector3_divide(v, length);
    }
    return v;
}

float vector3_distance(Vector3 v1, Vector3 v2) {
    return vector3_length(vector3_subtract(v2, v1));
}

Vector3 vector3_lerp(Vector3 v1, Vector3 v2, float t) {
    return (Vector3){
        lerp(v1.x, v2.x, t),
        lerp(v1.y, v2.y, t),
        lerp(v1.z, v2.z, t)
    };
}

// Matrix4 operations
Matrix4 matrix4_identity(void) {
    Matrix4 m = {0};
    m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = 1.0f;
    return m;
}

Matrix4 matrix4_multiply(Matrix4 m1, Matrix4 m2) {
    Matrix4 result = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += m1.m[i][k] * m2.m[k][j];
            }
        }
    }
    return result;
}

Vector4 matrix4_multiply_vector4(Matrix4 m, Vector4 v) {
    Vector4 result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    return result;
}

Matrix4 matrix4_translate(Matrix4 m, Vector3 v) {
    Matrix4 translation = matrix4_identity();
    translation.m[0][3] = v.x;
    translation.m[1][3] = v.y;
    translation.m[2][3] = v.z;
    return matrix4_multiply(m, translation);
}

Matrix4 matrix4_scale(Matrix4 m, Vector3 v) {
    Matrix4 scale = matrix4_identity();
    scale.m[0][0] = v.x;
    scale.m[1][1] = v.y;
    scale.m[2][2] = v.z;
    return matrix4_multiply(m, scale);
}

Matrix4 matrix4_rotate_x(Matrix4 m, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    Matrix4 rotation = matrix4_identity();
    rotation.m[1][1] = c;
    rotation.m[1][2] = -s;
    rotation.m[2][1] = s;
    rotation.m[2][2] = c;
    return matrix4_multiply(m, rotation);
}

Matrix4 matrix4_rotate_y(Matrix4 m, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    Matrix4 rotation = matrix4_identity();
    rotation.m[0][0] = c;
    rotation.m[0][2] = s;
    rotation.m[2][0] = -s;
    rotation.m[2][2] = c;
    return matrix4_multiply(m, rotation);
}

Matrix4 matrix4_rotate_z(Matrix4 m, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    Matrix4 rotation = matrix4_identity();
    rotation.m[0][0] = c;
    rotation.m[0][1] = -s;
    rotation.m[1][0] = s;
    rotation.m[1][1] = c;
    return matrix4_multiply(m, rotation);
}

Matrix4 matrix4_perspective(float fov, float aspect, float near, float far) {
    Matrix4 m = {0};
    float tan_half_fov = tanf(fov / 2.0f);
    m.m[0][0] = 1.0f / (aspect * tan_half_fov);
    m.m[1][1] = 1.0f / tan_half_fov;
    m.m[2][2] = -(far + near) / (far - near);
    m.m[2][3] = -1.0f;
    m.m[3][2] = -(2.0f * far * near) / (far - near);
    return m;
}

Matrix4 matrix4_orthographic(float left, float right, float bottom, float top, float near, float far) {
    Matrix4 m = matrix4_identity();
    m.m[0][0] = 2.0f / (right - left);
    m.m[1][1] = 2.0f / (top - bottom);
    m.m[2][2] = -2.0f / (far - near);
    m.m[0][3] = -(right + left) / (right - left);
    m.m[1][3] = -(top + bottom) / (top - bottom);
    m.m[2][3] = -(far + near) / (far - near);
    return m;
}

Matrix4 matrix4_look_at(Vector3 eye, Vector3 center, Vector3 up) {
    Vector3 f = vector3_normalize(vector3_subtract(center, eye));
    Vector3 s = vector3_normalize(vector3_cross(f, up));
    Vector3 u = vector3_cross(s, f);

    Matrix4 m = matrix4_identity();
    m.m[0][0] = s.x;
    m.m[0][1] = s.y;
    m.m[0][2] = s.z;
    m.m[1][0] = u.x;
    m.m[1][1] = u.y;
    m.m[1][2] = u.z;
    m.m[2][0] = -f.x;
    m.m[2][1] = -f.y;
    m.m[2][2] = -f.z;
    m.m[0][3] = -vector3_dot(s, eye);
    m.m[1][3] = -vector3_dot(u, eye);
    m.m[2][3] = vector3_dot(f, eye);
    return m;
}

float distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrtf(dx * dx + dy * dy);
}

// Quaternion operations
Quaternion quaternion_identity(void) {
    return (Quaternion){0, 0, 0, 1};
}

Quaternion quaternion_from_axis_angle(Vector3 axis, float angle) {
    float half_angle = angle * 0.5f;
    float s = sinf(half_angle);
    Vector3 normalized_axis = vector3_normalize(axis);
    return (Quaternion){
        normalized_axis.x * s,
        normalized_axis.y * s,
        normalized_axis.z * s,
        cosf(half_angle)
    };
}

Quaternion quaternion_multiply(Quaternion q1, Quaternion q2) {
    return (Quaternion){
        q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
        q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
        q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
        q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
    };
}

Vector3 quaternion_rotate_vector3(Quaternion q, Vector3 v) {
    Quaternion q_conj = {-q.x, -q.y, -q.z, q.w};
    Quaternion q_vec = {v.x, v.y, v.z, 0};
    Quaternion q_result = quaternion_multiply(quaternion_multiply(q, q_vec), q_conj);
    return (Vector3){q_result.x, q_result.y, q_result.z};
}

Quaternion quaternion_slerp(Quaternion q1, Quaternion q2, float t) {
    float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
    if (dot < 0) {
        q2.x = -q2.x; q2.y = -q2.y; q2.z = -q2.z; q2.w = -q2.w;
        dot = -dot;
    }
    if (dot > 0.9995f) {
        return (Quaternion){
            lerp(q1.x, q2.x, t),
            lerp(q1.y, q2.y, t),
            lerp(q1.z, q2.z, t),
            lerp(q1.w, q2.w, t)
        };
    }
    float angle = acosf(dot);
    float sin_angle = sinf(angle);
    float t1 = sinf((1 - t) * angle) / sin_angle;
    float t2 = sinf(t * angle) / sin_angle;
    return (Quaternion){
        q1.x * t1 + q2.x * t2,
        q1.y * t1 + q2.y * t2,
        q1.z * t1 + q2.z * t2,
        q1.w * t1 + q2.w * t2
    };
}

Matrix4 quaternion_to_matrix4(Quaternion q) {
    float xx = q.x * q.x, xy = q.x * q.y, xz = q.x * q.z, xw = q.x * q.w;
    float yy = q.y * q.y, yz = q.y * q.z, yw = q.y * q.w;
    float zz = q.z * q.z, zw = q.z * q.w;

    Matrix4 m = matrix4_identity();
    m.m[0][0] = 1 - 2 * (yy + zz);
    m.m[0][1] = 2 * (xy - zw);
    m.m[0][2] = 2 * (xz + yw);
    m.m[1][0] = 2 * (xy + zw);
    m.m[1][1] = 1 - 2 * (xx + zz);
    m.m[1][2] = 2 * (yz - xw);
    m.m[2][0] = 2 * (xz - yw);
    m.m[2][1] = 2 * (yz + xw);
    m.m[2][2] = 1 - 2 * (xx + yy);
    return m;
}

// Utility functions
float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float clamp(float value, float min, float max) {
    return (value < min) ? min : ((value > max) ? max : value);
}

float deg_to_rad(float degrees) {
    return degrees * DEG2RAD;
}

float rad_to_deg(float radians) {
    return radians * RAD2DEG;
}

float sign(float x) {
    return (x > 0) - (x < 0);
}

float step(float edge, float x) {
    return x < edge ? 0.0f : 1.0f;
}

float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

float map(float value, float start1, float stop1, float start2, float stop2) {
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

void set_random_seed(unsigned int seed) {
    srand(seed);
}

int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

float random_float(float min, float max) {
    return min + (max - min) * ((float)rand() / (float)RAND_MAX);
}