#include <cmath>
#include <iostream>
#include <vector>

#include "Renderer.h"

int force_x = 0.0;
int force_y = 200.0;
int windowWidth = 800;
int windowHeight = 600;

class Point {
 public:
  float x, y, mass, oldx, oldy;
  Point(float x, float y, float mass)
      : x(x), y(y), oldx(x), oldy(y), mass(mass) {}

  void update(float dt) {
    float vel_x = x - oldx;
    float vel_y = y - oldy;

    oldx = x;
    oldy = y;

    float acc_x = force_x / mass;
    float acc_y = force_y / mass;

    x += vel_x + acc_x * dt * dt;
    y += vel_y + acc_y * dt * dt;
  }

  void constraint() {
    float vel_x = x - oldx;
    float vel_y = y - oldy;

    if (x < 0) {
      x = 0;
      oldx = x + vel_x;
    } else if (x > windowWidth) {
      x = windowWidth;
      oldx = x + vel_x;
    }

    if (y < 0) {
      y = 0;
      oldy = y + vel_y;
    } else if (y > windowHeight) {
      y = windowHeight;
      oldy = y + vel_y;
    }
  }
};

class Stick {
 public:
  Point& p1;
  Point& p2;
  float length;

  Stick(Point& p1, Point& p2, float length) : p1(p1), p2(p2), length(length) {}

  void update() {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    float dist = std::sqrt(dx * dx + dy * dy);
    float diff = length - dist;
    float percent = (diff / dist) / 2;

    float offset_x = dx * percent;
    float offset_y = dy * percent;

    p1.x += offset_x;
    p1.y += offset_y;

    p2.x -= offset_x;
    p2.y -= offset_y;

    // rearranging the + and - value cause problem.
    // TODO::find why this lead to a crash
    //  p1.x -= offset_x;
    //  p1.y -= offset_y;
    //  p2.x += offset_x;
    //  p2.y += offset_y;
  }
  void draw(Renderer& renderer) {
    renderer.DrawLine(static_cast<int>(p1.x), static_cast<int>(p1.y),
                      static_cast<int>(p2.x), static_cast<int>(p2.y));
  }
};
float distance(const Point& p0, const Point& p1) {
  float dx = p1.x - p0.x;
  float dy = p1.y - p0.y;
  return std::sqrt(dx * dx + dy * dy);
}
int main() {
  Renderer render;
  std::vector<Point> points;
  std::vector<Stick> sticks;

  points.emplace_back(100.0f, 0.0f, 1.0f);    // 0
  points.emplace_back(200.0f, 0.0f, 1.0f);    // 1
  points.emplace_back(100.0f, 100.0f, 1.0f);  // 2
  points.emplace_back(200.0f, 100.0f, 1.0f);  // 3

  sticks.emplace_back(points[0], points[1], distance(points[0], points[1]));
  sticks.emplace_back(points[0], points[2], distance(points[0], points[2]));
  sticks.emplace_back(points[1], points[2], distance(points[1], points[2]));
  sticks.emplace_back(points[1], points[3], distance(points[1], points[3]));
  sticks.emplace_back(points[0], points[3], distance(points[0], points[3]));
  sticks.emplace_back(points[2], points[3], distance(points[2], points[3]));

  render.Initialize();
  bool isRunning = true;
  Uint32 lastTime = SDL_GetTicks();

  while (isRunning) {
    Uint32 currentTime = SDL_GetTicks();
    float dt = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        isRunning = false;
      } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          force_y = -200.0f;
        }
        if (event.button.button == SDL_BUTTON_RIGHT) {
          force_x = 100.0f;
        }
      } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          force_y = 200.0f;
        }
        if (event.button.button == SDL_BUTTON_RIGHT) {
          force_x = 0.0f;
        }
      }
    }
    render.Clear();

    for (auto& stick : sticks) {
      stick.update();
      stick.draw(render);
    }

    for (auto& point : points) {
      point.update(dt);
      point.constraint();
      render.DrawCircle(static_cast<int>(point.x), static_cast<int>(point.y),
                        10);
    }

    render.Present();
  }
  render.Shutdown();
  return 0;
}