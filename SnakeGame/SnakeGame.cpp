#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include "Constants.h"
#include "Game.h"

int main()
{
	using namespace SnakeGame;

	srand((unsigned int)time(nullptr));

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_TITLE);

	Game game;
	InitGame(game);

	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	while (window.isOpen())
	{
		sf::sleep(sf::milliseconds(16));

		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}

			// Exit via menu item or Escape
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
				break;
			}
			
			if (game.state == GameState::MainMenu &&
				event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Enter &&
				game.menuSelectedItem == 4)
			{
				window.close();
				break;
			}

			HandleGameEvent(game, event);
		}

		UpdateGame(game, deltaTime);

		window.clear();
		DrawGame(game, window);
		window.display();
	}

	return 0;
}
