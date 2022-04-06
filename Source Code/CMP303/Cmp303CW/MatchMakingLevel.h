#pragma once
#include "Framework\Input.h"
#include <SFML/Graphics.hpp>

class MatchMakingLevel
{
public:

	MatchMakingLevel(sf::RenderWindow* hwnd, Input* in, int id);

	//Functions for "Game Loop"
	bool handleInput();
	void update();
	void render();
	bool getReady();
private:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	// Default variables for level class.
	sf::RenderWindow* window;
	Input* input;
	bool ready = false;
	int playerID = 0;

	//Objects
	sf::Font font;

	sf::Text playersJoined;
	sf::RectangleShape background;
	sf::RectangleShape player;
	sf::Texture playerTexture;
};

