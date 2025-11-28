#pragma once
#include "ECS.h"
#include "PositionComponent.h"
#include "DrawingComponent.h"
#include "gameMap.h"

class InteractionComponent : public Component {

	PositionComponent* m_posComp;

	GameMap* m_gameMap;

	Point* interactionCoords;

	bool* m_hasInteracted;

	constexpr static int interactionDistance = 1; // in blocks

public:

	void interact();

	InteractionComponent(GameMap* gameMap, Point* interactionCoords, bool* hasInteracted);

	~InteractionComponent();

	void init() override;

	void update() override;

	void draw() override;

private:


};

