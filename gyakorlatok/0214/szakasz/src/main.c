#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <stdbool.h>
#include <stdio.h>

struct Color
{
  double r;
  double g;
  double b;
};

struct Szakasz
{
  double x0;
  double x1;
  double y0;
  double y1;
  struct Color color;
};

struct PalettaElem
{
  double x;
  double y;
};

struct Point
{
  double x;
  double y;
};

struct Point transformToOpenglCoordinates(int mouseX, int mouseY)
{
  struct Point p;
  p.x = ((double)mouseX / 400) - 1;
  p.y = 1 - ((double)mouseY / 300);

  return p;
}

void drawLine(struct Szakasz *sz)
{
  glBegin(GL_LINES);
  glVertex2f(sz->x0, sz->y0);
  glVertex2f(sz->x1, sz->y1);
  glColor3f(sz->color.r, sz->color.b, sz->color.g);
  glEnd();
}

/**
 * SDL2 OpenGL example
 */
int main(int argc, char *argv[])
{
  int error_code;
  SDL_Window *window;
  bool need_run;
  SDL_Event event;
  SDL_GLContext gl_context;
  int i;
  struct Szakasz sz1;
  struct Szakasz szakaszok[1];
  bool draw_sz1 = true;

  int mouseX;
  int mouseY;

  // Create szakasz
  sz1.x0 = -1;
  sz1.y0 = 0;
  sz1.x1 = 1;
  sz1.y1 = 0;
  sz1.color.r = 1;
  sz1.color.g = 1;
  sz1.color.b = 0;

  // szakaszok[0] = &sz1;

  error_code = SDL_Init(SDL_INIT_EVERYTHING);
  if (error_code != 0)
  {
    printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
    return error_code;
  }

  window = SDL_CreateWindow(
      "Hello SDL!",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      800, 600,
      SDL_WINDOW_OPENGL);

  gl_context = SDL_GL_CreateContext(window);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  need_run = true;
  while (need_run)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_KEYDOWN:
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_Q:
          need_run = false;
          break;

        default:
          break;
        }
        break;

      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
          SDL_GetMouseState(&mouseX, &mouseY);
          struct Point p = transformToOpenglCoordinates(mouseX, mouseY);
          printf("MouseState: %d %d %.2f %.2f\n", mouseX, mouseY, p.x, p.y);
          sz1.x0 = p.x;
          sz1.y0 = p.y;
        }

        if (event.button.button == SDL_BUTTON_RIGHT)
        {
          draw_sz1 = false;
        }
        break;

      case SDL_QUIT:
        need_run = false;
        break;
      }
    }

    //    glRotatef(0.1, 0, 0, 1);

    glClear(GL_COLOR_BUFFER_BIT);
    if (draw_sz1)
      drawLine(&sz1);

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
