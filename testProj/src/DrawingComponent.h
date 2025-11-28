#pragma once
#include "includes.h"
#include "ECS.h"
#include "PositionComponent.h"
#include "Camera.h"

class DrawingComponent : public Component{
	SDL_Rect* m_destRect, * m_srcRect;
	SDL_Renderer* m_ren;
	SpriteSheet* m_spriteSheet;
	PositionComponent* m_posComp;
	enum animationType {walkingAnimation, swordSwingAnimation, customAnimation};
	int currentAnimationType;
	int m_animationTick;
	unsigned int m_FPS;
	int m_animationFramesPerState;
	unsigned int m_framesPerImage;
	unsigned int m_frameCounter;
	unsigned int m_facingDir;
	const int* m_cameraOffsetX, * m_cameraOffsetY;
	const Camera* m_cam;
	int startX; // for animation. the x value to draw from the source
	int amountOfRepetitions; // for animation. 
	int amountOfFramesForCustomAnimation;
	int currentRepetition;
	int yValueBeforeCustomAnimation;
	int prevCustomAnimationY;
	bool isAbleToMoveDuringAnimation; // for custom animation
	bool customAnimationHasEnded_notifier;

	// versatile buffer
	int buf;

public:
	DrawingComponent(
		SDL_Renderer* inputRen, // for drawing
		const std::string spriteTitle, // path of texture sheet
		const int walkSpriteWidthNHeight,
		const int spriteSheetColumns, 
		const int spriteSheetRows, 
		const int amountOfWalkingAnimationFrames,
		const unsigned int fps, 
		const Camera* cam
		);

	~DrawingComponent();

	void update() override;

	void draw() override;

	void init() override;

	void loadNewCamera(Camera* m_cam);

	const void initCustomAnimation(
		int customAnimationY,
		int startX,
		int amountOfFrames,
		int amountOfRepetitions,
		bool isAbleToMoveDuringAnimation
		);

	const bool customAnimationHasEnded();

	const int getPrevCustomAnimationY() const;

	const Camera* getCurrentlyUsedCamPtr()const;

private:
	void update_walkingAnimation();

	void update_customAnimation();

	void draw_WalkingAnimationframe_according_to_direction();

	void draw_customAnimationFrame();

	const bool is_in_viewable_area()const;

};

