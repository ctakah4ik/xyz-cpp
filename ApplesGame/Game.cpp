#include "Game.h"
#include <cassert>
#include <iostream>
#include <string>

namespace ApplesGame
{
	// Leaderboard helpers

	// Predefined NPC records
	static const LeaderboardEntry NPC_RECORDS[] = {
		{"Alice",  120, false},
		{"Bob",     95, false},
		{"Carol",   72, false},
		{"Dave",    58, false},
		{"Eve",     43, false},
		{"Frank",   31, false},
		{"Grace",   18, false},
	};
	static const int NPC_COUNT = 7;

	// Fill leaderboard with fixed NPC data (no player yet)
	static void InitLeaderboard(Game& game)
	{
		game.leaderboard.clear();
		for (int i = 0; i < NPC_COUNT; ++i)
			game.leaderboard.push_back(NPC_RECORDS[i]);
	}

	// Insertion sort descending by score
	static void SortLeaderboard(std::vector<LeaderboardEntry>& lb)
	{
		for (int i = 1; i < (int)lb.size(); ++i)
		{
			LeaderboardEntry key = lb[i];
			int j = i - 1;
			while (j >= 0 && lb[j].score < key.score)
			{
				lb[j + 1] = lb[j];
				--j;
			}
			lb[j + 1] = key;
		}
	}

	// Add player with current score to the leaderboard, then sort
	static void FinalizeLeaderboard(Game& game)
	{
		LeaderboardEntry player;
		player.name     = "Player";
		player.score    = game.score;
		player.isPlayer = true;
		game.leaderboard.push_back(player);

		SortLeaderboard(game.leaderboard);
	}

	// Pad string on the right to a fixed width
	static std::string PadRight(const std::string& s, int width, char fill = ' ')
	{
		std::string r = s;
		while ((int)r.size() < width) r += fill;
		return r;
	}

	// Pad string on the left to a fixed width
	static std::string PadLeft(const std::string& s, int width, char fill = ' ')
	{
		std::string r = s;
		while ((int)r.size() < width) r = fill + r;
		return r;
	}

	// Rebuild the leaderboard from current data
	static void BuildLeaderboardText(Game& game)
	{
		const int NAME_WIDTH  = 12;
		const int SCORE_WIDTH = 4;

		std::string text = "===== LEADERBOARD =====\n";
		for (int i = 0; i < (int)game.leaderboard.size(); ++i)
		{
			const LeaderboardEntry& e = game.leaderboard[i];
			std::string name = e.name;
			if ((int)name.size() > NAME_WIDTH) name = name.substr(0, NAME_WIDTH);

			std::string rank      = PadRight(std::to_string(i + 1) + ".", 3);
			std::string nameField = PadRight(name, NAME_WIDTH, '.');
			std::string scoreStr  = PadLeft(std::to_string(e.score), SCORE_WIDTH);

			std::string line = rank + " " + nameField + " " + scoreStr;
			if (e.isPlayer) line = "> " + line + " <";
			else            line = "  " + line;

			text += line + "\n";
		}
		text += "=======================";
		game.leaderboardText.setString(text);
	}

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
		game.menuText.setString(text);
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
		InitLeaderboard(game);

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

		// Main-menu text
		game.menuText.setFont(game.font);
		game.menuText.setCharacterSize(28);
		game.menuText.setFillColor(sf::Color::White);
		game.menuText.setPosition(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 5.f);

		// Fill leaderboard with predefined NPC data
		InitLeaderboard(game);
		BuildLeaderboardText(game);

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
				FinalizeLeaderboard(game);
				BuildLeaderboardText(game);
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
					FinalizeLeaderboard(game);
					BuildLeaderboardText(game);
					game.state = GameState::GameOver;
					game.timeSinceGameOver = 0.f;
					break;
				}
			}

			// Screen border collision
			if (IsPlayerCollidesWithScreenBorder(game.player))
			{
				if (game.hitSound.getBuffer()) game.hitSound.play();
				FinalizeLeaderboard(game);
				BuildLeaderboardText(game);
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
		delete[] game.apples;
		game.apples = nullptr;
	}
}
