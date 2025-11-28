#pragma once
#include "Components.h"
#include "ECS.h"
#include "GameMap.h"
#include "GameStateManager.h"
#include "TheaterEngine.h"
#include "includes.h"

class GameMap;

class GameEngine {
 public:
  GameEngine(uint32_t nWidth, uint32_t nHeight, const std::string title,
             uint32_t fps);

  ~GameEngine();

  void handleEvents();

  void handeKeyInputBools();

  void update();

  void draw(bool isPresenting);

  void transitionDraw_boxes(const uint32_t&& r = 100, const uint32_t&& g = 100,
                            const uint32_t&& b = 100);

  void sortEntityArray();

  bool alive();

  SDL_Renderer* getRenPtr() const;

  const uint32_t getWinSize() const;

  void renderText();

  void delayAndUpdateWindowTitle();

  const void test_portalAnimationFunction();

 private:
  uint32_t nWinWidth, nWinHeight;
  SDL_Window* win;
  SDL_Renderer* renPtr;
  SDL_Event* Event;
  uint32_t unStartElapsedTime;
  uint32_t nElapsedTime;
  uint32_t elapsedFrames;
  uint32_t nFps;
  uint32_t nDelay;
  int mousePos[2];
  std::string sTitle;
  std::vector<Entity*>* arrayOfActiveEntitiesPtr;
  std::vector<Entity*> bufferArrayOfEntities;

  // key down bools
  bool w_keyDown;
  bool a_keyDown;
  bool s_keyDown;
  bool d_keyDown;

  // entity managaer
  Manager m_entityManager;

  // game entities
  Entity* playerEntity;

  // state manager
  GameStateManager* m_stateManagerPtr;

  // for transition animation
  SDL_Rect drawRect;
  int boxCount;
  int boxWidth;
  int boxHeight;

  // text
  TTF_Font* arialFont;
  std::string textMessage;
  std::string lastMessage;
  SDL_Surface* surfaceMessage;
  SDL_Texture* Message;

  // music / soundeffects
  Mix_Music* gMusic;

  // gamemap instances
  GameMap* Town1;
  GameMap* House1;

  // loaded maps per section of game!
  GameMap** currentMapPtr;

  // theater engine instance
  TheaterEngine* theaterEngine;

  bool bRunning;

 protected:
};
