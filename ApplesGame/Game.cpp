#include "Game.h"
#include <cassert>
#include <iostream>
#include <string>

namespace ApplesGame
{
	// Menu text

	static void UpdateMenuText(Game& game)
	{
		std::string text = "APPLES GAME\n\n";
		text += "Choose your mode:\n\n";
		text += "  [1]  Apples: ";
		text += (game.gameMode & GAME_MODE_INFINITE_APPLES) ? "Infinite (respawn)\n" : "Finite  (no respawn)\n";
		text += "  [2]  Acceleration: ";
		text += (game.gameMode & GAME_MODE_WITH_ACCELERATION) ? "On\n" : "Off\n";
		text += "\n  [Enter]  Start game";
		text += "\n  [L]      Leaderboard";
		game.menuText.setString(text);
	}

	// Update visual highlight of pause menu items based on pauseSelectedItem
	static void UpdatePauseMenuHighlight(Game& game)
	{
		auto applyStyle = [](sf::Text& text, bool selected)
		{
			text.setFillColor(selected ? sf::Color::White : sf::Color(120, 120, 120));
			text.setOutlineThickness(selected ? 2.f : 0.f);
		};
		applyStyle(game.pauseResumeText, game.pauseSelectedItem == 0);
		applyStyle(game.pauseExitText,   game.pauseSelectedItem == 1);
	}

	// Handle keyboard events
	void HandleGameEvent(Game& game, const sf::Event& event)
	{
		if (event.type != sf::Event::KeyPressed)
			return;

		if (game.state == GameState::MainMenu)
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
			else if (event.key.code == sf::Keyboard::L)
			{
				BuildLeaderboardText(game);
				game.state = GameState::Leaderboard;
			}
		}
		else if (game.state == GameState::Leaderboard)
		{
			if (event.key.code == sf::Keyboard::BackSpace)
			{
				game.state = GameState::MainMenu;
				UpdateMenuText(game);
			}
		}
		else if (game.state == GameState::Playing)
		{
			if (event.key.code == sf::Keyboard::P)
			{
				game.pauseSelectedItem = 0;
				UpdatePauseMenuHighlight(game);
				game.state = GameState::PauseMenu;
			}
		}
		else if (game.state == GameState::PauseMenu)
		{
			if (event.key.code == sf::Keyboard::Up)
			{
				game.pauseSelectedItem = 0;
				UpdatePauseMenuHighlight(game);
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				game.pauseSelectedItem = 1;
				UpdatePauseMenuHighlight(game);
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				if (game.pauseSelectedItem == 0)
				{
					game.state = GameState::Playing;
				}
				else
				{
					game.state = GameState::MainMenu;
					UpdateMenuText(game);
				}
			}
			else if (event.key.code == sf::Keyboard::P)
			{
				game.state = GameState::Playing;
			}
		}
		else if (game.state == GameState::GameOver || game.state == GameState::Win)
		{
			// Skip the timer — go straight to the menu
			if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space)
			{
				game.state = GameState::MainMenu;
				UpdateMenuText(game);
			}
		}
	}

	void RestartGame(Game& game)
	{
		// Reset leaderboard to NPC-only data (player will be added at game end)
		InitLeaderboard(game.leaderboard);

		// Pick a random apple count each game
		int numApples = MIN_APPLES + rand() % (MAX_APPLES - MIN_APPLES + 1);
		game.apples.resize(numApples);
		game.rocks.resize(NUM_ROCKS);

		InitPlayer(game.player, game);

		for (Apple& apple : game.apples)
		{
			InitApple(apple, game.appleTexture);
			apple.isEaten = false;
		}

		for (Rock& rock : game.rocks)
		{
			InitRock(rock, game.rockTexture);
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

		// Game-over overlay (positioned at top so leaderboard fits below)
		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(72);
		game.gameOverText.setFillColor(sf::Color::Red);
		game.gameOverText.setString("GAME OVER");
		{
			auto b = game.gameOverText.getLocalBounds();
			game.gameOverText.setOrigin(b.width / 2.f, b.height / 2.f);
			game.gameOverText.setPosition(SCREEN_WIDTH / 2.f, 65.f);
		}

		// Win overlay
		game.winText.setFont(game.font);
		game.winText.setCharacterSize(72);
		game.winText.setFillColor(sf::Color::Green);
		game.winText.setString("YOU WIN!");
		{
			auto b = game.winText.getLocalBounds();
			game.winText.setOrigin(b.width / 2.f, b.height / 2.f);
			game.winText.setPosition(SCREEN_WIDTH / 2.f, 65.f);
		}

		// Leaderboard text
		game.leaderboardText.setFont(game.font);
		game.leaderboardText.setCharacterSize(22);
		game.leaderboardText.setFillColor(sf::Color::White);
		game.leaderboardText.setPosition(SCREEN_WIDTH / 2.f - 160.f, 120.f);

		// Leaderboard screen hint
		game.leaderboardHintText.setFont(game.font);
		game.leaderboardHintText.setCharacterSize(20);
		game.leaderboardHintText.setFillColor(sf::Color(180, 180, 180));
		game.leaderboardHintText.setString("Backspace -- return to menu");
		{
			auto b = game.leaderboardHintText.getLocalBounds();
			game.leaderboardHintText.setOrigin(b.width / 2.f, b.height / 2.f);
			game.leaderboardHintText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 40.f);
		}

		// Pause overlay (semi-transparent dark fill over the game world)
		game.pauseOverlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));
		game.pauseOverlay.setPosition(0.f, 0.f);

		// Pause menu items
		game.pauseResumeText.setFont(game.font);
		game.pauseResumeText.setCharacterSize(40);
		game.pauseResumeText.setOutlineColor(sf::Color::Yellow);
		game.pauseResumeText.setString("Resume");
		{
			auto b = game.pauseResumeText.getLocalBounds();
			game.pauseResumeText.setOrigin(b.width / 2.f, b.height / 2.f);
			game.pauseResumeText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 50.f);
		}

		game.pauseExitText.setFont(game.font);
		game.pauseExitText.setCharacterSize(40);
		game.pauseExitText.setOutlineColor(sf::Color::Yellow);
		game.pauseExitText.setString("Exit to Menu");
		{
			auto b = game.pauseExitText.getLocalBounds();
			game.pauseExitText.setOrigin(b.width / 2.f, b.height / 2.f);
			game.pauseExitText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 50.f);
		}

		UpdatePauseMenuHighlight(game);

		// Main-menu text
		game.menuText.setFont(game.font);
		game.menuText.setCharacterSize(28);
		game.menuText.setFillColor(sf::Color::White);
		game.menuText.setPosition(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 5.f);

		// Fill leaderboard with predefined NPC data
		InitLeaderboard(game.leaderboard);
		BuildLeaderboardText(game.leaderboard, game.leaderboardText);

		// Start at main menu
		game.state = GameState::MainMenu;
		UpdateMenuText(game);
	}

	void UpdateGame(Game& game, float deltaTime)
	{
		if (game.state == GameState::MainMenu
			|| game.state == GameState::Leaderboard
			|| game.state == GameState::PauseMenu)
		{
			return; // input handled via HandleGameEvent
		}

		if (game.state == GameState::Playing)
		{
			UpdatePlayer(game.player, deltaTime);

			// Apple collisions
			int eatenCount = 0;
			for (Apple& apple : game.apples)
			{
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
			if (!(game.gameMode & GAME_MODE_INFINITE_APPLES) && eatenCount >= (int)game.apples.size())
			{
				FinalizeLeaderboard(game.leaderboard, game.score);
				BuildLeaderboardText(game.leaderboard, game.leaderboardText);
				game.state = GameState::Win;
				game.timeSinceGameOver = 0.f;
			}

			// Rock collisions
			for (Rock& rock : game.rocks)
			{
				if (rock.sprite.getGlobalBounds().intersects(game.player.sprite.getGlobalBounds()))
				{
					if (game.hitSound.getBuffer()) game.hitSound.play();
					FinalizeLeaderboard(game.leaderboard, game.score);
					BuildLeaderboardText(game.leaderboard, game.leaderboardText);
					game.state = GameState::GameOver;
					game.timeSinceGameOver = 0.f;
					break;
				}
			}

			// Screen border collision
			if (IsPlayerCollidesWithScreenBorder(game.player))
			{
				if (game.hitSound.getBuffer()) game.hitSound.play();
				FinalizeLeaderboard(game.leaderboard, game.score);
				BuildLeaderboardText(game.leaderboard, game.leaderboardText);
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

		if (game.state == GameState::Leaderboard)
		{
			window.draw(game.leaderboardText);
			window.draw(game.leaderboardHintText);
			return;
		}

		// Apples (skip eaten ones in finite mode)
		for (Apple& apple : game.apples)
		{
			if (!apple.isEaten)
			{
				apple.sprite.setPosition(apple.position.x, apple.position.y);
				window.draw(apple.sprite);
			}
		}

		// Rocks
		for (Rock& rock : game.rocks)
		{
			rock.sprite.setPosition(rock.position.x, rock.position.y);
			window.draw(rock.sprite);
		}

		window.draw(game.player.sprite);

		// HUD
		window.draw(game.scoreText);
		window.draw(game.controlsText);

		if (game.state == GameState::PauseMenu)
		{
			window.draw(game.pauseOverlay);
			window.draw(game.pauseResumeText);
			window.draw(game.pauseExitText);
		}
		else if (game.state == GameState::GameOver)
		{
			window.draw(game.gameOverText);
			window.draw(game.leaderboardText);
		}
		else if (game.state == GameState::Win)
		{
			window.draw(game.winText);
			window.draw(game.leaderboardText);
		}
	}

	void DeinializeGame(Game& game)
	{
	}
}
