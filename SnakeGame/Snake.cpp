#include "Snake.h"
#include <cassert>

namespace SnakeGame
{
	void InitSnake(Snake& snake, float speed)
	{
		snake.segments.clear();
		snake.nextDirection = Direction::Right;
		snake.moveTimer = 0.f;
		snake.moveInterval = 1.0f / speed;
		snake.alive = true;
		snake.pendingGrowth = 0;

		int startX = GRID_WIDTH / 2;
		int startY = GRID_HEIGHT / 2;

		for (int i = 0; i < INITIAL_SNAKE_LENGTH; ++i)
		{
			Segment seg;
			seg.position = { startX - i, startY };
			seg.direction = Direction::Right;
			snake.segments.push_back(seg);
		}
	}

	void SetSnakeDirection(Snake& snake, Direction dir)
	{
		assert(!snake.segments.empty());
		Direction current = snake.segments[0].direction;

		if (dir == Direction::Right && current == Direction::Left) return;
		if (dir == Direction::Left && current == Direction::Right) return;
		if (dir == Direction::Up && current == Direction::Down) return;
		if (dir == Direction::Down && current == Direction::Up) return;

		snake.nextDirection = dir;
	}

	bool UpdateSnake(Snake& snake, float deltaTime)
	{
		if (!snake.alive)
			return false;

		snake.moveTimer += deltaTime;

		if (snake.moveTimer < snake.moveInterval)
			return false;

		snake.moveTimer -= snake.moveInterval;

		Position2D oldTailPos = snake.segments.back().position;
		Direction oldTailDir = snake.segments.back().direction;

		for (int i = (int)snake.segments.size() - 1; i > 0; --i)
		{
			snake.segments[i].position = snake.segments[i - 1].position;
			snake.segments[i].direction = snake.segments[i - 1].direction;
		}

		snake.segments[0].direction = snake.nextDirection;

		switch (snake.nextDirection)
		{
		case Direction::Right: 
			snake.segments[0].position.x += 1; 
			break;
		case Direction::Left:  
			snake.segments[0].position.x -= 1; 
			break;
		case Direction::Up:    
			snake.segments[0].position.y -= 1; 
			break;
		case Direction::Down:  
			snake.segments[0].position.y += 1; 
			break;
		}

		if (snake.pendingGrowth > 0)
		{
			Segment newTail;
			newTail.position = oldTailPos;
			newTail.direction = oldTailDir;
			snake.segments.push_back(newTail);
			--snake.pendingGrowth;
		}

		return true;
	}

	void GrowSnake(Snake& snake, int amount)
	{
		snake.pendingGrowth += amount;
	}

	void SetSpriteToCell(sf::Sprite& sprite, const sf::Texture& texture, float x, float y)
	{
		sprite.setTexture(texture);
		sf::Vector2u size = texture.getSize();
		if (size.x > 0 && size.y > 0)
		{
			sprite.setScale(
				(float)CELL_SIZE / (float)size.x,
				(float)CELL_SIZE / (float)size.y
			);
		}
		sprite.setPosition(x, y);
	}

	// Determine relative direction from pos to neighbor
	enum class RelDir { Up, Down, Left, Right };

	static RelDir GetRelativeDir(const Position2D& from, const Position2D& to)
	{
		int dx = to.x - from.x;
		int dy = to.y - from.y;

		if (dx > 0) return RelDir::Right;
		if (dx < 0) return RelDir::Left;
		if (dy < 0) return RelDir::Up;
		return RelDir::Down;
	}

	void DrawSnake(const Snake& snake, const SnakeTextures& tex, sf::RenderWindow& window)
	{
		sf::Sprite sprite;

		for (size_t i = 0; i < snake.segments.size(); ++i)
		{
			const Segment& seg = snake.segments[i];
			float px = (float)seg.position.x * CELL_SIZE;
			float py = (float)seg.position.y * CELL_SIZE + GRID_OFFSET_Y;

			if (i == 0)
			{
				int dirIdx = (int)seg.direction;
				if (tex.head[dirIdx])
					SetSpriteToCell(sprite, *tex.head[dirIdx], px, py);
			}
			else if (i == snake.segments.size() - 1)
			{
				RelDir tipDir = GetRelativeDir(snake.segments[i - 1].position, seg.position);
				int dirIdx;
				switch (tipDir)
				{
				case RelDir::Up:    
					dirIdx = (int)Direction::Up;    
					break;
				case RelDir::Down:  
					dirIdx = (int)Direction::Down;  
					break;
				case RelDir::Left:  
					dirIdx = (int)Direction::Left;  
					break;
				case RelDir::Right: 
					dirIdx = (int)Direction::Right; 
					break;
				default: 
					dirIdx = 0; 
					break;
				}
				if (tex.tail[dirIdx])
					SetSpriteToCell(sprite, *tex.tail[dirIdx], px, py);
			}
			else
			{
				// Body: determine texture from neighbors
				RelDir toHead = GetRelativeDir(seg.position, snake.segments[i - 1].position);
				RelDir toTail = GetRelativeDir(seg.position, snake.segments[i + 1].position);

				const sf::Texture* bodyTex = nullptr;

				// Straight pieces
				if ((toHead == RelDir::Left && toTail == RelDir::Right) ||
					(toHead == RelDir::Right && toTail == RelDir::Left))
				{
					bodyTex = tex.bodyH;
				}
				else if ((toHead == RelDir::Up && toTail == RelDir::Down) ||
					(toHead == RelDir::Down && toTail == RelDir::Up))
				{
					bodyTex = tex.bodyV;
				}
				else if ((toHead == RelDir::Up && toTail == RelDir::Left) ||
					(toHead == RelDir::Left && toTail == RelDir::Up))
				{
					bodyTex = tex.bodyTL;
				}
				else if ((toHead == RelDir::Up && toTail == RelDir::Right) ||
					(toHead == RelDir::Right && toTail == RelDir::Up))
				{
					bodyTex = tex.bodyTR;
				}
				else if ((toHead == RelDir::Down && toTail == RelDir::Left) ||
					(toHead == RelDir::Left && toTail == RelDir::Down))
				{
					bodyTex = tex.bodyBL;
				}
				else if ((toHead == RelDir::Down && toTail == RelDir::Right) ||
					(toHead == RelDir::Right && toTail == RelDir::Down))
				{
					bodyTex = tex.bodyBR;
				}

				if (bodyTex)
					SetSpriteToCell(sprite, *bodyTex, px, py);
			}

			window.draw(sprite);
		}
	}

	const Segment& GetHead(const Snake& snake)
	{
		assert(!snake.segments.empty());
		return snake.segments[0];
	}

	bool IsPositionOnSnake(const Snake& snake, const Position2D& pos)
	{
		for (const Segment& seg : snake.segments)
		{
			if (seg.position == pos)
				return true;
		}
		return false;
	}

	bool IsSnakeCollidingSelf(const Snake& snake)
	{
		assert(!snake.segments.empty());
		const Position2D& headPos = snake.segments[0].position;
		for (size_t i = 1; i < snake.segments.size(); ++i)
		{
			if (snake.segments[i].position == headPos)
				return true;
		}
		return false;
	}


}
