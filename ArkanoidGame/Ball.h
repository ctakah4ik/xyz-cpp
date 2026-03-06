#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "GameSettings.h"
#include "Platform.h"
#include "Block.h"

namespace ArkanoidGame
{
	class Ball
	{
	public:
		Ball();

		void init();
		bool update(float timeDelta, const Platform& platform); // returns true if bounced off platform
		int checkBlockCollisions(std::vector<std::unique_ptr<Block>>& blocks,
			std::vector<sf::Vector2f>& destroyedPositions); // returns total score, fills destroyed positions
		void draw(sf::RenderWindow& window) const;
		bool isOutOfBounds() const;

		// Decorator pattern: fireball mode
		void setFireball(bool enabled);
		bool isFireball() const;

	private:
		sf::CircleShape shape_;
		sf::Vector2f velocity_;
		sf::Vector2f baseVelocity_; // velocity before fireball modifier
		bool fireball_ = false;

		void bounceOffWalls();
		void bounceOffCeiling();
		bool bounceOffPlatform(const Platform& platform);
	};
}
