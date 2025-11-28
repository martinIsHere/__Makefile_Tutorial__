#include "ECS.h"
#include "SceneCollection.h"
#include "TheaterEngine.h"
#include "components.h"

/*

1st house:
outside door : 7,5 & 8,5
inside spawn : 2, 3
inside door : 2, 4
outside spawn : 8, 6





plant house:
outside door
x: 29
y: 10
--------------
x: 30
y: 10
--------------
outside door spawn
x: 29
y: 11
--------------

2st house:

outside door
x: 29
y: 38
--------------
x: 30
y: 38
--------------
outside door spawn
x: 29
y: 39
--------------
inside door
x: 5
y: 9
--------------
x: 6
y: 9
--------------
inside door spawn
x: 5
y: 8
--------------

*/

bool isViableInteractCoords(Point* interactionCoords) {
  if (interactionCoords->x < 0) return false;
  return true;
}

// can be made more efficient for bigger points arrays. fine for small arrays
// tho.
template <size_t arraySize>
int arrayHasPoint(const std::array<Point, arraySize>* points,
                  const Point* targetPoint) {
  for (int i = 0; i < arraySize; i++) {
    if ((*points)[i].isEqual(targetPoint)) return i;
  }
  return -1;
}

// initializing doorInteractionArray a const array

/*#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#*/

OpeningScene::OpeningScene()
    : doorInteractionArray({Point(7, 5), Point(8, 5), Point(29, 38),
                            Point(30, 38), Point(29, 10), Point(30, 10)}) {
  NPCEntity = nullptr;
  Town1 = nullptr;
  frames = NULL;
}

// remember to initialize all entites after creation !!!
void OpeningScene::init() {
  frames = 0;

  // creating player
  if (!playerEntity) {
    playerEntity = entityManagerPtr->addEntity();
    playerEntity->addComponent<PositionComponent>(
        TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS);
  }

  // loading maps
  if (!Town1) {
    Town1 = new GameMap(
        renPtr,
        "res/map/Town1_sh1.bin",  // path to map file
        "res/imgs/sh1.bmp",       // path to spritesheet file
        12,                       // amount of tiles horizontally in spritesheet
        18,                       // amount of tiles vertically in spritesheet
        *nWinWidthPtr,
        *nWinHeightPtr,  // window-to-be-displayed-on's width and heigth
        &(playerEntity->getComponent<PositionComponent>()
              .getx()),  // pointer to coordinates for camera
        &(playerEntity->getComponent<PositionComponent>()
              .gety()),  // here: just equal to the player position
        false  // if the map is very small, like a house -> set to true
    );
  }

  if (!Scene::playerEntity->hasComponent<DrawingComponent>()) {
    playerEntity->addComponent<DrawingComponent>(
        renPtr,               // current working renderer
        "res/imgs/hero.bmp",  // path
        16,                   // sprite size
        6, 5,                 // sprite sheet columns and rows
        4,                    // amount of frames/imgs of walking animation
        8,                    // amount of animation frames per second
        Town1->getCam()       // current working camera
    );
    playerEntity->addComponent<CollisionComponent>(Town1);  // pass in map class
    playerEntity->addComponent<InteractionComponent>(Town1, interactionCoords,
                                                     playerHasInteracted);
    playerEntity->init();
  }

  if (!NPCEntity) {
    NPCEntity = entityManagerPtr->addEntity();

    NPCEntity->addComponent<PositionComponent>(
        TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS);

    NPCEntity->addComponent<DrawingComponent>(
        renPtr, "res/imgs/secondNPC.bmp", 16, 3, 4,
        2,  // amount of animation frames per second
        4,  // amount of animation frames per second
        Town1->getCam());

    NPCEntity->addComponent<CollisionComponent>(Town1);

    NPCEntity->getComponent<PositionComponent>().set_isFrictionless(true);

    NPCEntity->getComponent<PositionComponent>().set_default_acceleration(0);

    NPCEntity->init();
  }

  theaterEnginePtr->changeCurrentMap(Town1);

  // change this -----------------------------------
  for (Entity* ent : *entityManagerPtr->getEntityArrayPointer()) {
    ent->setInactive();
  }
  playerEntity->setActive();
  NPCEntity->setActive();
  // make this cleaner ------------------------------------
  // playerEntity->getComponent<PositionComponent>().setPos(TILE_SIZE_PIXELS *
  // 8, TILE_SIZE_PIXELS * 6);
  NPCEntity->getComponent<PositionComponent>().setPos_tileCoords(1, 1);
}

void OpeningScene::update() {
  // change this -----------------------------------
  if (*playerHasInteracted && isViableInteractCoords(interactionCoords)) {
    *playerHasInteracted = false;
    int i = arrayHasPoint<MAX_SIZE_DOOR_INTERACTIONS>(&doorInteractionArray,
                                                      interactionCoords);
    if (i >= 0) {  // player has interacted with interactable tile.
      switch (i) {
          // if i == 1 OR i ==2
        case 0:
        case 1:  // first house door
          theaterEnginePtr->changeCurrentScene<FirstHouseScene>();
          break;
        case 2:
        case 3:  // second house door
          playerEntity->getComponent<PositionComponent>().setPos_tileCoords(5,
                                                                            8);
          theaterEnginePtr->changeCurrentScene<SecondHouseScene>();
          break;
        case 4:
        case 5:  // first plant house door
          playerEntity->getComponent<PositionComponent>().setPos_tileCoords(2,
                                                                            4);
          theaterEnginePtr->changeCurrentScene<FirstPlantHouseScene>();
          break;
      }
    }
  }

  // change this -----------------------------------
  if (frames < 228) {
    NPCEntity->getComponent<PositionComponent>().setVel(4, 0);
    NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, true);
    NPCEntity->getComponent<PositionComponent>().setFacingDir(DIR_RIGHT);
  } else if (frames < 800) {
    NPCEntity->getComponent<PositionComponent>().setVel(-2.8f, 2.8f);
    NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, false);
    NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, true);
    NPCEntity->getComponent<PositionComponent>().setFacingDir(DIR_LEFT);
  } else if (frames < 1400) {
    NPCEntity->getComponent<PositionComponent>().setVel(2.8f, 2.8f);
    NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, false);
    NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, true);
    NPCEntity->getComponent<PositionComponent>().setFacingDir(DIR_RIGHT);
  } else if (frames < 1600) {
    NPCEntity->getComponent<PositionComponent>().setVel(-2.8f, 0);
    NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, false);
    NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, true);
    NPCEntity->getComponent<PositionComponent>().setFacingDir(DIR_LEFT);
  } else if (frames < 1700) {
    NPCEntity->getComponent<PositionComponent>().setVel(0, 2.8f);
    NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, false);
    NPCEntity->getComponent<PositionComponent>().setDir(DIR_DOWN, true);
    NPCEntity->getComponent<PositionComponent>().setFacingDir(DIR_DOWN);
  } else {
    NPCEntity->getComponent<PositionComponent>().setDir(DIR_DOWN, false);
    NPCEntity->getComponent<PositionComponent>().setVel(0, 0);
  }
  frames++;
}

void OpeningScene::draw() {
  // change this -----------------------------------
  SDL_Rect box = {interactionCoords->x * TILE_SIZE_PIXELS -
                      *(*currentMapPtrPtr)->getCam()->getOffsetXPtr(),
                  interactionCoords->y * TILE_SIZE_PIXELS -
                      *(*currentMapPtrPtr)->getCam()->getOffsetYPtr(),
                  TILE_SIZE_PIXELS, TILE_SIZE_PIXELS};
  SDL_SetRenderDrawColor(renPtr, 255, 0, 0, 255);
  SDL_RenderDrawRect(renPtr, &box);
}

void OpeningScene::end() { theaterEnginePtr->setTransitionState(); }

/*#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#*/

FirstHouseScene::FirstHouseScene() : doorInteractionArray({Point(2, 4)}) {
  House1 = nullptr;
  frames = NULL;
}

void FirstHouseScene::init() {
  // creating player
  if (!Scene::playerEntity) {
    Scene::playerEntity = Scene::entityManagerPtr->addEntity();
    Scene::playerEntity->addComponent<PositionComponent>(TILE_SIZE_PIXELS,
                                                         TILE_SIZE_PIXELS);
  }

  if (!House1) {
    House1 = new GameMap(
        renPtr, "res/map/House1_sh2.bin", "res/imgs/sh2.bmp", 7, 12,
        *nWinWidthPtr, *nWinHeightPtr,
        &(playerEntity->getComponent<PositionComponent>().getx()),
        &(playerEntity->getComponent<PositionComponent>().gety()), true);
  }

  if (!Scene::playerEntity->hasComponent<DrawingComponent>()) {
    playerEntity->addComponent<DrawingComponent>(
        renPtr,               // current working renderer
        "res/imgs/hero.bmp",  // path
        16,                   // sprite size
        6, 5,                 // sprite sheet columns and rows
        4,                    // amount of frames/imgs of walking animation
        8,                    // amount of animation frames per second
        House1->getCam()      // current working camera
    );
    playerEntity->addComponent<CollisionComponent>(
        House1);  // pass in map class
    playerEntity->addComponent<InteractionComponent>(House1, interactionCoords,
                                                     playerHasInteracted);
    playerEntity->init();
  }

  theaterEnginePtr->changeCurrentMap(House1);

  for (Entity* ent : *entityManagerPtr->getEntityArrayPointer()) {
    ent->setInactive();
  }
  playerEntity->setActive();
  playerEntity->getComponent<PositionComponent>().setPos_tileCoords(2, 3);
}

void FirstHouseScene::update() {
  if (*playerHasInteracted && isViableInteractCoords(interactionCoords)) {
    *playerHasInteracted = false;
    int i = arrayHasPoint<MAX_SIZE_DOOR_INTERACTIONS>(&doorInteractionArray,
                                                      interactionCoords);
    if (i >= 0) {  // player has interacted with interactable tile.
      switch (i) {
          // if i == 1
        case 0:  // first house door
          playerEntity->getComponent<PositionComponent>().setPos_tileCoords(8,
                                                                            6);
          theaterEnginePtr->changeCurrentScene<OpeningScene>();
          break;
      }
    }
  }
}

void FirstHouseScene::draw() {
  SDL_Rect box = {interactionCoords->x * TILE_SIZE_PIXELS -
                      *(*currentMapPtrPtr)->getCam()->getOffsetXPtr(),
                  interactionCoords->y * TILE_SIZE_PIXELS -
                      *(*currentMapPtrPtr)->getCam()->getOffsetYPtr(),
                  TILE_SIZE_PIXELS, TILE_SIZE_PIXELS};
  SDL_SetRenderDrawColor(renPtr, 255, 0, 0, 255);
  SDL_RenderDrawRect(renPtr, &box);
}

void FirstHouseScene::end() { theaterEnginePtr->setTransitionState(); }

/*#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#*/

SecondHouseScene::SecondHouseScene()
    : doorInteractionArray({Point(5, 9), Point(6, 9)}) {
  House2 = nullptr;
  frames = NULL;
}

void SecondHouseScene::init() {
  // creating player
  if (!Scene::playerEntity) {
    Scene::playerEntity = Scene::entityManagerPtr->addEntity();
    Scene::playerEntity->addComponent<PositionComponent>(TILE_SIZE_PIXELS,
                                                         TILE_SIZE_PIXELS);
  }

  if (!House2) {
    House2 = new GameMap(
        renPtr, "res/map/House2_sh2.bin", "res/imgs/sh2.bmp", 7, 12,
        *nWinWidthPtr, *nWinHeightPtr,
        &(playerEntity->getComponent<PositionComponent>().getx()),
        &(playerEntity->getComponent<PositionComponent>().gety()), true);
  }

  if (!Scene::playerEntity->hasComponent<DrawingComponent>()) {
    playerEntity->addComponent<DrawingComponent>(
        renPtr,               // current working renderer
        "res/imgs/hero.bmp",  // path
        16,                   // sprite size
        6, 5,                 // sprite sheet columns and rows
        4,                    // amount of frames/imgs of walking animation
        8,                    // amount of animation frames per second
        House2->getCam()      // current working camera
    );
    playerEntity->addComponent<CollisionComponent>(
        House2);  // pass in map class
    playerEntity->addComponent<InteractionComponent>(House2, interactionCoords,
                                                     playerHasInteracted);
    playerEntity->init();
  }

  theaterEnginePtr->changeCurrentMap(House2);

  for (Entity* ent : *entityManagerPtr->getEntityArrayPointer()) {
    ent->setInactive();
  }
  playerEntity->setActive();
}

void SecondHouseScene::update() {
  if (*playerHasInteracted && isViableInteractCoords(interactionCoords)) {
    *playerHasInteracted = false;
    int i = arrayHasPoint<MAX_SIZE_DOOR_INTERACTIONS>(&doorInteractionArray,
                                                      interactionCoords);
    if (i >= 0) {  // player has interacted with interactable tile.
      switch (i) {
          // if i == 1
        case 0:
        case 1:  // first house door
          playerEntity->getComponent<PositionComponent>().setPos_tileCoords(29,
                                                                            39);
          theaterEnginePtr->changeCurrentScene<OpeningScene>();
          break;
      }
    }
  }
}

void SecondHouseScene::draw() {
  SDL_Rect box = {interactionCoords->x * TILE_SIZE_PIXELS -
                      *(*currentMapPtrPtr)->getCam()->getOffsetXPtr(),
                  interactionCoords->y * TILE_SIZE_PIXELS -
                      *(*currentMapPtrPtr)->getCam()->getOffsetYPtr(),
                  TILE_SIZE_PIXELS, TILE_SIZE_PIXELS};
  SDL_SetRenderDrawColor(renPtr, 255, 0, 0, 255);
  SDL_RenderDrawRect(renPtr, &box);
}

void SecondHouseScene::end() { theaterEnginePtr->setTransitionState(); }

/*#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#*/

FirstPlantHouseScene::FirstPlantHouseScene()
    : doorInteractionArray({Point(2, 5)}) {
  plantHouse1 = nullptr;
  CashierEntity = nullptr;
  frames = NULL;
}

void FirstPlantHouseScene::init() {
  // creating player
  if (!Scene::playerEntity) {
    Scene::playerEntity = Scene::entityManagerPtr->addEntity();
    Scene::playerEntity->addComponent<PositionComponent>(
        TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS);
  }

  if (!plantHouse1) {
    plantHouse1 = new GameMap(
        renPtr, "res/map/PlantHouse1_sh2.bin", "res/imgs/sh2.bmp", 7, 12,
        *nWinWidthPtr, *nWinHeightPtr,
        &(playerEntity->getComponent<PositionComponent>().getx()),
        &(playerEntity->getComponent<PositionComponent>().gety()), true);
  }

  if (!Scene::playerEntity->hasComponent<DrawingComponent>()) {
    playerEntity->addComponent<DrawingComponent>(
        renPtr,                // current working renderer
        "res/imgs/hero.bmp",   // path
        16,                    // sprite size
        6, 5,                  // sprite sheet columns and rows
        4,                     // amount of frames/imgs of walking animation
        8,                     // amount of animation frames per second
        plantHouse1->getCam()  // current working camera
    );
    playerEntity->addComponent<CollisionComponent>(
        plantHouse1);  // pass in map class
    playerEntity->addComponent<InteractionComponent>(
        plantHouse1, interactionCoords, playerHasInteracted);
    playerEntity->init();
  }

  if (!CashierEntity) {
    CashierEntity = entityManagerPtr->addEntity();

    CashierEntity->addComponent<PositionComponent>(
        TILE_SIZE_PIXELS * 2, TILE_SIZE_PIXELS * 2, TILE_SIZE_PIXELS);

    CashierEntity->addComponent<DrawingComponent>(
        renPtr, "res/imgs/cashier.bmp", 16, 1, 3,
        2,                     // amount of frames/imgs of walking animation
        4,                     // amount of animation frames per second
        plantHouse1->getCam()  // current working camera
    );

    CashierEntity->addComponent<CollisionComponent>(plantHouse1);

    CashierEntity->getComponent<PositionComponent>().set_isFrictionless(true);

    CashierEntity->getComponent<PositionComponent>().setFacingDir(DIR_DOWN);

    CashierEntity->getComponent<PositionComponent>().set_default_acceleration(
        0);

    CashierEntity->init();
  }

  theaterEnginePtr->changeCurrentMap(plantHouse1);

  for (Entity* ent : *entityManagerPtr->getEntityArrayPointer()) {
    ent->setInactive();
  }
  CashierEntity->setActive();
  playerEntity->setActive();

  CashierEntity->getComponent<PositionComponent>().setPos_tileCoords(2, 2);
}

void FirstPlantHouseScene::update() {
  if (*playerHasInteracted && isViableInteractCoords(interactionCoords)) {
    *playerHasInteracted = false;
    int i = arrayHasPoint<MAX_SIZE_DOOR_INTERACTIONS>(&doorInteractionArray,
                                                      interactionCoords);
    if (i >= 0) {  // player has interacted with interactable tile.
      switch (i) {
          // if i == 1
        case 0:
        case 1:  // FirstPlantHouseScene inside house door
          playerEntity->getComponent<PositionComponent>().setPos_tileCoords(29,
                                                                            11);
          theaterEnginePtr->changeCurrentScene<OpeningScene>();
          break;
      }
    }
  }
}

void FirstPlantHouseScene::draw() {
  SDL_Rect box = {interactionCoords->x * TILE_SIZE_PIXELS -
                      *(*currentMapPtrPtr)->getCam()->getOffsetXPtr(),
                  interactionCoords->y * TILE_SIZE_PIXELS -
                      *(*currentMapPtrPtr)->getCam()->getOffsetYPtr(),
                  TILE_SIZE_PIXELS, TILE_SIZE_PIXELS};
  SDL_SetRenderDrawColor(renPtr, 255, 0, 0, 255);
  SDL_RenderDrawRect(renPtr, &box);
}

void FirstPlantHouseScene::end() { theaterEnginePtr->setTransitionState(); }

/*#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#*/
