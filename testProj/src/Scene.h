#pragma once
#include "includes.h"

class Entity;
class Manager;
class GameMap;
class TheaterEngine;
struct gameInteractionEvent;

class Scene {
public:
	static Manager* entityManagerPtr;
	static GameMap** currentMapPtrPtr;
	static std::vector<Entity*>* arrayOfActiveEntitiesPtr;
	static SDL_Renderer* renPtr;
	static uint32_t* nWinWidthPtr, * nWinHeightPtr;
	static Entity* playerEntity;
	static Point* interactionCoords;
	static bool* playerHasInteracted;
	static TheaterEngine* theaterEnginePtr;

	Scene() {}

	virtual void init() {

	}

	virtual void update() {

	}

	virtual void draw() {

	}

	virtual void end() {

	}

	// add unload scene

	static int getSceneTypeID() {
		static int lastID = 0;
		return lastID++;

	}
	// will return id of certain datatype
	// if datatype has not been entered add it

	template<typename T>
	static int getSceneTypeID() {
		static int typeID = getSceneTypeID();
		return typeID;

	}

private:

};
