#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Math.h"

namespace ApplesGame
{
	struct Apple
	{
		Position2D position;
		sf::Sprite sprite;
	};

	void InitApple(Apple& apple, const sf::Texture& texture);
	void DrawApple(const Apple& apple, sf::RenderWindow& window);
}
