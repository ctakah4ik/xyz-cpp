#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Menu.h"

namespace ArkanoidGame
{
	class GameStateMainMenu : public IGameStateData
	{
	public:
		void Init(Game& game) override;
		void Shutdown(Game& game) override;
		void HandleWindowEvent(Game& game, sf::Event& event) override;
		void Update(Game& game, float timeDelta) override;
		void Draw(Game& game, sf::RenderWindow& window) override;

	private:
		sf::Font font_;

		MenuItem startGameItem_;
		MenuItem recordsItem_;
		MenuItem exitGameItem_;
		MenuItem yesItem_;
		MenuItem noItem_;
		Menu menu_;
	};
}
