#include "GameStatePlaying.h"
#include "Text.h"
#include <cassert>

namespace ArkanoidGame
{
	void GameStatePlaying::Init(Game& game)
	{
		assert(font_.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));
		assert(deathSoundBuffer_.loadFromFile(SOUNDS_PATH + "Death.wav"));
		deathSound_.setBuffer(deathSoundBuffer_);

		background_.setSize(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
		background_.setPosition(0.f, 0.f);
		background_.setFillColor(sf::Color(30, 30, 50));

		platform_.init();
		ball_.init();
		score_ = 0;

		scoreText_.setFont(font_);
		scoreText_.setCharacterSize(24);
		scoreText_.setFillColor(sf::Color::Yellow);

		hintText_.setFont(font_);
		hintText_.setCharacterSize(24);
		hintText_.setFillColor(sf::Color::White);
		hintText_.setString("Arrow keys to move, ESC to pause");
		hintText_.setOrigin(GetTextOrigin(hintText_, { 1.f, 0.f }));
	}

	void GameStatePlaying::Shutdown(Game& game)
	{
	}

	void GameStatePlaying::HandleWindowEvent(Game& game, sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				game.pushState(GameStateType::ExitDialog, false);
			}
		}
	}

	void GameStatePlaying::Update(Game& game, float timeDelta)
	{
		platform_.handleInput(timeDelta);

		bool bounced = ball_.update(timeDelta, platform_);
		if (bounced)
			score_++;

		if (ball_.isOutOfBounds())
		{
			deathSound_.play();

			game.getRecordsTable()[PLAYER_NAME] = std::max(game.getRecordsTable()[PLAYER_NAME], score_);

			game.pushState(GameStateType::GameOver, false);
		}

		scoreText_.setString("Score: " + std::to_string(score_));
	}

	void GameStatePlaying::Draw(Game& game, sf::RenderWindow& window)
	{
		window.draw(background_);

		platform_.draw(window);
		ball_.draw(window);

		scoreText_.setOrigin(GetTextOrigin(scoreText_, { 0.f, 0.f }));
		scoreText_.setPosition(10.f, 10.f);
		window.draw(scoreText_);

		sf::Vector2f viewSize = window.getView().getSize();
		hintText_.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(hintText_);
	}
}
