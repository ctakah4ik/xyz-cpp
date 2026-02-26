#include "Game.h"
#include <cassert>
#include <iostream>
#include <string>

namespace ApplesGame
{
	static void UpdateMenuText(Game& game)
	{
		std::string text = "APPLES GAME\n\n";
		text += "Choose your mode:\n\n";
		text += "  [1]  Apples: ";
		text += (game.gameMode & GAME_MODE_INFINITE_APPLES) ? "Infinite (respawn)\n" : "Finite  (no respawn)\n";
		text += "  [2]  Acceleration: ";
		text += (game.gameMode & GAME_MODE_WITH_ACCELERATION) ? "On\n" : "Off\n";
		text += "\n  [Enter]  Start game";
		game.menuText.setString(text);
	}

	// Handle keyboard events while in the main menu
	void HandleGameEvent(Game& game, const sf::Event& event)
	{
		if (game.state != GameState::MainMenu)
			return;

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Num1)
			{
				game.gameMode ^= GAME_MODE_INFINITE_APPLES;   // toggle bit 0
				UpdateMenuText(game);
			}
			else if (event.key.code == sf::Keyboard::Num2)
			{
				game.gameMode ^= GAME_MODE_WITH_ACCELERATION; // toggle bit 1
				UpdateMenuText(game);
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				RestartGame(game);
			}
		}
	}

	void RestartGame(Game& game)
	{
		// Free previous apple allocation (safe if nullptr)
		delete[] game.apples;

		// Pick a random apple count each game
		game.numApples = MIN_APPLES + rand() % (MAX_APPLES - MIN_APPLES + 1);
		game.apples = new Apple[game.numApples];

		InitPlayer(game.player, game);

		for (int i = 0; i < game.numApples; ++i)
		{
			InitApple(game.apples[i], game.appleTexture);
			game.apples[i].isEaten = false;
		}

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
			std::cerr << "Failed to load player texture\n";
		if (!game.appleTexture.loadFromFile(RESOURCES_PATH + APPLE_TEXTURE_FILE))
			std::cerr << "Failed to load apple texture\n";
		if (!game.rockTexture.loadFromFile(RESOURCES_PATH + ROCK_TEXTURE_FILE))
			std::cerr << "Failed to load rock texture\n";
		if (!game.font.loadFromFile(RESOURCES_PATH + FONT_FILE))
			std::cerr << "Failed to load font\n";

		if (!game.eatSoundBuffer.loadFromFile(RESOURCES_PATH + EAT_SOUND_FILE))
			std::cerr << "Failed to load eat sound\n";
		else
			game.eatSound.setBuffer(game.eatSoundBuffer);

		if (!game.hitSoundBuffer.loadFromFile(RESOURCES_PATH + HIT_SOUND_FILE))
			std::cerr << "Failed to load hit sound\n";
		else
			game.hitSound.setBuffer(game.hitSoundBuffer);

		// Background
		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		// Score text
		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(24);
		game.scoreText.setFillColor(sf::Color::White);
		game.scoreText.setPosition(10.f, 10.f);

		// Controls hint
		game.controlsText.setFont(game.font);
		game.controlsText.setCharacterSize(18);
		game.controlsText.setFillColor(sf::Color::White);
		game.controlsText.setString("Controls: WASD or arrows to move. Esc to quit.");
		game.controlsText.setPosition(10.f, 40.f);

		// Game-over overlay
		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(72);
		game.gameOverText.setFillColor(sf::Color::Red);
		game.gameOverText.setString("GAME OVER");
		{
			auto b = game.gameOverText.getLocalBounds();
			game.gameOverText.setOrigin(b.width / 2.f, b.height / 2.f);
			game.gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
		}

		// Win overlay
		game.winText.setFont(game.font);
		game.winText.setCharacterSize(72);
		game.winText.setFillColor(sf::Color::Green);
		game.winText.setString("YOU WIN!");
		{
			auto b = game.winText.getLocalBounds();
			game.winText.setOrigin(b.width / 2.f, b.height / 2.f);
			game.winText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
		}

		// Main-menu text
		game.menuText.setFont(game.font);
		game.menuText.setCharacterSize(28);
		game.menuText.setFillColor(sf::Color::White);
		game.menuText.setPosition(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 5.f);

		// Start at main menu
		game.state = GameState::MainMenu;
		UpdateMenuText(game);
	}

	void UpdateGame(Game& game, float deltaTime)
	{
		if (game.state == GameState::MainMenu)
		{
			return; // input handled via HandleGameEvent
		}

		if (game.state == GameState::Playing)
		{
			UpdatePlayer(game.player, deltaTime);

			// Apple collisions
			int eatenCount = 0;
			for (int i = 0; i < game.numApples; ++i)
			{
				Apple& apple = game.apples[i];
				if (apple.isEaten)
				{
					++eatenCount;
					continue;
				}

				if (apple.sprite.getGlobalBounds().intersects(game.player.sprite.getGlobalBounds()))
				{
					++game.score;
					if (game.eatSound.getBuffer()) game.eatSound.play();

					// Speed up only when mode flag is set
					if (game.gameMode & GAME_MODE_WITH_ACCELERATION)
						game.player.speed += ACCELERATION;

					if (game.gameMode & GAME_MODE_INFINITE_APPLES)
					{
						// Respawn at a new random position
						apple.position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
						apple.sprite.setPosition(apple.position.x, apple.position.y);
					}
					else
					{
						// Finite mode: remove the apple
						apple.isEaten = true;
						++eatenCount;
					}
				}
			}

			// Win condition: all finite apples eaten
			if (!(game.gameMode & GAME_MODE_INFINITE_APPLES) && eatenCount >= game.numApples)
			{
				game.state = GameState::Win;
				game.timeSinceGameOver = 0.f;
			}

			// Rock collisions
			for (int i = 0; i < NUM_ROCKS; ++i)
			{
				Rock& rock = game.rocks[i];
				if (rock.sprite.getGlobalBounds().intersects(game.player.sprite.getGlobalBounds()))
				{
					if (game.hitSound.getBuffer()) game.hitSound.play();
					game.state = GameState::GameOver;
					game.timeSinceGameOver = 0.f;
					break;
				}
			}

			// Screen border collision
			if (IsPlayerCollidesWithScreenBorder(game.player))
			{
				if (game.hitSound.getBuffer()) game.hitSound.play();
				game.state = GameState::GameOver;
				game.timeSinceGameOver = 0.f;
			}

			game.scoreText.setString("Score: " + std::to_string(game.score));
		}
		else if (game.state == GameState::GameOver || game.state == GameState::Win)
		{
			game.timeSinceGameOver += deltaTime;
			if (game.timeSinceGameOver >= PAUSE_LENGTH)
			{
				// Return to menu so player can re-pick a mode
				game.state = GameState::MainMenu;
				UpdateMenuText(game);
			}
		}
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		window.draw(game.background);

		if (game.state == GameState::MainMenu)
		{
			window.draw(game.menuText);
			return;
		}

		// Apples (skip eaten ones in finite mode)
		for (int i = 0; i < game.numApples; ++i)
		{
			Apple& apple = game.apples[i];
			if (!apple.isEaten)
			{
				apple.sprite.setPosition(apple.position.x, apple.position.y);
				window.draw(apple.sprite);
			}
		}

		// Rocks
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			Rock& rock = game.rocks[i];
			rock.sprite.setPosition(rock.position.x, rock.position.y);
			window.draw(rock.sprite);
		}

		window.draw(game.player.sprite);

		// HUD
		window.draw(game.scoreText);
		window.draw(game.controlsText);

		if (game.state == GameState::GameOver)
			window.draw(game.gameOverText);
		else if (game.state == GameState::Win)
			window.draw(game.winText);
	}

	void DeinializeGame(Game& game)
	{
		delete[] game.apples;
		game.apples = nullptr;
	}
}
