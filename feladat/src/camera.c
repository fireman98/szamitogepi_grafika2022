#include "camera.h"
#include <GL/gl.h>

#include <math.h>

void init_camera(Camera *camera)
{
    camera->position.x = 0.0;
    camera->position.y = 1.0;
    camera->position.z = 0.0;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = -180.0;
    camera->speed.x = 0.0;
    camera->speed.y = -5.0;
    camera->speed.z = 0.0;
    camera->head_level = 50.0;
    camera->move_forward = camera->move_backward = camera->move_left = camera->move_right = camera->move_up = camera->kick = false;
}

void update_camera(Camera *camera, double time, Room *room)
{

    double distance, move_distance;

    distance = time * 10.0;
    move_distance = distance * 20;

    double speed_diff = pow(0.01036955548946418186686576786732, time);

    // Levegő miatti lassulás
    camera->speed.x *= speed_diff;
    camera->speed.y *= speed_diff;
    camera->speed.z *= speed_diff;

    move_camera_x(camera, distance * camera->speed.x, room);
    move_camera_y(camera, distance * camera->speed.y, room);
    move_camera_z(camera, distance * camera->speed.z, room);

    if (camera->move_up && camera->position.y < camera->head_level + 15.0) // Can jump
    {
        camera->speed.y += 10;
    }

    if (camera->position.y > camera->head_level)
    {
        camera->speed.y -= time * 100.0;
    }

    if (camera->move_forward == true)
    {
        double angle = degree_to_radian(camera->rotation.z);
        camera->speed.z -= cos(angle) * move_distance;
        camera->speed.x -= sin(angle) * move_distance;
    }

    if (camera->move_backward == true)
    {
        double angle = degree_to_radian(camera->rotation.z);
        camera->speed.z += cos(angle) * move_distance;
        camera->speed.x += sin(angle) * move_distance;
    }

    if (camera->move_left == true)
    {
        double angle = degree_to_radian(camera->rotation.z + 90.0);
        camera->speed.z -= cos(angle) * move_distance;
        camera->speed.x -= sin(angle) * move_distance;
    }

    if (camera->move_right == true)
    {
        double angle = degree_to_radian(camera->rotation.z - 90.0);
        camera->speed.z -= cos(angle) * move_distance;
        camera->speed.x -= sin(angle) * move_distance;
    }
}

void set_view(const Camera *camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z), 0, 1.0, 0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(Camera *camera, double horizontal, double vertical)
{
    camera->rotation.z += horizontal;
    camera->rotation.x += vertical;

    if (camera->rotation.z < 0)
    {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0)
    {
        camera->rotation.z -= 360.0;
    }

    if (camera->rotation.x < 0)
    {
        camera->rotation.x += 360.0;
    }

    if (camera->rotation.x > 360.0)
    {
        camera->rotation.x -= 360.0;
    }
}

void move_camera_x(struct Camera *camera, double distance, Room *room)
{
    camera->position.x += distance;
    can_move(camera, room);
}

void move_camera_y(struct Camera *camera, double distance, Room *room)
{
    camera->position.y += distance;
    can_move(camera, room);
}

void move_camera_z(struct Camera *camera, double distance, Room *room)
{
    camera->position.z += distance;
    can_move(camera, room);
}

void can_move(struct Camera *camera, Room *room)
{
    if (camera->position.x > room->size.x)
    {
        camera->position.x = room->size.x;
    }

    if (camera->position.x < -room->size.x)
    {
        camera->position.x = -room->size.x;
    }

    if (camera->position.y > room->size.y)
    {
        camera->position.y = room->size.y;
    }

    if (camera->position.y < camera->head_level)
    {
        camera->position.y = camera->head_level;
        camera->speed.y = 0;
    }

    if (camera->position.z > room->size.z)
    {
        camera->position.z = room->size.z;
    }

    if (camera->position.z < -room->size.z)
    {
        camera->position.z = -room->size.z;
    }
}