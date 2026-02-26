#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <vector>
#include "Leaderboard.h"
#include "Constants.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"

namespace ApplesGame
{
	enum class GameState
	{
		MainMenu,
		Leaderboard,
		Playing,
		PauseMenu,
		GameOver,
		Win
	};

	struct Game
	{
		Player player;

		std::vector<Apple> apples;
		std::vector<Rock> rocks;

		// Game mode bitmask (combination of GAME_MODE_* flags)
		int gameMode = GAME_MODE_INFINITE_APPLES | GAME_MODE_WITH_ACCELERATION;

		// Game state
		int score = 0;
		GameState state = GameState::MainMenu;
		float timeSinceGameOver = 0.f;
		int pauseSelectedItem = 0; // 0 = Resume, 1 = Exit to menu

		// Leaderboard: key = name, value = score + player flag
		std::unordered_map<std::string, LeaderboardEntry> leaderboard;

		// Visuals
		sf::RectangleShape background;
		sf::RectangleShape pauseOverlay;

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
		sf::Text leaderboardHintText;
		sf::Text pauseResumeText;
		sf::Text pauseExitText;

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
