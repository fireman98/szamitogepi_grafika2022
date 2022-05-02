#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "utils.h"

#include <obj/model.h>

typedef struct Entity
{
    Model model;
    GLuint texture;
    Material material;
    Lighting lighting;
    vec3 position;
    vec3 rotation;
    vec3 speed;
    double radius;

} Entity;

typedef struct Scene
{
    Entity ball;
    Entity sun;
    Room room;
    Material material;
    Lighting lighting;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene *scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

/**
 * Set the current material.
 */
void set_material(const Material *material);

/**
 * Update the scene.
 */
void update_scene(Scene *scene, Camera *camera, double time);

/**
 * Render the scene objects.
 */
void render_scene(const Scene *scene);

/**
 * @brief Set the global lightning position on axis x
 *
 */
void set_lightning_x_position(Lighting *lighting, double speed);

/**
 * @brief Set the global lightning position on axis y
 *
 */
void set_lightning_y_position(Lighting *lighting, double speed);

/**
 * @brief Set the global lightning position on axis z
 *
 */
void set_lightning_z_position(Lighting *lighting, double speed);

#endif /* SCENE_H */
