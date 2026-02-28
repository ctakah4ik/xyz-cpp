#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Menu.h"

namespace ArkanoidGame
{
	class GameStatePauseMenu : public IGameStateData
	{
	public:
		void Init(Game& game) override;
		void Shutdown(Game& game) override;
		void HandleWindowEvent(Game& game, sf::Event& event) override;
		void Update(Game& game, float timeDelta) override;
		void Draw(Game& game, sf::RenderWindow& window) override;

	private:
		sf::Font font_;
		sf::RectangleShape background_;
		sf::Text titleText_;

		Menu menu_;
		MenuItem resumeItem_;
		MenuItem exitItem_;
	};
}
