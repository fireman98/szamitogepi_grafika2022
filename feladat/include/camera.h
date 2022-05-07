#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

#include <stdbool.h>

/**
 * Camera, as a moving point with direction
 */
typedef struct Camera
{
    vec3 position;
    vec3 rotation;
    vec3 speed;
    BoundingBox boundingBox;
    int head_level;
    bool move_forward, move_backward, move_left, move_right, move_up, kick, shoot, run;
} Camera;

/**
 * Initialize the camera to the start position.
 */
void init_camera(Camera *camera);

/**
 * Update the position of the camera.
 */
void update_camera(Camera *camera, double time, Room *room);

/**
 * Apply the camera settings to the view transformation.
 */
void set_view(const Camera *camera);

/**
 * Set the horizontal and vertical rotation of the view angle.
 */
void rotate_camera(Camera *camera, double horizontal, double vertical);

/**
 * Move camera along the x axios
 */

void move_camera_x(struct Camera *camera, double distance, Room *room);

/**
 * Move camera along the y axios
 */

void move_camera_y(struct Camera *camera, double distance, Room *room);

/**
 * Move camera along the z axios
 */

void move_camera_z(struct Camera *camera, double distance, Room *room);

/**
 * Force camera into room
 *
 */
void can_move(struct Camera *camera, Room *room);

/**
 * Set the speed of forward and backward motion.
 */
void set_camera_speed(Camera *camera, double speed);

/**
 * Set the speed of left and right side steps.
 */
void set_camera_side_speed(Camera *camera, double speed);

/**
 * @brief Set the vertical speed of the camera
 *
 */
void set_camera_vertical_speed(Camera *camera, double speed);

#endif /* CAMERA_H */
