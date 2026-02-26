#include "Leaderboard.h"
#include <algorithm>
#include <fstream>
#include <sstream>

namespace SnakeGame
{
	void InitLeaderboard(Leaderboard& lb)
	{
		lb.entries.clear();
	}

	void AddLeaderboardEntry(Leaderboard& lb, const std::string& name, int score)
	{
		LeaderboardEntry entry;
		entry.name = name;
		entry.score = score;
		lb.entries.push_back(entry);

		// Sort descending by score
		std::sort(lb.entries.begin(), lb.entries.end(),
			[](const LeaderboardEntry& a, const LeaderboardEntry& b)
			{
				return a.score > b.score;
			});

		// Keep only top entries
		if ((int)lb.entries.size() > MAX_LEADERBOARD_ENTRIES)
			lb.entries.resize(MAX_LEADERBOARD_ENTRIES);

		SaveLeaderboard(lb);
	}

	void SaveLeaderboard(const Leaderboard& lb)
	{
		std::ofstream file(LEADERBOARD_FILE);
		for (const LeaderboardEntry& entry : lb.entries)
			file << entry.score << "|" << entry.name << "\n";
	}

	void LoadLeaderboard(Leaderboard& lb)
	{
		std::ifstream file(LEADERBOARD_FILE);
		if (!file.is_open())
			return; // First run — no file yet, start with empty leaderboard

		std::string line;
		while (std::getline(file, line))
		{
			size_t sep = line.find('|');
			if (sep == std::string::npos)
				continue;

			std::istringstream scoreStream(line.substr(0, sep));
			int score;
			std::string name = line.substr(sep + 1);

			if ((scoreStream >> score) && !name.empty())
			{
				LeaderboardEntry entry;
				entry.score = score;
				entry.name = name;
				lb.entries.push_back(entry);
			}
		}

		std::sort(lb.entries.begin(), lb.entries.end(),
			[](const LeaderboardEntry& a, const LeaderboardEntry& b)
			{
				return a.score > b.score;
			});

		if ((int)lb.entries.size() > MAX_LEADERBOARD_ENTRIES)
			lb.entries.resize(MAX_LEADERBOARD_ENTRIES);
	}

}
