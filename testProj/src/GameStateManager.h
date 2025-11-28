#pragma once
#include "includes.h"

class GameStateManager{

public:
	// all possible gamestates
	enum GameState : uint32_t {
		state_noState,
		state_currentlyLoading,
		state_startingScreen,
		state_startToGame_fade,
		state_gameRunning,
		state_blockTransition,
		state_theaterMode,
		state_win,
		state_death,
		state_pauseMenu,
		state_mainMenu,
		state_optionMenu,
		state_aboutMenu
	};
	
private:
	//current gamestate
	GameState* state;

public:

	GameStateManager();

	~GameStateManager();

	// setget
	void set(GameState state);

	GameState get();

	GameState* getPtr();
};

