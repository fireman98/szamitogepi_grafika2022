#ifndef UTILS_H
#define UTILS_H

/**
 * GLSL-like three dimensional vector
 */
typedef struct vec3
{
    float x;
    float y;
    float z;
} vec3;

typedef struct vec4
{
    float x;
    float y;
    float z;
    float w;
} vec4;

/**
 * Color with RGB components
 */
typedef struct Color
{
    float red;
    float green;
    float blue;
} Color;

/**
 * @brief Color with RGBA components
 *
 */
typedef struct RGBA
{
    float r;
    float g;
    float b;
    float a;
} RGBA;

/**
 * Material
 */
typedef struct Material
{
    struct Color ambient;
    struct Color diffuse;
    struct Color specular;
    float shininess;
} Material;

typedef struct Lighting
{
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float position[4];
} Lighting;

/**
 * Calculates radian from degree.
 */
double degree_to_radian(double degree);

#endif /* UTILS_H */
