#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "GameSettings.h"

namespace ArkanoidGame
{
	class Block : public GameObject
	{
	public:
		Block() = default;
		virtual ~Block() = default;

		virtual void init(float x, float y, sf::Color color);
		void draw(sf::RenderWindow& window) const override;
		sf::FloatRect getBounds() const override;

		bool isActive() const;
		virtual bool isUnbreakable() const;
		// Returns true if the block was destroyed by this hit
		virtual bool hit();

	protected:
		sf::RectangleShape shape_;
		bool active_ = true;
	};

	class UnbreakableBlock : public Block
	{
	public:
		void init(float x, float y, sf::Color color) override;
		bool isUnbreakable() const override;
		bool hit() override;
	};

	class DurableBlock : public Block
	{
	public:
		void init(float x, float y, sf::Color color) override;
		bool hit() override;

	private:
		int hitPoints_ = MULTIHIT_BLOCK_HP;
		void updateAppearance();
	};
}
