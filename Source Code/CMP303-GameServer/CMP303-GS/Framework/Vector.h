// This vector class provides a few addition vector functions for working with sf::Vector2f

#pragma once
#include "SFML\System\Vector2.hpp"
#include <math.h>

class Vector
{
public:
	// Returns a normalised vector
	static sf::Vector2f normalise(const sf::Vector2f &source);
	// Returns the Vector magnitude
	static float magnitude(sf::Vector2f vec);

};

