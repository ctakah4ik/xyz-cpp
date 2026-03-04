#include "Block.h"

namespace ArkanoidGame
{
	void Block::init(float x, float y, sf::Color color)
	{
		shape_.setSize(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
		shape_.setPosition(x, y);
		shape_.setFillColor(color);
		shape_.setOutlineColor(sf::Color(0, 0, 0, 120));
		shape_.setOutlineThickness(1.f);
		active_ = true;
	}

	void Block::draw(sf::RenderWindow& window) const
	{
		if (active_)
			window.draw(shape_);
	}

	sf::FloatRect Block::getBounds() const
	{
		return shape_.getGlobalBounds();
	}

	bool Block::isActive() const
	{
		return active_;
	}

	void Block::destroy()
	{
		active_ = false;
	}
}
