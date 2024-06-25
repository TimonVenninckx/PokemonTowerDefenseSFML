#ifndef SAVEINFOUI_H
#define SAVEINFOUI_H

#include "GameUI.h"
#include "RoundedRectangleShape.h"
#include "../Player.h"

class SaveInfoUI
{
	sf::RoundedRectangleShape m_box;
	sf::Text m_moneyText;
	sf::Text m_nameText;
	sf::Text m_pokemonText;
	

	SaveInfo m_info;

public:
	SaveInfoUI(sf::Vector2f position);
	void loadInfo(SaveInfo info);
	void resetSave();
	SaveInfo& getSaveInfo();
	void draw(sf::RenderWindow& window);
	bool contains(sf::Vector2f mousePos);
};



#endif
