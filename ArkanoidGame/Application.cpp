#include "Application.h"
#include <cstdlib>

namespace ArkanoidGame
{
	Application::Application(const std::string& gameName) :
		window_(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), gameName)
	{
		unsigned int seed = (unsigned int)time(nullptr);
		srand(seed);

		game_.init();
	}

	Application::~Application()
	{
		game_.shutdown();
	}

	void Application::Run()
	{
		sf::Clock gameClock;

		while (window_.isOpen())
		{
			float startTime = gameClock.getElapsedTime().asSeconds();

			game_.handleWindowEvents(window_);

			if (!window_.isOpen())
				break;

			if (game_.update(TIME_PER_FRAME))
			{
				window_.clear();
				game_.draw(window_);
				window_.display();
			}
			else
			{
				window_.close();
			}

			float endTime = gameClock.getElapsedTime().asSeconds();
			float deltaTime = endTime - startTime;
			if (deltaTime < TIME_PER_FRAME)
			{
				sf::sleep(sf::seconds(TIME_PER_FRAME - deltaTime));
			}
		}
	}
}
