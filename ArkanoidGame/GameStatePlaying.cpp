#include "GameStatePlaying.h"
#include "Text.h"
#include <cassert>
#include <cstdlib>
#include <ctime>

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

		initBlocks();

		scoreText_.setFont(font_);
		scoreText_.setCharacterSize(24);
		scoreText_.setFillColor(sf::Color::Yellow);

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
			score_++;

		int destroyed = ball_.checkBlockCollisions(blocks_);
		score_ += destroyed * 10;

		if (allBlocksDestroyed())
		{
			game.getRecordsTable()[PLAYER_NAME] = std::max(game.getRecordsTable()[PLAYER_NAME], score_);
			game.switchState(GameStateType::Win);
			return;
		}

		if (ball_.isOutOfBounds())
		{
			deathSound_.play();
			game.getRecordsTable()[PLAYER_NAME] = std::max(game.getRecordsTable()[PLAYER_NAME], score_);
			game.pushState(GameStateType::GameOver, false);
		}

		scoreText_.setString("Score: " + std::to_string(score_));
	}

	void GameStatePlaying::Draw(Game& game, sf::RenderWindow& window)
	{
		window.draw(background_);

		for (const auto& block : blocks_)
			block->draw(window);

		platform_.draw(window);
		ball_.draw(window);

		scoreText_.setOrigin(GetTextOrigin(scoreText_, { 0.f, 0.f }));
		scoreText_.setPosition(10.f, 10.f);
		window.draw(scoreText_);

		sf::Vector2f viewSize = window.getView().getSize();
		hintText_.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(hintText_);
	}
}
