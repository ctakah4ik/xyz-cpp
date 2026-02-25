#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Math.h"

namespace ApplesGame
{
	enum class PlayerDirection
	{
		Right = 0,
		Up,
		Left,
		Down
	};

	struct Player
	{
		Position2D position;
		PlayerDirection direction = PlayerDirection::Right; // current movement direction
		float speed = INITIAL_SPEED;
		sf::Sprite sprite;
	};

	struct Game;

	void InitPlayer(Player& player, const Game& game);
	void UpdatePlayer(Player& player, float deltaTime);
	void DrawPlayer(Player& plater, sf::RenderWindow& window);
	bool IsPlayerCollidesWithScreenBorder(const Player& player);
}
