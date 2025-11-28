#include "TheaterEngine.h"
#include "SceneCollection.h"
#include "Components.h"

TheaterEngine::TheaterEngine() {
	gameStateManagerPtr = nullptr;
	currentScenePtr = nullptr;
	entityManagerPtr = nullptr;
	currentlyLoadedScenesPtr = { };

}

TheaterEngine::~TheaterEngine() {

}

void TheaterEngine::init(
	GameStateManager* gameStateManagerPtr,
	Manager* entityManagerPtr,
	GameMap** currentMapPtrPtr,
	SDL_Renderer* renPtr,
	uint32_t* nWinWidthPtr,
	uint32_t* nWinHeightPtr
	) {
	this->entityManagerPtr = entityManagerPtr;
	this->gameStateManagerPtr = gameStateManagerPtr;
	Scene::entityManagerPtr = entityManagerPtr;
	Scene::currentMapPtrPtr = currentMapPtrPtr;
	Scene::arrayOfActiveEntitiesPtr = entityManagerPtr->getEntityArrayPointer();
	Scene::renPtr = renPtr;
	Scene::nWinWidthPtr = nWinWidthPtr;
	Scene::nWinHeightPtr = nWinHeightPtr;
	Scene::interactionCoords = new Point(NULL, NULL);
	Scene::playerHasInteracted = new bool(false);
	Scene::theaterEnginePtr = this;


	// TODO: relative to prev save init a scene
	changeCurrentScene<OpeningScene>();
	initCurrentScene();
}

void TheaterEngine::initCurrentScene() {
	if(currentScenePtr)
		currentScenePtr->init();
}

void TheaterEngine::makeEntityMove_constantSpeedNoDiagonalMovement(
	Entity* ent,
	int xDest,
	int yDest,
	float speed,
	const int waitTimeFirstStop_inSeconds, 
	const int waitTimeSecondStop_inSeconds, 
	bool xMove_before_yMove
	) {
	int waitTimeFirstStop_inFrames = waitTimeFirstStop_inSeconds * targetFPS;
	int waitTimeSecondStop_inFrames = waitTimeSecondStop_inSeconds * targetFPS;

}
void TheaterEngine::teleportEntity(
	Entity* ent,
	int xDest,
	int yDest
	) {
	ent->getComponent<PositionComponent>().setPos(xDest, yDest);
}

bool TheaterEngine::changeCurrentMap(GameMap* in_currentMapPtr) {
	if (in_currentMapPtr != nullptr) {
		for (Entity* entity : *entityManagerPtr->getEntityArrayPointer()) {
			if (entity->hasComponent<CollisionComponent>()) {
				entity->getComponent<CollisionComponent>().loadNewMap(in_currentMapPtr);
			}
			if (entity->hasComponent<DrawingComponent>()) {
				entity->getComponent<DrawingComponent>().loadNewCamera(in_currentMapPtr->getCam());
			}
		}
		*Scene::currentMapPtrPtr = in_currentMapPtr;
		return true;
	}
	return false;
}

void TheaterEngine::setTransitionState() {
	gameStateManagerPtr->set(GameStateManager::state_blockTransition);
}

Entity* TheaterEngine::getPlayerEntity() {
	return Scene::playerEntity;
}

GameMap* TheaterEngine::getCurrentMap() {
	return *Scene::currentMapPtrPtr;
}

void TheaterEngine::update() {
	currentScenePtr->update();
}

void TheaterEngine::draw() {
	currentScenePtr->draw();
}