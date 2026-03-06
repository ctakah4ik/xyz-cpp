#include "Ball.h"
#include <cmath>
#include <algorithm>

namespace ArkanoidGame
{
	Ball::Ball()
		: velocity_(BALL_INITIAL_SPEED_X, BALL_INITIAL_SPEED_Y)
		, baseVelocity_(BALL_INITIAL_SPEED_X, BALL_INITIAL_SPEED_Y)
	{
	}

	void Ball::init()
	{
		shape_.setRadius(BALL_RADIUS);
		shape_.setOrigin(BALL_RADIUS, BALL_RADIUS);
		shape_.setFillColor(sf::Color::White);
		// Start above platform, center of screen
		shape_.setPosition(SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT - PLATFORM_Y_OFFSET - PLATFORM_HEIGHT - BALL_RADIUS * 2.f);
		velocity_ = sf::Vector2f(BALL_INITIAL_SPEED_X, BALL_INITIAL_SPEED_Y);
		baseVelocity_ = velocity_;
		fireball_ = false;
	}

	bool Ball::update(float timeDelta, const Platform& platform)
	{
		shape_.move(velocity_ * timeDelta);

		bounceOffWalls();
		bounceOffCeiling();
		return bounceOffPlatform(platform);
	}

	void Ball::draw(sf::RenderWindow& window) const
	{
		window.draw(shape_);
	}

	bool Ball::isOutOfBounds() const
	{
		return shape_.getPosition().y - BALL_RADIUS > (float)SCREEN_HEIGHT;
	}

	void Ball::setFireball(bool enabled)
	{
		if (enabled && !fireball_)
		{
			fireball_ = true;
			baseVelocity_ = velocity_;
			velocity_.x *= FIREBALL_SPEED_MULTIPLIER;
			velocity_.y *= FIREBALL_SPEED_MULTIPLIER;
			shape_.setFillColor(sf::Color(255, 140, 0)); // Orange
		}
		else if (!enabled && fireball_)
		{
			fireball_ = false;
			// Restore direction but with base speed magnitude
			float currentSpeed = std::sqrt(velocity_.x * velocity_.x + velocity_.y * velocity_.y);
			float baseSpeed = std::sqrt(baseVelocity_.x * baseVelocity_.x + baseVelocity_.y * baseVelocity_.y);
			if (currentSpeed > 0.f)
			{
				velocity_.x = (velocity_.x / currentSpeed) * baseSpeed;
				velocity_.y = (velocity_.y / currentSpeed) * baseSpeed;
			}
			shape_.setFillColor(sf::Color::White);
		}
	}

	bool Ball::isFireball() const
	{
		return fireball_;
	}

	void Ball::bounceOffWalls()
	{
		float x = shape_.getPosition().x;

		// Left wall
		if (x - BALL_RADIUS <= 0.f)
		{
			shape_.setPosition(BALL_RADIUS, shape_.getPosition().y);
			velocity_.x = std::fabs(velocity_.x);
		}
		// Right wall
		if (x + BALL_RADIUS >= (float)SCREEN_WIDTH)
		{
			shape_.setPosition((float)SCREEN_WIDTH - BALL_RADIUS, shape_.getPosition().y);
			velocity_.x = -std::fabs(velocity_.x);
		}
	}

	void Ball::bounceOffCeiling()
	{
		float y = shape_.getPosition().y;
		if (y - BALL_RADIUS <= 0.f)
		{
			shape_.setPosition(shape_.getPosition().x, BALL_RADIUS);
			velocity_.y = std::fabs(velocity_.y);
		}
	}

	int Ball::checkBlockCollisions(std::vector<std::unique_ptr<Block>>& blocks,
		std::vector<sf::Vector2f>& destroyedPositions)
	{
		int totalScore = 0;
		sf::Vector2f ballPos = shape_.getPosition();
		sf::FloatRect ballBounds(ballPos.x - BALL_RADIUS, ballPos.y - BALL_RADIUS,
			BALL_RADIUS * 2.f, BALL_RADIUS * 2.f);

		for (auto& block : blocks)
		{
			if (!block->isActive())
				continue;

			sf::FloatRect blockBounds = block->getBounds();
			if (!ballBounds.intersects(blockBounds))
				continue;

			// Remember position before hit (block may become inactive)
			float cx = blockBounds.left + blockBounds.width / 2.f;
			float cy = blockBounds.top + blockBounds.height / 2.f;

			bool wasDestroyed = block->hit();
			if (wasDestroyed)
			{
				totalScore += block->getScoreValue();
				destroyedPositions.push_back({ cx, cy });
			}

			if (!fireball_)
			{
				// Determine collision axis by finding the smallest overlap
				float overlapLeft   = (ballPos.x + BALL_RADIUS) - blockBounds.left;
				float overlapRight  = (blockBounds.left + blockBounds.width) - (ballPos.x - BALL_RADIUS);
				float overlapTop    = (ballPos.y + BALL_RADIUS) - blockBounds.top;
				float overlapBottom = (blockBounds.top + blockBounds.height) - (ballPos.y - BALL_RADIUS);

				float minH = std::min(overlapLeft, overlapRight);
				float minV = std::min(overlapTop, overlapBottom);

				if (minH < minV)
					velocity_.x = -velocity_.x;
				else
					velocity_.y = -velocity_.y;

				// Only handle one block per frame to avoid double-reversals
				break;
			}
			// Fireball: don't bounce, continue to next block (pierce through)
		}

		return totalScore;
	}

	bool Ball::bounceOffPlatform(const Platform& platform)
	{
		sf::FloatRect platformBounds = platform.getBounds();
		sf::Vector2f ballPos = shape_.getPosition();

		// Only bounce if ball is moving downward
		if (velocity_.y <= 0.f)
			return false;

		// Check intersection with platform
		if (ballPos.x + BALL_RADIUS >= platformBounds.left &&
			ballPos.x - BALL_RADIUS <= platformBounds.left + platformBounds.width &&
			ballPos.y + BALL_RADIUS >= platformBounds.top &&
			ballPos.y - BALL_RADIUS <= platformBounds.top + platformBounds.height)
		{
			// Place ball above platform
			shape_.setPosition(ballPos.x, platformBounds.top - BALL_RADIUS);

			// Reflect Y
			velocity_.y = -std::fabs(velocity_.y);

			// Adjust X angle based on hit position (hit left side = go left, right = go right)
			float platformCenterX = platformBounds.left + platformBounds.width / 2.f;
			float hitOffset = (ballPos.x - platformCenterX) / (platformBounds.width / 2.f); // -1 to 1
			velocity_.x = hitOffset * std::fabs(velocity_.y); // angle depends on offset

			return true;
		}

		return false;
	}
}
