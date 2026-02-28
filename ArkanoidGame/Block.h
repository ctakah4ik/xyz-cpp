#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "GameSettings.h"

namespace ArkanoidGame
{
	class Block : public GameObject
	{
	public:
		Block() = default;

		void init(float x, float y, sf::Color color);
		void draw(sf::RenderWindow& window) const override;
		sf::FloatRect getBounds() const override;

		bool isActive() const;
		void destroy();

	private:
		sf::RectangleShape shape_;
		bool active_ = true;
	};
}
