#include "Apple.h"
#include "Snake.h"
#include <cstdlib>

namespace SnakeGame
{
	void SpawnApple(Apple& apple, const Snake& snake)
	{
		// Playable area excludes the 1-cell-wide wall border
		do
		{
			apple.position.x = 1 + rand() % (GRID_WIDTH - 2);
			apple.position.y = 1 + rand() % (GRID_HEIGHT - 2);
		}
		while (IsPositionOnSnake(snake, apple.position));
	}

	void DrawApple(const Apple& apple, const sf::Texture& texture, sf::RenderWindow& window)
	{
		sf::Sprite sprite;
		SetSpriteToCell(sprite, texture,
			(float)apple.position.x * CELL_SIZE,
			(float)apple.position.y * CELL_SIZE + GRID_OFFSET_Y);
		window.draw(sprite);
	}
}
