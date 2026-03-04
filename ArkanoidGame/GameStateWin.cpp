#include "GameStateWin.h"
#include "Text.h"
#include <cassert>

namespace ArkanoidGame
{
	void GameStateWin::Init(Game& game)
	{
		assert(font_.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

		congratsText_.setFont(font_);
		congratsText_.setCharacterSize(48);
		congratsText_.setFillColor(sf::Color::Yellow);
		congratsText_.setString("You Win!");

		promptText_.setFont(font_);
		promptText_.setCharacterSize(30);
		promptText_.setFillColor(sf::Color::White);
		promptText_.setString("Play again?");

		yesText_.setFont(font_);
		yesText_.setCharacterSize(28);
		yesText_.setString("Yes");

		noText_.setFont(font_);
		noText_.setCharacterSize(28);
		noText_.setString("No");

		selectedOption_ = 0;
	}

	void GameStateWin::Shutdown(Game& game)
	{
	}

	void GameStateWin::HandleWindowEvent(Game& game, sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
			{
				selectedOption_ = 1 - selectedOption_;
			}
			else if (event.key.code == sf::Keyboard::Return)
			{
				if (selectedOption_ == 0)
					game.switchState(GameStateType::Playing);
				else
					game.switchState(GameStateType::MainMenu);
			}
		}
	}

	void GameStateWin::Update(Game& game, float timeDelta)
	{
	}

	void GameStateWin::Draw(Game& game, sf::RenderWindow& window)
	{
		const float cx = SCREEN_WIDTH / 2.f;
		const float cy = SCREEN_HEIGHT / 2.f;

		congratsText_.setOrigin(GetTextOrigin(congratsText_, { 0.5f, 0.5f }));
		congratsText_.setPosition(cx, cy - 90.f);
		window.draw(congratsText_);

		promptText_.setOrigin(GetTextOrigin(promptText_, { 0.5f, 0.5f }));
		promptText_.setPosition(cx, cy - 10.f);
		window.draw(promptText_);

		yesText_.setFillColor(selectedOption_ == 0 ? sf::Color::Yellow : sf::Color::White);
		yesText_.setOrigin(GetTextOrigin(yesText_, { 0.5f, 0.5f }));
		yesText_.setPosition(cx, cy + 50.f);
		window.draw(yesText_);

		noText_.setFillColor(selectedOption_ == 1 ? sf::Color::Yellow : sf::Color::White);
		noText_.setOrigin(GetTextOrigin(noText_, { 0.5f, 0.5f }));
		noText_.setPosition(cx, cy + 95.f);
		window.draw(noText_);
	}
}
