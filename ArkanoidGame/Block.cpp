#include "Block.h"

namespace ArkanoidGame
{
	// --- Block ---

	Block::Block()
		: scoreStrategy_(std::make_unique<NormalBlockScore>())
	{
	}

	void Block::init(float x, float y, sf::Color color)
	{
		shape_.setSize(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
		shape_.setPosition(x, y);
		shape_.setFillColor(color);
		shape_.setOutlineColor(sf::Color(0, 0, 0, 120));
		shape_.setOutlineThickness(1.f);
		active_ = true;
		fragile_ = false;
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

	bool Block::isUnbreakable() const
	{
		return false;
	}

	bool Block::hit()
	{
		active_ = false;
		return true;
	}

	int Block::getScoreValue() const
	{
		return scoreStrategy_->calculateScore();
	}

	void Block::setFragile(bool fragile)
	{
		fragile_ = fragile;
		if (fragile)
		{
			shape_.setOutlineColor(sf::Color(255, 80, 80));
			shape_.setOutlineThickness(2.f);
		}
		else
		{
			shape_.setOutlineColor(sf::Color(0, 0, 0, 120));
			shape_.setOutlineThickness(1.f);
		}
	}

	bool Block::isFragile() const
	{
		return fragile_;
	}

	BlockState Block::saveState() const
	{
		return { active_, 0 };
	}

	void Block::restoreState(const BlockState& state)
	{
		active_ = state.active;
	}

	// --- UnbreakableBlock ---

	UnbreakableBlock::UnbreakableBlock()
	{
		scoreStrategy_ = std::make_unique<UnbreakableBlockScore>();
	}

	void UnbreakableBlock::init(float x, float y, sf::Color)
	{
		shape_.setSize(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
		shape_.setPosition(x, y);
		shape_.setFillColor(sf::Color(120, 120, 120));
		shape_.setOutlineColor(sf::Color(200, 200, 200));
		shape_.setOutlineThickness(2.f);
		active_ = true;
	}

	bool UnbreakableBlock::isUnbreakable() const
	{
		return true;
	}

	bool UnbreakableBlock::hit()
	{
		if (fragile_)
		{
			active_ = false;
			return true;
		}
		return false;
	}

	// --- DurableBlock ---

	DurableBlock::DurableBlock()
	{
		scoreStrategy_ = std::make_unique<DurableBlockScore>();
	}

	void DurableBlock::init(float x, float y, sf::Color)
	{
		shape_.setSize(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
		shape_.setPosition(x, y);
		shape_.setOutlineThickness(1.f);
		active_ = true;
		hitPoints_ = MULTIHIT_BLOCK_HP;
		updateAppearance();
	}

	bool DurableBlock::hit()
	{
		if (fragile_)
		{
			active_ = false;
			return true;
		}

		--hitPoints_;
		if (hitPoints_ <= 0)
		{
			active_ = false;
			return true;
		}
		updateAppearance();
		return false;
	}

	BlockState DurableBlock::saveState() const
	{
		return { active_, hitPoints_ };
	}

	void DurableBlock::restoreState(const BlockState& state)
	{
		active_ = state.active;
		hitPoints_ = state.hitPoints;
		if (active_)
			updateAppearance();
	}

	void DurableBlock::updateAppearance()
	{
		if (hitPoints_ >= 3)
		{
			shape_.setFillColor(sf::Color(180, 50, 180));   // Purple - full health
			shape_.setOutlineColor(sf::Color(220, 100, 220));
		}
		else if (hitPoints_ == 2)
		{
			shape_.setFillColor(sf::Color(180, 120, 50));   // Orange - damaged
			shape_.setOutlineColor(sf::Color(220, 160, 80));
		}
		else
		{
			shape_.setFillColor(sf::Color(200, 60, 60));    // Red - almost broken
			shape_.setOutlineColor(sf::Color(255, 100, 100));
		}
	}
}
