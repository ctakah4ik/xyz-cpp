#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "Game.h"
#include "Platform.h"
#include "Ball.h"
#include "Block.h"
#include "Bonus.h"
#include "GameMemento.h"

namespace ArkanoidGame
{
	// Active effect timer
	struct ActiveEffect
	{
		BonusType type;
		float remainingTime;
	};

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
		std::vector<std::unique_ptr<Block>> blocks_;
		int score_ = 0;
		int lives_ = INITIAL_LIVES;

		// Bonuses
		std::vector<std::unique_ptr<Bonus>> fallingBonuses_;
		std::vector<ActiveEffect> activeEffects_;

		// Memento pattern: caretaker holds the memento
		GameMemento memento_;

		sf::Font font_;
		sf::Text scoreText_;
		sf::Text livesText_;
		sf::Text hintText_;
		sf::RectangleShape background_;

		sf::SoundBuffer deathSoundBuffer_;
		sf::Sound deathSound_;

		void initBlocks();
		bool allBlocksDestroyed() const;

		// Memento pattern: create and restore snapshots
		GameMemento createMemento() const;
		void restoreFromMemento(const GameMemento& memento);

		// Bonus management
		void spawnBonus(float x, float y);
		void updateBonuses(float timeDelta);
		void updateEffects(float timeDelta);
		void applyEffect(BonusType type, float duration);
		void removeEffect(BonusType type);
		bool hasEffect(BonusType type) const;

		// Reset ball and platform positions (on life loss)
		void resetBallAndPlatform();
	};
}
