#include "InteractionComponent.h"

InteractionComponent::InteractionComponent(GameMap* gameMap, Point* interactionCoords, bool* hasInteracted) {
	m_gameMap = gameMap;
	m_posComp = nullptr;
	m_hasInteracted = hasInteracted;
	this->interactionCoords = interactionCoords;
}



void InteractionComponent::init() {
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
}
InteractionComponent::~InteractionComponent() {

}

void InteractionComponent::update() {
	if (m_posComp->getFacingDir() == DIR_RIGHT) {
		interactionCoords->x = m_gameMap->getCellx_fromCoord(m_posComp->getx() + int(AVERAGE_ENTITY_SIZE_PIXELS / 2) + m_gameMap->getBlockSize() * interactionDistance);
		interactionCoords->y = m_gameMap->getCelly_fromCoord(m_posComp->gety() + int(AVERAGE_ENTITY_SIZE_PIXELS / 2));
	} 
	else if (m_posComp->getFacingDir() == DIR_LEFT) {
		interactionCoords->x = m_gameMap->getCellx_fromCoord(m_posComp->getx() + int(AVERAGE_ENTITY_SIZE_PIXELS / 2) - m_gameMap->getBlockSize() * interactionDistance);
		interactionCoords->y = m_gameMap->getCelly_fromCoord(m_posComp->gety() + int(AVERAGE_ENTITY_SIZE_PIXELS / 2));
	}
	else if (m_posComp->getFacingDir() == DIR_UP) {
		interactionCoords->x = m_gameMap->getCellx_fromCoord(m_posComp->getx() + int(AVERAGE_ENTITY_SIZE_PIXELS / 2));
		interactionCoords->y = m_gameMap->getCelly_fromCoord(m_posComp->gety() + int(AVERAGE_ENTITY_SIZE_PIXELS / 2) - m_gameMap->getBlockSize() * interactionDistance);
	}
	else { // if DIR_DOWN
		interactionCoords->x = m_gameMap->getCellx_fromCoord(m_posComp->getx() + int(AVERAGE_ENTITY_SIZE_PIXELS / 2));
		interactionCoords->y = m_gameMap->getCelly_fromCoord(m_posComp->gety() + int(AVERAGE_ENTITY_SIZE_PIXELS / 2) + m_gameMap->getBlockSize() * interactionDistance);
	}
}

void InteractionComponent::draw() {
}

void InteractionComponent::interact() {
	*m_hasInteracted = true;
}