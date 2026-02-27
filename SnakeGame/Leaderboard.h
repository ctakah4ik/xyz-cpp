#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Constants.h"

namespace SnakeGame
{
	struct LeaderboardEntry
	{
		std::string name;
		int score = 0;
	};

	struct Leaderboard
	{
		std::vector<LeaderboardEntry> entries;
	};

	void InitLeaderboard(Leaderboard& lb);
	void AddLeaderboardEntry(Leaderboard& lb, const std::string& name, int score);
	void SaveLeaderboard(const Leaderboard& lb);
	void LoadLeaderboard(Leaderboard& lb);
}
