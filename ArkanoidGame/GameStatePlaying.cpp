#include "GameStatePlaying.h"
#include "Text.h"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace ArkanoidGame
{
	void GameStatePlaying::Init(Game& game)
	{
		assert(font_.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));
		assert(deathSoundBuffer_.loadFromFile(SOUNDS_PATH + "Death.wav"));
		deathSound_.setBuffer(deathSoundBuffer_);

		background_.setSize(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
		background_.setPosition(0.f, 0.f);
		background_.setFillColor(sf::Color(30, 30, 50));

		platform_.init();
		ball_.init();
		score_ = 0;
		lives_ = INITIAL_LIVES;

		fallingBonuses_.clear();
		activeEffects_.clear();

		initBlocks();

		// Memento: save initial state
		memento_ = createMemento();

		scoreText_.setFont(font_);
		scoreText_.setCharacterSize(24);
		scoreText_.setFillColor(sf::Color::Yellow);

		livesText_.setFont(font_);
		livesText_.setCharacterSize(24);
		livesText_.setFillColor(sf::Color::Cyan);

		hintText_.setFont(font_);
		hintText_.setCharacterSize(24);
		hintText_.setFillColor(sf::Color::White);
		hintText_.setString("Arrow keys to move, ESC to pause");
		hintText_.setOrigin(GetTextOrigin(hintText_, { 1.f, 0.f }));
	}

	void GameStatePlaying::initBlocks()
	{
		static const sf::Color rowColors[] = {
			sf::Color(220, 50,  50),
			sf::Color(220, 150, 50),
			sf::Color(50,  200, 50),
			sf::Color(50,  150, 220),
		};

		const float totalRowWidth = BLOCK_COLUMNS * BLOCK_WIDTH + (BLOCK_COLUMNS - 1) * BLOCK_PADDING;
		const float startX = (SCREEN_WIDTH - totalRowWidth) / 2.f;
		const float startY = BLOCK_TOP_OFFSET;

		const int totalBlocks = BLOCK_ROWS * BLOCK_COLUMNS;

		std::srand(static_cast<unsigned>(std::time(nullptr)));

		// 0 = normal, 1 = unbreakable, 2 = durable
		std::vector<int> specialTypes(totalBlocks, 0);
		int unbreakableCount = 0;
		int durableCount = 0;

		while (unbreakableCount < MAX_UNBREAKABLE_BLOCKS)
		{
			int idx = std::rand() % totalBlocks;
			if (specialTypes[idx] == 0)
			{
				specialTypes[idx] = 1;
				++unbreakableCount;
			}
		}

		while (durableCount < MAX_DURABLE_BLOCKS)
		{
			int idx = std::rand() % totalBlocks;
			if (specialTypes[idx] == 0)
			{
				specialTypes[idx] = 2;
				++durableCount;
			}
		}

		blocks_.clear();
		int index = 0;
		for (int row = 0; row < BLOCK_ROWS; ++row)
		{
			sf::Color color = rowColors[row % 4];
			for (int col = 0; col < BLOCK_COLUMNS; ++col)
			{
				float x = startX + col * (BLOCK_WIDTH + BLOCK_PADDING);
				float y = startY + row * (BLOCK_HEIGHT + BLOCK_PADDING);

				std::unique_ptr<Block> block;
				if (specialTypes[index] == 1)
					block = std::make_unique<UnbreakableBlock>();
				else if (specialTypes[index] == 2)
					block = std::make_unique<DurableBlock>();
				else
					block = std::make_unique<Block>();

				block->init(x, y, color);
				blocks_.push_back(std::move(block));
				++index;
			}
		}
	}

	bool GameStatePlaying::allBlocksDestroyed() const
	{
		for (const auto& block : blocks_)
			if (block->isActive() && !block->isUnbreakable())
				return false;
		return true;
	}

	void GameStatePlaying::Shutdown(Game& game)
	{
	}

	void GameStatePlaying::HandleWindowEvent(Game& game, sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				game.pushState(GameStateType::ExitDialog, false);
			}
		}
	}

	void GameStatePlaying::Update(Game& game, float timeDelta)
	{
		platform_.handleInput(timeDelta);

		bool bouncedPlatform = ball_.update(timeDelta, platform_);
		if (bouncedPlatform)
			score_ += PLATFORM_BOUNCE_SCORE;

		// Score from block collisions (Strategy pattern: each block returns its own score)
		std::vector<sf::Vector2f> destroyedPositions;
		int blockScore = ball_.checkBlockCollisions(blocks_, destroyedPositions);
		if (blockScore > 0)
		{
			score_ += blockScore;

			// Spawn bonuses from destroyed blocks (10% chance per block)
			for (const auto& pos : destroyedPositions)
				spawnBonus(pos.x, pos.y);
		}

		// Update falling bonuses
		updateBonuses(timeDelta);

		// Update active effects timers
		updateEffects(timeDelta);

		if (allBlocksDestroyed())
		{
			game.getRecordsTable()[PLAYER_NAME] = std::max(game.getRecordsTable()[PLAYER_NAME], score_);
			game.switchState(GameStateType::Win);
			return;
		}

		if (ball_.isOutOfBounds())
		{
			// Remove all active effects
			for (auto& effect : activeEffects_)
				removeEffect(effect.type);
			activeEffects_.clear();
			fallingBonuses_.clear();

			--lives_;
			if (lives_ <= 0)
			{
				deathSound_.play();
				game.getRecordsTable()[PLAYER_NAME] = std::max(game.getRecordsTable()[PLAYER_NAME], score_);
				game.pushState(GameStateType::GameOver, false);
			}
			else
			{
				deathSound_.play();
				// Memento: save current state before resetting
				memento_ = createMemento();
				resetBallAndPlatform();
			}
		}

		scoreText_.setString("Score: " + std::to_string(score_));
		livesText_.setString("Lives: " + std::to_string(lives_));
	}

	void GameStatePlaying::Draw(Game& game, sf::RenderWindow& window)
	{
		window.draw(background_);

		for (const auto& block : blocks_)
			block->draw(window);

		// Draw falling bonuses
		for (const auto& bonus : fallingBonuses_)
			bonus->draw(window);

		platform_.draw(window);
		ball_.draw(window);

		scoreText_.setOrigin(GetTextOrigin(scoreText_, { 0.f, 0.f }));
		scoreText_.setPosition(10.f, 10.f);
		window.draw(scoreText_);

		livesText_.setOrigin(GetTextOrigin(livesText_, { 0.f, 0.f }));
		livesText_.setPosition(180.f, 10.f);
		window.draw(livesText_);

		sf::Vector2f viewSize = window.getView().getSize();
		hintText_.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(hintText_);
	}

	// Memento pattern: create snapshot
	GameMemento GameStatePlaying::createMemento() const
	{
		std::vector<BlockState> blockStates;
		blockStates.reserve(blocks_.size());
		for (const auto& block : blocks_)
			blockStates.push_back(block->saveState());

		return GameMemento(score_, blockStates);
	}

	// Memento pattern: restore from snapshot
	void GameStatePlaying::restoreFromMemento(const GameMemento& memento)
	{
		score_ = memento.getScore();
		const auto& blockStates = memento.getBlockStates();
		for (size_t i = 0; i < blocks_.size() && i < blockStates.size(); ++i)
			blocks_[i]->restoreState(blockStates[i]);
	}

	void GameStatePlaying::resetBallAndPlatform()
	{
		ball_.init();
		platform_.init();
	}

	void GameStatePlaying::spawnBonus(float x, float y)
	{
		// 10% chance to spawn a bonus
		if ((std::rand() % 100) >= static_cast<int>(BONUS_DROP_CHANCE * 100))
			return;

		float cx = x;
		float cy = y;

		// Random bonus type
		int type = std::rand() % 3;
		std::unique_ptr<Bonus> bonus;
		switch (type)
		{
		case 0:
			bonus = std::make_unique<FireballBonus>(cx, cy);
			break;
		case 1:
			bonus = std::make_unique<FragileBlocksBonus>(cx, cy);
			break;
		case 2:
			bonus = std::make_unique<WidePlatformBonus>(cx, cy);
			break;
		}

		fallingBonuses_.push_back(std::move(bonus));
	}

	void GameStatePlaying::updateBonuses(float timeDelta)
	{
		sf::FloatRect platformBounds = platform_.getBounds();

		for (auto& bonus : fallingBonuses_)
		{
			if (!bonus->isActive())
				continue;

			bonus->update(timeDelta);

			// Check if bonus is collected by platform
			sf::FloatRect bonusBounds = bonus->getBounds();
			if (bonusBounds.intersects(platformBounds))
			{
				bonus->deactivate();
				applyEffect(bonus->getType(), bonus->getDuration());
			}
			// Remove if out of screen
			else if (bonus->isOutOfBounds())
			{
				bonus->deactivate();
			}
		}

		// Remove inactive bonuses
		fallingBonuses_.erase(
			std::remove_if(fallingBonuses_.begin(), fallingBonuses_.end(),
				[](const std::unique_ptr<Bonus>& b) { return !b->isActive(); }),
			fallingBonuses_.end());
	}

	void GameStatePlaying::updateEffects(float timeDelta)
	{
		for (auto it = activeEffects_.begin(); it != activeEffects_.end();)
		{
			it->remainingTime -= timeDelta;
			if (it->remainingTime <= 0.f)
			{
				removeEffect(it->type);
				it = activeEffects_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void GameStatePlaying::applyEffect(BonusType type, float duration)
	{
		// If effect already active, refresh timer
		for (auto& effect : activeEffects_)
		{
			if (effect.type == type)
			{
				effect.remainingTime = duration;
				return;
			}
		}

		// Apply new effect
		activeEffects_.push_back({ type, duration });

		switch (type)
		{
		case BonusType::Fireball:
			// Decorator: decorate ball with fireball behavior
			ball_.setFireball(true);
			break;
		case BonusType::FragileBlocks:
			// State: switch all blocks to fragile state
			for (auto& block : blocks_)
				if (block->isActive())
					block->setFragile(true);
			break;
		case BonusType::WidePlatform:
			// Strategy: change platform size strategy
			platform_.setWidthMultiplier(WIDE_PLATFORM_MULTIPLIER);
			break;
		}
	}

	void GameStatePlaying::removeEffect(BonusType type)
	{
		switch (type)
		{
		case BonusType::Fireball:
			// Remove decorator
			ball_.setFireball(false);
			break;
		case BonusType::FragileBlocks:
			// Restore blocks to normal state
			for (auto& block : blocks_)
				if (block->isActive())
					block->setFragile(false);
			break;
		case BonusType::WidePlatform:
			// Restore platform size strategy
			platform_.resetWidth();
			break;
		}
	}

	bool GameStatePlaying::hasEffect(BonusType type) const
	{
		for (const auto& effect : activeEffects_)
			if (effect.type == type)
				return true;
		return false;
	}
}
