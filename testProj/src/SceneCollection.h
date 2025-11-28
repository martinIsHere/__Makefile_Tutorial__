#pragma once
#include "includes.h"
#include "Scene.h"
#include "ECS.h"

struct gameInteractionEvent {
	int x, y; // coordinate of block that triggers event
	int playerNewX, playerNewY; // new player coordinates to be teleported to after switch
	int ID; // ID of scene be activated
};

#define MAX_SIZE_DOOR_INTERACTIONS 16

class OpeningScene : public Scene {
public:
	const std::array<Point, MAX_SIZE_DOOR_INTERACTIONS> doorInteractionArray;
	Entity* NPCEntity;
	uint32_t frames;
	GameMap* Town1;

	const int ID = 0;

	OpeningScene();

	void init() override;

	void update() override;

	void draw() override;

	void end() override;
};

class FirstHouseScene : public Scene {
public:
	const std::array<Point, MAX_SIZE_DOOR_INTERACTIONS> doorInteractionArray;
	uint32_t frames;
	GameMap* House1;

	const int ID = 1;

	FirstHouseScene();

	void init() override;

	void update() override;

	void draw() override;

	void end() override;
};

class SecondHouseScene : public Scene {
public:
	const std::array<Point, MAX_SIZE_DOOR_INTERACTIONS> doorInteractionArray;
	uint32_t frames;
	GameMap* House2;

	const int ID = 1;

	SecondHouseScene();

	void init() override;

	void update() override;

	void draw() override;

	void end() override;
};

class FirstPlantHouseScene : public Scene {
public:
	const std::array<Point, MAX_SIZE_DOOR_INTERACTIONS> doorInteractionArray;
	uint32_t frames;
	GameMap* plantHouse1;
	Entity* CashierEntity;

	const int ID = 1;

	FirstPlantHouseScene();

	void init() override;

	void update() override;

	void draw() override;

	void end() override;
};