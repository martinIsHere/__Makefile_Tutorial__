#pragma once
#include "includes.h"
#include "GameStateManager.h"
#include "Scene.h"
#include "ECS.h"

#define MAX_LOADED_SCENES 10

class TheaterEngine {
	 GameStateManager* gameStateManagerPtr;
	 Scene* currentScenePtr;
	 Manager* entityManagerPtr;
	 std::array<Scene*, MAX_LOADED_SCENES> currentlyLoadedScenesPtr;
public:
	TheaterEngine();

	~TheaterEngine();

	void init(
		GameStateManager* gameStateManagerPtr,
		Manager* entityManagerPtr,
		GameMap** currentMapPtrPtr,
		SDL_Renderer* renPtr,
		uint32_t* nWinWidthPtr,
		uint32_t* nWinHeightPtr
		);

	void initCurrentScene();

	void makeEntityMove_constantSpeedNoDiagonalMovement(
		Entity* ent, 
		int xDest, 
		int yDest, 
		float speed, 
		int waitTimeFirstStop,
		int waitTimeSecondStop,
		bool xMove_before_yMove
		);
	void teleportEntity(
		Entity* ent, 
		int xDest, 
		int yDest
		);

	bool changeCurrentMap(GameMap* in_currentMapPtr);

	template <typename T>
	Scene* loadOrGetScene() {
		if (currentlyLoadedScenesPtr[Scene::getSceneTypeID<T>()] != nullptr) // if wanted scene is already loaded
			return currentlyLoadedScenesPtr[Scene::getSceneTypeID<T>()]; // return the loaded scene
		T* newScene = new T(); // else return a newly created version of the scene type ...
		currentlyLoadedScenesPtr[Scene::getSceneTypeID<T>()] = newScene; // and add to array of loaded scenes.
		return newScene;
	}

	template <typename T>  // return true if the new scene has already been loaded
	bool changeCurrentScene() {
		Scene* in_currentScenePtr = loadOrGetScene<T>(); 
		
		if (currentScenePtr) { // if previous scene was not nullptr
			currentScenePtr->end(); // call an ending function.
		}
		currentScenePtr = in_currentScenePtr; // update current scenePtr
		return true;
	}

	void setTransitionState();

	Entity* getPlayerEntity();

	GameMap* getCurrentMap();

	void update();

	void draw();

private:

};