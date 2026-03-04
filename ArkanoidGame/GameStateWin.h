#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"

namespace ArkanoidGame
{
	class GameStateWin : public IGameStateData
	{
	public:
		void Init(Game& game) override;
		void Shutdown(Game& game) override;
		void HandleWindowEvent(Game& game, sf::Event& event) override;
		void Update(Game& game, float timeDelta) override;
		void Draw(Game& game, sf::RenderWindow& window) override;

	private:
		sf::Font font_;
		sf::Text congratsText_;
		sf::Text promptText_;
		sf::Text yesText_;
		sf::Text noText_;
		int selectedOption_ = 0; // 0 = Yes, 1 = No
	};
}
