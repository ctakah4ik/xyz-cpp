#include "Player.h"
#include "Game.h"
#include <cmath>

namespace ApplesGame
{
	void InitPlayer(Player& player, const Game& game)
	{
		// Init player state
		player.position = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
		player.speed = INITIAL_SPEED;
		player.direction = PlayerDirection::Right;

		// Init player sprite
		player.sprite.setTexture(game.playerTexture);
		SetSpriteSize(player.sprite, PLAYER_SIZE, PLAYER_SIZE);
		SetSpriteRelativeOrigin(player.sprite, 0.5f, 0.5f);
	}

	void UpdatePlayer(Player& player, float deltaTime)
	{
		// Handle input - change direction
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			player.direction = PlayerDirection::Right;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			player.direction = PlayerDirection::Up;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			player.direction = PlayerDirection::Left;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			player.direction = PlayerDirection::Down;
		}

		// Move player automatically in current direction
		switch (player.direction)
		{
		case PlayerDirection::Right:
			player.position.x += player.speed * deltaTime;
			player.sprite.setRotation(0.f);
			break;
		case PlayerDirection::Up:
			player.position.y -= player.speed * deltaTime;
			player.sprite.setRotation(-90.f);
			break;
		case PlayerDirection::Left:
			player.position.x -= player.speed * deltaTime;
			player.sprite.setRotation(180.f);
			break;
		case PlayerDirection::Down:
			player.position.y += player.speed * deltaTime;
			player.sprite.setRotation(90.f);
			break;
		}

		// Update sprite position
		player.sprite.setPosition(player.position.x, player.position.y);
	}

	void DrawPlayer(Player& player, sf::RenderWindow& window)
	{
		window.draw(player.sprite);
	}

	bool IsPlayerCollidesWithScreenBorder(const Player& player)
	{
		auto bounds = player.sprite.getGlobalBounds();
		return bounds.left < 0.f || (bounds.left + bounds.width) > SCREEN_WIDTH ||
			bounds.top < 0.f || (bounds.top + bounds.height) > SCREEN_HEIGHT;
	}
}
