#pragma once
#include <string>

namespace SnakeGame
{
	// Window
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const std::string GAME_TITLE = "Snake Game";

	// HUD strip at the top
	const int HUD_HEIGHT = 40; // pixels reserved for score display
	const int GRID_OFFSET_Y = HUD_HEIGHT;

	// Grid
	const int CELL_SIZE = 20;
	const int GRID_WIDTH  = SCREEN_WIDTH / CELL_SIZE;                    // 40
	const int GRID_HEIGHT = (SCREEN_HEIGHT - HUD_HEIGHT) / CELL_SIZE;   // 28

	// Snake
	const int INITIAL_SNAKE_LENGTH = 3;

	// Difficulty levels (1-5)
	struct DifficultyParams
	{
		const char* name;
		float speed;        // cells per second
		int pointsPerApple;
	};

	const int NUM_DIFFICULTY_LEVELS = 5;
	const DifficultyParams DIFFICULTY_LEVELS[NUM_DIFFICULTY_LEVELS] =
	{
		{ "Simple",             3.0f,  2 },
		{ "Harder than simple", 5.0f,  4 },
		{ "Medium",             7.0f,  6 },
		{ "Easier than hard",  10.0f,  8 },
		{ "Hard",              13.0f, 10 }
	};

	// Growth per apple
	const int GROWTH_PER_APPLE = 1;

	// Delay before session starts and after unpause
	const float SESSION_START_DELAY = 3.0f;

	// Leaderboard display counts
	const int LEADERBOARD_POPUP_COUNT = 5;
	const int LEADERBOARD_SCREEN_COUNT = 10;
	const int MAX_LEADERBOARD_ENTRIES = 10;

	// Default player name
	const std::string DEFAULT_PLAYER_NAME = "XYZ";
	const int MAX_NAME_LENGTH = 15;

	// Resources
	const std::string RESOURCES_PATH = "Resources/";
	const std::string LEADERBOARD_FILE = "leaderboard.dat";
	const std::string FONT_FILE = "Fonts/Roboto-Regular.ttf";

	// Sound files
	const std::string SFX_BUTTON = "button.wav";
	const std::string SFX_EAT    = "eat.wav";
	const std::string SFX_START  = "start.wav";
	const std::string SFX_FINISH = "finish.wav";
	const std::string MUSIC_SESSION = "main.wav";

	// Textures
	const std::string APPLE_TEXTURE = "apple.png";
	const std::string HEAD_UP_TEXTURE = "head_up.png";
	const std::string HEAD_DOWN_TEXTURE = "head_down.png";
	const std::string HEAD_LEFT_TEXTURE = "head_left.png";
	const std::string HEAD_RIGHT_TEXTURE = "head_right.png";
	const std::string BODY_HORIZONTAL_TEXTURE = "body_horizontal.png";
	const std::string BODY_VERTICAL_TEXTURE = "body_vertical.png";
	const std::string BODY_TOPLEFT_TEXTURE = "body_topleft.png";
	const std::string BODY_TOPRIGHT_TEXTURE = "body_topright.png";
	const std::string BODY_BOTTOMLEFT_TEXTURE = "body_bottomleft.png";
	const std::string BODY_BOTTOMRIGHT_TEXTURE = "body_bottomright.png";
	const std::string TAIL_UP_TEXTURE = "tail_up.png";
	const std::string TAIL_DOWN_TEXTURE = "tail_down.png";
	const std::string TAIL_LEFT_TEXTURE = "tail_left.png";
	const std::string TAIL_RIGHT_TEXTURE = "tail_right.png";
}
