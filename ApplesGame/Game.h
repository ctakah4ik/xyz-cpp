#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"

namespace ApplesGame
{
	enum class GameState
	{
		Playing,
		GameOver
	};

	struct Game
	{
		Player player;
		Apple apples[NUM_APPLES];
		Rock rocks[NUM_ROCKS];

		// Game state
		int score = 0;
		GameState state = GameState::Playing;
		float timeSinceGameOver = 0.f;

		// Visuals
		sf::RectangleShape background;

		// Resources
		sf::Texture playerTexture;
		sf::Texture appleTexture;
		sf::Texture rockTexture;
		sf::Font font;
		sf::Text scoreText;
		sf::Text controlsText;
		sf::Text gameOverText;

		// Sounds
		sf::SoundBuffer eatSoundBuffer;
		sf::Sound eatSound;
		sf::SoundBuffer hitSoundBuffer;
		sf::Sound hitSound;
	};

	void RestartGame(Game& game);
	void InitGame(Game& game);
	void UpdateGame(Game& game, float deltaTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinializeGame(Game& game);
}
