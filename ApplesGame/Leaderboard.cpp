#include "Leaderboard.h"
#include "Utils.h"
#include <string>
#include <vector>

namespace ApplesGame
{
	static const LeaderboardEntry NPC_RECORDS[] = {
		{120, false},
		{ 95, false},
		{ 72, false},
		{ 58, false},
		{ 43, false},
		{ 31, false},
		{ 18, false},
	};
	static const std::string NPC_NAMES[] = {
		"Alice", "Bob", "Carol", "Dave", "Eve", "Frank", "Grace"
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

	void InitLeaderboard(Leaderboard& leaderboard)
	{
		leaderboard.clear();
		for (int i = 0; i < NPC_COUNT; ++i)
			leaderboard[NPC_NAMES[i]] = NPC_RECORDS[i];
	}

	void FinalizeLeaderboard(Leaderboard& leaderboard, int playerScore)
	{
		leaderboard["Player"] = {playerScore, true};
	}

	void BuildLeaderboardText(const Leaderboard& leaderboard, sf::Text& text)
	{
		using Row = std::pair<std::string, LeaderboardEntry>;

		std::vector<Row> rows(leaderboard.begin(), leaderboard.end());
		Sort(rows, [](const Row& a, const Row& b) {
			return a.second.score > b.second.score;
		});

		const int NAME_WIDTH  = 12;
		const int SCORE_WIDTH = 4;

		std::string content = "===== LEADERBOARD =====\n";
		for (int i = 0; i < (int)rows.size(); ++i)
		{
			std::string name          = rows[i].first;
			const LeaderboardEntry& e = rows[i].second;
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
