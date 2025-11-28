#include "CollisionComponent.h"
#include "includes.h"
#define COLSN_OFST 0

CollisionComponent::CollisionComponent(GameMap* gameMap) {
	m_gameMap = gameMap;
	m_posComp = nullptr;
	buf = NULL;
	oldYPos = NULL;
	hitBoxOffsetX = NULL;
	hitBoxTopOffset = NULL;
	gameMapWidth_pixels = NULL;
	gameMapHeight_pixels = NULL;
	blockSize = m_gameMap->getBlockSize();
}

CollisionComponent::~CollisionComponent() {

}

void CollisionComponent::init() {
	if (!ent) {
		log("Component's entityPtr was nullptr!\n");
	}
	else {
		if (ent->hasComponent<PositionComponent>()) {

			m_posComp = &ent->getComponent<PositionComponent>();
		}
		else {
			m_posComp = nullptr;
			std::cout << "Entity does not have positionComponent!\n";
		}
	}
	gameMapWidth_pixels = m_gameMap->getMapWidth_tiles() * blockSize;
	gameMapHeight_pixels = m_gameMap->getMapHeight_tiles() * blockSize;
}

void CollisionComponent::loadNewMap(GameMap* gameMap) {
	m_gameMap = gameMap;
	gameMapWidth_pixels = m_gameMap->getMapWidth_tiles() * blockSize;
	gameMapHeight_pixels = m_gameMap->getMapHeight_tiles() * blockSize;
}

void CollisionComponent::update() {
	if (correct_possible_collision()) {
		//log("collision")
	}
}

void CollisionComponent::draw() {

}

inline bool CollisionComponent::apply_correction_in_LEFT_COLLISION() {
	if (m_gameMap->getState(
		int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / blockSize),
		int((oldYPos + COLSN_OFST + hitBoxTopOffset) / blockSize)
		)
		|| m_gameMap->getState(
			int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / blockSize),
			int(((oldYPos + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - 1) / blockSize)))
		) {

		// Correct collision: move back to the left boundary
		m_posComp->getx() =
			int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / blockSize) * blockSize + blockSize - hitBoxOffsetX;

		// Reset velocity to prevent movement into the collision
		m_posComp->getVelx() = 0;

		return true;
	}
	return false;
}

inline bool CollisionComponent::apply_correction_in_RIGHT_COLLISION() {
	if (m_gameMap->getState(
		int((m_posComp->getx() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - hitBoxOffsetX - 1) / blockSize),
		int((oldYPos + COLSN_OFST + hitBoxTopOffset) / blockSize))
		|| m_gameMap->getState(
			int(((m_posComp->getx() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - hitBoxOffsetX - 1) / blockSize)),
			int(((oldYPos + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - 1) / blockSize)))) {

		// Correct collision: move back to the right boundary
		m_posComp->getx() =
			int((m_posComp->getx() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - hitBoxOffsetX - 1) / blockSize) * blockSize - AVERAGE_ENTITY_SIZE_PIXELS + hitBoxOffsetX;

		// Reset velocity to prevent movement into the collision
		m_posComp->getVelx() = 0;

		//log("Collision right");
		return true;
	}
	return false;
}

inline bool CollisionComponent::apply_correction_in_ABOVE_COLLISION() {
	if (m_gameMap->getState(
		int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / blockSize),
		int((m_posComp->gety() + COLSN_OFST + hitBoxTopOffset) / blockSize))
		|| m_gameMap->getState(
			int(((m_posComp->getx() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - hitBoxOffsetX - 1) / blockSize)),
			int((m_posComp->gety() + COLSN_OFST + hitBoxTopOffset) / blockSize))) {

		// Correct collision: move back to the top boundary
		m_posComp->gety() =
			int((m_posComp->gety() + COLSN_OFST + hitBoxTopOffset) / blockSize) * blockSize + blockSize - hitBoxTopOffset;

		// Reset velocity to prevent movement into the collision
		m_posComp->getVely() = 0;

		//log("Collision up");
		return true;
	}
	return false;
}

inline bool CollisionComponent::apply_correction_in_BELOW_COLLISION() {
	if (m_gameMap->getState(
		int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / blockSize),
		int((m_posComp->gety() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - 1) / blockSize))
		|| m_gameMap->getState(
			int(((m_posComp->getx() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - hitBoxOffsetX - 1) / blockSize)),
			int((m_posComp->gety() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - 1) / blockSize))) {

		// Correct collision: move back to the bottom boundary
		m_posComp->gety() =
			int((m_posComp->gety() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - 1) / blockSize) * blockSize - AVERAGE_ENTITY_SIZE_PIXELS;

		// Reset velocity to prevent movement into the collision
		m_posComp->getVely() = 0;

		//log("Collision down");
		return true;
	}
	return false;
}

inline bool CollisionComponent::apply_correction_in_MAP_EDGE_COLLISION() {
	// unsure if the use of buf is efficient :/

	bool collision = false;
	if (m_posComp->getx() < 0) {
		m_posComp->getx() = 0;
		m_posComp->getVelx() = 0;
		collision = true;
	}
	if (m_posComp->gety() < 0) {
		m_posComp->gety() = 0;
		m_posComp->getVely() = 0;
		collision = true;
	}
	buf = gameMapWidth_pixels - AVERAGE_ENTITY_SIZE_PIXELS;
	if (m_posComp->getx() > buf) {
		m_posComp->getx() = buf;
		m_posComp->getVelx() = 0;
		collision = true;
	}
	buf = gameMapHeight_pixels - AVERAGE_ENTITY_SIZE_PIXELS;
	if (m_posComp->gety() > buf) {
		m_posComp->gety() = buf;
		m_posComp->getVely() = 0;
		collision = true;
	}
	return collision;
}

// could be wildy optimized
bool CollisionComponent::correct_possible_collision() {

	bool collision = false;

	// may act up in the future... idk we'll see ------------------------------------------------------------
	// (int)m_posComp->getVely() is sketchy
	oldYPos = int(m_posComp->gety() - (int)m_posComp->getVely());

	// Offsets for hitbox
	hitBoxOffsetX = int(AVERAGE_ENTITY_SIZE_PIXELS * 0.25f);
	hitBoxTopOffset = int(AVERAGE_ENTITY_SIZE_PIXELS * 0.5f);

	// TODO: check for collision with edges of map

	// X-axis collision handling
	if (m_posComp->isMovingVelX()) {
		if (m_posComp->isMovingLeftVel()) {
			// Check collision for top left and bottom left points and apply correction in case of collsion
			// collision is set to true if correction is needed
			// this check is done with previous y position
			if (apply_correction_in_LEFT_COLLISION()) collision = true;
		}
		else if (m_posComp->isMovingRightVel()) {
			// Check collision for top right and bottom right points and apply correction in case of collsion
			// collision is set to true if correction is needed
			// this check is done with previous y position
			if (apply_correction_in_RIGHT_COLLISION()) collision = true;
		}
	}

	// Y-axis collision handling
	if (m_posComp->isMovingVelY()) {
		if (m_posComp->isMovingUpVel()) {
			// Check collision for top left and top right points and apply correction in case of collsion
			// collision is set to true if correction is needed
			// this check is done with updated y position
			if (apply_correction_in_ABOVE_COLLISION()) collision = true;
		}
		else if (m_posComp->isMovingDownVel()) {
			// Check collision for bottom left and bottom right points and apply correction in case of collsion
			// collision is set to true if correction is needed
			// this check is done with updated y position
			if (apply_correction_in_BELOW_COLLISION()) collision = true;
		}
	}

	// collision agains edges of map
	if (apply_correction_in_MAP_EDGE_COLLISION()) collision = true;

	return collision;
}
