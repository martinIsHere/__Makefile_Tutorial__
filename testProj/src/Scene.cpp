#include "Scene.h"

Manager* Scene::entityManagerPtr = nullptr;
GameMap** Scene::currentMapPtrPtr = nullptr;
std::vector<Entity*>* Scene::arrayOfActiveEntitiesPtr = nullptr;
SDL_Renderer* Scene::renPtr = nullptr;
uint32_t* Scene::nWinWidthPtr = nullptr;
uint32_t* Scene::nWinHeightPtr = nullptr;
Entity* Scene::playerEntity = nullptr;
Point* Scene::interactionCoords = nullptr;
bool* Scene::playerHasInteracted = nullptr;
TheaterEngine* Scene::theaterEnginePtr = nullptr;