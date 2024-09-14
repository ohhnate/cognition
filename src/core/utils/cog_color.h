#ifndef COG_COLOR_H
#define COG_COLOR_H

#include "cog_types.h"

Color color_from_rgb(uint8_t r, uint8_t g, uint8_t b);
Color color_from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
Color color_from_hex(uint32_t hex);
Color color_from_hsv(float h, float s, float v);
uint32_t color_to_hex(Color color);
void color_to_hsv(Color color, float* h, float* s, float* v);

Color color_lerp(Color c1, Color c2, float t);
Color color_multiply(Color color, float factor);
Color color_add(Color c1, Color c2);
Color color_subtract(Color c1, Color c2);
Color color_invert(Color color);
Color color_grayscale(Color color);
Color color_with_alpha(Color color, uint8_t alpha);

bool color_equals(Color c1, Color c2);
float color_brightness(Color color);

extern const Color BLACK;
extern const Color WHITE;
extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;
extern const Color YELLOW;
extern const Color CYAN;
extern const Color MAGENTA;
extern const Color ORANGE;
extern const Color PURPLE;
extern const Color PINK;
extern const Color BROWN;
extern const Color GRAY;
extern const Color LIGHT_GRAY;
extern const Color DARK_GRAY;
extern const Color TRANSPARENT;

#endif // COG_COLOR_H