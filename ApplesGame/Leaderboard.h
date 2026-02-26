#pragma once
#include <string>
#include <unordered_map>
#include "SFML/Graphics.hpp"

namespace ApplesGame
{
	struct LeaderboardEntry
	{
		int score = 0;
		bool isPlayer = false;
	};

	using Leaderboard = std::unordered_map<std::string, LeaderboardEntry>;

	// Fill leaderboard with predefined NPC data (no player yet)
	void InitLeaderboard(Leaderboard& leaderboard);

	// Add (or update) the player record, then sort descending by score
	void FinalizeLeaderboard(Leaderboard& leaderboard, int playerScore);

	// Rebuild text from current leaderboard contents
	void BuildLeaderboardText(const Leaderboard& leaderboard, sf::Text& text);
}
