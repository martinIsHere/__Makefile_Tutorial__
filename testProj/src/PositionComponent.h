#pragma once
#include "includes.h"
#include "ECS.h"


class PositionComponent : public Component{
	// coordinates of entity in pixels on the map not the screen.
	int m_X, m_Y;

	uint32_t blockSize;
	// if unable to move -> acceleration and velocity will be set to zero 
	// and nothing else will be updated. Primarily used in animations.
	bool isAbleToMove;
	bool isFrictionless;
	Vector *m_velocity_vector;
	Vector *m_acceleration_vector;

	/*
	m_Directions describe the direction the entity will move.
	In update function, m_Directions will be used to determine 
	the acceleration and also then the velocity and position.
	*/
	std::array<bool, 4> m_Directions;

	// the direction that the entity is currently facing.
	uint32_t m_facingDir;

	float max_vel;
	float natural_deceleration;
	float default_acceleration;
	float default_natural_deceleration;

public:

	PositionComponent(int x, int y);

	PositionComponent(int x, int y, uint32_t blockSize);

	PositionComponent() = default;

	~PositionComponent();

	void init() override;

	int& getx();

	int& gety();

	float getMaxVel() const;

	void update() override;

	void draw() override;

	void moveForward(int d);

	void setPos(const int x, const int y);

	void setPos_tileCoords(const int x_inTiles, const int y_inTiles);

	void setAcc(const float& xAcc, const float& yAcc);
	
	void setDirX(const bool& left, const bool& right);

	void setDirY(const bool& up, const bool& down);

	void setDirX_left(const bool& dir);

	void setDirX_right(const bool& dir);

	void setDirY_up(const bool& dir);

	void setDirY_down(const bool& dir);

	void setDir(uint32_t dir, bool value);

	void set_isAbleToMove(bool a);

	void set_isFrictionless(bool a);

	bool isMoving();

	bool isMovingX();

	bool isMovingY();

	float getVelMagnitude();

	std::array<bool, 4>& getDir();

	uint32_t& getFacingDir();

	void setFacingDir(uint32_t dir); // doesnt have safety

	void setVel(const float x, const float y);

	void set_default_acceleration(float tilesPerSecond);

	float& getVelx();

	float& getVely();

	Vector* getVel_vector();

	bool isMovingLeftVel();

	bool isMovingRightVel();

	bool isMovingUpVel();

	bool isMovingDownVel();

	bool isMovingVel();

	bool isMovingVelX();

	bool isMovingVelY();

private: 

	void apply_natural_deceleration();

	void update_current_acceleration();

	void cap_velocity();

};
