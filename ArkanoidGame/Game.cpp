#include "Game.h"
#include <cassert>
#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "GameStateRecords.h"
#include "GameStateWin.h"

namespace ArkanoidGame
{
	// --- GameState ---

	GameState::GameState(GameStateType type, std::unique_ptr<IGameStateData> data, bool exclusivelyVisible)
		: type_(type), data_(std::move(data)), exclusivelyVisible_(exclusivelyVisible)
	{
	}

	GameStateType GameState::getType() const { return type_; }
	bool GameState::getExclusivelyVisible() const { return exclusivelyVisible_; }

	void GameState::init(Game& game)
	{
		if (data_) data_->Init(game);
	}

	void GameState::shutdown(Game& game)
	{
		if (data_) data_->Shutdown(game);
	}

	void GameState::handleEvent(Game& game, sf::Event& event)
	{
		if (data_) data_->HandleWindowEvent(game, event);
	}

	void GameState::update(Game& game, float timeDelta)
	{
		if (data_) data_->Update(game, timeDelta);
	}

	void GameState::draw(Game& game, sf::RenderWindow& window)
	{
		if (data_) data_->Draw(game, window);
	}

	// --- Game ---

	void Game::init()
	{
		recordsTable_ =
		{
			{"John", 50},
			{"Jane", 40},
			{"Alice", 30},
			{"Bob", 20},
			{"Clementine", 10},
		};

		pendingChangeType_ = GameStateChangeType::None;
		pendingStateType_ = GameStateType::None;
		pendingExclusivelyVisible_ = false;
		switchState(GameStateType::MainMenu);
	}

	void Game::handleWindowEvents(sf::RenderWindow& window)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (gameStateStack_.size() > 0)
			{
				gameStateStack_.back().handleEvent(*this, event);
			}
		}
	}

	bool Game::update(float timeDelta)
	{
		if (pendingChangeType_ == GameStateChangeType::Switch)
		{
			while (gameStateStack_.size() > 0)
			{
				gameStateStack_.back().shutdown(*this);
				gameStateStack_.pop_back();
			}
		}
		else if (pendingChangeType_ == GameStateChangeType::Pop)
		{
			if (gameStateStack_.size() > 0)
			{
				gameStateStack_.back().shutdown(*this);
				gameStateStack_.pop_back();
			}
		}

		if (pendingStateType_ != GameStateType::None)
		{
			auto data = createStateData(pendingStateType_);
			gameStateStack_.emplace_back(pendingStateType_, std::move(data), pendingExclusivelyVisible_);
			gameStateStack_.back().init(*this);
		}

		pendingChangeType_ = GameStateChangeType::None;
		pendingStateType_ = GameStateType::None;
		pendingExclusivelyVisible_ = false;

		if (gameStateStack_.size() > 0)
		{
			gameStateStack_.back().update(*this, timeDelta);
			return true;
		}

		return false;
	}

	void Game::draw(sf::RenderWindow& window)
	{
		if (gameStateStack_.size() > 0)
		{
			std::vector<GameState*> visibleStates;
			for (auto it = gameStateStack_.rbegin(); it != gameStateStack_.rend(); ++it)
			{
				visibleStates.push_back(&(*it));
				if (it->getExclusivelyVisible())
					break;
			}

			for (auto it = visibleStates.rbegin(); it != visibleStates.rend(); ++it)
			{
				(*it)->draw(*this, window);
			}
		}
	}

	void Game::shutdown()
	{
		while (gameStateStack_.size() > 0)
		{
			gameStateStack_.back().shutdown(*this);
			gameStateStack_.pop_back();
		}

		pendingChangeType_ = GameStateChangeType::None;
		pendingStateType_ = GameStateType::None;
		pendingExclusivelyVisible_ = false;
	}

	void Game::pushState(GameStateType type, bool exclusivelyVisible)
	{
		pendingStateType_ = type;
		pendingExclusivelyVisible_ = exclusivelyVisible;
		pendingChangeType_ = GameStateChangeType::Push;
	}

	void Game::popState()
	{
		pendingStateType_ = GameStateType::None;
		pendingExclusivelyVisible_ = false;
		pendingChangeType_ = GameStateChangeType::Pop;
	}

	void Game::switchState(GameStateType newState)
	{
		pendingStateType_ = newState;
		pendingExclusivelyVisible_ = false;
		pendingChangeType_ = GameStateChangeType::Switch;
	}

	std::unordered_map<std::string, int>& Game::getRecordsTable() { return recordsTable_; }
	const std::unordered_map<std::string, int>& Game::getRecordsTable() const { return recordsTable_; }

	std::unique_ptr<IGameStateData> Game::createStateData(GameStateType type)
	{
		switch (type)
		{
		case GameStateType::MainMenu:
			return std::make_unique<GameStateMainMenu>();
		case GameStateType::Playing:
			return std::make_unique<GameStatePlaying>();
		case GameStateType::GameOver:
			return std::make_unique<GameStateGameOver>();
		case GameStateType::ExitDialog:
			return std::make_unique<GameStatePauseMenu>();
		case GameStateType::Records:
			return std::make_unique<GameStateRecords>();
		case GameStateType::Win:
			return std::make_unique<GameStateWin>();
		default:
			assert(false && "Unknown game state type");
			return nullptr;
		}
	}
}
