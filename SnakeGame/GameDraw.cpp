#include "Game.h"
#include <string>

namespace SnakeGame
{
	static sf::Color SelectedColor()
	{
		return sf::Color(0, 200, 0);
	}

	static sf::Color UnselectedColor()
	{
		return sf::Color(180, 180, 180);
	}

	static void DrawCenteredText(sf::RenderWindow& window, sf::Text& text, float y)
	{
		auto b = text.getLocalBounds();
		text.setOrigin(b.width / 2.f, b.height / 2.f);
		text.setPosition(SCREEN_WIDTH / 2.f, y);
		window.draw(text);
	}

	static void DrawTextAt(sf::RenderWindow& window, sf::Text& text, float x, float y)
	{
		text.setPosition(x, y);
		window.draw(text);
	}

	static void DrawOverlay(sf::RenderWindow& window)
	{
		sf::RectangleShape overlay(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
		overlay.setFillColor(sf::Color(0, 0, 0, 150));
		window.draw(overlay);
	}

	// Game field height in pixels
	static constexpr float FIELD_TOP    = (float)GRID_OFFSET_Y;
	static constexpr float FIELD_BOTTOM = (float)SCREEN_HEIGHT;
	static constexpr float FIELD_HEIGHT = FIELD_BOTTOM - FIELD_TOP;

	static void DrawHUDBackground(sf::RenderWindow& window)
	{
		sf::RectangleShape bar(sf::Vector2f((float)SCREEN_WIDTH, (float)HUD_HEIGHT));
		bar.setFillColor(sf::Color(20, 20, 20));
		bar.setPosition(0.f, 0.f);
		window.draw(bar);

		// Separator line
		sf::RectangleShape sep(sf::Vector2f((float)SCREEN_WIDTH, 2.f));
		sep.setFillColor(sf::Color(80, 80, 80));
		sep.setPosition(0.f, (float)HUD_HEIGHT - 2.f);
		window.draw(sep);
	}

	static void DrawWalls(sf::RenderWindow& window)
	{
		sf::RectangleShape wall;
		wall.setFillColor(sf::Color(100, 100, 100));

		// Top wall
		wall.setSize(sf::Vector2f((float)SCREEN_WIDTH, (float)CELL_SIZE));
		wall.setPosition(0.f, FIELD_TOP);
		window.draw(wall);

		// Bottom wall
		wall.setPosition(0.f, FIELD_BOTTOM - (float)CELL_SIZE);
		window.draw(wall);

		// Left wall
		wall.setSize(sf::Vector2f((float)CELL_SIZE, FIELD_HEIGHT));
		wall.setPosition(0.f, FIELD_TOP);
		window.draw(wall);

		// Right wall
		wall.setPosition((float)(SCREEN_WIDTH - CELL_SIZE), FIELD_TOP);
		window.draw(wall);
	}

	static void DrawGrid(sf::RenderWindow& window)
	{
		sf::RectangleShape line;
		line.setFillColor(sf::Color(40, 40, 40));

		// Vertical lines only inside the play area
		for (int x = 0; x <= GRID_WIDTH; ++x)
		{
			line.setSize(sf::Vector2f(1.f, FIELD_HEIGHT));
			line.setPosition((float)x * CELL_SIZE, FIELD_TOP);
			window.draw(line);
		}

		// Horizontal lines
		for (int y = 0; y <= GRID_HEIGHT; ++y)
		{
			line.setSize(sf::Vector2f((float)SCREEN_WIDTH, 1.f));
			line.setPosition(0.f, FIELD_TOP + (float)y * CELL_SIZE);
			window.draw(line);
		}
	}

	// Draws the game field behind popups (grid, walls, snake, apple)
	static void DrawGameField(Game& game, sf::RenderWindow& window)
	{
		DrawGrid(window);
		DrawWalls(window);
		DrawApple(game.apple, game.appleTexture, window);
		DrawSnake(game.snake, BuildSnakeTextures(game), window);
	}

	static void DrawScoreHUD(Game& game, sf::RenderWindow& window)
	{
		DrawHUDBackground(window);

		const float rightMargin = 10.f;
		const float iconSize = 24.f;
		const float gap = 6.f;
		float iconY = ((float)HUD_HEIGHT - iconSize) / 2.f;

		// Score number
		sf::Text scoreText;
		scoreText.setFont(game.font);
		scoreText.setCharacterSize(22);
		scoreText.setFillColor(sf::Color::White);
		scoreText.setString(std::to_string(game.score));
		auto sb = scoreText.getLocalBounds();
		float scoreX = (float)SCREEN_WIDTH - rightMargin - sb.width - sb.left;
		float textY = ((float)HUD_HEIGHT - sb.height) / 2.f - sb.top / 2.f - 2.f;
		DrawTextAt(window, scoreText, scoreX, textY);

		// Apple icon
		sf::Sprite appleIcon;
		appleIcon.setTexture(game.appleTexture);
		sf::Vector2u aSize = game.appleTexture.getSize();
		if (aSize.x > 0 && aSize.y > 0)
			appleIcon.setScale(iconSize / (float)aSize.x, iconSize / (float)aSize.y);
		appleIcon.setPosition(scoreX - gap - iconSize, iconY);
		window.draw(appleIcon);
	}

	static void DrawLeaderboardTable(Game& game, sf::RenderWindow& window,
		int maxEntries, float leftX, float rightX, float startY, unsigned int charSize)
	{
		float lineH = (float)charSize * 1.5f;
		float rankColW = 36.f;

		sf::Text txt;
		txt.setFont(game.font);
		txt.setCharacterSize(charSize);

		// Header
		txt.setFillColor(sf::Color(160, 160, 160));
		txt.setString("#");
		DrawTextAt(window, txt, leftX, startY);
		txt.setString("Name");
		DrawTextAt(window, txt, leftX + rankColW, startY);
		txt.setString("Score");
		{
			auto b = txt.getLocalBounds();
			DrawTextAt(window, txt, rightX - b.width - b.left, startY);
		}

		// Separator
		sf::RectangleShape sep(sf::Vector2f(rightX - leftX, 1.f));
		sep.setFillColor(sf::Color(70, 70, 70));
		sep.setPosition(leftX, startY + lineH - 6.f);
		window.draw(sep);

		// Entries
		txt.setFillColor(sf::Color::White);
		int count = std::min(maxEntries, (int)game.leaderboard.entries.size());
		for (int i = 0; i < count; ++i)
		{
			float y = startY + lineH * (float)(i + 1);
			txt.setString(std::to_string(i + 1) + ".");
			DrawTextAt(window, txt, leftX, y);
			txt.setString(game.leaderboard.entries[i].name);
			DrawTextAt(window, txt, leftX + rankColW, y);
			txt.setString(std::to_string(game.leaderboard.entries[i].score));
			auto b = txt.getLocalBounds();
			DrawTextAt(window, txt, rightX - b.width - b.left, y);
		}

		if (game.leaderboard.entries.empty())
		{
			txt.setString("No records yet");
			DrawTextAt(window, txt, leftX + rankColW, startY + lineH);
		}
	}

	static void DrawMainMenu(Game& game, sf::RenderWindow& window)
	{
		sf::Text title;
		title.setFont(game.font);
		title.setCharacterSize(48);
		title.setFillColor(sf::Color::White);
		title.setString("SNAKE GAME");
		DrawCenteredText(window, title, 80.f);

		const char* items[] = { "Start Game", "Difficulty", "Leaderboard", "Settings", "Exit" };
		sf::Text item;
		item.setFont(game.font);
		item.setCharacterSize(28);

		float startY = 180.f;
		for (int i = 0; i < 5; ++i)
		{
			item.setString(items[i]);
			item.setFillColor(i == game.menuSelectedItem ? SelectedColor() : UnselectedColor());
			DrawCenteredText(window, item, startY + i * 50.f);
		}

		// Show current difficulty below menu
		sf::Text diffInfo;
		diffInfo.setFont(game.font);
		diffInfo.setCharacterSize(18);
		diffInfo.setFillColor(sf::Color(120, 120, 120));
		diffInfo.setString("Difficulty: " + std::string(DIFFICULTY_LEVELS[game.difficultyLevel].name));
		DrawCenteredText(window, diffInfo, startY + 5 * 50.f + 20.f);

		// Navigation hint
		sf::Text hint;
		hint.setFont(game.font);
		hint.setCharacterSize(16);
		hint.setFillColor(sf::Color(100, 100, 100));
		hint.setString("W/S - navigate | Enter - select");
		DrawCenteredText(window, hint, (float)SCREEN_HEIGHT - 30.f);
	}

	static void DrawDifficultySelect(Game& game, sf::RenderWindow& window)
	{
		sf::Text title;
		title.setFont(game.font);
		title.setCharacterSize(36);
		title.setFillColor(sf::Color::White);
		title.setString("SELECT DIFFICULTY");
		DrawCenteredText(window, title, 80.f);

		sf::Text item;
		item.setFont(game.font);
		item.setCharacterSize(24);

		float startY = 170.f;
		for (int i = 0; i < NUM_DIFFICULTY_LEVELS; ++i)
		{
			std::string label = std::string(DIFFICULTY_LEVELS[i].name);
			label += "  (Speed: " + std::to_string((int)DIFFICULTY_LEVELS[i].speed);
			label += ", Points: " + std::to_string(DIFFICULTY_LEVELS[i].pointsPerApple) + ")";

			item.setString(label);

			if (i == game.menuSelectedItem)
				item.setFillColor(SelectedColor());
			else if (i == game.difficultyLevel)
				item.setFillColor(sf::Color::Yellow); // current selection
			else
				item.setFillColor(UnselectedColor());

			DrawCenteredText(window, item, startY + i * 45.f);
		}

		sf::Text hint;
		hint.setFont(game.font);
		hint.setCharacterSize(16);
		hint.setFillColor(sf::Color(100, 100, 100));
		hint.setString("W/S - navigate | Enter - select | B - back");
		DrawCenteredText(window, hint, (float)SCREEN_HEIGHT - 30.f);
	}

	static void DrawLeaderboardScreen(Game& game, sf::RenderWindow& window)
	{
		sf::Text title;
		title.setFont(game.font);
		title.setCharacterSize(36);
		title.setFillColor(sf::Color::White);
		title.setString("LEADERBOARD");
		DrawCenteredText(window, title, 60.f);

		DrawLeaderboardTable(game, window, LEADERBOARD_SCREEN_COUNT,
			200.f, 600.f, 120.f, 20);

		sf::Text hint;
		hint.setFont(game.font);
		hint.setCharacterSize(16);
		hint.setFillColor(sf::Color(100, 100, 100));
		hint.setString("B - back to menu");
		DrawCenteredText(window, hint, (float)SCREEN_HEIGHT - 30.f);
	}

	static void DrawSettingsScreen(Game& game, sf::RenderWindow& window)
	{
		sf::Text title;
		title.setFont(game.font);
		title.setCharacterSize(36);
		title.setFillColor(sf::Color::White);
		title.setString("SETTINGS");
		DrawCenteredText(window, title, 80.f);

		sf::Text item;
		item.setFont(game.font);
		item.setCharacterSize(24);

		std::string soundLabel = std::string("Sound: ") + (game.soundEnabled ? "ON" : "OFF");
		std::string musicLabel = std::string("Music: ") + (game.musicEnabled ? "ON" : "OFF");

		item.setString(soundLabel);
		item.setFillColor(game.menuSelectedItem == 0 ? SelectedColor() : UnselectedColor());
		DrawCenteredText(window, item, 200.f);

		item.setString(musicLabel);
		item.setFillColor(game.menuSelectedItem == 1 ? SelectedColor() : UnselectedColor());
		DrawCenteredText(window, item, 250.f);

		sf::Text hint;
		hint.setFont(game.font);
		hint.setCharacterSize(16);
		hint.setFillColor(sf::Color(100, 100, 100));
		hint.setString("W/S - navigate | Enter - toggle | B - back");
		DrawCenteredText(window, hint, (float)SCREEN_HEIGHT - 30.f);
	}

	static void DrawCountdown(Game& game, sf::RenderWindow& window)
	{
		DrawGameField(game, window);
		DrawScoreHUD(game, window);

		DrawOverlay(window);
		DrawHUDBackground(window); // keep HUD visible through overlay

		int seconds = (int)game.countdownTimer + 1;
		sf::Text countdown;
		countdown.setFont(game.font);
		countdown.setCharacterSize(72);
		countdown.setFillColor(sf::Color::White);
		countdown.setString(std::to_string(seconds));
		// Center in play area
		float fieldCenterY = FIELD_TOP + FIELD_HEIGHT / 2.f;
		DrawCenteredText(window, countdown, fieldCenterY - 40.f);
	}

	static void DrawPlaying(Game& game, sf::RenderWindow& window)
	{
		DrawGameField(game, window);
		DrawScoreHUD(game, window);

		// Pause hint in top-left of HUD strip
		sf::Text pauseHint;
		pauseHint.setFont(game.font);
		pauseHint.setCharacterSize(14);
		pauseHint.setFillColor(sf::Color(100, 100, 100));
		pauseHint.setString("P - pause");
		auto hb = pauseHint.getLocalBounds();
		float hintY = ((float)HUD_HEIGHT - hb.height) / 2.f - 2.f;
		DrawTextAt(window, pauseHint, 10.f, hintY);
	}

	static void DrawPauseMenu(Game& game, sf::RenderWindow& window)
	{
		DrawGameField(game, window);
		DrawOverlay(window);

		sf::Text title;
		title.setFont(game.font);
		title.setCharacterSize(40);
		title.setFillColor(sf::Color::White);
		title.setString("PAUSED");
		DrawCenteredText(window, title, SCREEN_HEIGHT / 2.f - 80.f);

		sf::Text item;
		item.setFont(game.font);
		item.setCharacterSize(28);

		item.setString("Continue");
		item.setFillColor(game.pauseSelectedItem == 0 ? SelectedColor() : UnselectedColor());
		DrawCenteredText(window, item, SCREEN_HEIGHT / 2.f - 10.f);

		item.setString("Exit to Menu");
		item.setFillColor(game.pauseSelectedItem == 1 ? SelectedColor() : UnselectedColor());
		DrawCenteredText(window, item, SCREEN_HEIGHT / 2.f + 40.f);
	}

	static void DrawGameOverPopup(Game& game, sf::RenderWindow& window)
	{
		DrawGameField(game, window);
		DrawOverlay(window);

		sf::Text title;
		title.setFont(game.font);
		title.setCharacterSize(48);
		title.setFillColor(sf::Color::Red);
		title.setString("GAME OVER");
		DrawCenteredText(window, title, 40.f);

		// Score
		sf::Text scoreText;
		scoreText.setFont(game.font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::White);
		scoreText.setString("Score: " + std::to_string(game.score));
		DrawCenteredText(window, scoreText, 100.f);

		// Leaderboard
		DrawLeaderboardTable(game, window, LEADERBOARD_POPUP_COUNT,
			220.f, 580.f, 135.f, 18);

		if (!game.nameEntryAsked)
		{
			sf::Text ask;
			ask.setFont(game.font);
			ask.setCharacterSize(22);
			ask.setFillColor(sf::Color::White);
			ask.setString("Save your record?");
			DrawCenteredText(window, ask, 350.f);

			sf::Text choice;
			choice.setFont(game.font);
			choice.setCharacterSize(22);

			choice.setString("Yes");
			choice.setFillColor(game.nameEntryChoice == 0 ? SelectedColor() : UnselectedColor());
			DrawCenteredText(window, choice, 390.f);

			choice.setString("No");
			choice.setFillColor(game.nameEntryChoice == 1 ? SelectedColor() : UnselectedColor());
			DrawCenteredText(window, choice, 425.f);
		}
		else
		{
			// Action buttons: Start game / Go to menu
			sf::Text item;
			item.setFont(game.font);
			item.setCharacterSize(26);

			item.setString("Start Game");
			item.setFillColor(game.gameOverSelectedItem == 0 ? SelectedColor() : UnselectedColor());
			DrawCenteredText(window, item, 380.f);

			item.setString("Go to Menu");
			item.setFillColor(game.gameOverSelectedItem == 1 ? SelectedColor() : UnselectedColor());
			DrawCenteredText(window, item, 425.f);
		}
	}

	static void DrawNameEntry(Game& game, sf::RenderWindow& window)
	{
		DrawOverlay(window);

		sf::Text title;
		title.setFont(game.font);
		title.setCharacterSize(32);
		title.setFillColor(sf::Color::White);
		title.setString("ENTER YOUR NAME");
		DrawCenteredText(window, title, SCREEN_HEIGHT / 2.f - 80.f);

		// Name input field
		sf::Text nameText;
		nameText.setFont(game.font);
		nameText.setCharacterSize(28);
		nameText.setFillColor(SelectedColor());

		std::string displayName = game.playerName + "_";
		nameText.setString(displayName);
		DrawCenteredText(window, nameText, SCREEN_HEIGHT / 2.f - 20.f);

		sf::Text hint;
		hint.setFont(game.font);
		hint.setCharacterSize(16);
		hint.setFillColor(sf::Color(100, 100, 100));
		hint.setString("Type your name and press Enter");
		DrawCenteredText(window, hint, SCREEN_HEIGHT / 2.f + 40.f);
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		window.draw(game.background);

		switch (game.state)
		{
		case GameState::MainMenu:
			DrawMainMenu(game, window);
			break;
		case GameState::DifficultySelect:
			DrawDifficultySelect(game, window);
			break;
		case GameState::LeaderboardScreen:
			DrawLeaderboardScreen(game, window);
			break;
		case GameState::SettingsScreen:
			DrawSettingsScreen(game, window);
			break;
		case GameState::Countdown:
			DrawCountdown(game, window);
			break;
		case GameState::Playing:
			DrawPlaying(game, window);
			break;
		case GameState::PauseMenu:
			DrawPauseMenu(game, window);
			break;
		case GameState::GameOverPopup:
			DrawGameOverPopup(game, window);
			break;
		case GameState::NameEntry:
			DrawNameEntry(game, window);
			break;
		}
	}
}
