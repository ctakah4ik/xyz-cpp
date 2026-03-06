#include "Bonus.h"

namespace ArkanoidGame
{
	// --- Bonus base ---

	Bonus::Bonus(BonusType type, float x, float y)
		: type_(type)
	{
		shape_.setSize(sf::Vector2f(BONUS_WIDTH, BONUS_HEIGHT));
		shape_.setOrigin(BONUS_WIDTH / 2.f, BONUS_HEIGHT / 2.f);
		shape_.setPosition(x, y);
		shape_.setOutlineColor(sf::Color::White);
		shape_.setOutlineThickness(1.f);
		active_ = true;
	}

	void Bonus::update(float timeDelta)
	{
		if (!active_) return;
		shape_.move(0.f, BONUS_FALL_SPEED * timeDelta);
	}

	void Bonus::draw(sf::RenderWindow& window) const
	{
		if (active_)
		{
			window.draw(shape_);
		}
	}

	sf::FloatRect Bonus::getBounds() const
	{
		return shape_.getGlobalBounds();
	}

	bool Bonus::isActive() const
	{
		return active_;
	}

	void Bonus::deactivate()
	{
		active_ = false;
	}

	BonusType Bonus::getType() const
	{
		return type_;
	}

	bool Bonus::isOutOfBounds() const
	{
		return shape_.getPosition().y - BONUS_HEIGHT / 2.f > (float)SCREEN_HEIGHT;
	}

	// --- FireballBonus (Decorator) ---

	FireballBonus::FireballBonus(float x, float y)
		: Bonus(BonusType::Fireball, x, y)
	{
		shape_.setFillColor(getColor());
	}

	float FireballBonus::getDuration() const
	{
		return FIREBALL_DURATION;
	}

	sf::Color FireballBonus::getColor() const
	{
		return sf::Color(255, 140, 0); // Orange
	}

	// --- FragileBlocksBonus (State) ---

	FragileBlocksBonus::FragileBlocksBonus(float x, float y)
		: Bonus(BonusType::FragileBlocks, x, y)
	{
		shape_.setFillColor(getColor());
	}

	float FragileBlocksBonus::getDuration() const
	{
		return FRAGILE_DURATION;
	}

	sf::Color FragileBlocksBonus::getColor() const
	{
		return sf::Color(255, 80, 80); // Red
	}

	// --- WidePlatformBonus (Strategy) ---

	WidePlatformBonus::WidePlatformBonus(float x, float y)
		: Bonus(BonusType::WidePlatform, x, y)
	{
		shape_.setFillColor(getColor());
	}

	float WidePlatformBonus::getDuration() const
	{
		return WIDE_PLATFORM_DURATION;
	}

	sf::Color WidePlatformBonus::getColor() const
	{
		return sf::Color(100, 255, 100); // Green
	}
}
