#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>

void init_scene(Scene *scene)
{
    load_model(&(scene->cube), "assets/models/duck.obj");
    scene->texture_duck = load_texture("assets/textures/duck.jpg");
    scene->texture_cube = load_texture("assets/textures/cube.png");

    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 1.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 1.0;

    scene->material.shininess = 127.0;

    scene->lighting.ambient_light[0] = 1.0f;
    scene->lighting.ambient_light[1] = 1.0f;
    scene->lighting.ambient_light[2] = 1.0f;
    scene->lighting.ambient_light[3] = 1.0f;

    scene->lighting.diffuse_light[0] = 1.0f;
    scene->lighting.diffuse_light[1] = 1.0f;
    scene->lighting.diffuse_light[2] = 1.0f;
    scene->lighting.diffuse_light[3] = 1.0f;

    scene->lighting.specular_light[0] = 1.0f;
    scene->lighting.specular_light[1] = 1.0f;
    scene->lighting.specular_light[2] = 1.0f;
    scene->lighting.specular_light[3] = 1.0f;

    scene->lighting.position[0] = 10.0f;
    scene->lighting.position[1] = 0.0f;
    scene->lighting.position[2] = 10.0f;
    scene->lighting.position[3] = 1.0f;
}

void set_lighting(Lighting *lighting)
{
    glLightfv(GL_LIGHT0, GL_AMBIENT, lighting->ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lighting->diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lighting->specular_light);
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
}

void render_scene(const Scene *scene)
{
    set_material(&(scene->material));
    set_lighting(&(scene->lighting));
    draw_origin();
    glBindTexture(GL_TEXTURE_2D, scene->texture_cube);
    draw_lighting_position(&(scene->lighting), scene);
    glBindTexture(GL_TEXTURE_2D, scene->texture_duck);
    draw_model(&(scene->cube));
}

void draw_lighting_position(Lighting *lighting, Scene *scene)
{
    glPushMatrix();
    glScalef(
        0.1f,
        0.1f,
        0.1f);
    glTranslatef(lighting->position[0], lighting->position[1], lighting->position[2]);
    draw_model(&(scene->cube));

    glPopMatrix();
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}
