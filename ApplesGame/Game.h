#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <vector>
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"

namespace ApplesGame
{
	enum class GameState
	{
		MainMenu,
		Playing,
		GameOver,
		Win
	};

	struct LeaderboardEntry
	{
		std::string name;
		int score = 0;
		bool isPlayer = false;
	};

	struct Game
	{
		Player player;

		// Dynamic apple array
		Apple* apples = nullptr;
		int numApples = 0;

		Rock rocks[NUM_ROCKS];

		// Game mode bitmask (combination of GAME_MODE_* flags)
		int gameMode = GAME_MODE_INFINITE_APPLES | GAME_MODE_WITH_ACCELERATION;

		// Game state
		int score = 0;
		GameState state = GameState::MainMenu;
		float timeSinceGameOver = 0.f;

		// Leaderboard (persists across restarts, regenerated on full init)
		std::vector<LeaderboardEntry> leaderboard;

		// Visuals
		sf::RectangleShape background;

		// Resources
		sf::Texture playerTexture;
		sf::Texture appleTexture;
		sf::Texture rockTexture;
		sf::Font font;

		// UI texts
		sf::Text scoreText;
		sf::Text controlsText;
		sf::Text gameOverText;
		sf::Text winText;
		sf::Text menuText;
		sf::Text leaderboardText;

		// Sounds
		sf::SoundBuffer eatSoundBuffer;
		sf::Sound eatSound;
		sf::SoundBuffer hitSoundBuffer;
		sf::Sound hitSound;
	};

	void HandleGameEvent(Game& game, const sf::Event& event);
	void RestartGame(Game& game);
	void InitGame(Game& game);
	void UpdateGame(Game& game, float deltaTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinializeGame(Game& game);
}
