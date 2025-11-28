#include "GameMap.h"


/*

Map structure

2 bytes - map width
2 bytes - map height

repeating:
	
	2 bytes foregourndLayer id TODO
	2 bytes frontLayer id
	2 bytes backLayer id

	1 byte: {
	7
	6
	5
	4 = mirrorState 2 - back
	3 = mirrorState 2
	2 = mirrorState 1 - front
	1 = mirrorState 1
	0 = solidState
	}

	NOTE -> mirrorStates:
	0 = nothing
	1 = vertical mirroring
	2 = horizontal mirroring
	3 = both
*/

void GameMap::loadMap(std::string map) {
	
	m_mapFile.open(map, std::fstream::in | std::fstream::binary);

	// get first 2 bytes
	m_mapFile.read(m_readingBuffer, 2);
	m_mapWidth = m_readingBuffer[1] & 0xFFFF;


	// get 2 bytes for height
	m_mapFile.read(m_readingBuffer, 2);
	m_mapHeight = m_readingBuffer[1] & 0xFFFF;

	//m_frontLayer_array = new uint16_t[m_mapWidth*m_mapHeight];
	//m_backLayer_array = new uint16_t[m_mapWidth*m_mapHeight];
	//m_blockState_array = new bool[m_mapWidth * m_mapHeight];
	
	for (int y = 0; y < m_mapHeight; y++) {
		for (int x = 0; x < m_mapWidth; x++) {
			
			// foreground id
			m_mapFile.read(m_readingBuffer, 2);
			m_current_ID = (m_readingBuffer[0] & 0xFF) << 8;
			m_current_ID += m_readingBuffer[1] & 0xFF;
			m_foregroundLayer_array.push_back(m_current_ID);

			// front id
			m_mapFile.read(m_readingBuffer, 2);
			m_current_ID = (m_readingBuffer[0] & 0xFF)<<8;
			m_current_ID += m_readingBuffer[1] & 0xFF;
			m_frontLayer_array.push_back(m_current_ID);
			//m_frontLayer_array[x*y] = m_current_ID;

			// back id
			m_mapFile.read(m_readingBuffer, 2);
			m_current_ID = (m_readingBuffer[0] & 0xFF) << 8;
			m_current_ID += m_readingBuffer[1] & 0xFF;
			m_backLayer_array.push_back(m_current_ID);
			//m_backLayer_array[x * y] = m_current_ID;


			// TODO : add mirror bit
			// state id
			m_mapFile.read(m_readingBuffer, 1);

			m_currentState = m_readingBuffer[0] & 0b1;
			m_blockState_array.push_back(m_currentState == 1);
			
			m_currentMirrorState = (uint8_t)((m_readingBuffer[0] >> 1) & 0b11);
			m_backMirrorState_array.push_back(m_currentMirrorState);

			m_currentMirrorState = (uint8_t)((m_readingBuffer[0] >> 3) & 0b11);
			m_frontMirrorState_array.push_back(m_currentMirrorState);

			m_currentMirrorState = (uint8_t)((m_readingBuffer[0] >> 5) & 0b11);
			m_foregroundMirrorState_array.push_back(m_currentMirrorState);
			
			//m_blockState_array[x * y] = m_currentState;

		}
	}
	
	m_mapFile.close();
}
GameMap::GameMap() {
	m_ren = nullptr;
	m_tempSrcRect = nullptr;
	m_tempDstRect = nullptr;
	m_mapWidth = NULL;
	m_mapHeight = NULL;
	m_windowWidth = NULL;
	m_windowHeight = NULL;
	m_current_ID = NULL;
	m_xId = NULL;
	m_yId = NULL;
	m_currentState = NULL;
	m_currentMirrorState = NULL;
	m_blockSize = TILE_SIZE_PIXELS;
	m_visibleTilesX_pixels = NULL;
	m_visibleTilesY_pixels = NULL;
	m_mainCamera = nullptr;
	m_readingBuffer[0] = {NULL};
	m_readingBuffer[1] = {NULL};
	m_isDrawingSolidStates = false;
	m_spriteSheet0 = nullptr;
}

GameMap::GameMap(
	SDL_Renderer* ren, 
	const std::string map, 
	const std::string spriteSheetPath,
	const int spriteSheetTilesX,
	const int spriteSheetTilesY,
	const int windowWidth,
	const int windowHeight,
	int *cameraPosx, 
	int *cameraPosy,
	bool smallMap
	) {
	m_ren = ren;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	// buffers for drawing
	m_xId = 0;
	m_yId = 0;
	m_current_ID = 0;
	m_currentState = 0;
	m_isDrawingSolidStates = false;

	// block width and height in pixels
	m_blockSize = TILE_SIZE_PIXELS;

	// sdl renderer
	m_ren = m_ren;

	// sdl rect structs
	m_tempSrcRect = new SDL_Rect;
	m_tempDstRect = new SDL_Rect;

	//load spriteSheet
	m_spriteSheet0 = 
		new SpriteSheet{
		SDL_CreateTextureFromSurface(m_ren, SDL_LoadBMP(spriteSheetPath.c_str())),
		16, 
		spriteSheetTilesX,   
		spriteSheetTilesX   
		};

	if (!m_spriteSheet0->tex) log("Failed to load texture.");

	// load map
	loadMap(map);
	mapPath = map;

	// create camera at players position
	m_mainCamera = new Camera(
		cameraPosx, cameraPosy, // position
		m_mapWidth, m_mapHeight, // map size (for bounds)
		m_windowWidth, m_windowHeight, // window size (for bounds)
		smallMap  // if it is a small map i.e. in one screen
		);

	m_visibleTilesX_pixels = m_mainCamera->getVisibleTilesX() * TILE_SIZE_PIXELS;
	m_visibleTilesY_pixels = m_mainCamera->getVisibleTilesY() * TILE_SIZE_PIXELS;
}



/*
//temp   = SDL_LoadBMP("sprite.bmp");
sprite = SDL_DisplayFormat(SDL_LoadBMP("sprite.bmp"));
SDL_FreeSurface(temp);
*/

GameMap::~GameMap() {
	delete m_spriteSheet0, m_tempSrcRect, m_tempDstRect, m_mainCamera;
}

const std::string GameMap::getLoadedMapPath() const {
	return mapPath;
}

void GameMap::update() {
	m_mainCamera->update();
}

int mousePositionX;
int mousePositionY;

SDL_Rect* dstRect2 = new SDL_Rect;

const int& GameMap::getMapWidth_tiles(){
	return m_mapWidth;
}

const int& GameMap::getMapHeight_tiles() {
	return m_mapHeight;
}

void GameMap::draw() {

	// offset from camera body to top left corner of viewing area
	int offsetX = *m_mainCamera->getOffsetXPtr();
	int offsetY = *m_mainCamera->getOffsetYPtr();

	for (int y = int(offsetY / m_blockSize); y < (m_visibleTilesY_pixels + offsetY) / m_blockSize; y++) {
		for (int x = int(offsetX / m_blockSize); x < (m_visibleTilesX_pixels + offsetX) / m_blockSize; x++) {
			*m_tempDstRect = { 
				int((x * m_blockSize) - offsetX),
				int((y * m_blockSize) - offsetY),
				int(m_blockSize),
				int(m_blockSize) 
			};

			// back layer
			int id = getBackID(x, y);
			if (id != 0) {
				m_xId = id % m_spriteSheet0->nWidth;
				m_yId = (int)(id / m_spriteSheet0->nWidth);
				*m_tempSrcRect = {
					m_xId* m_spriteSheet0->nSize,
					m_yId* m_spriteSheet0->nSize,
					m_spriteSheet0->nSize, 
					m_spriteSheet0->nSize
				};
				
				SDL_RenderCopyEx(
					m_ren,
					m_spriteSheet0->tex,
					m_tempSrcRect, m_tempDstRect,
					0, 0,
					(SDL_RendererFlip)getBackMirrorState(x, y)
					);
				
				
			}

			// draw frontlayer
			id = getFrontID(x, y);
			if (id != 0) {
				m_xId = id % m_spriteSheet0->nWidth;
				m_yId = (int)(id / m_spriteSheet0->nWidth);
				*m_tempSrcRect = {
					m_xId * m_spriteSheet0->nSize, 
					m_yId * m_spriteSheet0->nSize,
					m_spriteSheet0->nSize,
					m_spriteSheet0->nSize 
				};

				SDL_RenderCopyEx(
					m_ren, 
					m_spriteSheet0->tex,
					m_tempSrcRect, m_tempDstRect, 
					0, 0, 
					(SDL_RendererFlip)getFrontMirrorState(x, y)
					);
				
				
			}
			if (m_isDrawingSolidStates) {
				if (getState(x, y)) {
					SDL_SetRenderDrawColor(m_ren, 255, 0, 0, 255);
					SDL_RenderDrawRect(m_ren, m_tempDstRect);
				}
			}

		}
	}
	
}

void GameMap::drawSecondLayer() {

	int offsetX = *m_mainCamera->getOffsetXPtr();
	int offsetY = *m_mainCamera->getOffsetYPtr();

	SDL_GetMouseState(&mousePositionX, &mousePositionY);
	mousePositionX += offsetX;
	mousePositionY += offsetY;

	dstRect2->x = (mousePositionX >= 0 ? int(mousePositionX / TILE_SIZE_PIXELS) : int(mousePositionX / TILE_SIZE_PIXELS) - 1) * TILE_SIZE_PIXELS - offsetX;
	dstRect2->y = (mousePositionY >= 0 ? int(mousePositionY / TILE_SIZE_PIXELS) : int(mousePositionY / TILE_SIZE_PIXELS) - 1) * TILE_SIZE_PIXELS - offsetY;
	dstRect2->w = TILE_SIZE_PIXELS;
	dstRect2->h = TILE_SIZE_PIXELS;

	for (int y = (int)(offsetY / m_blockSize); y < (m_visibleTilesY_pixels + offsetY) / m_blockSize; y++) {
		for (int x = (int)(offsetX / m_blockSize); x < (m_visibleTilesX_pixels + offsetX) / m_blockSize; x++) {


			*m_tempDstRect = {
				int((x * m_blockSize) - offsetX),
				int((y * m_blockSize) - offsetY),
				int(m_blockSize),
				int(m_blockSize)
			};

			// foreground layer
			int id = getForegroundID(x, y);
			if (id != 0) {
				m_xId = id % m_spriteSheet0->nWidth;
				m_yId = (int)(id / m_spriteSheet0->nWidth);
				*m_tempSrcRect = {
					m_xId * m_spriteSheet0->nSize,
					m_yId * m_spriteSheet0->nSize,
					m_spriteSheet0->nSize,
					m_spriteSheet0->nSize
				};

				SDL_RenderCopyEx(
					m_ren,
					m_spriteSheet0->tex,
					m_tempSrcRect, m_tempDstRect,
					0, 0,
					(SDL_RendererFlip)getForegroundMirrorState(x, y)
					);


			}

		}
	}

	SDL_SetRenderDrawColor(m_ren, 0, 255, 255, 255);
	SDL_RenderDrawRect(m_ren, dstRect2);

}

uint16_t GameMap::getForegroundID(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight)) {
		return m_foregroundLayer_array[(size_t(y) * m_mapWidth) + x];
	}
	return 0;
}
void GameMap::setForegroundID(int x, int y, int ID) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		m_foregroundLayer_array[(size_t(y) * m_mapWidth) + x] = ID;
}

uint16_t GameMap::getBackID(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight)) {
		return m_backLayer_array[(size_t(y) * m_mapWidth) + x];
	}
	return 0;
}
void GameMap::setBackID(int x, int y, int ID) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		m_backLayer_array[(size_t(y) * m_mapWidth) + x] = ID;
}
uint16_t GameMap::getFrontID(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight)) {
		return m_frontLayer_array[(size_t(y) * m_mapWidth) + x];
	}
	return 0;
}
void GameMap::setFrontID(int x, int y, int ID) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		m_frontLayer_array[(size_t(y) * m_mapWidth) + x] = ID;
}
bool GameMap::getState(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight)) {
		return m_blockState_array[(size_t(y) * m_mapWidth) + x];
	}
	return 0;
}
void GameMap::setState(int x, int y, bool state) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		m_blockState_array[(size_t(y) * m_mapWidth) + x] = state;
}
uint8_t& GameMap::getBackMirrorState(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight)) {
		return m_backMirrorState_array[(size_t(y) * m_mapWidth) + x];
	}
	return m_backMirrorState_array[0];
}
uint8_t& GameMap::getFrontMirrorState(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight)) {
		return m_frontMirrorState_array[(size_t(y) * int(m_mapWidth)) + x];
	}
	return m_frontMirrorState_array[0];
}
uint8_t& GameMap::getForegroundMirrorState(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight)){
		return m_foregroundMirrorState_array[(size_t(y) * int(m_mapWidth)) + x];
	}
	return m_foregroundMirrorState_array[0];
}
int& GameMap::getBlockSize() {
	return m_blockSize;
}

Camera* GameMap::getCam() {
	return m_mainCamera;
}

bool& GameMap::get_drawingSolidStates_bool() {
	return m_isDrawingSolidStates;
}

const int GameMap::getXcoordOnscreen_fromMapcoord(const int x) const {
	return x * m_blockSize;
}

const int GameMap::getYcoordOnscreen_fromMapcoord(const int y) const {
	return y * m_blockSize;
}

const int GameMap::getCellx_fromCoord(const int x)const {
	if(x >= 0)
		return int(x / m_blockSize);
	if (x < 0) 
		return int(x / m_blockSize)-1;
}

const int GameMap::getCelly_fromCoord(const int y)const {
	if (y >= 0)
		return int(y / m_blockSize);
	if (y < 0)
		return int(y / m_blockSize) - 1;
}