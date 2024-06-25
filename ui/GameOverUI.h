#ifndef GAMEOVERUI_H
#define GAMEOVERUI_H

#include <SFML/Graphics.hpp>
#include "RoundedRectangleShape.h"

class GameOverUI
{
	sf::RoundedRectangleShape m_box;
	sf::RoundedRectangleShape m_button;
	sf::Text m_text;
	sf::Text m_buttonText;

	sf::Texture m_texture;
	sf::Sprite m_sprite;
public:
	
	GameOverUI();
	void draw(sf::RenderWindow& window);
	bool exitUI(sf::Vector2f mousePosition);
	void setText(const std::string& text);
	void setSprite(const std::string& imageName);

};



#endif
