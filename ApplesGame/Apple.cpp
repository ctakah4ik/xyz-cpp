#include "Apple.h"

namespace ApplesGame
{
	void InitApple(Apple& apple, const sf::Texture& texture)
	{
		apple.position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

		apple.sprite.setTexture(texture);
		SetSpriteSize(apple.sprite, APPLE_SIZE, APPLE_SIZE);
		SetSpriteRelativeOrigin(apple.sprite, 0.5f, 0.5f);
		apple.sprite.setPosition(apple.position.x, apple.position.y);
	}

	void DrawApple(const Apple& apple, sf::RenderWindow& window)
	{
		window.draw(apple.sprite);
	}
}
