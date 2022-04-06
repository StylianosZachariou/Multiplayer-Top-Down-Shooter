// Game State Class
#pragma once

// Possible states. This enum can be modified to add additional states as required.
enum class State { MENU, CONTROL, LEVEL1, LEVEL2, LEVEL3, LEVEL4, PAUSE, DEATH, WIN };

class GameState
{
public:
	// Set the current state
	void setCurrentState(State s);
	// Returns the current state.
	State getCurrentState();

protected:
	State currentState;
};