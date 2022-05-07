#ifndef UTILS_H
#define UTILS_H
#include <GL/gl.h>

#define M_PI 3.14159265358979323846

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

typedef struct
{
    GLuint front, back, left, right, top, bottom;
    vec3 size;
} Room;

typedef struct BoundingBox
{
    double top;
    double bottom;
    double left;
    double right;
    double front;
    double back;

} BoundingBox;

#endif /* UTILS_H */
