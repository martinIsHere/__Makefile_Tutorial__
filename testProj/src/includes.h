#pragma once
#include <stdint.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "Vector.h"
#define PI 3.1416  // 59
#define log(a)              \
  {                         \
    std::cout << a << "\n"; \
  }
#define logh(a)                         \
  {                                     \
    std::cout << std::hex << a << "\n"; \
  }
#define assertN(v, t)         \
  {                           \
    if (v == t) {             \
      std::cout << "Error: "; \
      exit(-1);               \
    }                         \
  }
// #define assert(v, t) { if (v != t){std::cout << "Error: "; exit(-1);} }
#define assert(v)                       \
  {                                     \
    if (v == nullptr) {                 \
      std::cout << "!---nullptr---!\n"; \
    }                                   \
  }
#define DIR_LEFT 0
#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_DOWN 3
#define TILE_SIZE_PIXELS 64
#define AVERAGE_ENTITY_SIZE_PIXELS 64
class GameEngine;
class GameMap;
class Entity;
class Manager;
class Scene;
class GameMap;
class TheaterEngine;
extern uint32_t actualFPS;
extern uint32_t targetFPS;

// unused
extern uint32_t WIN_WIDTH;
extern uint32_t WIN_HEIGHT;
extern bool debugKeyDown;

// SDL_Texture* tex, int nSize, int nWidth, int nHeight;
struct SpriteSheet {
  SDL_Texture* tex;
  int nSize, nWidth, nHeight;
};
