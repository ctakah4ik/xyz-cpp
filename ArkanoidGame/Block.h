#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "GameObject.h"
#include "GameSettings.h"
#include "ScoreStrategy.h"

namespace ArkanoidGame
{
	struct BlockState
	{
		bool active = true;
		int hitPoints = 0;
	};

	class Block : public GameObject
	{
	public:
		Block();
		virtual ~Block() = default;

		virtual void init(float x, float y, sf::Color color);
		void draw(sf::RenderWindow& window) const override;
		sf::FloatRect getBounds() const override;

		bool isActive() const;
		virtual bool isUnbreakable() const;
		// Returns true if the block was destroyed by this hit
		virtual bool hit();

		// Strategy pattern: score value delegation
		int getScoreValue() const;

		// State pattern: fragile mode
		void setFragile(bool fragile);
		bool isFragile() const;

		// Memento pattern: save/restore state
		virtual BlockState saveState() const;
		virtual void restoreState(const BlockState& state);

	protected:
		sf::RectangleShape shape_;
		bool active_ = true;
		bool fragile_ = false;
		std::unique_ptr<IScoreStrategy> scoreStrategy_;
	};

	class UnbreakableBlock : public Block
	{
	public:
		UnbreakableBlock();
		void init(float x, float y, sf::Color color) override;
		bool isUnbreakable() const override;
		bool hit() override;
	};

	class DurableBlock : public Block
	{
	public:
		DurableBlock();
		void init(float x, float y, sf::Color color) override;
		bool hit() override;

		BlockState saveState() const override;
		void restoreState(const BlockState& state) override;

	private:
		int hitPoints_ = MULTIHIT_BLOCK_HP;
		void updateAppearance();
	};
}
