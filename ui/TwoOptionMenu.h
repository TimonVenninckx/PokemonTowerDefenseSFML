#ifndef TWOOPTIONMENU_H
#define TWOOPTIONMENU_H

#include "GameUI.h"
#include "RoundedRectangleShape.h"


class TwoOptionMenu
{
	sf::RoundedRectangleShape m_box;
	sf::Text				  m_infoText;
	sf::Text				  m_button1Text;
	sf::Text				  m_button2Text;
	sf::RoundedRectangleShape m_button1;
	sf::RoundedRectangleShape m_button2;

public:
	
	// 1 or 2 0 = none;
	TwoOptionMenu();
	void setInfoText(const std::string& text);
	void setButton1Text(const std::string& text);
	void setButton2Text(const std::string& text);
	int pickedOption(sf::Vector2f mousePos);
	void draw(sf::RenderWindow& window);
};



#endif
