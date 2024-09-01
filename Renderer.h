#pragma once
#include <SDL2/SDL.h>

class Renderer {
 public:
  Renderer();
  ~Renderer();

  bool Initialize();
  void Shutdown();

  void Clear();
  void Present();
  bool ProcessEvents();
  void DrawCircle(int centerX, int centerY, int radius);

 private:
  SDL_Window *mWindow;
  SDL_Renderer *mSDLRenderer;
};