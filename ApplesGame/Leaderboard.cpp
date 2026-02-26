#include "Leaderboard.h"
#include "Utils.h"
#include <string>

namespace ApplesGame
{
	static const LeaderboardEntry NPC_RECORDS[] = {
		{"Alice",  120, false},
		{"Bob",     95, false},
		{"Carol",   72, false},
		{"Dave",    58, false},
		{"Eve",     43, false},
		{"Frank",   31, false},
		{"Grace",   18, false},
	};
	static const int NPC_COUNT = 7;

	static std::string PadRight(const std::string& s, int width, char fill = ' ')
	{
		std::string r = s;
		while ((int)r.size() < width) r += fill;
		return r;
	}

	static std::string PadLeft(const std::string& s, int width, char fill = ' ')
	{
		std::string r = s;
		while ((int)r.size() < width) r = fill + r;
		return r;
	}

	void InitLeaderboard(std::vector<LeaderboardEntry>& leaderboard)
	{
		leaderboard.clear();
		for (int i = 0; i < NPC_COUNT; ++i)
			leaderboard.push_back(NPC_RECORDS[i]);
	}

	void FinalizeLeaderboard(std::vector<LeaderboardEntry>& leaderboard, int playerScore)
	{
		LeaderboardEntry player;
		player.name     = "Player";
		player.score    = playerScore;
		player.isPlayer = true;
		leaderboard.push_back(player);

		Sort(leaderboard, [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
			return a.score > b.score;
		});
	}

	void BuildLeaderboardText(const std::vector<LeaderboardEntry>& leaderboard, sf::Text& text)
	{
		const int NAME_WIDTH  = 12;
		const int SCORE_WIDTH = 4;

		std::string content = "===== LEADERBOARD =====\n";
		for (int i = 0; i < (int)leaderboard.size(); ++i)
		{
			const LeaderboardEntry& e = leaderboard[i];
			std::string name = e.name;
			if ((int)name.size() > NAME_WIDTH) name = name.substr(0, NAME_WIDTH);

			std::string rank      = PadRight(std::to_string(i + 1) + ".", 3);
			std::string nameField = PadRight(name, NAME_WIDTH, '.');
			std::string scoreStr  = PadLeft(std::to_string(e.score), SCORE_WIDTH);

			std::string line = rank + " " + nameField + " " + scoreStr;
			if (e.isPlayer) line = "> " + line + " <";
			else            line = "  " + line;

			content += line + "\n";
		}
		content += "=======================";
		text.setString(content);
	}
}
