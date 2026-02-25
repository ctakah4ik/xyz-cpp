#include "Rock.h"

namespace ApplesGame
{
	void InitRock(Rock& rock, const sf::Texture& texture)
	{
		rock.position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

		rock.sprite.setTexture(texture);
		SetSpriteSize(rock.sprite, ROCK_SIZE, ROCK_SIZE);
		SetSpriteRelativeOrigin(rock.sprite, 0.5f, 0.5f);
		rock.sprite.setPosition(rock.position.x, rock.position.y);
	}

	void DrawRock(const Rock& rock, sf::RenderWindow& window)
	{
		window.draw(rock.sprite);
	}
}
