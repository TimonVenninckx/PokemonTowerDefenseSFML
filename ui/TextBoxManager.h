#ifndef TEXTBOXMANAGER_H
#define TEXTBOXMANAGER_H

#include "GameUI.h"
#include "RoundedRectangleShape.h"
#include <queue>

struct MessageInfo {
	std::string text;
	std::string characterLocation;
};

class TextBoxManager
{

	bool m_active;
	std::queue<MessageInfo>  m_messages;
	sf::RoundedRectangleShape m_textBox;
	sf::Text				  m_text;
	sf::Text				  m_nextText;
	sf::RoundedRectangleShape m_nextButton;
	sf::RectangleShape		  m_line;

	sf::Texture			  m_texture;
	sf::Sprite				  m_sprite;
public:
	TextBoxManager(int levelId);

	bool isActive();
	void update(sf::Event& event, sf::Vector2f mousePos);
	void draw(sf::RenderWindow& window);
};


#endif
