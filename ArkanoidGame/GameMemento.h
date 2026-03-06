#pragma once
#include <vector>
#include "Block.h"

namespace ArkanoidGame
{
	// Memento pattern: stores a snapshot of the game state
	// Originator: GameStatePlaying
	// Caretaker: GameStatePlaying (holds the memento)
	class GameMemento
	{
	public:
		GameMemento() = default;
		GameMemento(int score, const std::vector<BlockState>& blockStates)
			: score_(score), blockStates_(blockStates) {}

		int getScore() const { return score_; }
		const std::vector<BlockState>& getBlockStates() const { return blockStates_; }

	private:
		int score_ = 0;
		std::vector<BlockState> blockStates_;
	};
}
