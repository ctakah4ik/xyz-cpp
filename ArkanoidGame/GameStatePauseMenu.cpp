#include "GameStatePauseMenu.h"
#include <cassert>

namespace ArkanoidGame
{
	void GameStatePauseMenu::Init(Game& game)
	{
		assert(font_.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		background_.setFillColor(sf::Color(0, 0, 0, 128));

		titleText_.setString("Pause");
		titleText_.setFont(font_);
		titleText_.setCharacterSize(48);
		titleText_.setFillColor(sf::Color::Red);

		menu_.getRootItem().setChildrenLayout(Orientation::Vertical, Alignment::Middle, 10.f);
		menu_.getRootItem().addChild(&resumeItem_);
		menu_.getRootItem().addChild(&exitItem_);

		resumeItem_.setText("Return to game", font_, 24);
		exitItem_.setText("Exit to main menu", font_, 24);

		menu_.init();
		menu_.select(&resumeItem_);
	}

	void GameStatePauseMenu::Shutdown(Game& game)
	{
	}

	void GameStatePauseMenu::HandleWindowEvent(Game& game, sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				game.popState();
			}

			if (menu_.getSelectedItem() == nullptr)
				return;

			if (event.key.code == sf::Keyboard::Enter)
			{
				if (menu_.getSelectedItem() == &resumeItem_)
				{
					game.popState();
				}
				else if (menu_.getSelectedItem() == &exitItem_)
				{
					game.switchState(GameStateType::MainMenu);
				}
			}

			if (event.key.code == sf::Keyboard::Up)
			{
				menu_.selectPrevious();
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				menu_.selectNext();
			}
		}
	}

	void GameStatePauseMenu::Update(Game& game, float timeDelta)
	{
	}

	void GameStatePauseMenu::Draw(Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		background_.setSize(viewSize);
		window.draw(background_);

		titleText_.setOrigin(GetTextOrigin(titleText_, { 0.5f, 0.f }));
		titleText_.setPosition(viewSize.x / 2.f, 100);
		window.draw(titleText_);

		menu_.draw(window, window.getView().getCenter(), { 0.5f, 0.f });
	}
}
