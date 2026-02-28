#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"

namespace ArkanoidGame
{
	class Application
	{
	public:
		Application(const std::string& gameName);
		~Application();

		void Run();

	private:
		Game game_;
		sf::RenderWindow window_;
	};
}
