#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "GameSettings.h"

namespace ArkanoidGame
{
	enum class GameStateType
	{
		None = 0,
		MainMenu,
		Playing,
		GameOver,
		ExitDialog,
		Records,
		Win,
	};

	enum class GameStateChangeType
	{
		None,
		Push,
		Pop,
		Switch
	};

	class Game; // forward declaration

	// Abstract base for all game state data (RAII: destructor cleans up automatically)
	class IGameStateData
	{
	public:
		virtual ~IGameStateData() = default;
		virtual void Init(Game& game) = 0;
		virtual void Shutdown(Game& game) = 0;
		virtual void HandleWindowEvent(Game& game, sf::Event& event) = 0;
		virtual void Update(Game& game, float timeDelta) = 0;
		virtual void Draw(Game& game, sf::RenderWindow& window) = 0;
	};

	// GameState owns its data via unique_ptr (RAII)
	class GameState
	{
	public:
		GameState(GameStateType type, std::unique_ptr<IGameStateData> data, bool exclusivelyVisible);
		GameState(GameState&& other) noexcept = default;
		GameState& operator=(GameState&& other) noexcept = default;

		GameStateType getType() const;
		bool getExclusivelyVisible() const;

		void init(Game& game);
		void shutdown(Game& game);
		void handleEvent(Game& game, sf::Event& event);
		void update(Game& game, float timeDelta);
		void draw(Game& game, sf::RenderWindow& window);

	private:
		GameStateType type_ = GameStateType::None;
		std::unique_ptr<IGameStateData> data_;
		bool exclusivelyVisible_ = false;
	};

	// Main game class with encapsulated state
	class Game
	{
	public:
		void init();
		void handleWindowEvents(sf::RenderWindow& window);
		bool update(float timeDelta);
		void draw(sf::RenderWindow& window);
		void shutdown();

		void pushState(GameStateType type, bool exclusivelyVisible);
		void popState();
		void switchState(GameStateType newState);

		std::unordered_map<std::string, int>& getRecordsTable();
		const std::unordered_map<std::string, int>& getRecordsTable() const;

	private:
		std::vector<GameState> gameStateStack_;
		GameStateChangeType pendingChangeType_ = GameStateChangeType::None;
		GameStateType pendingStateType_ = GameStateType::None;
		bool pendingExclusivelyVisible_ = false;

		std::unordered_map<std::string, int> recordsTable_;

		std::unique_ptr<IGameStateData> createStateData(GameStateType type);
	};
}
