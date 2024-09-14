#include "cog_color.h"
#include <math.h>

const Color BLACK = {0, 0, 0, 255};
const Color WHITE = {255, 255, 255, 255};
const Color RED = {255, 0, 0, 255};
const Color GREEN = {0, 255, 0, 255};
const Color BLUE = {0, 0, 255, 255};
const Color YELLOW = {255, 255, 0, 255};
const Color CYAN = {0, 255, 255, 255};
const Color MAGENTA = {255, 0, 255, 255};
const Color ORANGE = {255, 165, 0, 255};
const Color PURPLE = {128, 0, 128, 255};
const Color PINK = {255, 192, 203, 255};
const Color BROWN = {165, 42, 42, 255};
const Color GRAY = {128, 128, 128, 255};
const Color LIGHT_GRAY = {192, 192, 192, 255};
const Color DARK_GRAY = {64, 64, 64, 255};
const Color TRANSPARENT = {0, 0, 0, 0};

Color color_from_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (Color){r, g, b, 255};
}

Color color_from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (Color){r, g, b, a};
}

Color color_from_hex(uint32_t hex) {
    return (Color){
        (hex >> 16) & 0xFF,
        (hex >> 8) & 0xFF,
        hex & 0xFF,
        255
    };
}

Color color_from_hsv(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
    float m = v - c;

    float r, g, b;
    if (h < 60) {
        r = c; g = x; b = 0;
    } else if (h < 120) {
        r = x; g = c; b = 0;
    } else if (h < 180) {
        r = 0; g = c; b = x;
    } else if (h < 240) {
        r = 0; g = x; b = c;
    } else if (h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }

    return (Color){
        (uint8_t)((r + m) * 255),
        (uint8_t)((g + m) * 255),
        (uint8_t)((b + m) * 255),
        255
    };
}

uint32_t color_to_hex(Color color) {
    return (color.r << 16) | (color.g << 8) | color.b;
}

void color_to_hsv(Color color, float* h, float* s, float* v) {
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;

    float max = fmaxf(r, fmaxf(g, b));
    float min = fminf(r, fminf(g, b));
    float delta = max - min;

    *v = max;

    if (max != 0) {
        *s = delta / max;
    } else {
        *s = 0;
        *h = -1;
        return;
    }

    if (r == max) {
        *h = (g - b) / delta;
    } else if (g == max) {
        *h = 2 + (b - r) / delta;
    } else {
        *h = 4 + (r - g) / delta;
    }

    *h *= 60;
    if (*h < 0) *h += 360;
}

Color color_lerp(Color c1, Color c2, float t) {
    return (Color){
        (uint8_t)(c1.r + (c2.r - c1.r) * t),
        (uint8_t)(c1.g + (c2.g - c1.g) * t),
        (uint8_t)(c1.b + (c2.b - c1.b) * t),
        (uint8_t)(c1.a + (c2.a - c1.a) * t)
    };
}

Color color_multiply(Color color, float factor) {
    return (Color){
        (uint8_t)fminf(color.r * factor, 255),
        (uint8_t)fminf(color.g * factor, 255),
        (uint8_t)fminf(color.b * factor, 255),
        color.a
    };
}

Color color_add(Color c1, Color c2) {
    return (Color){
        (uint8_t)fminf(c1.r + c2.r, 255),
        (uint8_t)fminf(c1.g + c2.g, 255),
        (uint8_t)fminf(c1.b + c2.b, 255),
        (uint8_t)fminf(c1.a + c2.a, 255)
    };
}

Color color_subtract(Color c1, Color c2) {
    return (Color){
        (uint8_t)fmaxf(c1.r - c2.r, 0),
        (uint8_t)fmaxf(c1.g - c2.g, 0),
        (uint8_t)fmaxf(c1.b - c2.b, 0),
        (uint8_t)fmaxf(c1.a - c2.a, 0)
    };
}

Color color_invert(Color color) {
    return (Color){
        255 - color.r,
        255 - color.g,
        255 - color.b,
        color.a
    };
}

Color color_grayscale(Color color) {
    uint8_t gray = (uint8_t)(0.299f * color.r + 0.587f * color.g + 0.114f * color.b);
    return (Color){gray, gray, gray, color.a};
}

Color color_with_alpha(Color color, uint8_t alpha) {
    return (Color){color.r, color.g, color.b, alpha};
}

bool color_equals(Color c1, Color c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

float color_brightness(Color color) {
    return (0.299f * color.r + 0.587f * color.g + 0.114f * color.b) / 255.0f;
}