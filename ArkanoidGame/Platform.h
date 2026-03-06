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

		// Strategy pattern: width multiplier for bonus
		void setWidthMultiplier(float multiplier);
		void resetWidth();

	private:
		sf::RectangleShape shape_;
		float speed_;
		float currentWidth_ = PLATFORM_WIDTH;
	};
}
