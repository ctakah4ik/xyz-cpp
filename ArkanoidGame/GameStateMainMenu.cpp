#include "GameStateMainMenu.h"
#include <cassert>

namespace ArkanoidGame
{
	void GameStateMainMenu::Init(Game& game)
	{
		assert(font_.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		menu_.getRootItem().setHintText("Arkanoid Game", font_, 48, sf::Color::Red);
		menu_.getRootItem().setChildrenLayout(Orientation::Vertical, Alignment::Middle, 10.f);
		menu_.getRootItem().addChild(&startGameItem_);
		menu_.getRootItem().addChild(&recordsItem_);
		menu_.getRootItem().addChild(&exitGameItem_);

		startGameItem_.setText("Start Game", font_, 24);

		recordsItem_.setText("Records", font_, 24);

		exitGameItem_.setText("Exit Game", font_, 24);
		exitGameItem_.setHintText("Are you sure?", font_, 48, sf::Color::Red);
		exitGameItem_.setChildrenLayout(Orientation::Horizontal, Alignment::Middle, 10.f);
		exitGameItem_.addChild(&yesItem_);
		exitGameItem_.addChild(&noItem_);

		yesItem_.setText("Yes", font_, 24);
		noItem_.setText("No", font_, 24);

		menu_.init();
		menu_.select(&startGameItem_);
	}

	void GameStateMainMenu::Shutdown(Game& game)
	{
	}

	void GameStateMainMenu::HandleWindowEvent(Game& game, sf::Event& event)
	{
		if (!menu_.getSelectedItem())
			return;

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				menu_.collapseSelected();
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				if (menu_.getSelectedItem() == &startGameItem_)
				{
					game.switchState(GameStateType::Playing);
				}
				else if (menu_.getSelectedItem() == &recordsItem_)
				{
					game.pushState(GameStateType::Records, true);
				}
				else if (menu_.getSelectedItem() == &exitGameItem_)
				{
					menu_.expandSelected();
				}
				else if (menu_.getSelectedItem() == &yesItem_)
				{
					game.switchState(GameStateType::None);
				}
				else if (menu_.getSelectedItem() == &noItem_)
				{
					menu_.collapseSelected();
				}
				else
				{
					menu_.expandSelected();
				}
			}

			Orientation orientation = menu_.getSelectedItem()->getParent()->getChildrenOrientation();
			if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Up ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Left)
			{
				menu_.selectPrevious();
			}
			else if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Down ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Right)
			{
				menu_.selectNext();
			}
		}
	}

	void GameStateMainMenu::Update(Game& game, float timeDelta)
	{
	}

	void GameStateMainMenu::Draw(Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		sf::Text* hintText = &menu_.getCurrentContext()->getHintText();
		hintText->setOrigin(GetTextOrigin(*hintText, { 0.5f, 0.f }));
		hintText->setPosition(viewSize.x / 2.f, 150.f);
		window.draw(*hintText);

		menu_.draw(window, viewSize / 2.f, { 0.5f, 0.f });
	}
}
