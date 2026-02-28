#include "GameStateRecords.h"
#include "Text.h"
#include <cassert>
#include <sstream>
#include <map>

namespace ArkanoidGame
{
	void GameStateRecords::Init(Game& game)
	{
		assert(font_.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		titleText_.setString("RECORDS");
		titleText_.setFont(font_);
		titleText_.setFillColor(sf::Color::Red);
		titleText_.setCharacterSize(48);

		tableTexts_.reserve(MAX_RECORDS_TABLE_SIZE);

		std::map<int, std::string> sortedRecords;
		for (const auto& item : game.getRecordsTable())
		{
			sortedRecords[item.second] = item.first;
		}

		auto it = sortedRecords.rbegin();
		for (int i = 0; i < MAX_RECORDS_TABLE_SIZE && it != sortedRecords.rend(); ++i, ++it)
		{
			tableTexts_.emplace_back();
			sf::Text& text = tableTexts_.back();

			std::stringstream sstream;
			sstream << i + 1 << ". " << it->second << ": " << it->first;
			text.setString(sstream.str());
			text.setFont(font_);
			text.setFillColor(sf::Color::White);
			text.setCharacterSize(24);
		}

		hintText_.setString("Press ESC to return back to main menu");
		hintText_.setFont(font_);
		hintText_.setFillColor(sf::Color::White);
		hintText_.setCharacterSize(24);
	}

	void GameStateRecords::Shutdown(Game& game)
	{
	}

	void GameStateRecords::HandleWindowEvent(Game& game, sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				game.popState();
			}
		}
	}

	void GameStateRecords::Update(Game& game, float timeDelta)
	{
	}

	void GameStateRecords::Draw(Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = window.getView().getSize();

		titleText_.setOrigin(GetTextOrigin(titleText_, { 0.5f, 0.f }));
		titleText_.setPosition(viewSize.x / 2.f, 50.f);
		window.draw(titleText_);

		std::vector<sf::Text*> textsList;
		textsList.reserve(tableTexts_.size());
		for (auto& text : tableTexts_)
		{
			textsList.push_back(&text);
		}

		sf::Vector2f tablePosition = { titleText_.getGlobalBounds().left, viewSize.y / 2.f };
		DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Min, tablePosition, { 0.f, 0.f });

		hintText_.setOrigin(GetTextOrigin(hintText_, { 0.5f, 1.f }));
		hintText_.setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
		window.draw(hintText_);
	}
}
