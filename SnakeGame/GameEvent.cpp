#include "Game.h"

namespace SnakeGame
{
	static void HandleMainMenuEvent(Game& game, const sf::Event& event)
	{
		const int MENU_ITEMS = 5;

		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
		{
			game.menuSelectedItem = (game.menuSelectedItem - 1 + MENU_ITEMS) % MENU_ITEMS;
			PlaySFX(game, game.buttonSound);
		}
		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
		{
			game.menuSelectedItem = (game.menuSelectedItem + 1) % MENU_ITEMS;
			PlaySFX(game, game.buttonSound);
		}
		else if (event.key.code == sf::Keyboard::Enter)
		{
			PlaySFX(game, game.buttonSound);
			switch (game.menuSelectedItem)
			{
			case 0:
				StartSession(game);
				break;
			case 1:
				game.menuSelectedItem = game.difficultyLevel;
				game.state = GameState::DifficultySelect;
				break;
			case 2:
				game.state = GameState::LeaderboardScreen;
				break;
			case 3:
				game.menuSelectedItem = 0;
				game.state = GameState::SettingsScreen;
				break;
			}
		}
	}

	static void HandleDifficultyEvent(Game& game, const sf::Event& event)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
		{
			game.menuSelectedItem = (game.menuSelectedItem - 1 + NUM_DIFFICULTY_LEVELS) % NUM_DIFFICULTY_LEVELS;
			PlaySFX(game, game.buttonSound);
		}
		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
		{
			game.menuSelectedItem = (game.menuSelectedItem + 1) % NUM_DIFFICULTY_LEVELS;
			PlaySFX(game, game.buttonSound);
		}
		else if (event.key.code == sf::Keyboard::Enter)
		{
			game.difficultyLevel = game.menuSelectedItem;
			game.menuSelectedItem = 0;
			game.state = GameState::MainMenu;
			PlaySFX(game, game.buttonSound);
		}
		else if (event.key.code == sf::Keyboard::B || event.key.code == sf::Keyboard::BackSpace)
		{
			game.menuSelectedItem = 1;
			game.state = GameState::MainMenu;
			PlaySFX(game, game.buttonSound);
		}
	}

	static void HandleLeaderboardEvent(Game& game, const sf::Event& event)
	{
		if (event.key.code == sf::Keyboard::B || event.key.code == sf::Keyboard::BackSpace)
		{
			game.menuSelectedItem = 2;
			game.state = GameState::MainMenu;
			PlaySFX(game, game.buttonSound);
		}
	}

	static void HandleSettingsEvent(Game& game, const sf::Event& event)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
		{
			game.menuSelectedItem = 0;
			PlaySFX(game, game.buttonSound);
		}
		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
		{
			game.menuSelectedItem = 1;
			PlaySFX(game, game.buttonSound);
		}
		else if (event.key.code == sf::Keyboard::Enter)
		{
			if (game.menuSelectedItem == 0)
				game.soundEnabled = !game.soundEnabled;
			else
				game.musicEnabled = !game.musicEnabled;
			PlaySFX(game, game.buttonSound);
		}
		else if (event.key.code == sf::Keyboard::B || event.key.code == sf::Keyboard::BackSpace)
		{
			game.menuSelectedItem = 3;
			game.state = GameState::MainMenu;
			PlaySFX(game, game.buttonSound);
		}
	}

	static void HandlePlayingEvent(Game& game, const sf::Event& event)
	{
		if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right)
			SetSnakeDirection(game.snake, Direction::Right);
		else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
			SetSnakeDirection(game.snake, Direction::Up);
		else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left)
			SetSnakeDirection(game.snake, Direction::Left);
		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
			SetSnakeDirection(game.snake, Direction::Down);
		else if (event.key.code == sf::Keyboard::P)
		{
			PauseMusic(game);
			game.pauseSelectedItem = 0;
			game.state = GameState::PauseMenu;
			PlaySFX(game, game.buttonSound);
		}
	}

	static void HandlePauseEvent(Game& game, const sf::Event& event)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
		{
			game.pauseSelectedItem = 0;
			PlaySFX(game, game.buttonSound);
		}
		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
		{
			game.pauseSelectedItem = 1;
			PlaySFX(game, game.buttonSound);
		}
		else if (event.key.code == sf::Keyboard::Enter)
		{
			PlaySFX(game, game.buttonSound);
			if (game.pauseSelectedItem == 0)
			{
				// Continue — resume music and start countdown
				StartMusic(game); // resumes from paused position
				game.countdownTimer = SESSION_START_DELAY;
				game.state = GameState::Countdown;
			}
			else
			{
				// Exit to menu — stop music
				StopMusic(game);
				game.menuSelectedItem = 0;
				game.state = GameState::MainMenu;
			}
		}
		else if (event.key.code == sf::Keyboard::P)
		{
			PlaySFX(game, game.buttonSound);
			StartMusic(game);
			game.countdownTimer = SESSION_START_DELAY;
			game.state = GameState::Countdown;
		}
	}

	static void HandleGameOverEvent(Game& game, const sf::Event& event)
	{
		if (!game.nameEntryAsked)
		{
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
			{
				game.nameEntryChoice = 0;
				PlaySFX(game, game.buttonSound);
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
			{
				game.nameEntryChoice = 1;
				PlaySFX(game, game.buttonSound);
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				PlaySFX(game, game.buttonSound);
				if (game.nameEntryChoice == 0)
				{
					game.playerName = "";
					game.state = GameState::NameEntry;
				}
				else
				{
					// User chose not to save — dismiss the prompt
					game.nameEntryAsked = true;
					game.gameOverSelectedItem = 0;
				}
			}
		}
		else
		{
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
			{
				game.gameOverSelectedItem = 0;
				PlaySFX(game, game.buttonSound);
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
			{
				game.gameOverSelectedItem = 1;
				PlaySFX(game, game.buttonSound);
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				PlaySFX(game, game.buttonSound);
				if (game.gameOverSelectedItem == 0)
					StartSession(game);
				else
				{
					game.menuSelectedItem = 0;
					game.state = GameState::MainMenu;
				}
			}
		}
	}

	static void HandleNameEntryEvent(Game& game, const sf::Event& event)
	{
		if (event.key.code == sf::Keyboard::Enter)
		{
			PlaySFX(game, game.buttonSound);
			std::string name = game.playerName.empty() ? DEFAULT_PLAYER_NAME : game.playerName;
			AddLeaderboardEntry(game.leaderboard, name, game.score);
			game.nameEntryAsked = true;
			game.gameOverSelectedItem = 0;
			game.state = GameState::GameOverPopup;
		}
		else if (event.key.code == sf::Keyboard::BackSpace)
		{
			if (!game.playerName.empty())
				game.playerName.pop_back();
		}
	}

	static void HandleNameEntryTextEvent(Game& game, const sf::Event& event)
	{
		if (event.type != sf::Event::TextEntered)
			return;

		char c = static_cast<char>(event.text.unicode);
		// Accept printable ASCII characters
		if (c >= 32 && c < 127 && (int)game.playerName.size() < MAX_NAME_LENGTH)
		{
			game.playerName += c;
		}
	}

	void HandleGameEvent(Game& game, const sf::Event& event)
	{
		// Handle text input for name entry separately
		if (game.state == GameState::NameEntry && event.type == sf::Event::TextEntered)
		{
			HandleNameEntryTextEvent(game, event);
			return;
		}

		if (event.type != sf::Event::KeyPressed)
			return;

		switch (game.state)
		{
		case GameState::MainMenu:
			HandleMainMenuEvent(game, event);
			break;
		case GameState::DifficultySelect:
			HandleDifficultyEvent(game, event);
			break;
		case GameState::LeaderboardScreen:
			HandleLeaderboardEvent(game, event);
			break;
		case GameState::SettingsScreen:
			HandleSettingsEvent(game, event);
			break;
		case GameState::Playing:
			HandlePlayingEvent(game, event);
			break;
		case GameState::PauseMenu:
			HandlePauseEvent(game, event);
			break;
		case GameState::GameOverPopup:
			HandleGameOverEvent(game, event);
			break;
		case GameState::NameEntry:
			HandleNameEntryEvent(game, event);
			break;
		case GameState::Countdown:
			break;
		}
	}
}
