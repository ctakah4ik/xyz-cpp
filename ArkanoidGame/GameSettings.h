#pragma once
#include <string>

namespace ArkanoidGame
{
	// Resources path
	const std::string RESOURCES_PATH = "Resources/";
	const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
	const std::string SOUNDS_PATH = RESOURCES_PATH + "Sounds/";

	// Screen
	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEIGHT = 600;
	const float TIME_PER_FRAME = 1.f / 60.f;

	// Platform
	const float PLATFORM_WIDTH = 120.f;
	const float PLATFORM_HEIGHT = 20.f;
	const float PLATFORM_SPEED = 500.f;
	const float PLATFORM_Y_OFFSET = 40.f; // distance from bottom

	// Ball
	const float BALL_RADIUS = 8.f;
	const float BALL_INITIAL_SPEED_X = 200.f;
	const float BALL_INITIAL_SPEED_Y = -300.f;

	// Records
	const int MAX_RECORDS_TABLE_SIZE = 5;
	const char* const PLAYER_NAME = "Player";
}
