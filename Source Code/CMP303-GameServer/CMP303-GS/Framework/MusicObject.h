// Music object class

#pragma once
#include "SFML\Audio.hpp"

class MusicObject
{
public:
	MusicObject();
	~MusicObject();

	// Store the music file path and key.
	void setMusic(std::string lfilename, std::string lname);

	// Music variables (shouldn't be public)
	std::string filename;
	std::string name;
};

