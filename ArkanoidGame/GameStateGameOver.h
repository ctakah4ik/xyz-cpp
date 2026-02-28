#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Game.h"

namespace ArkanoidGame
{
	class GameStateGameOver : public IGameStateData
	{
	public:
		void Init(Game& game) override;
		void Shutdown(Game& game) override;
		void HandleWindowEvent(Game& game, sf::Event& event) override;
		void Update(Game& game, float timeDelta) override;
		void Draw(Game& game, sf::RenderWindow& window) override;

	private:
		sf::Font font_;
		float timeSinceGameOver_ = 0.f;

		sf::RectangleShape background_;
		sf::Text gameOverText_;
		sf::Text hintText_;
		std::vector<sf::Text> recordsTableTexts_;
	};
}
