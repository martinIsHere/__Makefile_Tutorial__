#pragma once
#include "includes.h"
#include "Camera.h"


class GameEngine;

class GameMap {
public:
	GameMap(
		SDL_Renderer* ren,
		const std::string map, 
		const std::string spriteSheetPath,
		const int spriteSheetTilesX,
		const int spriteSheetTilesY,
		const int windowWidth,
		const int windowHeight,
		int* cameraPosx, 
		int* cameraPosy,
		bool smallMap
		);

	GameMap();
	
	~GameMap();
	
	void loadMap(std::string map);

	const std::string getLoadedMapPath()const;

	void draw();

	void drawSecondLayer();

	void update();

	const int& getMapWidth_tiles();

	const int& getMapHeight_tiles();

	uint16_t getForegroundID(int x, int y);

	void setForegroundID(int x, int y, int id);

	uint16_t getBackID(int x, int y);

	void setBackID(int x, int y, int id);

	uint16_t getFrontID(int x, int y);

	void setFrontID(int x, int y, int id);

	bool getState(int x, int y);

	void setState(int x, int y, bool state);

	uint8_t& getBackMirrorState(int x, int y);

	uint8_t& getFrontMirrorState(int x, int y);

	uint8_t& getForegroundMirrorState(int x, int y);

	int& getBlockSize();

	Camera* getCam();

	bool& get_drawingSolidStates_bool();

	const int getXcoordOnscreen_fromMapcoord(const int x)const;

	const int getYcoordOnscreen_fromMapcoord(const int y)const;

	const int getCellx_fromCoord(const int x)const;

	const int getCelly_fromCoord(const int y)const;

private:

	std::vector <uint16_t> m_backLayer_array;
	std::vector <uint16_t> m_frontLayer_array;
	std::vector <uint16_t> m_foregroundLayer_array;
	std::vector <bool> m_blockState_array;
	std::vector <uint8_t> m_foregroundMirrorState_array;
	std::vector <uint8_t> m_frontMirrorState_array;
	std::vector <uint8_t> m_backMirrorState_array;

	std::string mapPath;

	SDL_Renderer* m_ren;

	SDL_Rect* m_tempSrcRect, * m_tempDstRect;

	int m_mapWidth, m_mapHeight;
	int m_windowWidth, m_windowHeight;
	uint16_t m_current_ID, m_xId, m_yId;
	uint8_t m_currentState;
	uint8_t m_currentMirrorState;
	int m_blockSize;

	// amount of visible tiles in pixels. tiles * TILE_SIZE_PIXELS
	int m_visibleTilesX_pixels, m_visibleTilesY_pixels;
	Camera* m_mainCamera;


	char m_readingBuffer[2];

	bool m_isDrawingSolidStates;

	std::ifstream m_mapFile;

	SpriteSheet* m_spriteSheet0;

};