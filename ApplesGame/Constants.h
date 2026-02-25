#pragma once
#include <string>

namespace ApplesGame
{
	const std::string RESOURCES_PATH = "Resources/";
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const float INITIAL_SPEED = 100.f; // Pixels per second
	const float PLAYER_SIZE = 20.f;
	const float ACCELERATION = 20.f; // Pixels per second
	const int NUM_APPLES = 20;
	const float APPLE_SIZE = 20.f;
	const float PAUSE_LENGTH = 3.f;
	const int NUM_ROCKS = 10;
	const float ROCK_SIZE = 15.f;

	// Resource file names (relative to RESOURCES_PATH)
	const std::string PLAYER_TEXTURE_FILE = "Player.png";
	const std::string APPLE_TEXTURE_FILE = "Apple.png";
	const std::string ROCK_TEXTURE_FILE = "Rock.png";
	const std::string EAT_SOUND_FILE = "AppleEat.wav";
	const std::string HIT_SOUND_FILE = "Death.wav";
	const std::string FONT_FILE = "Fonts/Roboto-Regular.ttf";
}
