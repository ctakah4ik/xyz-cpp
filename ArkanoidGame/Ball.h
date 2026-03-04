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
		int checkBlockCollisions(std::vector<std::unique_ptr<Block>>& blocks); // returns number of blocks destroyed
		void draw(sf::RenderWindow& window) const;
		bool isOutOfBounds() const;

	private:
		sf::CircleShape shape_;
		sf::Vector2f velocity_;

		void bounceOffWalls();
		void bounceOffCeiling();
		bool bounceOffPlatform(const Platform& platform);
	};
}
