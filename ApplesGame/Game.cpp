#include "Game.h"
#include <cassert>
#include <iostream>

namespace ApplesGame
{
	void RestartGame(Game& game)
	{
		InitPlayer(game.player, game);

		// Init apples
		for (int i = 0; i < NUM_APPLES; ++i)
		{
			InitApple(game.apples[i], game.appleTexture);
		}

		// Init rocks
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			InitRock(game.rocks[i], game.rockTexture);
		}

		game.score = 0;
		game.state = GameState::Playing;
		game.timeSinceGameOver = 0.f;
	}

	void InitGame(Game& game)
	{
		// Load resources
		if (!game.playerTexture.loadFromFile(RESOURCES_PATH + PLAYER_TEXTURE_FILE))
		{
			std::cerr << "Failed to load player texture\n";
		}
		if (!game.appleTexture.loadFromFile(RESOURCES_PATH + APPLE_TEXTURE_FILE))
		{
			std::cerr << "Failed to load apple texture\n";
		}
		if (!game.rockTexture.loadFromFile(RESOURCES_PATH + ROCK_TEXTURE_FILE))
		{
			std::cerr << "Failed to load rock texture\n";
		}

		if (!game.font.loadFromFile(RESOURCES_PATH + FONT_FILE))
		{
			std::cerr << "Failed to load font\n";
		}

		// Sounds
		if (!game.eatSoundBuffer.loadFromFile(RESOURCES_PATH + EAT_SOUND_FILE))
		{
			std::cerr << "Failed to load eat sound\n";
		}
		else
		{
			game.eatSound.setBuffer(game.eatSoundBuffer);
		}

		if (!game.hitSoundBuffer.loadFromFile(RESOURCES_PATH + HIT_SOUND_FILE))
		{
			std::cerr << "Failed to load hit sound\n";
		}
		else
		{
			game.hitSound.setBuffer(game.hitSoundBuffer);
		}

		// Background
		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		// UI
		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(24);
		game.scoreText.setFillColor(sf::Color::White);
		game.scoreText.setPosition(10.f, 10.f);

		game.controlsText.setFont(game.font);
		game.controlsText.setCharacterSize(18);
		game.controlsText.setFillColor(sf::Color::White);
		game.controlsText.setString("Controls: WASD or arrows to move. Esc to quit.");
		game.controlsText.setPosition(10.f, 40.f);

		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(72);
		game.gameOverText.setFillColor(sf::Color::Red);
		game.gameOverText.setString("GAME OVER");
	
	auto bounds = game.gameOverText.getLocalBounds();
		game.gameOverText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		game.gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

		RestartGame(game);
	}

	void UpdateGame(Game& game, float deltaTime)
	{
		if (game.state == GameState::Playing)
		{
			// Update player
			UpdatePlayer(game.player, deltaTime);

			// Apples collisions
			for (int i = 0; i < NUM_APPLES; ++i)
			{
				auto& apple = game.apples[i];
				if (apple.sprite.getGlobalBounds().intersects(game.player.sprite.getGlobalBounds()))
				{
					// Eat apple
					++game.score;
					game.player.speed += ACCELERATION;
					if (game.eatSound.getBuffer()) game.eatSound.play();
					// Reposition apple
					apple.position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
					apple.sprite.setPosition(apple.position.x, apple.position.y);
				}
			}

			// Rocks collisions
			for (int i = 0; i < NUM_ROCKS; ++i)
			{
				auto& rock = game.rocks[i];
				if (rock.sprite.getGlobalBounds().intersects(game.player.sprite.getGlobalBounds()))
				{
					if (game.hitSound.getBuffer()) game.hitSound.play();
					game.state = GameState::GameOver;
					game.timeSinceGameOver = 0.f;
					break;
				}
			}

			// Screen borders
			if (IsPlayerCollidesWithScreenBorder(game.player))
			{
				if (game.hitSound.getBuffer()) game.hitSound.play();
				game.state = GameState::GameOver;
				game.timeSinceGameOver = 0.f;
			}

			// Update score text
			game.scoreText.setString(std::string("Score: ") + std::to_string(game.score));
		}
		else if (game.state == GameState::GameOver)
		{
			game.timeSinceGameOver += deltaTime;
			if (game.timeSinceGameOver >= PAUSE_LENGTH)
			{
				// Restart
				RestartGame(game);
			}
		}
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		window.draw(game.background);

		for (int i = 0; i < NUM_APPLES; ++i)
		{
			auto& apple = game.apples[i];
			apple.sprite.setPosition(apple.position.x, apple.position.y);
			window.draw(apple.sprite);
		}

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			auto& rock = game.rocks[i];
			rock.sprite.setPosition(rock.position.x, rock.position.y);
			window.draw(rock.sprite);
		}

		window.draw(game.player.sprite);

		// UI
		window.draw(game.scoreText);
		window.draw(game.controlsText);

		if (game.state == GameState::GameOver)
		{
			window.draw(game.gameOverText);
		}
	}

	void DeinializeGame(Game& game)
	{
		// nothing to do for now
	}
}
