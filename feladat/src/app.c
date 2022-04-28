#include "app.h"
#include "stdio.h"

#include <SDL2/SDL_image.h>

void init_app(App *app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->is_running = false;
    app->uptime = 0.0;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0)
    {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Ball!!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL)
    {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    SDL_SetWindowResizable(app->window, TRUE);

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0)
    {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL)
    {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO)
    {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else
    {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }
    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 5000);
}

void handle_app_events(App *app)
{
    SDL_Event event;
    // TODO: mouse dolgokat átrakni az app structba
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_W:
                app->camera.move_forward = true;
                break;
            case SDL_SCANCODE_S:
                app->camera.move_backward = true;
                break;
            case SDL_SCANCODE_A:
                app->camera.move_left = true;
                break;
            case SDL_SCANCODE_D:
                app->camera.move_right = true;
                break;
            case SDL_SCANCODE_LSHIFT:
                // set_camera_vertical_speed(&(app->camera), 1.0);
                break;
            case SDL_SCANCODE_LCTRL:
                // set_camera_vertical_speed(&(app->camera), -1.0);
                break;
            // TODO: a lightningnak is legyen speedje
            case SDL_SCANCODE_KP_4:
                set_lightning_z_position(&(app->scene.lighting), -1);
                break;
            case SDL_SCANCODE_KP_6:
                set_lightning_z_position(&(app->scene.lighting), 1);
                break;
            case SDL_SCANCODE_KP_8:
                set_lightning_y_position(&(app->scene.lighting), 1);
                break;
            case SDL_SCANCODE_KP_2:
                set_lightning_y_position(&(app->scene.lighting), -1);
                break;
            case SDL_SCANCODE_KP_1:
                set_lightning_x_position(&(app->scene.lighting), 1);
                break;
            case SDL_SCANCODE_KP_3:
                set_lightning_x_position(&(app->scene.lighting), -1);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
                app->camera.move_forward = false;
                break;
            case SDL_SCANCODE_S:
                app->camera.move_backward = false;
                break;
            case SDL_SCANCODE_A:
                app->camera.move_left = false;
                break;
            case SDL_SCANCODE_D:
                app->camera.move_right = false;
                break;
            case SDL_SCANCODE_LSHIFT:
            case SDL_SCANCODE_LCTRL:
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            is_mouse_down = true;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (is_mouse_down)
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_app(App *app)
{
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->camera), elapsed_time, &(app->scene.room));
    update_scene(&(app->scene), elapsed_time);
}

void render_app(App *app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App *app)
{
    if (app->gl_context != NULL)
    {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL)
    {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
