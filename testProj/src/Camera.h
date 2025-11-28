#pragma once
#include "includes.h"

class Camera{

	int m_xCameraOffset, m_yCameraOffset;
	int* m_xPos, *m_yPos;
	int m_xOffsetFromScreenEdge, m_yOffsetFromScreenEdge;
	int m_winWidth, m_winHeight;
	int m_mapWidth, m_mapHeight;
	int m_visibleTilesX, m_visibleTilesY;
	bool m_isSmallMap;

public:

	Camera(
		int* x, 
		int* y, 
		const int mapWidth,
		const int mapHeight,
		const int winWidth,
		const int winHeight,
		bool isSmallMap
		);

	Camera() = default;
	
	void update();

	void update(const int& x, const int& y);

	const int& getVisibleTilesX() const;

	const int& getVisibleTilesY() const;

	const int* getOffsetXPtr()const;

	const int* getOffsetYPtr()const;

	uint32_t getWinWidth()const;

	uint32_t getWinHeight()const;

	int translateXcoordFromWinToMap(const int x) const;

	int translateYcoordFromWinToMap(const int y) const;
};

