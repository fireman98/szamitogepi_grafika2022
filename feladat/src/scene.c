#include "scene.h"
#include "stdio.h"

#include <obj/load.h>
#include <obj/draw.h>

#ifndef GL_EXT_texture_edge_clamp
#define GL_EXT_CLAMP_TO_EDGE 0x812F
#endif

void init_scene(Scene *scene)
{
    load_model(&(scene->sun.model), "assets/models/duck.obj");
    load_model(&(scene->ball.model), "assets/models/duck.obj");

    scene->sun.texture = load_texture("assets/textures/duck.jpg");
    scene->ball.texture = load_texture("assets/textures/duck.jpg");

    scene->room.front = load_texture("assets//textures//wall.jpg");
    scene->room.left = load_texture("assets//textures//wall.jpg");
    scene->room.right = load_texture("assets//textures//wall.jpg");
    scene->room.back = load_texture("assets//textures//wall.jpg");
    scene->room.top = load_texture("assets//textures//wall.jpg");
    scene->room.bottom = load_texture("assets//textures//wall.jpg");

    scene->room.size.x = 2400.0;
    scene->room.size.y = 400.0;
    scene->room.size.z = 1000.0;

    scene->ball.position.x = scene->ball.position.y = scene->ball.position.z = 0;

    scene->material.ambient.red = 1.0;
    scene->material.ambient.green = 1.0;
    scene->material.ambient.blue = 1.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 1.0;

    scene->material.specular.red = 1.0;
    scene->material.specular.green = 1.0;
    scene->material.specular.blue = 1.0;

    scene->material.shininess = 127.0;

    scene->lighting.ambient[0] = 1.0f;
    scene->lighting.ambient[1] = 1.0f;
    scene->lighting.ambient[2] = 1.0f;
    scene->lighting.ambient[3] = 1.0f;

    scene->lighting.diffuse[0] = 1.0f;
    scene->lighting.diffuse[1] = 1.0f;
    scene->lighting.diffuse[2] = 1.0f;
    scene->lighting.diffuse[3] = 1.0f;

    scene->lighting.specular[0] = 1.0f;
    scene->lighting.specular[1] = 1.0f;
    scene->lighting.specular[2] = 1.0f;
    scene->lighting.specular[3] = 1.0f;

    scene->lighting.position[0] = 10.0f;
    scene->lighting.position[1] = 0.0f;
    scene->lighting.position[2] = 10.0f;
    scene->lighting.position[3] = 1.0f;
}

void set_lighting(Lighting *lighting)
{
    glLightfv(GL_LIGHT0, GL_AMBIENT, lighting->ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lighting->diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lighting->specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lighting->position);
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

void set_lightning_z_position(Lighting *lighting, double speed)
{
    lighting->position[0] += speed;
}

void set_lightning_y_position(Lighting *lighting, double speed)
{
    lighting->position[2] += speed;
}

void set_lightning_x_position(Lighting *lighting, double speed)
{
    lighting->position[1] += speed;
}

void update_scene(Scene *scene, double time)
{
    scene->sun.position.x = scene->lighting.position[0];
    scene->sun.position.y = scene->lighting.position[1];
    scene->sun.position.z = scene->lighting.position[2];
}

void render_scene(const Scene *scene)
{
    set_material(&(scene->material));
    set_lighting(&(scene->lighting));
    render_entity(&(scene->sun));
    render_entity(&(scene->ball));
    render_environment(scene);
}

void render_entity(Entity *entity)
{
    glBindTexture(GL_TEXTURE_2D, entity->texture);
    glPushMatrix();
    glTranslatef(entity->position.x, entity->position.y, entity->position.z);
    draw_model(&(entity->model));

    glPopMatrix();
}

void render_environment(const Scene *scene)
{
    GLuint boxList = glGenLists(1);
    glNewList(boxList, GL_COMPILE);

    GLfloat zeros[] = {0, 0, 0};
    GLfloat ones[] = {1, 1, 1};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, zeros);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ones);
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
    glTexCoord2f(1.0, 1.0);
    glVertex3f(room.size.x, room.size.y, room.size.z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(room.size.x, room.size.y, -room.size.z);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-room.size.x, room.size.y, -room.size.z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-room.size.x, room.size.y, room.size.z);

    glEnd();
}

void draw_room_bottom(Room room)
{
    glBindTexture(GL_TEXTURE_2D, room.bottom);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(room.size.x, 0, room.size.z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(room.size.x, 0, -room.size.z);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-room.size.x, 0, -room.size.z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-room.size.x, 0, room.size.z);

    glEnd();
}