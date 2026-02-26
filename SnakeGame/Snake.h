#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"

namespace SnakeGame
{
	enum class Direction
	{
		Right = 0,
		Up,
		Left,
		Down
	};

	struct Segment
	{
		Position2D position;
		Direction direction = Direction::Right;
	};

	struct Snake
	{
		std::vector<Segment> segments;
		Direction nextDirection = Direction::Right;
		float moveTimer = 0.f;
		float moveInterval = 1.0f; // set from difficulty
		bool alive = true;
		int pendingGrowth = 0; // segments to add
	};

	// Texture pack for rendering the snake
	struct SnakeTextures
	{
		const sf::Texture* head[4] = {};   // indexed by Direction (Right, Up, Left, Down)
		const sf::Texture* bodyH = nullptr;
		const sf::Texture* bodyV = nullptr;
		const sf::Texture* bodyTL = nullptr;
		const sf::Texture* bodyTR = nullptr;
		const sf::Texture* bodyBL = nullptr;
		const sf::Texture* bodyBR = nullptr;
		const sf::Texture* tail[4] = {};   // indexed by Direction
	};
	
	void SetSpriteToCell(sf::Sprite& sprite, const sf::Texture& texture, float x, float y);

	void InitSnake(Snake& snake, float speed);
	void SetSnakeDirection(Snake& snake, Direction dir);
	bool UpdateSnake(Snake& snake, float deltaTime);
	void GrowSnake(Snake& snake, int amount);
	void DrawSnake(const Snake& snake, const SnakeTextures& textures, sf::RenderWindow& window);

	const Segment& GetHead(const Snake& snake);
	bool IsPositionOnSnake(const Snake& snake, const Position2D& pos);
	bool IsSnakeCollidingSelf(const Snake& snake);
}
