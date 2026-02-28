#include "Platform.h"
#include <cmath>

namespace ArkanoidGame
{
	Platform::Platform()
		: speed_(PLATFORM_SPEED)
	{
	}

	void Platform::init()
	{
		shape_.setSize(sf::Vector2f(PLATFORM_WIDTH, PLATFORM_HEIGHT));
		shape_.setOrigin(PLATFORM_WIDTH / 2.f, PLATFORM_HEIGHT / 2.f);
		shape_.setFillColor(sf::Color::White);
		shape_.setPosition(SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT - PLATFORM_Y_OFFSET);
	}

	void Platform::handleInput(float timeDelta)
	{
		float dx = 0.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			dx -= speed_ * timeDelta;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			dx += speed_ * timeDelta;

		float newX = shape_.getPosition().x + dx;

		// Clamp to screen bounds
		float halfW = PLATFORM_WIDTH / 2.f;
		if (newX < halfW)
			newX = halfW;
		if (newX > SCREEN_WIDTH - halfW)
			newX = (float)SCREEN_WIDTH - halfW;

		shape_.setPosition(newX, shape_.getPosition().y);
	}

	void Platform::draw(sf::RenderWindow& window) const
	{
		window.draw(shape_);
	}

	sf::FloatRect Platform::getBounds() const
	{
		return shape_.getGlobalBounds();
	}
}
