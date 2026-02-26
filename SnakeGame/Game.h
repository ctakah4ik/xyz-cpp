#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "Constants.h"
#include "Snake.h"
#include "Apple.h"
#include "Leaderboard.h"

namespace SnakeGame
{
	enum class GameState
	{
		MainMenu,          // Start, Difficulty, Leaderboard, Settings, Exit
		DifficultySelect,  // Choose difficulty 1-5
		LeaderboardScreen, // View records
		SettingsScreen,    // Sound/Music toggles
		Countdown,         // T-second delay before playing
		Playing,           // Active gameplay
		PauseMenu,         // Pause popup (Continue / Exit)
		GameOverPopup,     // Death popup with score + records + buttons
		NameEntry          // Enter name for leaderboard
	};

	struct Game
	{
		Snake snake;
		Apple apple;

		// Game params
		int score = 0;
		int difficultyLevel = 0; // index into DIFFICULTY_LEVELS
		GameState state = GameState::MainMenu;

		// Countdown
		float countdownTimer = 0.f;

		// Settings
		bool soundEnabled = true;
		bool musicEnabled = true;

		// Menu navigation (selected item index)
		int menuSelectedItem = 0;

		// GameOver popup navigation: 0 = Start game, 1 = Go to menu
		int gameOverSelectedItem = 0;

		// Pause popup: 0 = Continue, 1 = Exit
		int pauseSelectedItem = 0;

		// Name entry
		std::string playerName;
		bool nameEntryAsked = false; // are we asking Yes/No to save?
		int nameEntryChoice = 1;     // 0 = Yes, 1 = No (default No)

		// Leaderboard
		Leaderboard leaderboard;

		// Resources
		sf::Font font;

		// Sounds (short one-shot effects)
		sf::SoundBuffer buttonBuffer;
		sf::Sound       buttonSound;
		sf::SoundBuffer eatBuffer;
		sf::Sound       eatSound;
		sf::SoundBuffer startBuffer;
		sf::Sound       startSound;
		sf::SoundBuffer finishBuffer;
		sf::Sound       finishSound;

		// Background music (streams from file)
		sf::Music sessionMusic;

		// Textures
		sf::Texture appleTexture;
		sf::Texture headTextures[4];    // Right, Up, Left, Down
		sf::Texture bodyHorizontal;
		sf::Texture bodyVertical;
		sf::Texture bodyTopLeft;
		sf::Texture bodyTopRight;
		sf::Texture bodyBottomLeft;
		sf::Texture bodyBottomRight;
		sf::Texture tailTextures[4];    // Right, Up, Left, Down

		// Visuals
		sf::RectangleShape background;
	};

	void InitGame(Game& game);
	void HandleGameEvent(Game& game, const sf::Event& event);
	void UpdateGame(Game& game, float deltaTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
}
