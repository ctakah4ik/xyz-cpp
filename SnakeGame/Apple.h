#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"

namespace SnakeGame
{
	struct Snake;

	struct Apple
	{
		Position2D position;
	};

	void SpawnApple(Apple& apple, const Snake& snake);
	void DrawApple(const Apple& apple, const sf::Texture& texture, sf::RenderWindow& window);
}
