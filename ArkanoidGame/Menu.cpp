#include "Menu.h"
#include <cassert>
#include <algorithm>

namespace ArkanoidGame
{
	// --- MenuItem ---

	MenuItem::MenuItem() {}

	void MenuItem::setText(const std::string& str, const sf::Font& font, unsigned int charSize)
	{
		text_.setString(str);
		text_.setFont(font);
		text_.setCharacterSize(charSize);
	}

	void MenuItem::setHintText(const std::string& str, const sf::Font& font, unsigned int charSize, sf::Color color)
	{
		hintText_.setString(str);
		hintText_.setFont(font);
		hintText_.setCharacterSize(charSize);
		hintText_.setFillColor(color);
	}

	void MenuItem::setChildrenLayout(Orientation orientation, Alignment alignment, float spacing)
	{
		childrenOrientation_ = orientation;
		childrenAlignment_ = alignment;
		childrenSpacing_ = spacing;
	}

	void MenuItem::setColors(sf::Color selected, sf::Color deselected)
	{
		selectedColor_ = selected;
		deselectedColor_ = deselected;
	}

	void MenuItem::setEnabled(bool enabled) { isEnabled_ = enabled; }

	void MenuItem::addChild(MenuItem* child) { children_.push_back(child); }

	sf::Text& MenuItem::getText() { return text_; }
	sf::Text& MenuItem::getHintText() { return hintText_; }
	Orientation MenuItem::getChildrenOrientation() const { return childrenOrientation_; }
	Alignment MenuItem::getChildrenAlignment() const { return childrenAlignment_; }
	float MenuItem::getChildrenSpacing() const { return childrenSpacing_; }
	bool MenuItem::isEnabled() const { return isEnabled_; }
	const std::vector<MenuItem*>& MenuItem::getChildren() const { return children_; }
	MenuItem* MenuItem::getParent() const { return parent_; }

	// --- Menu ---

	Menu::Menu() {}

	void Menu::init()
	{
		initItem(rootItem_);
	}

	MenuItem& Menu::getRootItem() { return rootItem_; }
	MenuItem* Menu::getSelectedItem() const { return selectedItem_; }

	MenuItem* Menu::getCurrentContext() const
	{
		return selectedItem_ ? selectedItem_->parent_ : const_cast<MenuItem*>(&rootItem_);
	}

	void Menu::select(MenuItem* item)
	{
		assert(item != &rootItem_);

		if (selectedItem_ == item)
			return;

		if (item && !item->isEnabled_)
			return;

		if (selectedItem_)
			selectedItem_->text_.setFillColor(selectedItem_->deselectedColor_);

		selectedItem_ = item;

		if (selectedItem_)
			selectedItem_->text_.setFillColor(selectedItem_->selectedColor_);
	}

	bool Menu::selectPrevious()
	{
		if (!selectedItem_)
			return false;

		MenuItem* parent = selectedItem_->parent_;
		assert(parent);

		auto it = std::find(parent->children_.begin(), parent->children_.end(), selectedItem_);
		if (it != parent->children_.begin())
		{
			select(*(--it));
			return true;
		}
		return false;
	}

	bool Menu::selectNext()
	{
		if (!selectedItem_)
			return false;

		MenuItem* parent = selectedItem_->parent_;
		assert(parent);

		auto it = std::find(parent->children_.begin(), parent->children_.end(), selectedItem_);
		if (it != parent->children_.end() - 1)
		{
			select(*(++it));
			return true;
		}
		return false;
	}

	bool Menu::expandSelected()
	{
		if (selectedItem_ && selectedItem_->children_.size() > 0)
		{
			select(selectedItem_->children_.front());
			return true;
		}
		return false;
	}

	bool Menu::collapseSelected()
	{
		if (selectedItem_ && selectedItem_->parent_ && selectedItem_->parent_ != &rootItem_)
		{
			select(selectedItem_->parent_);
			return true;
		}
		return false;
	}

	void Menu::draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin)
	{
		MenuItem* context = getCurrentContext();

		std::vector<sf::Text*> texts;
		texts.reserve(context->children_.size());
		for (auto& child : context->children_)
		{
			if (child->isEnabled_)
				texts.push_back(&child->text_);
		}

		DrawTextList(window, texts, context->childrenSpacing_, context->childrenOrientation_, context->childrenAlignment_, position, origin);
	}

	void Menu::initItem(MenuItem& item)
	{
		for (auto& child : item.children_)
		{
			child->parent_ = &item;
			initItem(*child);
		}
	}
}
