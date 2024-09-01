#include "Renderer.h"

#include <cmath>

Renderer::Renderer() : mWindow(nullptr), mSDLRenderer(nullptr) {}

Renderer::~Renderer() {}

bool Renderer::Initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow("Physics Simulation", 100, 100, 800, 600, 0);
  if (!mWindow) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  mSDLRenderer = SDL_CreateRenderer(
      mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!mSDLRenderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  return true;
}

void Renderer::Shutdown() {
  SDL_DestroyRenderer(mSDLRenderer);
  SDL_DestroyWindow(mWindow);
}

void Renderer::Clear() {
  SDL_SetRenderDrawColor(mSDLRenderer, 0, 0, 0, 255);
  SDL_RenderClear(mSDLRenderer);
}

void Renderer::Present() { SDL_RenderPresent(mSDLRenderer); }

bool Renderer::ProcessEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return false;  // Return false to indicate that we should quit
    }
  }
  return true;  // Continue running
}

void Renderer::DrawCircle(int centerX, int centerY, int radius) {
  const int diameter = (radius * 2);

  int x = (radius - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);
  SDL_SetRenderDrawColor(mSDLRenderer, 255, 255, 255, 255);
  while (x >= y) {
    SDL_RenderDrawPoint(mSDLRenderer, centerX + x, centerY - y);
    SDL_RenderDrawPoint(mSDLRenderer, centerX + x, centerY + y);
    SDL_RenderDrawPoint(mSDLRenderer, centerX - x, centerY - y);
    SDL_RenderDrawPoint(mSDLRenderer, centerX - x, centerY + y);
    SDL_RenderDrawPoint(mSDLRenderer, centerX + y, centerY - x);
    SDL_RenderDrawPoint(mSDLRenderer, centerX + y, centerY + x);
    SDL_RenderDrawPoint(mSDLRenderer, centerX - y, centerY - x);
    SDL_RenderDrawPoint(mSDLRenderer, centerX - y, centerY + x);

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}