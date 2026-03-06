#include "Platform.h"
#include <cmath>

namespace ArkanoidGame
{
	Platform::Platform()
		: speed_(PLATFORM_SPEED)
		, currentWidth_(PLATFORM_WIDTH)
	{
	}

	void Platform::init()
	{
		currentWidth_ = PLATFORM_WIDTH;
		shape_.setSize(sf::Vector2f(currentWidth_, PLATFORM_HEIGHT));
		shape_.setOrigin(currentWidth_ / 2.f, PLATFORM_HEIGHT / 2.f);
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

		// Clamp to screen bounds using current width
		float halfW = currentWidth_ / 2.f;
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

	void Platform::setWidthMultiplier(float multiplier)
	{
		currentWidth_ = PLATFORM_WIDTH * multiplier;
		float posX = shape_.getPosition().x;
		float posY = shape_.getPosition().y;
		shape_.setSize(sf::Vector2f(currentWidth_, PLATFORM_HEIGHT));
		shape_.setOrigin(currentWidth_ / 2.f, PLATFORM_HEIGHT / 2.f);
		shape_.setPosition(posX, posY);
		shape_.setFillColor(sf::Color(100, 255, 100)); // Green when wide
	}

	void Platform::resetWidth()
	{
		currentWidth_ = PLATFORM_WIDTH;
		float posX = shape_.getPosition().x;
		float posY = shape_.getPosition().y;
		shape_.setSize(sf::Vector2f(currentWidth_, PLATFORM_HEIGHT));
		shape_.setOrigin(currentWidth_ / 2.f, PLATFORM_HEIGHT / 2.f);
		shape_.setPosition(posX, posY);
		shape_.setFillColor(sf::Color::White);
	}
}
