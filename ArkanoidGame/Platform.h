#pragma once
#include <SFML/Graphics.hpp>
#include "GameSettings.h"

namespace ArkanoidGame
{
	class Platform
	{
	public:
		Platform();

		void init();
		void handleInput(float timeDelta);
		void draw(sf::RenderWindow& window) const;
		sf::FloatRect getBounds() const;

	private:
		sf::RectangleShape shape_;
		float speed_;
	};
}
