#include "scene.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include <time.h>

#include <obj/load.h>
#include <obj/draw.h>

#ifndef GL_EXT_texture_edge_clamp
#define GL_EXT_CLAMP_TO_EDGE 0x812F
#endif

void init_scene(Scene *scene)
{
    srand(time(NULL));

    load_model(&(scene->sun.model), "assets/models/duck.obj");
    scene->sun.texture = load_texture("assets/textures/duck.jpg");

    scene->sun.radius = 50.0;
    scene->sun.rotation.x = -90.0;
    scene->sun.rotation.y = 0.0;
    scene->sun.rotation.z = 0.0;

    scene->sun.material.ambient.red = 1.0;
    scene->sun.material.ambient.green = 1.0;
    scene->sun.material.ambient.blue = 1.0;
    scene->sun.material.diffuse.red = 1.0;
    scene->sun.material.diffuse.green = 1.0;
    scene->sun.material.diffuse.blue = 1.0;
    scene->sun.material.specular.red = 1.0;
    scene->sun.material.specular.green = 1.0;
    scene->sun.material.specular.blue = 1.0;
    scene->sun.material.shininess = 127;

    scene->entityCount = 1;
    scene->entities = malloc(scene->entityCount * sizeof *scene->entities);

    init_ball(&(scene->entities[0]), "assets/models/ball6.obj", "assets/textures/duck.jpg", 50, 50);

    scene->room.front = load_texture("assets//textures//wall.jpg");
    scene->room.left = load_texture("assets//textures//wall_left.png");
    scene->room.right = load_texture("assets//textures//wall.jpg");
    scene->room.back = load_texture("assets//textures//wall.jpg");
    scene->room.top = load_texture("assets//textures//wall_top.png");
    scene->room.bottom = load_texture("assets//textures//wall_bottom.jpg");

    scene->room.size.x = 4800.0;
    scene->room.size.y = 4000.0;
    scene->room.size.z = 2000.0;

    scene->lighting.ambient[0] = 0.4f;
    scene->lighting.ambient[1] = 0.4f;
    scene->lighting.ambient[2] = 0.4f;
    scene->lighting.ambient[3] = 1.0f;

    scene->lighting.diffuse[0] = 1.0f;
    scene->lighting.diffuse[1] = 1.0f;
    scene->lighting.diffuse[2] = 1.0f;
    scene->lighting.diffuse[3] = 1.0f;

    scene->lighting.specular[0] = 1.0f;
    scene->lighting.specular[1] = 1.0f;
    scene->lighting.specular[2] = 1.0f;
    scene->lighting.specular[3] = 1.0f;

    scene->lighting.position[0] = 0.0f;
    scene->lighting.position[1] = 850.0f;
    scene->lighting.position[2] = 0.0f;
    scene->lighting.position[3] = 1.0f;

    scene->lighting.rotation[0] = 0.0f;
    scene->lighting.rotation[1] = -1.0f;
    scene->lighting.rotation[2] = 0.0f;
}

void init_ball(Entity *ball, char modelPath[], char texturePath[], float x, float z)
{
    load_model(&(ball->model), modelPath);
    // ball->texture = load_texture(texturePath);
    ball->texture = NULL;

    ball->radius = 10.0;

    ball->position.x = x;
    ball->position.y = 250.0;
    ball->position.z = z;

    ball->rotation.x = ball->rotation.y = ball->rotation.z = 0.0;
    ball->speed.x = ball->speed.y = ball->speed.z = 0.0;

    ball->material.ambient.red = 0.0;
    ball->material.ambient.green = 0.0;
    ball->material.ambient.blue = 0.0;
    ball->material.diffuse.red = ((float)rand()) / RAND_MAX;
    ball->material.diffuse.green = ((float)rand()) / RAND_MAX;
    ball->material.diffuse.blue = ((float)rand()) / RAND_MAX;
    ball->material.specular.red = ((float)rand()) / RAND_MAX;
    ball->material.specular.green = ((float)rand()) / RAND_MAX;
    ball->material.specular.blue = ((float)rand()) / RAND_MAX;
    ball->material.shininess = 50;
}

void push_entity(Scene *scene, Camera *camera)
{
    if (scene->entityCount >= 10)
        return;

    Entity *entity_pointer;

    scene->entityCount += 1;
    entity_pointer = (Entity *)realloc(scene->entities, scene->entityCount * sizeof(Entity));

    if (entity_pointer == NULL)
    {
        printf("Unable to reallocate memory, exiting.\n");
        free(entity_pointer);
        exit(0);
    }
    else
    {
        scene->entities = entity_pointer;

        double angle = degree_to_radian(camera->rotation.z);
        float x = camera->position.x - sin(angle) * 150;
        float z = camera->position.z - cos(angle) * 150;
        printf("%.2f %.2f %.2f\n", angle, x, z);
        init_ball(&(scene->entities[scene->entityCount - 1]), "assets/models/ball6.obj", "assets/textures/duck.jpg", x, z);
    }
}

void pop_entity(Scene *scene)
{
    if (scene->entityCount <= 1)
        return;

    Entity *entity_pointer;

    scene->entityCount -= 1;
    entity_pointer = (Entity *)realloc(scene->entities, scene->entityCount * sizeof(Entity));

    if (entity_pointer == NULL)
    {
        printf("Unable to reallocate memory, exiting.\n");
        free(entity_pointer);
        exit(0);
    }
    else
    {
        scene->entities = entity_pointer;
    }
}

void set_lighting(const Lighting *lighting)
{
    glLightfv(GL_LIGHT0, GL_AMBIENT, lighting->ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lighting->diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lighting->specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lighting->position);
    // glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lighting->rotation);
    //  printf("%.2f %.2f %.2f \n", lighting->rotation[0], lighting->rotation[1], lighting->rotation[2]);
}

void set_material(const Material *material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue};

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue};

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void set_lightning_x_position(Lighting *lighting, double speed)
{
    lighting->position[0] += speed * 50;
}

void set_lightning_z_position(Lighting *lighting, double speed)
{
    lighting->position[2] += speed * 50;
}

void set_lightning_y_position(Lighting *lighting, double speed)
{
    lighting->position[1] += speed * 50;
}

void update_entity_bounding_box(Entity *entity)
{
    entity->boundingBox.right = entity->position.x + entity->radius;
    entity->boundingBox.left = entity->position.x - entity->radius;
    entity->boundingBox.top = entity->position.y + entity->radius;
    entity->boundingBox.bottom = entity->position.y - entity->radius;
    entity->boundingBox.front = entity->position.z + entity->radius;
    entity->boundingBox.back = entity->position.z - entity->radius;
}

bool is_boundingbox_overlap(BoundingBox b1, BoundingBox b2)
{
    return !(b1.right < b2.left || b1.left > b2.right ||
             b1.top < b2.bottom || b1.bottom > b2.top ||
             b1.front < b2.back || b1.back > b2.front);
}

void entity_collision_handler(Entity *e1, Entity *e2)
{
    e1->speed.x += (e1->boundingBox.left - e2->boundingBox.left) * 0.5;
    e1->speed.y += (e1->boundingBox.bottom - e2->boundingBox.bottom) * 0.5;
    e1->speed.z += (e1->boundingBox.back - e2->boundingBox.back) * 0.5;
}

void update_entity(Entity *entity, Camera *camera, Room *room, Entity *entities, int entityCount, double elapsed_time)
{
    double base_speed = elapsed_time * 10;
    double speed_diff = pow(0.99, base_speed * 8);

    // Levegő miatti lassulás
    entity->speed.x *= speed_diff;
    entity->speed.y *= speed_diff;
    entity->speed.z *= speed_diff;

    // Gravitáció
    if (entity->boundingBox.bottom > 0)
    {
        entity->speed.y -= 80 * elapsed_time;
    }

    //Ütközés
    if (is_boundingbox_overlap(camera->boundingBox, entity->boundingBox))
    {
        double shot_power = 1;
        if (camera->shoot)
            shot_power = 4;

        entity->speed.x += (entity->boundingBox.left - camera->boundingBox.left) * 1.8 * shot_power;
        entity->speed.z += (entity->boundingBox.back - camera->boundingBox.back) * 1.8 * shot_power;

        if (camera->kick)
        {
            entity->speed.y += 80;
        }
    };

    for (int i = 0; i < entityCount; i++)
    {
        if (entity != &(entities[i]) && is_boundingbox_overlap(entity->boundingBox, entities[i].boundingBox))
        {
            entity_collision_handler(&(entities[i]), entity);
        }
    }

    entity->position.x += base_speed * entity->speed.x;
    entity->position.y += base_speed * entity->speed.y;
    entity->position.z += base_speed * entity->speed.z;

    double length = sqrt(pow(entity->speed.x, 2) + pow(entity->speed.z, 2));
    double theta = acos(length ? entity->speed.x / length : 0) * 57.2957795;
    double angle;

    if (entity->speed.x > 0 && entity->speed.z < 0)
    {
        angle = 270 + theta;
    }
    else if (entity->speed.x > 0 && entity->speed.z > 0)
    {
        angle = 270 - theta;
    }

    else if (entity->speed.x < 0 && entity->speed.z < 0)
    {
        angle = theta - 90;
    }
    else if (entity->speed.x < 0 && entity->speed.z > 0)
    {
        angle = 270 - theta;
    }
    else
    {
        angle = 0;
    }

    entity->rotation.x -= ((base_speed * (length)) / (2 * entity->radius * M_PI)) * 360;
    entity->rotation.y = angle;

    if ((entity->boundingBox.right > room->size.x && entity->speed.x > 0) || (entity->boundingBox.left < -room->size.x && entity->speed.x < 0))
    {
        entity->speed.x *= -0.8;
    }

    if ((entity->boundingBox.top > room->size.y && entity->speed.y > 0) || (entity->boundingBox.bottom < 0 && entity->speed.y < 0))
    {
        entity->speed.y *= -0.8;
    }

    if ((entity->boundingBox.front > room->size.z && entity->speed.z > 0) || (entity->boundingBox.back < -room->size.z && entity->speed.z < 0))
    {
        entity->speed.z *= -0.8;
    }
}

void update_scene(Scene *scene, Camera *camera, double time)
{
    /* scene->lighting.rotation[0] = -sin(degree_to_radian(camera->rotation.z));
     scene->lighting.rotation[1] = sin(degree_to_radian(camera->rotation.x));
     scene->lighting.rotation[2] = -cos(degree_to_radian(camera->rotation.z));*/

    scene->sun.position.x = scene->lighting.position[0];
    scene->sun.position.y = scene->lighting.position[1];
    scene->sun.position.z = scene->lighting.position[2];
    for (int i = 0; i < scene->entityCount; i++)
    {
        update_entity_bounding_box(&(scene->entities[i]));
    }

    for (int i = 0; i < scene->entityCount; i++)
    {
        update_entity(&(scene->entities[i]), camera, &(scene->room), scene->entities, scene->entityCount, time);
    }
}

void render_scene(const Scene *scene)
{
    set_lighting(&(scene->lighting));
    render_entity(&(scene->sun));

    for (int i = 0; i < scene->entityCount; i++)
    {
        render_entity(&(scene->entities[i]));
    }
    render_environment(scene);
}

void render_entity(const Entity *entity)
{
    glBindTexture(GL_TEXTURE_2D, entity->texture);
    set_material(&(entity->material));
    glPushMatrix();
    glTranslatef(entity->position.x, entity->position.y, entity->position.z);
    glScalef(entity->radius, entity->radius, entity->radius);
    glRotatef(entity->rotation.z, 0, 0, 1);
    glRotatef(entity->rotation.y, 0, 1, 0);
    glRotatef(entity->rotation.x, 1, 0, 0);
    draw_model(&(entity->model));
    glPopMatrix();
}

void render_environment(const Scene *scene)
{
    GLuint boxList = glGenLists(1);
    glNewList(boxList, GL_COMPILE);

    GLfloat zeros[] = {0, 0, 0};
    GLfloat ones[] = {1, 1, 1};
    const GLfloat shininess = 50;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ones);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ones);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ones);
    glMaterialfv(GL_FRONT, GL_SHININESS, &(shininess));
    draw_room_left(scene->room);
    draw_room_right(scene->room);
    draw_room_front(scene->room);
    draw_room_back(scene->room);
    draw_room_top(scene->room);
    draw_room_bottom(scene->room);
    glEndList();

    glCallList(boxList);
}

void draw_room_left(Room room)
{
    glBindTexture(GL_TEXTURE_2D, room.left);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-room.size.x, 0, -room.size.z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-room.size.x, room.size.y, -room.size.z);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-room.size.x, room.size.y, room.size.z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-room.size.x, 0, room.size.z);
    glEnd();
}
void draw_room_right(Room room)
{
    glBindTexture(GL_TEXTURE_2D, room.right);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(room.size.x, 0, room.size.z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(room.size.x, room.size.y, room.size.z);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(room.size.x, room.size.y, -room.size.z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(room.size.x, 0, -room.size.z);
    glEnd();
}
void draw_room_front(Room room)
{
    glBindTexture(GL_TEXTURE_2D, room.front);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-room.size.x, 0, -room.size.z);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(room.size.x, 0, -room.size.z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(room.size.x, room.size.y, -room.size.z);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-room.size.x, room.size.y, -room.size.z);
    glEnd();
}
void draw_room_back(Room room)
{
    glBindTexture(GL_TEXTURE_2D, room.back);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-room.size.x, 0, room.size.z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-room.size.x, room.size.y, room.size.z);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(room.size.x, room.size.y, room.size.z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(room.size.x, 0, room.size.z);
    glEnd();
}
void draw_room_top(Room room)
{
    glBindTexture(GL_TEXTURE_2D, room.top);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(room.size.x, room.size.y, room.size.z);
    glNormal3f(0, -1, 0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(room.size.x, room.size.y, -room.size.z);
    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-room.size.x, room.size.y, -room.size.z);
    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-room.size.x, room.size.y, room.size.z);

    glEnd();
}

void draw_room_bottom(Room room)
{
    glBindTexture(GL_TEXTURE_2D, room.bottom);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(room.size.x, 0, room.size.z);
    glNormal3f(0, 1, 0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(room.size.x, 0, -room.size.z);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-room.size.x, 0, -room.size.z);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-room.size.x, 0, room.size.z);

    glEnd();
}