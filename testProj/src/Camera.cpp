#include "Camera.h"

Camera::Camera(int* x, int* y, const int mapWidth, const int mapHeight,
               const int winWidth, const int winHeight, bool isSmallMap) {
  m_mapWidth = mapWidth;
  m_mapHeight = mapHeight;

  m_winWidth = winWidth;
  m_winHeight = winHeight;
  m_xPos = x;
  m_yPos = y;
  m_xCameraOffset = 0;
  m_yCameraOffset = 0;
  m_xOffsetFromScreenEdge = (int)((float)winWidth * 0.5f);
  m_yOffsetFromScreenEdge = (int)((float)winHeight * 0.5f);
  m_visibleTilesX = int((float)winWidth / (float)TILE_SIZE_PIXELS) +
                    1;  // one extra in case of decimal
  m_visibleTilesY = int((float)winHeight / (float)TILE_SIZE_PIXELS) + 1;
  m_isSmallMap = isSmallMap;
}

const int& Camera::getVisibleTilesX() const { return m_visibleTilesX; }

const int& Camera::getVisibleTilesY() const { return m_visibleTilesY; }

void Camera::update() {
  m_xCameraOffset = *m_xPos - m_xOffsetFromScreenEdge;
  m_yCameraOffset = *m_yPos - m_yOffsetFromScreenEdge;

  if (!m_isSmallMap) {
    // check if offset is behind 0
    if (m_xCameraOffset < 0) {
      m_xCameraOffset = 0;
    }
    if (m_yCameraOffset < 0) {
      m_yCameraOffset = 0;
    }

    // check if offset + screen size is more than the map's length
    if (m_xCameraOffset > int((m_mapWidth * TILE_SIZE_PIXELS) - m_winWidth)) {
      m_xCameraOffset = (m_mapWidth * TILE_SIZE_PIXELS) - m_winWidth;
    }
    if (m_yCameraOffset > int((m_mapHeight * TILE_SIZE_PIXELS) - m_winHeight)) {
      m_yCameraOffset = (m_mapHeight * TILE_SIZE_PIXELS) - m_winHeight;
    }
  }
}

const int* Camera::getOffsetXPtr() const { return &m_xCameraOffset; }

const int* Camera::getOffsetYPtr() const { return &m_yCameraOffset; }

uint32_t Camera::getWinWidth() const { return m_winWidth; }

uint32_t Camera::getWinHeight() const { return m_winHeight; }

int Camera::translateXcoordFromWinToMap(const int x) const {
  return x + m_xCameraOffset;
}

int Camera::translateYcoordFromWinToMap(const int y) const {
  return y + m_yCameraOffset;
}
