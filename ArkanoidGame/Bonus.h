#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "GameSettings.h"

namespace ArkanoidGame
{
	// Bonus type enum
	enum class BonusType
	{
		Fireball,       // Decorator pattern: ball pierces blocks and moves faster
		FragileBlocks,  // State pattern: all blocks break in one hit
		WidePlatform,   // Strategy pattern: platform becomes wider
	};

	// Base bonus class - falls down from destroyed blocks
	class Bonus : public GameObject
	{
	public:
		Bonus(BonusType type, float x, float y);
		virtual ~Bonus() = default;

		void update(float timeDelta);
		void draw(sf::RenderWindow& window) const override;
		sf::FloatRect getBounds() const override;

		bool isActive() const;
		void deactivate();
		BonusType getType() const;
		bool isOutOfBounds() const;

		virtual float getDuration() const = 0;
		virtual sf::Color getColor() const = 0;

	protected:
		sf::RectangleShape shape_;
		BonusType type_;
		bool active_ = true;
	};

	// Decorator pattern: Fireball bonus
	// Decorates ball behavior - adds piercing and speed boost
	class FireballBonus : public Bonus
	{
	public:
		FireballBonus(float x, float y);
		float getDuration() const override;
		sf::Color getColor() const override;
	};

	// State pattern: Fragile blocks bonus
	// Changes blocks' state so they break in one hit
	class FragileBlocksBonus : public Bonus
	{
	public:
		FragileBlocksBonus(float x, float y);
		float getDuration() const override;
		sf::Color getColor() const override;
	};

	// Strategy pattern: Wide platform bonus
	// Changes platform's size strategy
	class WidePlatformBonus : public Bonus
	{
	public:
		WidePlatformBonus(float x, float y);
		float getDuration() const override;
		sf::Color getColor() const override;
	};
}
