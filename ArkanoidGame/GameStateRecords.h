#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Game.h"

namespace ArkanoidGame
{
	class GameStateRecords : public IGameStateData
	{
	public:
		void Init(Game& game) override;
		void Shutdown(Game& game) override;
		void HandleWindowEvent(Game& game, sf::Event& event) override;
		void Update(Game& game, float timeDelta) override;
		void Draw(Game& game, sf::RenderWindow& window) override;

	private:
		sf::Font font_;
		sf::Text titleText_;
		std::vector<sf::Text> tableTexts_;
		sf::Text hintText_;
	};
}
