#include "GameStateGameOver.h"
#include "Text.h"
#include <cassert>
#include <sstream>
#include <map>

namespace ArkanoidGame
{
	void GameStateGameOver::Init(Game& game)
	{
		assert(font_.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		timeSinceGameOver_ = 0.f;

		sf::Color backgroundColor = sf::Color::Black;
		backgroundColor.a = 200;
		background_.setFillColor(backgroundColor);

		gameOverText_.setFont(font_);
		gameOverText_.setCharacterSize(48);
		gameOverText_.setStyle(sf::Text::Bold);
		gameOverText_.setFillColor(sf::Color::Red);
		gameOverText_.setString("GAME OVER");

		recordsTableTexts_.reserve(MAX_RECORDS_TABLE_SIZE);

		std::multimap<int, std::string> sortedRecords;
		int playerScore = game.getRecordsTable()[PLAYER_NAME];
		for (const auto& item : game.getRecordsTable())
		{
			sortedRecords.insert(std::make_pair(item.second, item.first));
		}

		bool isPlayerInTable = false;
		auto it = sortedRecords.rbegin();
		for (int i = 0; i < MAX_RECORDS_TABLE_SIZE && it != sortedRecords.rend(); ++i, ++it)
		{
			recordsTableTexts_.emplace_back();
			sf::Text& text = recordsTableTexts_.back();

			std::stringstream sstream;
			sstream << i + 1 << ". " << it->second << ": " << it->first;
			text.setString(sstream.str());
			text.setFont(font_);
			text.setCharacterSize(24);
			if (it->second == PLAYER_NAME)
			{
				text.setFillColor(sf::Color::Green);
				isPlayerInTable = true;
			}
			else
			{
				text.setFillColor(sf::Color::White);
			}
		}

		if (!isPlayerInTable && !recordsTableTexts_.empty())
		{
			sf::Text& text = recordsTableTexts_.back();
			std::stringstream sstream;
			sstream << MAX_RECORDS_TABLE_SIZE << ". " << PLAYER_NAME << ": " << playerScore;
			text.setString(sstream.str());
			text.setFillColor(sf::Color::Green);
		}

		hintText_.setFont(font_);
		hintText_.setCharacterSize(24);
		hintText_.setFillColor(sf::Color::White);
		hintText_.setString("Press Space to restart\nEsc to exit to main menu");
	}

	void GameStateGameOver::Shutdown(Game& game)
	{
	}

	void GameStateGameOver::HandleWindowEvent(Game& game, sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Space)
			{
				game.switchState(GameStateType::Playing);
			}
			else if (event.key.code == sf::Keyboard::Escape)
			{
				game.switchState(GameStateType::MainMenu);
			}
		}
	}

	void GameStateGameOver::Update(Game& game, float timeDelta)
	{
		timeSinceGameOver_ += timeDelta;

		sf::Color color = (int)timeSinceGameOver_ % 2 ? sf::Color::Red : sf::Color::Yellow;
		gameOverText_.setFillColor(color);
	}

	void GameStateGameOver::Draw(Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = window.getView().getSize();

		background_.setOrigin(0.f, 0.f);
		background_.setSize(viewSize);
		window.draw(background_);

		gameOverText_.setOrigin(GetTextOrigin(gameOverText_, { 0.5f, 1.f }));
		gameOverText_.setPosition(viewSize.x / 2.f, viewSize.y / 2 - 50.f);
		window.draw(gameOverText_);

		std::vector<sf::Text*> textsList;
		textsList.reserve(recordsTableTexts_.size());
		for (auto& text : recordsTableTexts_)
		{
			textsList.push_back(&text);
		}

		sf::Vector2f tablePosition = { viewSize.x / 2, viewSize.y / 2.f };
		DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Min, tablePosition, { 0.5f, 0.f });

		hintText_.setOrigin(GetTextOrigin(hintText_, { 0.5f, 1.f }));
		hintText_.setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
		window.draw(hintText_);
	}
}
