#pragma once
#include "ECS.h"
#include "PositionComponent.h"
#include "gameMap.h"

class CollisionComponent : public Component{

	PositionComponent* m_posComp;

	GameMap* m_gameMap;

	int gameMapWidth_pixels;
	int gameMapHeight_pixels;

	// versatile buffer
	int buf;

	int hitBoxOffsetX;

	int hitBoxTopOffset;

	int oldYPos;

	uint32_t blockSize;
	
public:

	CollisionComponent(GameMap* gameMap);

	~CollisionComponent();

	void init() override;

	void update() override;

	void draw() override;

	void loadNewMap(GameMap* gameMap);

private:

	// true = entity has collided
	bool correct_possible_collision();

	// true = entity has collided
	inline bool apply_correction_in_LEFT_COLLISION();

	// true = entity has collided
	inline bool apply_correction_in_RIGHT_COLLISION();

	// true = entity has collided
	inline bool apply_correction_in_ABOVE_COLLISION();

	// true = entity has collided
	inline bool apply_correction_in_BELOW_COLLISION();

	// true = entity has collided
	inline bool apply_correction_in_MAP_EDGE_COLLISION();
};

