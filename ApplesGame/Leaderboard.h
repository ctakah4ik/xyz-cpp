#pragma once
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"

namespace ApplesGame
{
	struct LeaderboardEntry
	{
		std::string name;
		int score = 0;
		bool isPlayer = false;
	};

	// Fill leaderboard with predefined NPC data (no player yet)
	void InitLeaderboard(std::vector<LeaderboardEntry>& leaderboard);

	// Add (or update) the player record, then sort descending by score
	void FinalizeLeaderboard(std::vector<LeaderboardEntry>& leaderboard, int playerScore);

	// Rebuild text from current leaderboard contents
	void BuildLeaderboardText(const std::vector<LeaderboardEntry>& leaderboard, sf::Text& text);
}
