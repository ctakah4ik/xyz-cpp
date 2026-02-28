#pragma once
#include "SFML/Graphics.hpp"
#include "Text.h"
#include <vector>

namespace ArkanoidGame
{
	class MenuItem
	{
	public:
		MenuItem();

		void setText(const std::string& str, const sf::Font& font, unsigned int charSize);
		void setHintText(const std::string& str, const sf::Font& font, unsigned int charSize, sf::Color color);
		void setChildrenLayout(Orientation orientation, Alignment alignment, float spacing);
		void setColors(sf::Color selected, sf::Color deselected);
		void setEnabled(bool enabled);
		void addChild(MenuItem* child);

		sf::Text& getText();
		sf::Text& getHintText();
		Orientation getChildrenOrientation() const;
		Alignment getChildrenAlignment() const;
		float getChildrenSpacing() const;
		bool isEnabled() const;
		const std::vector<MenuItem*>& getChildren() const;
		MenuItem* getParent() const;

	private:
		friend class Menu;

		sf::Text text_;
		sf::Text hintText_;
		Orientation childrenOrientation_ = Orientation::Vertical;
		Alignment childrenAlignment_ = Alignment::Min;
		float childrenSpacing_ = 0.f;

		sf::Color selectedColor_ = sf::Color::Yellow;
		sf::Color deselectedColor_ = sf::Color::White;

		bool isEnabled_ = true;
		std::vector<MenuItem*> children_;
		MenuItem* parent_ = nullptr;
	};

	class Menu
	{
	public:
		Menu();

		void init();
		MenuItem& getRootItem();
		MenuItem* getSelectedItem() const;
		MenuItem* getCurrentContext() const;

		void select(MenuItem* item);
		bool selectPrevious();
		bool selectNext();
		bool expandSelected();
		bool collapseSelected();

		void draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);

	private:
		MenuItem rootItem_;
		MenuItem* selectedItem_ = nullptr;

		void initItem(MenuItem& item);
	};
}
