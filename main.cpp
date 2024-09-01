#include <SDL2/SDL.h>

#include <iostream>

#include "Renderer.h"

int force_x = 0.0;
int force_y = 200.0;

class Point {
 public:
  float x, y, vel_x, vel_y, mass;
  Point(float x, float y, float vel_x, float vel_y, float mass)
      : x(x), y(y), vel_x(vel_x), vel_y(vel_y), mass(mass) {}
  void update(float dt) {
    int acc_x = force_x / mass;
    int acc_y = force_y / mass;

    x += vel_x * dt;
    y += vel_y * dt;

    vel_x += acc_x * dt;
    vel_y += acc_y * dt;
  }

  void constraint() {
    if (x < 0) {
      x = 0;
      vel_x *= -1;
    } else if (x > 800) {
      x = 800;
      vel_x *= -1;
    } else if (y < 0) {
      y = 0;
      vel_y *= -1;
    } else if (y > 600) {
      y = 600;
      vel_y *= -1;
    }
  }
};
int main() {
  Renderer render;
  Point p1(100.0f, 20.0f, 100.0f, 100.0f, 1.0f);

  render.Initialize();
  bool isRunning = true;
  Uint32 lastTime = SDL_GetTicks();
  while (isRunning) {
    Uint32 currentTime = SDL_GetTicks();
    float dt = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    isRunning = render.ProcessEvents();
    render.Clear();

    p1.update(dt);
    p1.constraint();
    render.DrawCircle(p1.x, p1.y, 10);
    render.Present();
  }
  render.Shutdown();
  return 0;
}