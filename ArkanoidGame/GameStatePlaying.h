#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include "Platform.h"
#include "Ball.h"

namespace ArkanoidGame
{
	class GameStatePlaying : public IGameStateData
	{
	public:
		void Init(Game& game) override;
		void Shutdown(Game& game) override;
		void HandleWindowEvent(Game& game, sf::Event& event) override;
		void Update(Game& game, float timeDelta) override;
		void Draw(Game& game, sf::RenderWindow& window) override;

	private:
		Platform platform_;
		Ball ball_;
		int score_ = 0;

		sf::Font font_;
		sf::Text scoreText_;
		sf::Text hintText_;
		sf::RectangleShape background_;

		sf::SoundBuffer deathSoundBuffer_;
		sf::Sound deathSound_;
	};
}
