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
    vec3 position;
    vec3 rotation;
    vec3 speed;
    BoundingBox boundingBox;
    double radius;

} Entity;

typedef struct Scene
{
    Entity *entities;
    int entityCount;
    Entity sun;
    Room room;
    Lighting lighting;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene *scene);

/**
 * Initializes a new entity as a ball
 */
void init_ball(Entity *ball, char modelPath[], char texturePath[], float x, float z);

/**
 * Push an entity to the entites array
 * Then initalize it as a ball and put it front of the camera
 */
void push_entity(Scene *scene, Camera *camera);

/*
 * Remove the last entity from the entities array
 */
void pop_entity(Scene *scene);

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
 * Render an entity
 */
void render_entity(const Entity *entity);

/**
 * Render the environment
 */
void render_environment(const Scene *scene);

/*
 * Draw room
 */
void draw_room_left(Room room);
void draw_room_right(Room room);
void draw_room_front(Room room);
void draw_room_back(Room room);
void draw_room_top(Room room);
void draw_room_bottom(Room room);

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
